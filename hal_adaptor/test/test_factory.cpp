#include "test_factory.h"
#include "test_sensor.h"
#include "test_frame_sink.h"

namespace hal_adaptor {

test_factory::test_factory() = default;
test_factory::~test_factory() = default;

std::shared_ptr<hal::node>
test_factory::create_hal_adaptor(hal::HW_TYPE hw_type)
{
    switch (hw_type) {
    case hal::HW_TYPE_SENSOR:
        return std::make_shared<test_sensor>();
    case hal::HW_TYPE_FRAME_SINK:
        return std::make_shared<test_frame_sink>();
    default:
        return nullptr;
    }
}

}
