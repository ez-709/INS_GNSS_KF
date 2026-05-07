#pragma once

struct GNSS_data {
    double lon, lat, alt;
    double VE, VN, Vh;
};

class GNSS_parser {
public:
    GNSS_parser();
    GNSS_data read();
private:
    int fd = -1;
    GNSS_data last = {};
};