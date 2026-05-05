#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include "gnss_parser.hpp"

GNSS_parser::GNSS_parser() {
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) return;
    struct termios tty = {};
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);
    tty.c_cflag = CS8 | CREAD | CLOCAL;
    tcsetattr(fd, TCSANOW, &tty);
}

GNSS_data GNSS_parser::read() {
    if (fd < 0) return last;

    static char buf[256];
    static int  len = 0;

    int n = ::read(fd, buf + len, sizeof(buf) - len - 1);
    if (n > 0) len += n;
    buf[len] = '\0';

    char* nl;
    char* p = buf;
    while ((nl = strchr(p, '\n')) != nullptr) {
        *nl = '\0';
        if (strncmp(p, "$GNRMC", 6) == 0 || strncmp(p, "$GPRMC", 6) == 0) {
            char tmp[128]; strncpy(tmp, p, 127);
            char* f[12] = {}; int fi = 0;
            for (char* t = strtok(tmp, ",*"); t && fi < 12; t = strtok(nullptr, ",*"))
                f[fi++] = t;
            if (fi >= 9 && f[2][0] == 'A') {
                auto nmea2deg = [](const char* s) {
                    double r = atof(s);
                    int d = (int)(r / 100);
                    return d + (r - d * 100) / 60.0;
                };
                last.lot = nmea2deg(f[3]); if (f[4][0]=='S') last.lot=-last.lot;
                last.lon = nmea2deg(f[5]); if (f[6][0]=='W') last.lon=-last.lon;
                double spd = atof(f[7]) * 0.51444;
                double hdg = atof(f[8]) * M_PI / 180.0;
                last.VE = spd * sin(hdg);
                last.VN = spd * cos(hdg);
            }
        }
        p = nl + 1;
    }
    int rem = len - (int)(p - buf);
    if (rem > 0) memmove(buf, p, rem);
    len = rem;

    return last;
}