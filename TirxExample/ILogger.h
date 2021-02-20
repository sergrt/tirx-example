#pragma once

#include "tirx.h"

#include <string>

class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void log_unsafe(const std::string& record) = 0;

    using log_data_istream = tirx::Observable<std::string>;
    virtual tirx::Subscription add_data_source(log_data_istream* stream) = 0;
};
