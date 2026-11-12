#include "sigma_308_sensor.h"
#include "custom_protocols/sensor_demo.h"
#include <cstring>

namespace hal_adaptor {

std::int8_t sigma_308_sensor::init()
{
    m_initialized = true;
    return 0;
}

std::int8_t sigma_308_sensor::deinit()
{
    m_initialized = false;
    return 0;
}

std::int32_t sigma_308_sensor::set_exposure(std::uint32_t exposure_us)
{
    if (!m_initialized) {
        return hal::CONTROL_INVALID_STATE;
    }
    // 示例范围，不代表真实 Sensor 能力。
    if (exposure_us < 1 || exposure_us > 1000000) {
        return hal::CONTROL_INVALID_ARGUMENT;
    }
    m_exposure_us = exposure_us;
    return hal::CONTROL_OK;
}

std::int32_t sigma_308_sensor::get_exposure(std::uint32_t& exposure_us)
{
    if (!m_initialized) {
        return hal::CONTROL_INVALID_STATE;
    }
    exposure_us = m_exposure_us;
    return hal::CONTROL_OK;
}

std::int32_t sigma_308_sensor::custom_process(std::uint32_t command, void* payload,
                                             std::uint32_t payload_size)
{
    namespace protocol = sensor_demo_protocol;
    if (command != protocol::SET_TEST_PATTERN && command != protocol::GET_TEST_PATTERN) {
        return hal::node::custom_process(command, payload, payload_size);
    }
    if (!payload || payload_size != sizeof(protocol::test_pattern_v1)) {
        return hal::CONTROL_INVALID_ARGUMENT;
    }

    // 解析及校验完全属于适配层。复制的是小型控制参数，不是图像数据。
    protocol::test_pattern_v1 config{};
    std::memcpy(&config, payload, sizeof(config));
    if (config.version != 1) {
        return hal::CONTROL_VERSION_MISMATCH;
    }
    if (!m_initialized) {
        return hal::CONTROL_INVALID_STATE;
    }
    if (command == protocol::SET_TEST_PATTERN) {
        if (config.pattern > 1) {
            return hal::CONTROL_INVALID_ARGUMENT;
        }
        m_test_pattern = config.pattern;
    } else {
        config.pattern = m_test_pattern;
        std::memcpy(payload, &config, sizeof(config));
    }
    return hal::CONTROL_OK;
}

}
