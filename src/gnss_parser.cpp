#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include "gnss_parser.hpp"

GNSS_parser::GNSS_parser() {
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
    struct termios tty = {};
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);
    tty.c_cflag  = CS8 | CREAD | CLOCAL;
    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 0;
    tcsetattr(fd, TCSANOW, &tty);
}

GNSS_data GNSS_parser::read() {
    if (fd < 0) return last;

    static char buf[4096];
    static int buf_len = 0;

    int bytes_read = ::read(fd, buf + buf_len, sizeof(buf) - buf_len - 1);
    if (bytes_read > 0) buf_len += bytes_read;
    buf[buf_len] = '\0';

    if (buf_len >= (int)sizeof(buf) - 1) buf_len = 0;

    char* current = buf;
    char* line_end;

    while ((line_end = strchr(current, '\n')) != nullptr) {
        *line_end = '\0';

        char* dollar = strchr(current, '$');
        if (dollar == nullptr) {
            current = line_end + 1;
            continue;
        }

        char* cr = strchr(dollar, '\r');
        if (cr) *cr = '\0';

        if (strncmp(dollar, "$GNRMC", 6) == 0) {
            char line[256];
            strncpy(line, dollar, 255);
            line[255] = '\0';

            char* fields[15] = {};
            int field_count = 0;
            char* saveptr = nullptr;
            char* t = strtok_r(line, ",", &saveptr);
            while (t && field_count < 15) {
                fields[field_count++] = t;
                t = strtok_r(nullptr, ",", &saveptr);
            }

            printf("GNRMC fields: count=%d f2=%s\n", field_count,
                   fields[2] ? fields[2] : "NULL");

            if (field_count >= 9 && fields[2] && fields[2][0] == 'A') {
                double raw = atof(fields[3]);
                int deg  = (int)(raw / 100);
                last.lat = deg + (raw - deg * 100) / 60.0;
                if (fields[4] && fields[4][0] == 'S') last.lat = -last.lat;

                raw      = atof(fields[5]);
                deg      = (int)(raw / 100);
                last.lon = deg + (raw - deg * 100) / 60.0;
                if (fields[6] && fields[6][0] == 'W') last.lon = -last.lon;

                double speed = atof(fields[7]) * 0.51444;
                double heading = fields[8] && fields[8][0] != '\0'
                                 ? atof(fields[8]) * M_PI / 180.0
                                 : 0.0;
                last.VE = speed * sin(heading);
                last.VN = speed * cos(heading);
                last.valid = true;
                last.fresh = true;
                printf("PARSED: lat=%.6f lon=%.6f\n", last.lat, last.lon);
            }
        }

        if (strncmp(dollar, "$GNGGA", 6) == 0) {
            char line[256];
            strncpy(line, dollar, 255);
            line[255] = '\0';

            char* fields[15] = {};
            int field_count = 0;
            char* saveptr = nullptr;
            char* t = strtok_r(line, ",", &saveptr);
            while (t && field_count < 15) {
                fields[field_count++] = t;
                t = strtok_r(nullptr, ",", &saveptr);
            }

            if (field_count >= 10 && fields[9] && fields[9][0] != '\0')
                last.alt = atof(fields[9]);
        }

        current = line_end + 1;
    }

    int remaining = buf_len - (int)(current - buf);
    if (remaining > 0) memmove(buf, current, remaining);
    else remaining = 0;
    buf_len = remaining;

    return last;
}