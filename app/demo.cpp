#include "demo.h"
#include "hal/hal_pipeline.h"
#include <iostream>

int run_demo(hal::node_factory& factory)
{
    auto pipeline = std::make_shared<hal::pipeline>();
    if (pipeline->init() != 0) {
        return 1;
    }

    // 此处只演示通过 HAL 接口请求节点，尚不搭建真实管线。
    auto sensor = factory.create_hal_adaptor(hal::HW_TYPE_SENSOR);
    if (!sensor) {
        std::cout << "Sensor adaptor is not implemented yet; "
                     "only the factory handoff is demonstrated.\n";
    }

    return pipeline->deinit() == 0 ? 0 : 1;
}
