#ifndef TEST_FRAME_SINK_H
#define TEST_FRAME_SINK_H
#include "hal/hal_frame_sink.h"
#include "test_endpoint.h"
#include "bsp/test/file_save.h"
namespace hal_adaptor {
class test_frame_sink final : public hal::hal_frame_sink, public test_endpoint {
public:
    std::int32_t set_path(const std::string& path) override { return m_bsp->set_path(path); }
    std::int8_t init() override { return m_bsp->init(); }
    std::int8_t deinit() override { return m_bsp->deinit(); }
    std::int8_t start() override { return m_bsp->start(); }
    std::int8_t stop() override { return m_bsp->stop(); }
    std::shared_ptr<test_bsp::node> bsp_node() const override { return m_bsp; }
    test_bsp::file_statistics statistics() const { return m_bsp->statistics(); }
private:
    std::shared_ptr<test_bsp::file_save> m_bsp=std::make_shared<test_bsp::file_save>();
};
}
#endif
