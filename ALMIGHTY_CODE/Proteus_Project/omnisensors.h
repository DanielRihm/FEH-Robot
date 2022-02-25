#include <FEHIO.h>
#include <FEHLCD.h>
#include <OmniUtility.h>

AnalogInputPin cds(FEHIO::P0_0);
// left is on the actual left side of the robot (same side as left motor).
//AnalogInputPin leftOpt(FEHIO::P1_0);
AnalogInputPin rightOpt(FEHIO::P3_0);
AnalogInputPin middleOpt(FEHIO::P2_0);

int detectLight();
void readOpto(bool*, bool*, bool*);

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

/**
 * @brief Reads the optosensors and returns whether each optosensor is on a line or not. Left is actual left (same side as left motor).
 * 
 * @param leftLine Whether the left sensor is on the line.
 * @param middleLine Whether the middle sensor is on the line.
 * @param rightLine Whether the right sensor is on the line.
 */
void readOpto(bool* leftLine, bool* middleLine, bool* rightLine) {

    //*leftLine = leftOpt.Value() > LINE_CUTOFF;
    *middleLine = middleOpt.Value() < LINE_CUTOFF;
    *rightLine = rightOpt.Value() < LINE_CUTOFF;
}
