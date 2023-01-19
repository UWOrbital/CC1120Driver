#include "cc1120_txrx.h"
#include "cc1120_logging.h"
#include "cc1120_mcu.h"
#include "cc1120_spi.h"
#include <stdbool.h>
#define min(a,b) (a < b ? a : b)

registerSetting_t txSettingsStd[] = {
    { CC1120_REGS_IOCFG3,            0xB0U },
    { CC1120_REGS_IOCFG2,            0x06U },
    { CC1120_REGS_IOCFG1,            0xB0U },
    { CC1120_REGS_IOCFG0,            0x40U },
    { CC1120_REGS_SYNC_CFG1,         0x08U },
    { CC1120_REGS_DEVIATION_M,       0x3AU },
    { CC1120_REGS_MODCFG_DEV_E,      0x0AU },
    { CC1120_REGS_DCFILT_CFG,        0x1CU },
    { CC1120_REGS_PREAMBLE_CFG1,     0x18U },
    { CC1120_REGS_IQIC,              0xC6U },
    { CC1120_REGS_CHAN_BW,           0x08U },
    { CC1120_REGS_MDMCFG0,           0x05U },
    { CC1120_REGS_SYMBOL_RATE2,      0x73U },
    { CC1120_REGS_AGC_REF,           0x20U },
    { CC1120_REGS_AGC_CS_THR,        0x19U },
    { CC1120_REGS_AGC_CFG1,          0xA9U },
    { CC1120_REGS_AGC_CFG0,          0xCFU },
    { CC1120_REGS_FIFO_CFG,          0x00U },
    { CC1120_REGS_FS_CFG,            0x14U },
    { CC1120_REGS_PKT_CFG0,          0x00U },
    { CC1120_REGS_PA_CFG0,           0x7DU },
    { CC1120_REGS_PKT_LEN,           0x0CU }
};

registerSetting_t txSettingsExt[] = {
    { CC1120_REGS_EXT_IF_MIX_CFG,    0x00U },
    { CC1120_REGS_EXT_FREQOFF_CFG,   0x22U },
    { CC1120_REGS_EXT_FREQ2,         0x6CU },
    { CC1120_REGS_EXT_FREQ1,         0x7AU },
    { CC1120_REGS_EXT_FREQ0,         0xE1U },
    { CC1120_REGS_EXT_FS_DIG1,       0x00U },
    { CC1120_REGS_EXT_FS_DIG0,       0x5FU },
    { CC1120_REGS_EXT_FS_CAL1,       0x40U },
    { CC1120_REGS_EXT_FS_CAL0,       0x0EU },
    { CC1120_REGS_EXT_FS_DIVTWO,     0x03U },
    { CC1120_REGS_EXT_FS_DSM0,       0x33U },
    { CC1120_REGS_EXT_FS_DVC0,       0x17U },
    { CC1120_REGS_EXT_FS_PFD,        0x50U },
    { CC1120_REGS_EXT_FS_PRE,        0x6EU },
    { CC1120_REGS_EXT_FS_REG_DIV_CML,0x14U },
    { CC1120_REGS_EXT_FS_SPARE,      0xACU },
    { CC1120_REGS_EXT_FS_VCO0,       0xB4U },
    { CC1120_REGS_EXT_XOSC5,         0x0EU },
    { CC1120_REGS_EXT_XOSC1,         0x03U }
};

/**
 * @brief Gets the number of packets queued in the TX FIFO
 * 
 * @param numPackets - A pointer to an 8-bit integer to store the number of packets in
 * @return cc1120_status_code - Whether or not the registe read was successful
 */
cc1120_status_code cc1120_get_packets_in_tx_fifo(uint8_t *numPackets) {
    return cc1120_read_ext_addr_spi(CC1120_REGS_EXT_NUM_TXBYTES, numPackets, 1);
}

/**
 * @brief Gets the state of the CC1120 from the MARCSTATE register
 * 
 * @param stateNum - A pointer to an 8-bit integer to store the state in
 * @return cc1120_status_code - Whether or not the register read was successful
 */
cc1120_status_code cc1120_get_state(uint8_t *stateNum) {
    cc1120_status_code status = cc1120_read_ext_addr_spi(CC1120_REGS_EXT_MARCSTATE, stateNum, 1);
    *stateNum &= 0b11111;
    return status;
}

/**
 * @brief Resets CC1120 & initializes transmit mode
 * 
 * @return cc1120_status_code - Whether or not the setup was a success
 */
cc1120_status_code cc1120_tx_init() {
    cc1120_status_code status;

    for (uint8_t i=0; i<sizeof(txSettingsStd)/sizeof(registerSetting_t); i++) {
        status = cc1120_write_spi(txSettingsStd[i].addr, &txSettingsStd[i].val, 1);
        RETURN_IF_ERROR(status)
    }        

    for (uint8_t i=0; i<sizeof(txSettingsExt)/sizeof(registerSetting_t); i++) {
        status = cc1120_write_ext_addr_spi(txSettingsExt[i].addr, &txSettingsExt[i].val, 1);
        RETURN_IF_ERROR(status)
    }
    
    return cc1120_strobe_spi(CC1120_STROBE_SFSTXON);
}

/**
 * @brief Adds the given data to the CC1120 FIFO buffer and transmits
 * 
 * @param data - The packet to transmit
 * @param len - The size of the provided packet in bytes
 * @return cc1120_status_code 
 */
cc1120_status_code cc1120_send(uint8_t *data, uint32_t len) {
    cc1120_status_code status;
    
    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_send: Invalid data size!\n");
        return CC1120_ERROR_CODE_INVALID_PARAM;
    }

    bool largePacketFlag = false;

    // See section 8.1.5
    if (len > CC1120_MAX_PACKET_LEN) {
        // Temporarily set packet size to infinite
        uint8_t temp = 0x40;
        status = cc1120_write_spi(CC1120_REGS_PKT_CFG0, &temp, 1);
        RETURN_IF_ERROR(status)

        // Set packet length to mod(len, 256) so that the correct number of bits
        // are sent when fixed packet mode gets reactivated
        temp = len % 256;
        status = cc1120_write_spi(CC1120_REGS_PKT_LEN, &temp, 1);
        RETURN_IF_ERROR(status)
        
        largePacketFlag = true;
    } else { // If packet size < 255, use variable packet length mode
        // Set to variable packet length mode
        uint8_t temp = 0x20;
        status = cc1120_write_spi(CC1120_REGS_PKT_CFG0, &temp, 1);
        RETURN_IF_ERROR(status)

        // Set max packet size
        temp = CC1120_MAX_PACKET_LEN;
        status = cc1120_write_spi(CC1120_REGS_PKT_LEN, &temp, 1);
        RETURN_IF_ERROR(status)

        // Write current packet size
        uint8_t variableDataLen = (uint8_t) len;
        status = cc1120_write_fifo(&variableDataLen, 1); // Write packet size
        RETURN_IF_ERROR(status)
    }
    
    // Write packet
    for (uint8_t i=0; i<len/CC1120_TX_FIFO_SIZE+1; i++) {
        
        status = cc1120_write_fifo(data + i*CC1120_TX_FIFO_SIZE, min(CC1120_TX_FIFO_SIZE, len - i*CC1120_TX_FIFO_SIZE));
        RETURN_IF_ERROR(status)

        status = cc1120_strobe_spi(CC1120_STROBE_STX);
        RETURN_IF_ERROR(status)
    }
    
    return status;
}
