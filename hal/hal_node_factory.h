/*
    接受参数，初始化一组hal_adaptors实例
    hal_adaptor实例， 按照hal的抽象，封装了实际的硬件接口
*/
#ifndef HAL_NODE_FACTORY_H
#define HAL_NODE_FACTORY_H

#include <stdlib.h>
#include <memory>
#include <vector>
#include "soc_type.h"
#include "hardware_type.h"
#include "hal_base_node.h"

namespace hal {
    class node_factory {
    public:
        node_factory() = delete;
        node_factory(SOC_TYPE soc_type);
        virtual ~node_factory();
        static std::shared_ptr<node_factory> get_instance(SOC_TYPE soc_type);
        static int8_t clear_all_instances();
        virtual std::shared_ptr<base_node> create_hal_adaptor(HW_TYPE hw_type) = 0;

    private:
        SOC_TYPE m_soc_type;
        static std::shared_ptr<node_factory> instances[SOC_TYPE_MAX];
    };
};

#endif
