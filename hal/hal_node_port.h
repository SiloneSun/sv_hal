#ifndef HAL_NODE_PORT_H
#define HAL_NODE_PORT_H
#include <cstdint>
namespace hal {
class node;
// 只描述逻辑端点，生命周期属于节点，不接收或传递图像缓冲。
class node_port {
public:
    node* owner() const { return m_owner; }
    std::uint32_t index() const { return m_index; }
protected:
    node_port(node* owner, std::uint32_t index) : m_owner(owner), m_index(index) {}
private:
    node* m_owner;
    std::uint32_t m_index;
};
class hal_node_port_in final : public node_port {
public:
    hal_node_port_in(node* owner, std::uint32_t index) : node_port(owner,index) {}
};
class hal_node_port_out final : public node_port {
public:
    hal_node_port_out(node* owner, std::uint32_t index) : node_port(owner,index) {}
};
}
#endif
