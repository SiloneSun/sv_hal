#ifndef SIGMA_308_SENSOR_H
#define SIGMA_308_SENSOR_H

#include "hal/hal_sensor.h"

namespace hal_adaptor {

// 模拟实现，不接 SDK。控制操作由调用方串行执行。
class sigma_308_sensor final : public hal::hal_sensor {
public:
    std::int8_t init() override;
    std::int8_t deinit() override;
    std::int32_t set_exposure(std::uint32_t exposure_us) override;
    std::int32_t get_exposure(std::uint32_t& exposure_us) override;
    std::int32_t custom_process(std::uint32_t command, void* payload,
                                std::uint32_t payload_size) override;

private:
    bool m_initialized = false;
    std::uint32_t m_exposure_us = 10000;
    std::uint32_t m_test_pattern = 0;
};

}
#endif
