#ifndef HAL_APP_DEMO_H
#define HAL_APP_DEMO_H

#include "hal/hal_node_factory.h"

// 业务入口只接受 HAL 工厂，不接触平台枚举和具体适配类。
int run_demo(hal::node_factory& factory);

#endif
