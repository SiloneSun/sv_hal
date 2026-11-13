#ifndef BSP_TEST_FILE_SAVE_H
#define BSP_TEST_FILE_SAVE_H
#include "node.h"
#include <condition_variable>
#include <deque>
#include <string>
#include <thread>
namespace test_bsp {
struct file_statistics {
    std::uint64_t accepted=0,written=0,dropped=0,failed=0;
    std::size_t queued=0,peak_queued=0;
    bool faulted=false;
};
class file_save final : public node {
public:
    static constexpr std::size_t queue_capacity=15;
    file_save() : node(true,false) {}
    ~file_save() override;
    int set_path(const std::string& path);
    int init() override;
    int deinit() override;
    int start() override;
    int stop() override;
    bool data_callback(const frame& value) override;
    file_statistics statistics() const;
private:
    void consume();
    mutable std::mutex m_mutex;
    std::condition_variable m_ready;
    std::deque<frame> m_queue;
    std::thread m_thread;
    std::string m_path="capture.yuv";
    int m_fd=-1;
    bool m_initialized=false,m_accepting=false;
    file_statistics m_stats;
};
}
#endif
