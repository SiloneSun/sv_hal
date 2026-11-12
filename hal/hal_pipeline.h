#ifndef HAL_PIPELINE_H
#define HAL_PIPELINE_H
#include <cstdint>
#include <memory>
#include "hal_base_node.h"


namespace hal {
    class pipeline
    { 
    public:
        pipeline(); 
        ~pipeline(); 
        std::int8_t init(); 
        std::int8_t deinit();
        std::int8_t start();
        std::int8_t stop();
        std::int8_t add_node(std::string name, std::shared_ptr<hal::node> node);
        std::int8_t remove_node(std::string name);
        std::shared_ptr<hal::node> get_node(std::string name);
    }; 
};
#endif