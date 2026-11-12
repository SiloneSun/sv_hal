#ifndef HAL_PIPELINE_H
#define HAL_PIPELINE_H
#include <cstdint>
#include <memory>


namespace hal {
    class pipeline
    { 
    public:
        pipeline(); 
        ~pipeline(); 
        std::int8_t init(); 
        std::int8_t deinit(); 
    }; 
};
#endif