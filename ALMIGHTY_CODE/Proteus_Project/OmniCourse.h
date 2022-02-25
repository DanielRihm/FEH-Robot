#include <FEHLCD.h>
#include <OmniMotion.h>
#include <OmniSensors.h>
#include <OmniUtility.h>

void moveUpRamp(Robot, float);
int goTillLight(Robot, float);
void guessToLine(Robot, float);
void guessToButton(Robot, float);
void goTillLine(Robot);
void lineFollowing(Robot);
void waitForTouch();
int waitForLight();
void goToLineFirst(Robot, float);
void buttonToRamp(Robot, float, int);

/**
 * @brief Drives the robot over from the button to the ramp.
 * 
 * @param wall_E6 The robot.
 * @param ips inches per second.
 * @param light The light that was detected.
 */
void buttonToRamp(Robot wall_E6, float ips, int light) {
    LCD.WriteLine("Going to the Ramp");
    wall_E6.move(FRONT_ANGLE, 1.0/ips, SPEED);
    float distToRamp = 9.0;
    if (light == 1) {
        LCD.WriteLine("At red so go further.");
        wall_E6.move(RIGHT_ANGLE, (distToRamp + BUTTON_DISTANCE)/ips, SPEED);
    } else {
        LCD.WriteLine("At blue so go less.");
        wall_E6.move(RIGHT_ANGLE, (distToRamp - BUTTON_DISTANCE)/ips, SPEED);
    }

    LCD.WriteLine("Driving up.");
    wall_E6.move(FRONT_ANGLE, 28.0/(ips * (FAST_SPEED / SPEED)), FAST_SPEED);
    LCD.WriteLine("Driving down.");
    wall_E6.move(BACK_ANGLE, 34.0/ips, SPEED);
}

/**
 * @brief Drives the robot straight over to the light.
 * 
 * @param wall_E6 The robot.
 * @param ips Inches per second.
 */
void goToLineFirst(Robot wall_E6, float ips) {
    LCD.WriteLine("Going to the line.");
    wall_E6.move(FRONT_ANGLE, 10.0/ips, SPEED);
    wall_E6.move(LEFT_ANGLE, 17.0/ips, SPEED);
}

/**
 * @brief Robot moves a set distance to where the line should be.
 * 
 * @param wall_E6 The robot.
 * @param ips Inches per second.
 */
void guessToLine(Robot wall_E6, float ips) {
    wall_E6.move(LEFT_ANGLE, 10.0/ips, SPEED);
}

/**
 * @brief The robot moves a set distance to where the button should be.
 * 
 * @param wall_E6 The robot.
 * @param ips Inches per second.
 */
void guessToButton(Robot wall_E6, float ips) {
    LCD.WriteLine("Driving to the button.");
    wall_E6.move(BACK_ANGLE, 8.0/ips, SPEED);
}

/**
 * @brief Moves the robot from the start, up, and then back down the ramp.
 * Requires ips > 0.0
 * 
 * @param wall_E6 The robot's motor configuration.
 * @param ips Inches per second.
 */
void moveUpRamp(Robot wall_E6, float ips) {
    LCD.Clear();
    LCD.WriteLine("Moving up the ramp.");
    wall_E6.move(LEFT_ANGLE, 7.0/ips, SPEED);
    wall_E6.move(FRONT_ANGLE, 28.0/(ips * (FAST_SPEED / SPEED)), FAST_SPEED);
    LCD.WriteLine("And back down the ramp.");
    wall_E6.move(BACK_ANGLE, 22.0/ips, SPEED);
}

/**
 * @brief Follows a line until no sensors are on a line.
 * 
 * @param wall_E6 The robot.
 */
void lineFollowing(Robot wall_E6) {
    bool leftLine;
    bool rightLine;
    bool middleLine;

    LCD.Clear();
    while (true) {
        readOpto(&leftLine, &middleLine, &rightLine);

        if (middleLine && rightLine) {
            // Prefers the right side
            wall_E6.moveUnbounded(BACK_ANGLE - 20.0, SPEED);
            LCD.WriteLine("MIDDLE");
        } else if (rightLine) {
            wall_E6.moveUnbounded(RIGHT_ANGLE, SPEED);
            LCD.WriteLine("(ACTUAL) LEFT OF LINE");
        } else if (middleLine) {
            wall_E6.moveUnbounded(BACK_ANGLE + 10.0, SPEED);
            LCD.WriteLine("UHH. MAYBE TO THE RIGHT");
        } else {
            wall_E6.stop();
            LCD.WriteLine("No Line Detected.");
            break;
        }

        Sleep(0.05);
    }
}

/**
 * @brief Moves the robot forward until it detects a light and the aligns with the respective button.
 * 
 * @param wall_E6 The robot.
 * @param ips Inches per second.
 * @return returns the light value detected.
 */
int goTillLight(Robot wall_E6, float ips) {
    LCD.Clear();
    LCD.WriteLine("Seeking out light...");
    wall_E6.moveUnbounded(BACK_ANGLE, SLOW_SPEED);

    waitForLight();
    Sleep(0.1);
    wall_E6.stop();

    int light = detectLight();
    if (light == 1) {
        LCD.WriteLine("Red light!");
        wall_E6.move(LEFT_ANGLE, BUTTON_DISTANCE/ips, SPEED);
    } else {
        LCD.WriteLine("Blue light!");
        wall_E6.move(RIGHT_ANGLE, BUTTON_DISTANCE/ips, SPEED);
    }

    return light;
}

/**
 * @brief Robot drives left until it meets a line.
 * 
 * @param wall_E6 The robot.
 */
void goTillLine(Robot wall_E6) {
    wall_E6.moveUnbounded(LEFT_ANGLE, SPEED);

    bool* leftLine;
    bool* rightLine;
    bool* middleLine;

    readOpto(leftLine, middleLine, rightLine);
    while (!leftLine && !middleLine && !rightLine) {
        readOpto(leftLine, middleLine, rightLine);
    }

    wall_E6.stop();
}

/**
 * @brief Sleeps until a light is detected.
 * 
 * @return The specific light detected.
 */
int waitForLight() {
    int light = 0;
    LCD.WriteLine("Waiting for light.");
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
