#ifndef HAL_PIPELINE_H
#define HAL_PIPELINE_H
#include "hal_base_node.h"
#include <map>
#include <string>
#include <vector>
namespace hal {
// 控制操作由调用方串行执行；不转发数据，不管理图像缓冲。
class pipeline {
public:
    pipeline() = default;
    ~pipeline();
    pipeline(const pipeline&) = delete;
    pipeline& operator=(const pipeline&) = delete;
    std::int8_t init();
    std::int8_t deinit();
    std::int8_t start();
    std::int8_t stop();
    std::int8_t pause();
    std::int8_t resume();
    std::int8_t add_node(std::string name, std::shared_ptr<node> value);
    std::int8_t remove_node(std::string name);
    std::shared_ptr<node> get_node(std::string name);
    std::int8_t connect(hal_node_port_out* out, hal_node_port_in* in);
    std::int8_t disconnect(hal_node_port_out* out, hal_node_port_in* in);
private:
    struct edge { hal_node_port_out* out; hal_node_port_in* in; };
    std::vector<node*> order() const;
    std::map<std::string, std::shared_ptr<node>> m_nodes;
    std::vector<edge> m_edges;
    bool m_initialized = false;
    bool m_running = false;
};
}
#endif
