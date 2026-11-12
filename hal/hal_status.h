#ifndef HAL_STATUS_H
#define HAL_STATUS_H

#include <cstdint>

namespace hal {

enum control_status : std::int32_t {
    CONTROL_OK = 0,
    CONTROL_UNSUPPORTED = -1,
    CONTROL_INVALID_ARGUMENT = -2,
    CONTROL_VERSION_MISMATCH = -3,
    CONTROL_INVALID_STATE = -4
};

}
#endif
