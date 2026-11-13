#ifndef HAL_BASE_NODE_H
#define HAL_BASE_NODE_H
#include <cstdint>
#include <memory>
#include <vector>
#include "hal_status.h"
#include "hal_node_port.h"
namespace hal {
class pipeline;
class node {
public:
    node(std::uint32_t inputs = 0, std::uint32_t outputs = 0);
    virtual ~node();
    node(const node&) = delete;
    node& operator=(const node&) = delete;
    virtual std::int8_t init() = 0;
    virtual std::int8_t deinit() = 0;
    virtual std::int8_t start() { return CONTROL_UNSUPPORTED; }
    virtual std::int8_t stop() { return CONTROL_UNSUPPORTED; }
    virtual std::int8_t pause() { return CONTROL_UNSUPPORTED; }
    virtual std::int8_t resume() { return CONTROL_UNSUPPORTED; }
    hal_node_port_in* get_inport(std::uint32_t index = 0);
    hal_node_port_out* get_outport(std::uint32_t index = 0);
    // 同步定制控制：HAL 不解释协议，返回后不再访问 payload。
    virtual std::int32_t custom_process(std::uint32_t command, void* payload,
                                       std::uint32_t payload_size);
protected:
    // 仅 pipeline 可以发起公共连接变更；适配实现负责实际绑定。
    virtual std::int8_t connect_next(node&, std::uint32_t, std::uint32_t)
    { return CONTROL_UNSUPPORTED; }
    virtual std::int8_t disconnect_next(node&, std::uint32_t, std::uint32_t)
    { return CONTROL_UNSUPPORTED; }
private:
    friend class pipeline;
    pipeline* m_pipeline = nullptr;
    std::vector<std::unique_ptr<hal_node_port_in>> m_inputs;
    std::vector<std::unique_ptr<hal_node_port_out>> m_outputs;
};
}
#endif
