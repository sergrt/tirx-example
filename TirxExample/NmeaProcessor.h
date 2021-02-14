#pragma once

#include "INmeaSource.h"
#include "ILoadable.h"
#include "INmeaProcessor.h"

#include "Observable.h"

#include <memory>

class NmeaProcessor : public INmeaProcessor, public ILoadable {
public:
    NmeaProcessor();
    ~NmeaProcessor() override;
    void initialize() override;
    void uninitialize() override;

    pos_data_observable* get_pos_data_stream() override;
    sentences_observable* get_sentences_stream() override;

private:
    std::shared_ptr<INmeaSource> nmea_source_;
    pos_data_observable gps_data_observable_;
    sentences_observable sentences_data_observable_;
    tirx::Subscription raw_data_stream_subscription_;
};
