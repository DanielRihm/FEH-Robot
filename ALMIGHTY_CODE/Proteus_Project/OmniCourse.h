#include <FEHLCD.h>
#include <OmniMotion.h>
#include <OmniSensors.h>
#include <OmniUtility.h>

void waitForTouch();
int waitForLight();
void moveUpRamp(Robot);
int goTillLight(Robot, float); // not needed for test 2
void guessToButton(Robot, float); // not needed for test 2
void goToLineFirst(Robot, float); // not needed for test 2
void buttonToRamp(Robot, float, int); // not needed for test 2

/**
 * @brief Drives the robot over from the button to the ramp.
 * 
 * @param wall_E6 The robot.
 * @param ips inches per second.
 * @param light The light that was detected.
 */
void buttonToRamp(Robot wall_E6, float ips, int light) {
    reportMessage("Going to the Ramp");
    wall_E6.move(FRONT_ANGLE, 1.0/ips, SPEED);
    float distToRamp = 9.0;
    if (light == RED_LIGHT) {
        reportMessage("At red so go further.");
        wall_E6.move(RIGHT_ANGLE, (distToRamp + BUTTON_DISTANCE)/ips, SPEED);
    } else {
        reportMessage("At blue so go less.");
        wall_E6.move(RIGHT_ANGLE, (distToRamp - BUTTON_DISTANCE)/ips, SPEED);
    }

    reportMessage("Driving up.");
    wall_E6.move(FRONT_ANGLE, 40.0/(ips), SPEED);
    reportMessage("Driving down.");
    wall_E6.move(BACK_ANGLE, 28.0/ips, SPEED);
}

/**
 * @brief Drives the robot straight over to the light.
 * 
 * @param wall_E6 The robot.
 * @param ips Inches per second.
 */
void goToLineFirst(Robot wall_E6, float ips) {
    reportMessage("Going to the line.");
    wall_E6.move(FRONT_ANGLE, 10.0/ips, SPEED);
    wall_E6.move(LEFT_ANGLE, 17.0/ips, SPEED);
}

/**
 * @brief The robot moves a set distance to where the button should be.
 * 
 * @param wall_E6 The robot.
 * @param ips Inches per second.
 */
void guessToButton(Robot wall_E6, float ips) {
    reportMessage("Driving to the button.");
    wall_E6.move(BACK_ANGLE, 10.0/ips, SPEED);
}

/**
 * @brief Moves the robot from the start, up, and then back down the ramp.
 * Requires ips > 0.0
 * 
 * @param wall_E6 The robot's motor configuration.
 * @param ips Inches per second.
 */
void moveUpRamp(Robot wall_E6) {
    reportMessage("Moving up the ramp.");
    wall_E6.move(LEFT_ANGLE, 7.0/IPS_SPEED, SPEED);
    wall_E6.move(FRONT_ANGLE, 40.0/IPS_SPEED, SPEED);
}

/**
 * @brief Moves the robot forward until it detects a light and the aligns with the respective button.
 * 
 * @param wall_E6 The robot.
 * @param ips Inches per second.
 * @return returns the light value detected.
 */
int goTillLight(Robot wall_E6, float ips) {
    reportMessage("Seeking out light...");
    wall_E6.moveUnbounded(BACK_ANGLE, SLOW_SPEED);

    waitForLight();
    Sleep(0.2);
    wall_E6.stop();

    int light = detectLight();
    if (light == 1) {
        reportMessage("Red light!");
        wall_E6.move(LEFT_ANGLE, BUTTON_DISTANCE/ips, SPEED);
    } else {
        reportMessage("Blue light!");
        wall_E6.move(RIGHT_ANGLE, BUTTON_DISTANCE/ips, SPEED);
    }

    return light;
}

/**
 * @brief Sleeps until a light is detected.
 * 
 * @return The specific light detected.
 */
int waitForLight() {
    int light = 0;
    reportMessage("Waiting for light.");
    while (light == 0) {
        light = detectLight();
        Sleep(0.5);
    }

    return light;
}

/**
 * @brief Just waits for a user touch lol
 * 
 */
void waitForTouch() {
    float x, y;
    while (!LCD.Touch(&x,&y));
}
