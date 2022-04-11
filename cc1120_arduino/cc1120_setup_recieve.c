#include "cc1120_setup_recieve.h"

#include "cc1120_utility.h"
#include "cc1120_spi.h"
#include "cc1120_regs.h"
#include <math.h>

/**
 * @brief Sets RX Filter Bandwidth.
 *
 * @param RXFilterBandwidth - The RX Filter Bandwidth to set to.
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool set_rx_filter_bandwidth(uint32_t RXFilterBandwidth)
{
    uint8_t CHAN_BW;
    if (!cc1120_read_spi(CC1120_REGS_CHAN_BW, &CHAN_BW, 1))
    {
        return false;
    }

    uint8_t ADC_CIC_DEFACT = extract_bits(CHAN_BW, 6, 6);
    uint8_t decimationFactor = ADC_CIC_DEFACT == 0 ? 20 : 32;

    uint8_t CHFILT_BYPASS = extract_bits(CHAN_BW, 7, 7);
    uint8_t chfiltConstant = CHFILT_BYPASS == 0 ? 8 : 2;

    uint32_t BB_CIC_DEFACT = FXOSC / (decimationFactor * RXFilterBandwidth * chfiltConstant);

    if (BB_CIC_DEFACT > pow(2, 6) - 1)
    {
        // Larger than largest 6 bit unsigned integer
        return false;
    }

    uint8_t data = extract_bits(CHAN_BW, 6, 7) << 6 || BB_CIC_DEFACT;

    return cc1120_write_spi(CC1120_REGS_CHAN_BW, &data, 1);
}

/**
 * @brief Gets the RX Filter Bandwidth.
 *
 * @param RXFilterBandwidth - The address to write the result the value of the RX Filter Bandwidth to.
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool get_rx_filter_bandwidth(uint32_t *RXFilterBandwidth)
{
    uint8_t CHAN_BW;
    if (!cc1120_read_spi(CC1120_REGS_CHAN_BW, &CHAN_BW, 1))
    {
        return false;
    }

    uint8_t ADC_CIC_DEFACT = extract_bits(CHAN_BW, 6, 6);
    uint8_t decimationFactor = ADC_CIC_DEFACT == 0 ? 20 : 32;

    uint8_t CHFILT_BYPASS = extract_bits(CHAN_BW, 7, 7);
    uint8_t chfiltConstant = CHFILT_BYPASS == 0 ? 8 : 2;

    uint8_t BB_CIC_DEFACT = extract_bits(CHAN_BW, 0, 5);

    *RXFilterBandwidth = FXOSC / (decimationFactor * BB_CIC_DEFACT * chfiltConstant);

    return true;
}

/**
 * @brief Sets optimal AGC Reference using Equation 12, based off of RSSI Offset.
 *
 * @param RSSIOffset - The RSSI Offset.
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool set_agc_ref(uint8_t RSSIOffset)
{
    uint32_t RXFilterBW;
    if (!get_rx_filter_bandwidth(&RXFilterBW))
    {
        return false;
    }

    uint8_t AGC_REFERENCE = (uint8_t)(10 * log10(RXFilterBW) - 106 - RSSIOffset);

    if (AGC_REFERENCE > pow(2, 8) - 1)
    {
        // Larger than largest 8 bit unsigned integer
        return false;
    }

    return cc1120_write_spi(CC1120_REGS_AGC_REF, &AGC_REFERENCE, 1);
}
