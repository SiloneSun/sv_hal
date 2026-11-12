#include "sigma_308_factory.h"

namespace hal_adaptor {

sigma_308_factory::sigma_308_factory() = default;
sigma_308_factory::~sigma_308_factory() = default;

std::shared_ptr<hal::base_node>
sigma_308_factory::create_hal_adaptor(hal::HW_TYPE hw_type)
{
    switch (hw_type) {
    case hal::HW_TYPE_SENSOR:
        // 后续在这里创建具体 Sensor 适配节点。
        return nullptr;
    default:
        return nullptr;
    }
}

}
