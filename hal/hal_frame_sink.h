#ifndef HAL_FRAME_SINK_H
#define HAL_FRAME_SINK_H
#include "hal_base_node.h"
#include <string>
namespace hal {
class hal_frame_sink : public node {
public:
    virtual std::int32_t set_path(const std::string& path) = 0;
protected:
    hal_frame_sink() : node(1, 0) {}
};
}
#endif
