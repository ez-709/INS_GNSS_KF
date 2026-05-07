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

GNSS_data GNSS_parser::read() {
    if (fd < 0) return last;

    static char buf[512];
    static int buf_len = 0;

    int bytes_read = ::read(fd, buf + buf_len, sizeof(buf) - buf_len - 1);
    if (bytes_read > 0) buf_len += bytes_read;
    buf[buf_len] = '\0';

    char* current = buf;
    char* line_end;

    while ((line_end = strchr(current, '\n')) != nullptr) {
        *line_end = '\0';

        if (strncmp(current, "$GNRMC", 6) == 0) {
            char line[128];
            strncpy(line, current, 127);
            char* fields[12] = {};
            int field_count = 0;
            for (char* t = strtok(line, ",*"); t && field_count < 12; t = strtok(nullptr, ",*"))
                fields[field_count++] = t;

            if (field_count >= 9 && fields[2] && fields[2][0] == 'A') {
                double raw = atof(fields[3]);
                int deg  = (int)(raw / 100);
                last.lat = deg + (raw - deg * 100) / 60.0;
                if (fields[4][0] == 'S') last.lat = -last.lat;

                raw      = atof(fields[5]);
                deg      = (int)(raw / 100);
                last.lon = deg + (raw - deg * 100) / 60.0;
                if (fields[6][0] == 'W') last.lon = -last.lon;

                double speed   = atof(fields[7]) * 0.51444;
                double heading = atof(fields[8]) * M_PI / 180.0;
                last.VE = speed * sin(heading);
                last.VN = speed * cos(heading);
            }
        }

        if (strncmp(current, "$GNGGA", 6) == 0) {
            char line[128];
            strncpy(line, current, 127);
            char* fields[15] = {};
            int field_count = 0;
            for (char* t = strtok(line, ",*"); t && field_count < 15; t = strtok(nullptr, ",*"))
                fields[field_count++] = t;

            if (field_count >= 10 && fields[9] && fields[9][0] != '\0')
                last.alt = atof(fields[9]);
        }

        current = line_end + 1;
    }

    int remaining = buf_len - (int)(current - buf);
    if (remaining > 0) memmove(buf, current, remaining);
    buf_len = remaining;

    return last;
}