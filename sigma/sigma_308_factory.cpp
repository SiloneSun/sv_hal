#include "sigma_308_factory.h"
#include <memory>
#include "hardware_type.h"

using namespace hal;


sigma_308_factory::sigma_308_factory(SOC_TYPE soc_type) : node_factory(soc_type)
{ 
}

sigma_308_factory::~sigma_308_factory()
{
}

std::shared_ptr<base_node> sigma_308_factory::create_hal_adaptor(HW_TYPE hw_type)
{
    switch (hw_type)
    {
    case HW_TYPE_SENSOR:
        // sigma_308_sensor_adaptor
        break;
    
    default:
        break;
    }
}