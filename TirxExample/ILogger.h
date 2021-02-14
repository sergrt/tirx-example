#pragma once


#include <string>


#include "Observable.h"

class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void log_unsafe(const std::string& record) = 0;

    using log_data_istream = tirx::Observable<std::string>;

    virtual tirx::Subscription add_data_source(log_data_istream* stream) = 0;
    //virtual void remove_data_source(log_data_istream* stream) = 0;
};
