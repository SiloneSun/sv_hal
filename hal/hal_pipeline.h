#ifndef HAL_PIPELINE_H
#define HAL_PIPELINE_H
#include <stdlib.h>
#include <memory>


namespace hal {
    class pipeline
    { 
    public:
        pipeline(); 
        ~pipeline(); 
        int8_t init(); 
        int8_t deinit(); 
    }; 
};
#endif