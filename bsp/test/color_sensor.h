#ifndef BSP_TEST_COLOR_SENSOR_H
#define BSP_TEST_COLOR_SENSOR_H
#include "node.h"
#include <atomic>
#include <condition_variable>
#include <thread>
namespace test_bsp {
class color_sensor final : public node {
public:
    static constexpr unsigned width=640,height=480,fps=30;
    static constexpr std::size_t pool_capacity=15;
    color_sensor() : node(false,true) {}
    ~color_sensor() override;
    int init() override;
    int deinit() override;
    int start() override;
    int stop() override;
    int set_exposure(std::uint32_t value);
    int get_exposure(std::uint32_t& value) const;
    std::uint64_t produced() const { return m_produced.load(); }
    std::uint64_t dropped() const { return m_dropped.load(); }
private:
    void run();
    void fill_bars(frame_buffer& buffer);
    std::vector<std::shared_ptr<frame_buffer>> m_pool;
    std::thread m_thread;
    std::mutex m_wait_mutex;
    std::condition_variable m_wake;
    std::atomic<bool> m_running{false};
    std::atomic<std::uint64_t> m_produced{0},m_dropped{0};
    std::uint32_t m_exposure_us=10000;
};
}
#endif
