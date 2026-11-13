#ifndef HARDWARE_TYPE_H
#define HARDWARE_TYPE_H

#include <stdlib.h>

namespace hal {
    enum HW_TYPE {
        HW_TYPE_UNKNOWN = 0,
        HW_TYPE_SENSOR,
        HW_TYPE_ISP,
        HW_TYPE_SCL,
        HW_TYPE_VENC,
        HW_TYPE_FRAME_SINK,
        // 其他硬件类型...
    };
   
};

#endif