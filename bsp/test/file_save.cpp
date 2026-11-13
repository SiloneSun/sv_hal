#include "file_save.h"
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
namespace test_bsp {
file_save::~file_save() { stop(); }
int file_save::set_path(const std::string& path)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_fd!=-1 || path.empty()) { return -4; }
    m_path=path;
    return 0;
}
int file_save::init()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_initialized=true;
    return 0;
}
int file_save::deinit()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_fd!=-1) { return -4; }
    m_initialized=false;
    return 0;
}
int file_save::start()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_accepting) { return 0; }
    if (!m_initialized || m_fd!=-1) { return -4; }
    m_stats={};
    m_fd=::open(m_path.c_str(),O_WRONLY|O_CREAT|O_TRUNC,0666);
    if (m_fd<0) {
        m_stats.faulted=true;
        std::cerr << "[file_save] open " << m_path << ": " << std::strerror(errno) << '\n';
        return -6;
    }
    m_accepting=true;
    try { m_thread=std::thread(&file_save::consume,this); }
    catch (...) { m_accepting=false; ::close(m_fd); m_fd=-1; return -6; }
    return 0;
}
bool file_save::data_callback(const frame& value)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_accepting || !value.buffer) { return false; }
    if (m_queue.size()>=queue_capacity) { ++m_stats.dropped; return false; }
    m_queue.push_back(value);
    ++m_stats.accepted;
    m_stats.peak_queued=std::max(m_stats.peak_queued,m_queue.size());
    m_ready.notify_one();
    return true;
}
void file_save::consume()
{
    while (true) {
        frame value;
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_ready.wait(lock,[&]{return !m_accepting || !m_queue.empty();});
            if (m_queue.empty() && !m_accepting) { break; }
            value=std::move(m_queue.front()); m_queue.pop_front();
        }
        std::shared_lock<std::shared_mutex> access(value.buffer->access_mutex);
        const auto& bytes=value.buffer->bytes;
        std::size_t offset=0;
        int error=0;
        while (offset<bytes.size()) {
            auto count=::write(m_fd,bytes.data()+offset,bytes.size()-offset);
            if (count<0 && errno==EINTR) { continue; }
            if (count<=0) { error=count<0 ? errno : EIO; break; }
            offset+=static_cast<std::size_t>(count);
        }
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (error) {
                ++m_stats.failed;
                if (!m_stats.faulted) {
                    std::cerr << "[file_save] write " << m_path << ": " << std::strerror(error)
                              << "; pipeline remains running, failed frames are discarded\n";
                }
                m_stats.faulted=true;
            } else { ++m_stats.written; }
        }
    }
    return;
}
int file_save::stop()
{
    { std::lock_guard<std::mutex> lock(m_mutex); m_accepting=false; }
    m_ready.notify_all();
    if (m_thread.joinable()) { m_thread.join(); }
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_fd!=-1) {
        if (::close(m_fd)!=0) {
            m_stats.faulted=true;
            std::cerr << "[file_save] close " << m_path << ": " << std::strerror(errno) << '\n';
        }
        m_fd=-1;
    }
    return 0;
}
file_statistics file_save::statistics() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto result=m_stats; result.queued=m_queue.size();
    return result;
}
}
