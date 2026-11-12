#ifndef SIGMA_308_FACTORY_H
#define SIGMA_308_FACTORY_H



#include <stdlib.h>
#include <memory>
#include "hal_node_factory.h"
namespace hal {

    class sigma_308_factory : public node_factory {
        public:
            sigma_308_factory() = delete;
            sigma_308_factory(SOC_TYPE soc_type);
            ~sigma_308_factory();
            virtual std::shared_ptr<base_node> create_hal_adaptor(HW_TYPE hw_type) override;
    };
}

#endif