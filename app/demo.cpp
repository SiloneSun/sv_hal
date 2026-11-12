#include "demo.h"
#include "hal/hal_pipeline.h"
#include "hal/hal_sensor.h"
#include "custom_protocols/sensor_demo.h"
#include <iostream>

namespace {

int demonstrate_sensor(hal::hal_sensor& sensor)
{
    namespace protocol = sensor_demo_protocol;
    if (sensor.init() != 0) {
        return 1;
    }

    // 通用业务：正式 HAL 接口。
    std::uint32_t exposure_us = 0;
    const auto set_result = sensor.set_exposure(20000);
    const auto get_result = sensor.get_exposure(exposure_us);
    const auto invalid_exposure = sensor.set_exposure(0);
    std::uint32_t unchanged_exposure = 0;
    const auto reread_result = sensor.get_exposure(unchanged_exposure);

    // 项目定制：App 与适配器遵循同一协议，HAL 不认识其内容。
    protocol::test_pattern_v1 config{1, 1};
    const auto custom_set = sensor.custom_process(protocol::SET_TEST_PATTERN,
                                                  &config, sizeof(config));
    config.pattern = 0;
    const auto custom_get = sensor.custom_process(protocol::GET_TEST_PATTERN,
                                                  &config, sizeof(config));
    const auto unknown = sensor.custom_process(0xffffffffU, nullptr, 0);
    const auto short_payload = sensor.custom_process(protocol::GET_TEST_PATTERN,
                                                     &config, sizeof(config) - 1);
    const auto null_payload = sensor.custom_process(protocol::GET_TEST_PATTERN,
                                                    nullptr, sizeof(config));
    config.version = 2;
    const auto version = sensor.custom_process(protocol::GET_TEST_PATTERN,
                                               &config, sizeof(config));
    const auto deinit_result = sensor.deinit();
    config.version = 1;
    const auto stopped = sensor.custom_process(protocol::GET_TEST_PATTERN,
                                               &config, sizeof(config));
    std::uint32_t stopped_output = 123;
    const auto stopped_get = sensor.get_exposure(stopped_output);

    std::cout << "HAL exposure: " << exposure_us << " us; SET=" << set_result
              << ", GET=" << get_result << '\n'
              << "Custom simulated pattern: " << config.pattern
              << "; SET=" << custom_set << ", GET=" << custom_get << '\n'
              << "Custom errors: unknown=" << unknown << ", short=" << short_payload
              << ", null=" << null_payload << ", version=" << version
              << ", after deinit=" << stopped << '\n';

    return set_result == hal::CONTROL_OK && get_result == hal::CONTROL_OK &&
           exposure_us == 20000 && invalid_exposure == hal::CONTROL_INVALID_ARGUMENT &&
           reread_result == hal::CONTROL_OK && unchanged_exposure == 20000 &&
           custom_set == hal::CONTROL_OK && custom_get == hal::CONTROL_OK &&
           config.pattern == 1 && unknown == hal::CONTROL_UNSUPPORTED &&
           short_payload == hal::CONTROL_INVALID_ARGUMENT &&
           null_payload == hal::CONTROL_INVALID_ARGUMENT &&
           version == hal::CONTROL_VERSION_MISMATCH &&
           stopped == hal::CONTROL_INVALID_STATE &&
           stopped_get == hal::CONTROL_INVALID_STATE && stopped_output == 123 &&
           deinit_result == 0 ? 0 : 1;
}

}

int run_demo(hal::node_factory& factory)
{
    auto pipeline = std::make_shared<hal::pipeline>();
    if (pipeline->init() != 0) {
        return 1;
    }

    auto node = factory.create_hal_adaptor(hal::HW_TYPE_SENSOR);
    int result = 0;
    if (node) {
        // 当前工厂统一返回 node；核对能力类别后使用 Sensor 正式接口。
        auto sensor = std::dynamic_pointer_cast<hal::hal_sensor>(node);
        result = sensor ? demonstrate_sensor(*sensor) : 1;
    } else {
        std::cout << "Sensor adaptor is not implemented yet.\n";
    }
    const auto cleanup_result = pipeline->deinit();
    return result == 0 && cleanup_result == 0 ? 0 : 1;
}
