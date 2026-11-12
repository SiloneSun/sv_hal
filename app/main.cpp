#include "demo.h"
#include "hal/hal_node_factory.h"
#include <iostream>

int main()
{
    // 平台由 CMake 选择，App 只调用 HAL 公共入口。
    auto factory = hal::create_factory();
    if (!factory) {
        std::cerr << "Failed to create platform factory.\n";
        return 1;
    }

    return run_demo(*factory);
}
