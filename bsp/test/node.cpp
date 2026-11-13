#include "node.h"
#include <algorithm>
namespace test_bsp {
int node::connect_next(const std::shared_ptr<node>& next)
{
    if (!m_output || !next || !next->m_input || next.get()==this) { return -2; }
    std::lock_guard<std::mutex> lock(m_links_mutex);
    for (auto& link:m_next) { if (link.lock()==next) { return -2; } }
    m_next.push_back(next);
    return 0;
}
int node::disconnect_next(const std::shared_ptr<node>& next)
{
    std::lock_guard<std::mutex> lock(m_links_mutex);
    auto it=std::find_if(m_next.begin(),m_next.end(),[&](const auto& link){return link.lock()==next;});
    if (it==m_next.end()) { return -2; }
    m_next.erase(it);
    return 0;
}
void node::dispatch_frame(const frame& value)
{
    std::vector<std::shared_ptr<node>> receivers;
    {
        std::lock_guard<std::mutex> lock(m_links_mutex);
        for (auto& link:m_next) { if (auto next=link.lock()) { receivers.push_back(next); } }
    }
    for (auto& next:receivers) { next->data_callback(value); }
    return;
}
}
