#include "NmeaSource.h"

NmeaSource::NmeaSource() {
    std::cout << "NmeaSource()\n";
    current_sentence_index_ = 0;
    nmea_sentences_ = {
        "$GPGGA,194546.127,5231.525,N,01323.391,E,1,12,1.0,0.0,M,0.0,M,,*6E",
        "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30",
        "$GPRMC,194546.127,A,5231.525,N,01323.391,E,2372.1,093.7,200220,000.0,W*40",
        "$GPGGA,194547.127,5231.456,N,01324.467,E,1,12,1.0,0.0,M,0.0,M,,*63",
        "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30",
        "$GPRMC,194547.127,A,5231.456,N,01324.467,E,3968.7,122.8,200220,000.0,W*4F",
        "$GPGGA,194548.127,5230.657,N,01325.713,E,1,12,1.0,0.0,M,0.0,M,,*6F",
        "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30",
        "$GPRMC,194548.127,A,5230.657,N,01325.713,E,3968.7,122.8,200220,000.0,W*43",
    };

    active_ = true;
    thread_ = std::thread([this]() {
        while (active_) {
            nmea_sentences_stream_.next(get_next_message());
            std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        }
    });
}

NmeaSource::~NmeaSource() {
    std::cout << "~NmeaSource()\n";
    active_ = false;
    thread_.join();
}

std::string NmeaSource::get_next_message() const {
    auto sentence = nmea_sentences_[current_sentence_index_];
    if (++current_sentence_index_ >= nmea_sentences_.size())
        current_sentence_index_ = 0;
    return sentence;
}

tirx::Observable<std::string>* NmeaSource::get_sentences_stream() {
    return &nmea_sentences_stream_;
}

void NmeaSource::initialize() {
    std::cout << "NmeaSource initialize()" << "\n";
}

void NmeaSource::uninitialize() {
    std::cout << "NmeaSource uninitialize()" << "\n";
}

