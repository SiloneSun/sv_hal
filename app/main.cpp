#include <iostream>
#include <string>


#include "hal_base_node.h"
#include "hal_pipeline.h"

using namespace std;


int main()
{
    std::shared_ptr<hal::pipeline> pipeline_ptr = std::make_shared<hal::pipeline>();
    return 0; 
}