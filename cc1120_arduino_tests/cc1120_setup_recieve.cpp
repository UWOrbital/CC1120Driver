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
int setRXFilterBandwidth(int RXFilterBandwidth)
{
    uint8_t CHAN_BW;
    if (!cc1120_read_spi(CC1120_REGS_CHAN_BW, &CHAN_BW, 1))
    {
        return false;
    }

    int ADC_CIC_DEFACT = NtoKBits(CHAN_BW, 6, 6);
    int decimationFactor = ADC_CIC_DEFACT == 0 ? 20 : 32;

    int CHFILT_BYPASS = NtoKBits(CHAN_BW, 7, 7);
    int chfiltConstant = CHFILT_BYPASS == 0 ? 8 : 2;

    long int BB_CIC_DEFACT = FXOSC / (decimationFactor * RXFilterBandwidth * chfiltConstant);

    if (BB_CIC_DEFACT > pow(2, 6) - 1)
    {
        // Larger than largest 6 bit unsigned integer
        return false;
    }

    uint8_t data = NtoKBits(CHAN_BW, 6, 7) << 6 || BB_CIC_DEFACT;

    return cc1120_write_spi(CC1120_REGS_CHAN_BW, &data, 1);
}
