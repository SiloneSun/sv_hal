#include "hal/hal_pipeline.h"
#include "hal/hal_frame_sink.h"
#include "hal_adaptor/test/test_sensor.h"
#include "hal_adaptor/test/test_frame_sink.h"
#include "bsp/test/file_save.h"
#include "bsp/test/color_sensor.h"
#include <mutex>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>

namespace {
void check(bool value, const char* message)
{
    if (!value) { std::cerr << message << '\n'; std::exit(1); }
    return;
}

class holding_sink final : public test_bsp::node {
public:
    holding_sink() : node(true,false) {}
    int init() override { return 0; }
    int deinit() override { return 0; }
    int start() override { return 0; }
    int stop() override { return 0; }
    bool data_callback(const test_bsp::frame& value) override
    {
        std::lock_guard<std::mutex> lock(mutex);
        frames.push_back(value);
        return true;
    }
    std::mutex mutex;
    std::vector<test_bsp::frame> frames;
};
}

int main()
{
    namespace fs = std::filesystem;
    auto path = fs::temp_directory_path() / ("hal-pipeline-" +
        std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + ".yuv");
    auto source = std::make_shared<hal_adaptor::test_sensor>();
    auto sink = std::make_shared<hal_adaptor::test_frame_sink>();
    hal::pipeline graph;
    check(sink->set_path(path.string()) == 0, "set output path");
    check(graph.add_node("sensor", source) == 0 && graph.add_node("file", sink) == 0,
          "add nodes");
    check(graph.add_node("sensor", sink) != 0 && graph.add_node("alias", source) != 0,
          "reject duplicate names/instances");
    hal::pipeline other;
    check(other.add_node("shared", source) != 0, "reject multiple pipeline ownership");
    check(graph.init() == 0, "initialize");
    check(graph.connect(source->get_outport(), sink->get_inport()) == 0, "connect");
    check(graph.connect(source->get_outport(), sink->get_inport()) != 0, "duplicate connection");
    check(graph.start() == 0 && graph.start() == 0, "start");
    check(graph.disconnect(source->get_outport(), sink->get_inport()) != 0,
          "reject running disconnect");
    std::this_thread::sleep_for(std::chrono::milliseconds(180));
    check(graph.stop() == 0 && graph.stop() == 0, "stop and drain");
    auto stats = sink->statistics();
    check(stats.written > 0 && stats.accepted == stats.written && stats.queued == 0,
          "all accepted frames drained");
    constexpr std::size_t bytes = 640 * 480 * 3 / 2;
    check(fs::file_size(path) == stats.written * bytes, "complete I420 frames");
    std::ifstream input(path, std::ios::binary);
    const unsigned char y[8] = {235,210,170,145,106,81,41,16};
    const unsigned char u[8] = {128,16,166,54,202,90,240,128};
    const unsigned char v[8] = {128,146,16,34,222,240,110,128};
    for (int plane = 0; plane < 3; ++plane) {
        unsigned width = plane ? 320 : 640, height = plane ? 240 : 480;
        const auto* expected = plane == 0 ? y : (plane == 1 ? u : v);
        for (unsigned row = 0; row < height; ++row) {
            for (unsigned col = 0; col < width; ++col) {
                check(static_cast<unsigned char>(input.get()) == expected[col * 8 / width],
                      "I420 color bars");
            }
        }
    }
    check(graph.start() == 0, "restart");
    std::this_thread::sleep_for(std::chrono::milliseconds(80));
    check(graph.stop() == 0, "restart stop");
    check(fs::file_size(path) == sink->statistics().written * bytes, "restart truncates");
    check(graph.disconnect(source->get_outport(), sink->get_inport()) == 0 &&
          graph.deinit() == 0, "disconnect and deinit");
    check(graph.remove_node("sensor") == 0 && graph.remove_node("file") == 0,
          "remove stopped nodes");
    fs::remove(path);

    // 固定池耗尽时跳过产帧；持有的帧在反复调度、停止和释放源后不被改写。
    {
        auto source_bsp=std::make_shared<test_bsp::color_sensor>();
        auto collector=std::make_shared<holding_sink>();
        check(source_bsp->init()==0 && source_bsp->connect_next(collector)==0 &&
              source_bsp->start()==0, "buffer pool setup");
        const auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(2);
        while (!source_bsp->dropped() && std::chrono::steady_clock::now()<deadline) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        check(source_bsp->dropped()>0 && source_bsp->produced()==15, "pool must stay bounded");
        check(source_bsp->stop()==0, "stop source with all buffers held");
        test_bsp::frame retained;
        {
            std::lock_guard<std::mutex> lock(collector->mutex);
            check(collector->frames.size()==15,"all pool slots retained");
            for (std::size_t i=0;i<collector->frames.size();++i) {
                for (std::size_t j=0;j<i;++j) {
                    check(collector->frames[i].buffer!=collector->frames[j].buffer,
                          "in-use buffers cannot be reused");
                }
            }
            retained=collector->frames.front();
            collector->frames.clear();
        }
        check(source_bsp->start()==0,"restart pool");
        std::this_thread::sleep_for(std::chrono::milliseconds(70));
        check(source_bsp->stop()==0 && source_bsp->produced()>0,"released buffers reused");
        check(source_bsp->deinit()==0,"release pool");
        source_bsp.reset();
        check(retained.buffer->bytes[0]==235,"held buffer survives source destruction");
    }

    // 一对多：两个文件消费者共享源，两个输出内容完全一致。
    {
        auto path2=path.string()+".second";
        hal::pipeline branch;
        auto src=std::make_shared<hal_adaptor::test_sensor>();
        auto a=std::make_shared<hal_adaptor::test_frame_sink>();
        auto b=std::make_shared<hal_adaptor::test_frame_sink>();
        check(a->set_path(path.string())==0 && b->set_path(path2)==0,"branch paths");
        check(branch.add_node("src",src)==0 && branch.add_node("a",a)==0 &&
              branch.add_node("b",b)==0 && branch.init()==0 &&
              branch.connect(src->get_outport(),a->get_inport())==0 &&
              branch.connect(src->get_outport(),b->get_inport())==0 && branch.start()==0,
              "one output to two sinks");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        check(branch.stop()==0 && a->statistics().written>0 &&
              a->statistics().written==b->statistics().written &&
              fs::file_size(path)==fs::file_size(path2),"shared source drains both sinks");
        fs::remove(path); fs::remove(path2);
    }

    // 未启动的 file_save 拒收；停止后的 file_save 同样拒收。
    test_bsp::file_save file;
    check(file.set_path(path.string()) == 0 && file.init() == 0, "file setup");
    auto storage = std::make_shared<test_bsp::frame_buffer>(bytes);
    test_bsp::frame frame{storage, 0, 0};
    check(!file.data_callback(frame), "reject frames before start");
    check(file.start() == 0, "file start");
    // 快速入队，验证容量始终有界；允许消费线程同时运行。
    for (int i = 0; i < 2000; ++i) { file.data_callback(frame); }
    check(file.stop() == 0, "file drain");
    stats = file.statistics();
    check(stats.peak_queued <= 15 && stats.dropped > 0 && stats.queued == 0 &&
          stats.accepted == stats.written, "bounded queue drops new frames and drains");
    check(!file.data_callback(frame), "reject frames after stop");
    check(file.deinit() == 0, "file deinit");
    fs::remove(path);

    // Linux 的 /dev/full 每次写入失败，验证只影响 sink 而不终止生产。
    if (fs::exists("/dev/full")) {
        hal::pipeline failing;
        auto src = std::make_shared<hal_adaptor::test_sensor>();
        auto dst = std::make_shared<hal_adaptor::test_frame_sink>();
        check(dst->set_path("/dev/full") == 0, "full path");
        check(failing.add_node("source",src) == 0 && failing.add_node("sink",dst) == 0 &&
              failing.init() == 0 && failing.connect(src->get_outport(),dst->get_inport()) == 0 &&
              failing.start() == 0, "failing pipeline still starts");
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
        check(dst->statistics().faulted && src->produced() > 1, "file failure is local");
        check(failing.stop() == 0 && dst->statistics().queued == 0, "stop after write failure");
    }
    {
        hal::pipeline unopened;
        auto src = std::make_shared<hal_adaptor::test_sensor>();
        auto dst = std::make_shared<hal_adaptor::test_frame_sink>();
        dst->set_path("/nonexistent-hal-directory/frame.yuv");
        check(unopened.add_node("source", src) == 0 && unopened.add_node("sink", dst) == 0 &&
              unopened.init() == 0 && unopened.connect(src->get_outport(), dst->get_inport()) == 0,
              "open failure setup");
        check(unopened.start() != 0 && src->produced() == 0, "failed sink prevents source start");
    }
    std::cout << "Automatic pipeline, I420, queue bounds/drain, restart and local faults passed.\n";
    return 0;
}
