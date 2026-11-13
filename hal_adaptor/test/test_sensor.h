#ifndef HAL_ADAPTOR_TEST_SENSOR_H
#define HAL_ADAPTOR_TEST_SENSOR_H
#include "hal/hal_sensor.h"
#include "test_endpoint.h"
#include "bsp/test/color_sensor.h"
namespace hal_adaptor {
class test_sensor final : public hal::hal_sensor, public test_endpoint {
public:
    std::int8_t init() override;
    std::int8_t deinit() override;
    std::int8_t start() override;
    std::int8_t stop() override;
    std::int32_t set_exposure(std::uint32_t value) override;
    std::int32_t get_exposure(std::uint32_t& value) override;
    std::shared_ptr<test_bsp::node> bsp_node() const override { return m_bsp; }
    std::uint64_t produced() const { return m_bsp->produced(); }
protected:
    std::int8_t connect_next(hal::node& next,std::uint32_t out,std::uint32_t in) override;
    std::int8_t disconnect_next(hal::node& next,std::uint32_t out,std::uint32_t in) override;
private:
    std::shared_ptr<test_bsp::color_sensor> m_bsp=std::make_shared<test_bsp::color_sensor>();
};
}
#endif
