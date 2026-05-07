#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include "gnss_parser.hpp"

GNSS_parser::GNSS_parser() {
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
    struct termios tty = {};
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);
    tty.c_cflag = CS8 | CREAD | CLOCAL;
    tcsetattr(fd, TCSANOW, &tty);
}
