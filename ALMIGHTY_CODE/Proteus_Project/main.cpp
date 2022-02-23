#include <FEHLCD.h>
#include <OmniMotion.h>
#include <OmniSensors.h>

#define RIGHT_ANGLE 90.0
#define FRONT_ANGLE 0.0
#define LEFT_ANGLE 270.0
#define BACK_ANGLE 180.0
#define SPEED 45
#define SLOW_SPEED 25
#define FAST_SPEED 60
#define IPS 3.9375

void test(Robot);
void waitForLight();
void moveUpRamp(Robot, float);
void goTillLight(Robot, float);
void goTillLine(Robot);
void lineFollowing(Robot);
void testForward(Robot, float);
void waitForTouch();

int main(void)
{
    Robot wall_E6(0);
    waitForTouch();
    moveUpRamp(wall_E6, IPS);
}

void testForward(Robot wall_E6, float time) {
    LCD.Clear();
    LCD.WriteLine("Waiting...");
    float x, y;
    waitForTouch();
    LCD.WriteLine("Yay! :)");
}

void test(Robot wall_E6) {
    LCD.Clear();
    wall_E6.turn(5.0, SPEED);
    for (int i = 0; i < 360; i++) {
        wall_E6.moveUnbounded(i, SPEED);
        if (i % 5 == 0) {
            LCD.Clear();
            LCD.Write("Moving at angle ");
            LCD.WriteLine(i);
        }
        Sleep(0.01);
    }

    Sleep(1.0);
    LCD.Clear();
    LCD.WriteLine("Moving forward.");
    wall_E6.move(FRONT_ANGLE, 1.0, SPEED);
    LCD.WriteLine("Moving right.");
    wall_E6.move(RIGHT_ANGLE, 1.0, SPEED);
    LCD.WriteLine("Moving left.");
    wall_E6.move(LEFT_ANGLE, 1.0, SPEED);
    LCD.WriteLine("Moving backwards.");
    wall_E6.move(BACK_ANGLE, 1.0, SPEED);

    LCD.WriteLine("Waiting...");
    waitForTouch();
    wall_E6.move(FRONT_ANGLE, 1.0, SPEED);
}

void waitForLight() {
    int light = 0;
    while (light == 0) {
        light = detectLight();
        Sleep(0.5);
    }
}

/**
 * @brief Moves the robot from the start, up, and then back down the ramp.
 * Requires ips > 0.0
 * 
 * @param wall_E6 The robot's motor configuration.
 * @param ips Inches per second.
 */
void moveUpRamp(Robot wall_E6, float ips) {
    wall_E6.move(LEFT_ANGLE, 7.0/ips, SPEED);
    wall_E6.move(FRONT_ANGLE, 32.0/(ips * (SPEED / FAST_SPEED)), FAST_SPEED);
    wall_E6.move(BACK_ANGLE, 20.0/ips, SPEED);
}

/**
 * @brief Follows a line until no sensors are on a line.
 * 
 * @param wall_E6 The robot.
 */
void lineFollowing(Robot wall_E6) {
    bool* leftLine;
    bool* rightLine;
    bool* middleLine;

    while (true) {
        readOpto(leftLine, middleLine, rightLine);

        if (middleLine) {
            wall_E6.moveUnbounded(BACK_ANGLE, SPEED);
            LCD.WriteLine("MIDDLE");
        } else if (leftLine) {
            wall_E6.moveUnbounded(LEFT_ANGLE, SPEED);
            LCD.WriteLine("(ACTUAL) RIGHT OF LINE");
        } else if (rightLine) {
            wall_E6.moveUnbounded(RIGHT_ANGLE, SPEED);
            LCD.WriteLine("(ACTUAL) LEFT OF LINE");
        } else {
            wall_E6.stop();
            break;
        }
    }
}

/**
 * @brief Moves the robot forward until it detects a light and the aligns with the respective button.
 * 
 * @param wall_E6 The robot.
 * @param ips Inches per second.
 */
void goTillLight(Robot wall_E6, float ips) {
    wall_E6.moveUnbounded(BACK_ANGLE, SLOW_SPEED);

    while(detectLight() == 0);
    wall_E6.stop();

    int light = detectLight();
    if (light == 1) {
        wall_E6.move(LEFT_ANGLE, 3.5/ips, SPEED);
    } else {
        wall_E6.move(RIGHT_ANGLE, 3.5/ips, SPEED);
    }
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
 * @brief Just waits for a user touch lol
 * 
 */
void waitForTouch() {
    float x, y;
    while (!LCD.Touch(&x,&y));
}