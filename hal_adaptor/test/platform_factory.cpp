#include "hal/hal_node_factory.h"
#include "test_factory.h"

namespace hal {

// 只有选中 test 时，此入口才会参与编译和链接。
std::shared_ptr<node_factory> create_factory()
{
    return std::make_shared<hal_adaptor::test_factory>();
}

}
