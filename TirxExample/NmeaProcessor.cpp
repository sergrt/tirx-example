#include "NmeaProcessor.h"

#include "Common.h"

#include <sstream>

namespace {

std::vector<std::string> split(const std::string& src) {
    std::vector<std::string> strings;
    std::istringstream f(src);
    std::string s;
    while (getline(f, s, ',')) {
        strings.push_back(s);
    }
    return strings;
}

}

INmeaProcessor::pos_data_observable* NmeaProcessor::get_pos_data_stream() {
    return &gps_data_observable_;
}

INmeaProcessor::sentences_observable* NmeaProcessor::get_sentences_stream() {
    return &sentences_data_observable_;
}


NmeaProcessor::NmeaProcessor() {
    std::cout << "NmeaProcessor()\n";
}

NmeaProcessor::~NmeaProcessor() {
    std::cout << "~NmeaProcessor()\n";
    raw_data_stream_subscription_.unsubscribe();
}

void NmeaProcessor::initialize() {
    std::cout << "NmeaProcessor initialize()" << "\n";

    nmea_source_ = findObject<INmeaSource>("NmeaSource");
    if (!nmea_source_)
        return;

    auto observable = nmea_source_->get_sentences_stream();
    raw_data_stream_subscription_ = observable->subscribe([this](std::string sentence) {
        sentences_data_observable_.next(sentence);
        const auto source = sentence.substr(3, 3);

        if (source == "GGA") {
            const auto parts = split(sentence);
            auto lat = std::make_pair(atof(parts[2].c_str()), parts[3]);
            auto lon = std::make_pair(atof(parts[4].c_str()), parts[5]);
            gps_data_observable_.next(std::move(lat), std::move(lon));
        }
    });
}

void NmeaProcessor::uninitialize() {
    std::cout << "NmeaProcessor uninitialize()" << "\n";
}