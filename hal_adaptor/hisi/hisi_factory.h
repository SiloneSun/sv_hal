#ifndef HISI_FACTORY_H
#define HISI_FACTORY_H

#include "hal/hal_node_factory.h"

namespace hal_adaptor {

class hisi_factory : public hal::node_factory {
public:
    hisi_factory();
    ~hisi_factory() override;
    std::shared_ptr<hal::node> create_hal_adaptor(hal::HW_TYPE hw_type) override;
};

}
#endif
