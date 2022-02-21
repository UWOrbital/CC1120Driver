#ifndef REGS_CC1120_H
#define REGS_CC1120_H

#define IOCFG3              0x00
#define IOCFG2              0x01
#define IOCFG1              0x02
#define IOCFG0              0x03
#define SYNC3               0x04
#define SYNC2               0x05
#define SYNC1               0x06
#define SYNC0               0x07
#define SYNC_CFG1           0x08
#define SYNC_CFG0           0x09
#define DEVIATION_M         0x0A
#define MODCFG_DEV_E        0x0B
#define DCFILT_CFG          0x0C
#define PREAMBLE_CFG1       0x0D
#define PREAMBLE_CFG0       0x0E
#define FREQ_IF_CFG         0x0F
#define IQIC                0x10
#define CHAN_BW             0x11
#define MDMCFG1             0x12
#define MDMCFG0             0x13
#define SYMBOL_RATE2        0x14
#define SYMBOL_RATE1        0x15
#define SYMBOL_RATE0        0x16
#define AGC_REF             0x17
#define AGC_CS_THR          0x18
#define AGC_GAIN_ADJUST     0x19
#define AGC_CFG3            0x1A
#define AGC_CFG2            0x1B
#define AGC_CFG1            0x1C
#define AGC_CFG0            0x1D
#define FIFO_CFG            0x1E
#define DEV_ADDR            0x1F
#define SETTLING_CFG        0x20
#define FS_CFG              0x21
#define WOR_CFG1            0x22
#define WOR_CFG0            0x23
#define WOR_EVENT0_MSB      0x24
#define WOR_EVENT0_LSB      0x25
#define PKT_CFG2            0x26
#define PKT_CFG1            0x27
#define PKT_CFG0            0x28
#define RFEND_CFG1          0x29
#define RFEND_CFG0          0x2A
#define PA_CFG2             0x2B
#define PA_CFG1             0x2C
#define PA_CFG0             0x2D
#define PKT_LEN             0x2E

#define EXT_ADDR            0x2F

#define SRES                0x30
#define SFSTXON             0x31
#define SXOFF               0x32
#define SCAL                0x33
#define SRX                 0x34
#define STX                 0x35
#define SIDLE               0x36
#define SWOR                0x38
#define SPWD                0x39
#define SFRX                0x3A
#define SFTX                0x3B
#define SWORRST             0x3C
#define SNOP                0x3D

#define DIR_FIFO_ACCESS     0x3E
#define FIFO_WRITE_START    0x00
#define FIFO_WRITE_END      0x7F
#define FIFO_READ_START     0x80
#define FIFO_READ_END       0xFF

#endif /* REGS_CC1120_H */
