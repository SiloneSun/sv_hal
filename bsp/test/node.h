#ifndef TEST_BSP_NODE_H
#define TEST_BSP_NODE_H
#include <cstdint>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <vector>
namespace test_bsp {
// 仅 BSP 使用。已发布帧只读；生产者须等全部消费者释放后才可改写。
struct frame_buffer {
    explicit frame_buffer(std::size_t bytes) : bytes(bytes) {}
    // 引用计数决定是否仍在使用；读写锁补充跨线程内存访问的同步。
    mutable std::shared_mutex access_mutex;
    std::vector<std::uint8_t> bytes;
};
struct frame {
    std::shared_ptr<const frame_buffer> buffer;
    std::uint64_t sequence=0;
    std::uint64_t timestamp_us=0;
};
// 测试软件后端专用。其他平台可以继续使用各自 element 框架。
class node {
public:
    node(bool input, bool output) : m_input(input),m_output(output) {}
    virtual ~node() = default;
    virtual int init() = 0;
    virtual int deinit() = 0;
    virtual int start() = 0;
    virtual int stop() = 0;
    virtual bool data_callback(const frame&) { return false; }
    int connect_next(const std::shared_ptr<node>& next);
    int disconnect_next(const std::shared_ptr<node>& next);
protected:
    void dispatch_frame(const frame& value);
private:
    bool m_input,m_output;
    std::mutex m_links_mutex;
    std::vector<std::weak_ptr<node>> m_next;
};
}
#endif
