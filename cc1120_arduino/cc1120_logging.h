#ifndef CC1120_LOGGING_H
#define CC1120_LOGGING_H

#include <stdarg.h>

typedef enum {
    CC1120_LOG_LEVEL_OFF = 0,
    CC1120_LOG_LEVEL_FATAL,
    CC1120_LOG_LEVEL_ERROR,
    CC1120_LOG_LEVEL_WARN,
    CC1120_LOG_LEVEL_INFO,
    CC1120_LOG_LEVEL_DEBUG
    
} cc1120_log_level_t;

extern cc1120_log_level_t CC1120_FILE_LOG_LEVEL;
extern cc1120_log_level_t CC1120_SERIAL_LOG_LEVEL;

#endif /* CC1120_LOGGING_H */
