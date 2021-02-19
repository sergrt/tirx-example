#include "Logger.h"

#include "Common.h"
#include "INmeaSource.h"
#include "ThreadPoolExecutor.h"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <algorithm>

namespace {

std::string format_log_record(const std::string& record) {
    const auto now = std::chrono::system_clock::now();
    const time_t t_c = std::chrono::system_clock::to_time_t(now);

    std::stringstream sstream;
    struct tm t;
    localtime_s(&t, &t_c);
    sstream << "[" << std::put_time(&t, "%Y%m%d %T")
        << "] [" << std::this_thread::get_id() << "] " << record << "\n";
    return sstream.str();
}

}

Logger::~Logger() {
    nmea_subscription_.unsubscribe();
    for (auto& s : active_subscriptions_)
        s.unsubscribe();
    try {
        if (file_.is_open())
            file_.close();
    } catch (...) {
        // unable to close file
    }
}

void Logger::initialize() {
    std::cout << "Logger initialize()" << "\n";

    file_.open("_log.txt");
    log_unsafe(format_log_record("Logger creation thread"));

    if (auto nmea_source_ = findObject<INmeaSource>("NmeaSource")) {
        auto observable = nmea_source_->get_sentences_stream();

        nmea_subscription_ = observable->subscribe_on(log_thread_executor_).subscribe([this](std::string sentence) {
            log_unsafe(format_log_record(sentence));
        });
    }
}

void Logger::uninitialize() {
    std::cout << "Logger uninitialize()" << "\n";
}

void Logger::log_unsafe(const std::string& record) {
    if (file_.is_open()) {
        file_.write(record.c_str(), record.size());
    }

    for (size_t i = 0; i < record.size(); ++i) {
        std::cout << record[i];
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

tirx::Subscription Logger::add_data_source(log_data_istream* stream) {
    auto func = [this](std::string log_str) {
        log_unsafe(format_log_record(log_str));
    };

    auto map_func = [this](std::string log_str) {
        std::transform(log_str.begin(), log_str.end(), log_str.begin(), ::toupper);
        log_str = ">map< " + log_str;
        log_unsafe(format_log_record("_inside map_ " + log_str));
        return log_str;
    };

    //auto& x = stream->map(map_func);
    //stream->map(map_func);
    auto subscription = stream->subscribe_on(map_thread_executor_).map(map_func).subscribe_on(log_thread_executor_).subscribe(func);
    active_subscriptions_.push_back(subscription);
    return active_subscriptions_.back();
}
