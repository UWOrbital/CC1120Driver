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

typedef enum  {
  CC1120_ERROR_CODE_SUCCESS = 0,
  CC1120_ERROR_CODE_READ_SPI_INVALID_REGISTER,
  CC1120_ERROR_CODE_READ_SPI_INVALID_LENGTH,
  CC1120_ERROR_CODE_READ_SPI_INVALID_STATUS_BYTE,
  CC1120_ERROR_CODE_READ_EXT_ADDR_SPI_INVALID_REGISTER,
  CC1120_ERROR_CODE_READ_EXT_ADDR_SPI_INVALID_LENGTH,
  CC1120_ERROR_CODE_READ_EXT_ADDR_SPI_INVALID_STATUS_BYTE,
  CC1120_ERROR_CODE_READ_EXT_ADDR_SPI_FAILED,
  CC1120_ERROR_CODE_WRITE_SPI_INVALID_REGISTER,
  CC1120_ERROR_CODE_WRITE_SPI_INVALID_LENGTH,
  CC1120_ERROR_CODE_WRITE_SPI_INVALID_STATUS_BYTE_HEADER,
  CC1120_ERROR_CODE_WRITE_SPI_INVALID_STATUS_BYTE_DATA,
  CC1120_ERROR_CODE_WRITE_EXT_ADDR_SPI_INVALID_REGISTER,
  CC1120_ERROR_CODE_WRITE_EXT_ADDR_SPI_INVALID_LENGTH,
  CC1120_ERROR_CODE_WRITE_EXT_ADDR_SPI_INVALID_STATUS_BYTE_HEADER,
  CC1120_ERROR_CODE_WRITE_EXT_ADDR_SPI_FAILED,
  CC1120_ERROR_CODE_WRITE_EXT_ADDR_SPI_INVALID_STATUS_BYTE_DATA,
  CC1120_ERROR_CODE_STROBE_SPI_INVALID_REGISTER,
  CC1120_ERROR_CODE_STROBE_SPI_INVALID_STATUS_BYTE,
  CC1120_ERROR_CODE_READ_FIFO_INVALID_LENGTH,
  CC1120_ERROR_CODE_READ_FIFO_INVALID_STATUS_BYTE,
  CC1120_ERROR_CODE_WRITE_FIFO_INVALID_LENGTH,
  CC1120_ERROR_CODE_WRITE_FIFO_INVALID_STATUS_BYTE,
  CC1120_ERROR_CODE_READ_FIFO_DIRECT_INVALID_REGISTER,
  CC1120_ERROR_CODE_READ_FIFO_DIRECT_INVALID_LENGTH,
  CC1120_ERROR_CODE_READ_FIFO_DIRECT_INVALID_STATUS_BYTE,
  CC1120_ERROR_CODE_WRITE_FIFO_DIRECT_INVALID_RGISTER,
  CC1120_ERROR_CODE_WRITE_FIFO_DIRECT_INVALID_LENGTH,
  CC1120_ERROR_CODE_WRITE_FIFO_DIRECT_INVALID_STATUS_BYTE,
  CC1120_ERROR_CODE_SEND_BYTE_RECEIVE_STATUS_INVALID_STATUS_BYTE,
  CC1120_ERROR_CODE_TEST_SPI_READ_INCORRECT_BURST_READ,
  CC1120_ERROR_CODE_TEST_SPI_READ_INCORRECT_SINGLE_EXT_ADDR_READ,
  CC1120_ERROR_CODE_TEST_SPI_READ_INCORRECT_BURST_EXT_ADDR_READ,
  CC1120_ERROR_CODE_TEST_SPI_WRITE_INCORRECT_SINGLE_ADDR_WRITE,
  CC1120_ERROR_CODE_TEST_SPI_WRITE_INCORRECT_BURST_ADDR_WRITE,
  CC1120_ERROR_CODE_TEST_SPI_WRITE_INCORRECT_SINGLE_EXT_ADDR_WRITE,
  CC1120_ERROR_CODE_TEST_SPI_WRITE_INCORRECT_BURST_EXT_ADDR_WRITE,
  CC1120_ERROR_CODE_TEST_SPI_STROBE_INCORRECT_MARCSTATE_READ,
  CC1120_ERROR_CODE_TEST_FIFO_READ_WRITE_SINGLE_WRITE_DIRECT_READ_FAILED,
  CC1120_ERROR_CODE_TEST_FIFO_READ_WRITE_BURST_WRITE_DIRECT_READ_FAILED,
  CC1120_ERROR_CODE_TEST_FIFO_READ_WRITE_SINGLE_DIRECT_WRITE_FAILED,
  CC1120_ERROR_CODE_TEST_FIFO_READ_WRITE_BURST_DIRECT_WRITE_FAILED
  
} cc1120_status_code;

#endif /* CC1120_LOGGING_H */
