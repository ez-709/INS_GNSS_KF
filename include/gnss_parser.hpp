#pragma once

struct GNSS_data {
    double lon, lot, alt;
    double VE, VN, Vh;
    double timestamp;
};

class GNSS_parser {
public:
    GNSS_parser();
    GNSS_data read();
private:
    int fd = -1;
    GNSS_data last = {};
};