#pragma once

struct GNSS_data {
    double lon = 0.0, lat = 0.0, alt = 0.0;
    double VE = 0.0, VN = 0.0, Vh = 0.0;
    double timestamp = 0.0;
    bool valid = false;
    bool fresh = false;
};

class GNSS_parser {
public:
    GNSS_parser();
    GNSS_data read();
private:
    int fd = -1;
    GNSS_data last = {};
};