#include "sigma_308_factory.h"
#include "sigma_308_sensor.h"

namespace hal_adaptor {

sigma_308_factory::sigma_308_factory() = default;
sigma_308_factory::~sigma_308_factory() = default;

std::shared_ptr<hal::node>
sigma_308_factory::create_hal_adaptor(hal::HW_TYPE hw_type)
{
    switch (hw_type) {
    case hal::HW_TYPE_SENSOR:
        return std::make_shared<sigma_308_sensor>();
    default:
        return nullptr;
    }
}

}
