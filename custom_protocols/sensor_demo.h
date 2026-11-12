#ifndef CUSTOM_PROTOCOLS_SENSOR_DEMO_H
#define CUSTOM_PROTOCOLS_SENSOR_DEMO_H

#include <cstdint>
#include <type_traits>

// App 与支持该协议的适配器共享；不属于 HAL 公共能力规范。
// 仅用模拟测试图案展示项目定制协议，不代表真实芯片功能。
namespace sensor_demo_protocol {

// 为该协议预留 0x80010000～0x8001ffff，避免与其他项目命令冲突。
enum command : std::uint32_t {
    SET_TEST_PATTERN = 0x80010001,
    GET_TEST_PATTERN = 0x80010002
};

// V1：SET 输入两个字段；GET 输入 version，输出 pattern。
// pattern：0=关闭、1=模拟彩条；数据只在当前同步调用内使用。
struct test_pattern_v1 {
    std::uint32_t version;
    std::uint32_t pattern;
};
static_assert(sizeof(test_pattern_v1) == 8, "Test pattern V1 layout changed");
static_assert(std::is_standard_layout<test_pattern_v1>::value &&
              std::is_trivially_copyable<test_pattern_v1>::value,
              "Payload must support byte copying");

}
#endif
