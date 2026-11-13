#include "color_sensor.h"
#include <chrono>
namespace test_bsp {
color_sensor::~color_sensor() { stop(); }
int color_sensor::init()
{
    if (!m_pool.empty()) { return 0; }
    std::vector<std::shared_ptr<frame_buffer>> pool;
    for (std::size_t i=0;i<pool_capacity;++i) {
        pool.push_back(std::make_shared<frame_buffer>(width*height*3/2));
    }
    m_pool=std::move(pool);
    return 0;
}
int color_sensor::deinit()
{
    if (m_running) { return -4; }
    m_pool.clear();
    return 0;
}
int color_sensor::start()
{
    if (m_running) { return 0; }
    if (m_pool.empty()) { return -4; }
    m_produced=0; m_dropped=0; m_running=true;
    try { m_thread=std::thread(&color_sensor::run,this); }
    catch (...) { m_running=false; return -6; }
    return 0;
}
int color_sensor::stop()
{
    { std::lock_guard<std::mutex> lock(m_wait_mutex); m_running=false; }
    m_wake.notify_all();
    if (m_thread.joinable()) { m_thread.join(); }
    return 0;
}
void color_sensor::fill_bars(frame_buffer& buffer)
{
    // BT.601 limited-range I420：白黄青绿品红红蓝黑。
    static constexpr std::uint8_t y[8]={235,210,170,145,106,81,41,16};
    static constexpr std::uint8_t u[8]={128,16,166,54,202,90,240,128};
    static constexpr std::uint8_t v[8]={128,146,16,34,222,240,110,128};
    for (unsigned row=0;row<height;++row) {
        for (unsigned col=0;col<width;++col) { buffer.bytes[row*width+col]=y[col*8/width]; }
    }
    for (unsigned row=0;row<height/2;++row) {
        for (unsigned col=0;col<width/2;++col) {
            auto offset=row*(width/2)+col;
            buffer.bytes[width*height+offset]=u[col*8/(width/2)];
            buffer.bytes[width*height*5/4+offset]=v[col*8/(width/2)];
        }
    }
    return;
}
void color_sensor::run()
{
    using clock=std::chrono::steady_clock;
    const auto period=std::chrono::nanoseconds(1000000000/fps);
    auto next=clock::now();
    while (m_running) {
        bool available=false;
        for (const auto& buffer:m_pool) {
            if (buffer.use_count()!=1) { continue; }
            {
                std::unique_lock<std::shared_mutex> access(buffer->access_mutex);
                fill_bars(*buffer);
            }
            frame value{buffer,m_produced.load(),static_cast<std::uint64_t>(
                std::chrono::duration_cast<std::chrono::microseconds>(clock::now().time_since_epoch()).count())};
            dispatch_frame(value);
            ++m_produced; available=true;
            break;
        }
        if (!available) { ++m_dropped; }
        next+=period;
        if (next<clock::now()) { next=clock::now()+period; }
        std::unique_lock<std::mutex> lock(m_wait_mutex);
        m_wake.wait_until(lock,next,[&]{return !m_running.load();});
    }
    return;
}
int color_sensor::set_exposure(std::uint32_t value)
{
    if (m_pool.empty()) { return -4; }
    if (value<1 || value>1000000) { return -2; }
    m_exposure_us=value;
    return 0;
}
int color_sensor::get_exposure(std::uint32_t& value) const
{
    if (m_pool.empty()) { return -4; }
    value=m_exposure_us;
    return 0;
}
}
