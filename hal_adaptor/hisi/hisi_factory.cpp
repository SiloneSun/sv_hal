#include "hisi_factory.h"

namespace hal_adaptor {

hisi_factory::hisi_factory() = default;
hisi_factory::~hisi_factory() = default;

std::shared_ptr<hal::node>
hisi_factory::create_hal_adaptor(hal::HW_TYPE hw_type)
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
