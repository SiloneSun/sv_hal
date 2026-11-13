#include "test_sensor.h"
namespace hal_adaptor {
std::int8_t test_sensor::init() { return m_bsp->init(); }
std::int8_t test_sensor::deinit() { return m_bsp->deinit(); }
std::int8_t test_sensor::start() { return m_bsp->start(); }
std::int8_t test_sensor::stop() { return m_bsp->stop(); }
std::int32_t test_sensor::set_exposure(std::uint32_t value) { return m_bsp->set_exposure(value); }
std::int32_t test_sensor::get_exposure(std::uint32_t& value) { return m_bsp->get_exposure(value); }
std::int8_t test_sensor::connect_next(hal::node& next,std::uint32_t out,std::uint32_t in)
{
    auto* endpoint=dynamic_cast<test_endpoint*>(&next);
    if (!endpoint || out!=0 || in!=0) { return hal::CONTROL_UNSUPPORTED; }
    return m_bsp->connect_next(endpoint->bsp_node());
}
std::int8_t test_sensor::disconnect_next(hal::node& next,std::uint32_t out,std::uint32_t in)
{
    auto* endpoint=dynamic_cast<test_endpoint*>(&next);
    if (!endpoint || out!=0 || in!=0) { return hal::CONTROL_UNSUPPORTED; }
    return m_bsp->disconnect_next(endpoint->bsp_node());
}
}
