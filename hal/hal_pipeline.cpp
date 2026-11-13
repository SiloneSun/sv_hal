#include "hal_pipeline.h"
#include <algorithm>
namespace hal {
pipeline::~pipeline()
{
    stop();
    while (!m_edges.empty()) {
        auto edge = m_edges.back();
        if (disconnect(edge.out,edge.in) != 0) { break; }
    }
    deinit();
    for (auto& item : m_nodes) { item.second->m_pipeline = nullptr; }
}
std::int8_t pipeline::add_node(std::string name, std::shared_ptr<node> value)
{
    if (m_initialized || !value || name.empty() || m_nodes.count(name) || value->m_pipeline) {
        return CONTROL_INVALID_ARGUMENT;
    }
    m_nodes.emplace(std::move(name),value);
    value->m_pipeline = this;
    return 0;
}
std::int8_t pipeline::remove_node(std::string name)
{
    auto it=m_nodes.find(name);
    if (m_initialized || it==m_nodes.end()) { return CONTROL_INVALID_STATE; }
    it->second->m_pipeline=nullptr;
    m_nodes.erase(it);
    return 0;
}
std::shared_ptr<node> pipeline::get_node(std::string name)
{
    auto it=m_nodes.find(name);
    return it==m_nodes.end() ? nullptr : it->second;
}
std::vector<node*> pipeline::order() const
{
    std::map<node*,unsigned> indegree;
    for (const auto& item:m_nodes) { indegree[item.second.get()]=0; }
    for (const auto& e:m_edges) { ++indegree[e.in->owner()]; }
    std::vector<node*> ready,result;
    for (const auto& item:indegree) { if (!item.second) { ready.push_back(item.first); } }
    while (!ready.empty()) {
        auto* current=ready.back(); ready.pop_back(); result.push_back(current);
        for (const auto& e:m_edges) {
            if (e.out->owner()==current && --indegree[e.in->owner()]==0) {
                ready.push_back(e.in->owner());
            }
        }
    }
    return result;
}
std::int8_t pipeline::init()
{
    if (m_initialized) { return 0; }
    std::vector<node*> done;
    for (auto* value:order()) {
        auto status=value->init();
        if (status!=0) {
            for (auto it=done.rbegin();it!=done.rend();++it) { (*it)->deinit(); }
            return status;
        }
        done.push_back(value);
    }
    m_initialized=true;
    return 0;
}
std::int8_t pipeline::deinit()
{
    if (m_running || !m_edges.empty()) { return CONTROL_INVALID_STATE; }
    if (!m_initialized) { return 0; }
    std::int8_t result=0;
    for (auto* value:order()) {
        auto status=value->deinit(); if (status!=0) { result=status; }
    }
    if (!result) { m_initialized=false; }
    return result;
}
std::int8_t pipeline::connect(hal_node_port_out* out, hal_node_port_in* in)
{
    if (!m_initialized || m_running) { return CONTROL_INVALID_STATE; }
    if (!out || !in || !out->owner() || !in->owner() ||
        out->owner()->get_outport(out->index())!=out ||
        in->owner()->get_inport(in->index())!=in ||
        out->owner()->m_pipeline!=this || in->owner()->m_pipeline!=this ||
        out->owner()==in->owner()) { return CONTROL_INVALID_ARGUMENT; }
    for (const auto& e:m_edges) {
        if (e.in==in) { return CONTROL_INVALID_ARGUMENT; }
    }
    // 先验证拓扑，再执行底层绑定；失败不留下逻辑连接。
    m_edges.push_back({out,in});
    if (order().size()!=m_nodes.size()) { m_edges.pop_back(); return CONTROL_INVALID_ARGUMENT; }
    auto status=out->owner()->connect_next(*in->owner(),out->index(),in->index());
    if (status!=0) { m_edges.pop_back(); }
    return status;
}
std::int8_t pipeline::disconnect(hal_node_port_out* out, hal_node_port_in* in)
{
    if (m_running) { return CONTROL_INVALID_STATE; }
    auto it=std::find_if(m_edges.begin(),m_edges.end(),[&](const edge& e){return e.out==out && e.in==in;});
    if (it==m_edges.end()) { return CONTROL_INVALID_ARGUMENT; }
    auto status=out->owner()->disconnect_next(*in->owner(),out->index(),in->index());
    if (!status) { m_edges.erase(it); }
    return status;
}
std::int8_t pipeline::start()
{
    if (m_running) { return 0; }
    if (!m_initialized) { return CONTROL_INVALID_STATE; }
    auto nodes=order();
    std::vector<node*> started;
    // 先准备下游，再启动上游生产者；共享节点只执行一次。
    for (auto it=nodes.rbegin();it!=nodes.rend();++it) {
        auto status=(*it)->start();
        if (status!=0) {
            for (auto rollback=started.rbegin();rollback!=started.rend();++rollback) { (*rollback)->stop(); }
            return status;
        }
        started.push_back(*it);
    }
    m_running=true;
    return 0;
}
std::int8_t pipeline::stop()
{
    if (!m_running) { return 0; }
    std::int8_t result=0;
    // 上游先退出并 join；下游随后排空队列。
    for (auto* value:order()) {
        auto status=value->stop(); if (status!=0) { result=status; }
    }
    if (!result) { m_running=false; }
    return result;
}
std::int8_t pipeline::pause() { return CONTROL_UNSUPPORTED; }
std::int8_t pipeline::resume() { return CONTROL_UNSUPPORTED; }
}
