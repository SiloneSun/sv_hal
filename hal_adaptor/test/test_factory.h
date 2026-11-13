#ifndef TEST_FACTORY_H
#define TEST_FACTORY_H

#include "hal/hal_node_factory.h"

namespace hal_adaptor {

class test_factory : public hal::node_factory {
public:
    test_factory();
    ~test_factory() override;
    std::shared_ptr<hal::node> create_hal_adaptor(hal::HW_TYPE hw_type) override;
};

}
#endif
