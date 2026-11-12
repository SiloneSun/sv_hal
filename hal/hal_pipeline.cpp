#include "hal_pipeline.h"

using namespace hal;

pipeline::pipeline()
{
}

pipeline::~pipeline()
{
}

int8_t pipeline::init()
{
    return 0;
}

int8_t pipeline::deinit()
{
    return 0;
}

std::int8_t pipeline::start(){
    return 0;
}
std::int8_t pipeline::stop(){
    return 0;
}
std::int8_t pipeline::add_node(std::string name, std::shared_ptr<hal::node> node)
{
    return 0;
}
std::int8_t pipeline::remove_node(std::string name)
{
    return 0;
}
std::shared_ptr<hal::node> pipeline::get_node(std::string name)
{
    return nullptr;
}