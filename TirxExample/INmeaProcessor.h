#pragma once

#include "Observable.h"

class INmeaProcessor {
public:
    virtual ~INmeaProcessor() = default;
    using pos_data_observable = tirx::Observable<std::pair<double, std::string>, std::pair<double, std::string>>;
    virtual pos_data_observable* get_pos_data_stream() = 0;

    using sentences_observable = tirx::Observable<std::string>;
    virtual sentences_observable* get_sentences_stream() = 0;
};
