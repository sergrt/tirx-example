#pragma once

#include "INmeaSource.h"
#include "ILoadable.h"

#include <thread>

class NmeaSource : public INmeaSource, public ILoadable {
public:
    NmeaSource();
    ~NmeaSource() override;

    void initialize() override;
    void uninitialize() override;

    std::string get_next_message() const override;
    tirx::Observable<std::string>* get_sentences_stream() override;

private:
    std::thread thread_;
    std::atomic<bool> active_{false};

    tirx::Observable<std::string> nmea_sentences_stream_;

    std::vector<std::string> nmea_sentences_;
    mutable size_t current_sentence_index_ = 0;
};
