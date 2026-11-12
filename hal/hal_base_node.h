#ifndef HAL_BASE_NODE_H
#define HAL_BASE_NODE_H
#include <cstdint>



namespace hal { 

    class base_node {
    public:
        base_node();
        virtual ~base_node();

        virtual std::int8_t init() = 0;
        virtual std::int8_t deinit() = 0;

        
    };
}
#endif