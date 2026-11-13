#include "demo.h"
#include "hal/hal_pipeline.h"
#include "hal/hal_frame_sink.h"
#include <chrono>
#include <iostream>
#include <thread>

int run_demo(hal::node_factory& factory)
{
    auto sensor=factory.create_hal_adaptor(hal::HW_TYPE_SENSOR);
    auto sink=std::dynamic_pointer_cast<hal::hal_frame_sink>(
        factory.create_hal_adaptor(hal::HW_TYPE_FRAME_SINK));
    if (!sensor || !sink) {
        std::cout << "This platform has not implemented the Sensor -> file demo.\n";
        return 0;
    }
    hal::pipeline pipeline;
    // I420 640x480，30 FPS，裸数据；每次 start 覆盖该文件。
    if (sink->set_path("capture.yuv")!=0 || pipeline.add_node("sensor",sensor)!=0 ||
        pipeline.add_node("file",sink)!=0 || pipeline.init()!=0 ||
        pipeline.connect(sensor->get_outport(),sink->get_inport())!=0 || pipeline.start()!=0) {
        std::cerr << "Pipeline setup/start failed.\n";
        return 1;
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
    if (pipeline.stop()!=0 || pipeline.disconnect(sensor->get_outport(),sink->get_inport())!=0 ||
        pipeline.deinit()!=0) { return 1; }
    std::cout << "Stopped after 3 seconds; output: capture.yuv (I420, 640x480, nominal 30 FPS).\n";
    return 0;
}
