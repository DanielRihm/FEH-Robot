#include <FEHIO.h>
#include <FEHLCD.h>
#include <OmniUtility.h>

AnalogInputPin cds(FEHIO::P0_0);

int detectLight();

/**
 * @brief Detects the particular light color for a yellow filter.
 * 
 * @return 1 for a red light, 2 for blue light, 0 for no light.
 */
int detectLight() {
    float cdsValue = cds.Value();
    if (cdsValue < RED_LIGHT_CUTOFF) {
        return RED_LIGHT;
    } else if (cdsValue < BLUE_LIGHT_CUTOFF) {
        return BLUE_LIGHT;
    } else {
        return NO_LIGHT;
    }
}
