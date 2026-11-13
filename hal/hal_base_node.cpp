#include "hal_base_node.h"
namespace hal {
node::node(std::uint32_t inputs, std::uint32_t outputs)
{
    for (std::uint32_t i=0; i<inputs; ++i) {
        m_inputs.push_back(std::make_unique<hal_node_port_in>(this,i));
    }
    for (std::uint32_t i=0; i<outputs; ++i) {
        m_outputs.push_back(std::make_unique<hal_node_port_out>(this,i));
    }
}
node::~node() = default;
hal_node_port_in* node::get_inport(std::uint32_t index)
{ return index < m_inputs.size() ? m_inputs[index].get() : nullptr; }
hal_node_port_out* node::get_outport(std::uint32_t index)
{ return index < m_outputs.size() ? m_outputs[index].get() : nullptr; }
std::int32_t node::custom_process(std::uint32_t, void*, std::uint32_t)
{ return CONTROL_UNSUPPORTED; }
}
