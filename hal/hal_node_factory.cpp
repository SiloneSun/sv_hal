#include "hal_node_factory.h"
#include "sigma_308_factory.h"

using namespace hal;


std::shared_ptr<node_factory> node_factory::instances[SOC_TYPE_MAX] = {nullptr};


node_factory::~node_factory() {
    // 清理代码
}

node_factory::node_factory(SOC_TYPE soc_type)
: m_soc_type(soc_type) {
}

std::shared_ptr<node_factory> node_factory::get_instance(SOC_TYPE soc_type) {
    if(instances[soc_type] == nullptr)
    {
        switch(soc_type)
        {
        case SIGMA_308:
            instances[soc_type] = std::make_shared<sigma_308_factory>(soc_type);
            break;
        default:
            return nullptr;
            break;
        }
        
    }
    return instances[soc_type];
}

// int8_t clear_all_instances();
int8_t node_factory::clear_all_instances()
{
    for(int i = 0; i < SOC_TYPE_MAX; i++)
    {
        instances[i] = nullptr;
    }
}