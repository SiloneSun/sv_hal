#ifndef HAL_SENSOR_H
#define HAL_SENSOR_H

#include "hal_base_node.h"

namespace hal {

class hal_sensor : public node {
public:
    ~hal_sensor() override = default;

    // 通用能力使用正式接口。单位：微秒；失败时不修改调用方输出。
    // 这里演示手动曝光时间，不代表完整的自动曝光（AE）算法配置。
    virtual std::int32_t set_exposure(std::uint32_t exposure_us) = 0;
    virtual std::int32_t get_exposure(std::uint32_t& exposure_us) = 0;

protected:
    hal_sensor() : node(0, 1) {}
};

}
#endif
