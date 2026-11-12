#ifndef HAL_BASE_NODE_H
#define HAL_BASE_NODE_H
#include <stdlib.h>


using namespace std;

namespace hal { 

    class base_node {
    public:
        base_node();
        virtual ~base_node();

        virtual int8_t init() = 0;
        virtual int8_t deinit() = 0;

        
    };
}
#endif