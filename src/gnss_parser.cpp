#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include "gnss_parser.hpp"

GNSS_data GNSS_parser::read() {
    GNSS_data data;
    data.lon = 0;
    data.lot = 0;
    data.alt = 0;
    data.VE  = 0;
    data.VN  = 0;
    data.Vh  = 0;
    return data;
}