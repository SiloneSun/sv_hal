#include "hal_base_node.h"

namespace hal {

node::node() = default;
node::~node() = default;

std::int32_t node::custom_process(std::uint32_t, void*, std::uint32_t)
{
    return CONTROL_UNSUPPORTED;
}

}
