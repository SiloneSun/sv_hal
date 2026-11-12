#ifndef HAL_NODE_FACTORY_H
#define HAL_NODE_FACTORY_H

#include <memory>
#include "hardware_type.h"
#include "hal_base_node.h"

namespace hal {

// HAL 只声明创建能力，不选择芯片或持有具体平台类型。
class node_factory {
public:
    virtual ~node_factory();
    virtual std::shared_ptr<base_node> create_hal_adaptor(HW_TYPE hw_type) = 0;

protected:
    node_factory();
};

// 由构建时选中的适配层实现，每次调用创建独立工厂。
std::shared_ptr<node_factory> create_factory();

}
#endif
