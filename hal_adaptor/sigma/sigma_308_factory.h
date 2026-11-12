#ifndef SIGMA_308_FACTORY_H
#define SIGMA_308_FACTORY_H

#include "hal/hal_node_factory.h"

namespace hal_adaptor {

class sigma_308_factory : public hal::node_factory {
public:
    sigma_308_factory();
    ~sigma_308_factory() override;
    std::shared_ptr<hal::node> create_hal_adaptor(hal::HW_TYPE hw_type) override;
};

}
#endif
