#ifndef HAL_BASE_NODE_H
#define HAL_BASE_NODE_H

#include <cstdint>
#include "hal_status.h"

namespace hal {

class node {
public:
    node();
    virtual ~node();

    virtual std::int8_t init() = 0;
    virtual std::int8_t deinit() = 0;

    // 定制扩展：HAL 不解释命令或参数，协议由 App 与适配实现约定。
    // 同步调用；返回后不得继续访问 payload。调用方负责指针的有效性。
    // 参数方向、长度和版本由具体协议规定。默认不支持任何定制命令。
    virtual std::int32_t custom_process(std::uint32_t command, void* payload,
                                       std::uint32_t payload_size);
};

}
#endif
