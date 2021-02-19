#pragma once

#include "ILoadable.h"
#include "ILogger.h"

#include "SingleThreadExecutor.h"

#include <fstream>

class Logger : public ILogger, public ILoadable {
public:
    ~Logger() override;

    void initialize() override;
    void uninitialize() override;

    void log_unsafe(const std::string& record) override;

    tirx::Subscription add_data_source(log_data_istream* stream) override;

private:
    std::ofstream file_;

    std::shared_ptr<tirx::SingleThreadExecutor> log_thread_executor_ = std::make_shared<tirx::SingleThreadExecutor>();
    std::shared_ptr<tirx::SingleThreadExecutor> map_thread_executor_ = std::make_shared<tirx::SingleThreadExecutor>();

    tirx::Subscription nmea_subscription_;
    std::vector<tirx::Subscription> active_subscriptions_;
};
