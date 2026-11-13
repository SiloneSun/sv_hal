#ifndef TEST_ADAPTOR_ENDPOINT_H
#define TEST_ADAPTOR_ENDPOINT_H
#include "bsp/test/node.h"
namespace hal_adaptor {
class test_endpoint {
public:
    virtual ~test_endpoint() = default;
    virtual std::shared_ptr<test_bsp::node> bsp_node() const = 0;
};
}
#endif
