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
int waitForLight();
void moveUpRamp(Robot, float);
void goTillLight(Robot, float);
void goTillLine(Robot);
void lineFollowing(Robot);
void testForward(Robot, float);
void testSideToSide(Robot, float);
void waitForTouch();
void guessToLine(Robot, float);
void guessToButton(Robot, float);

int main(void)
{
    Robot wall_E6(0);
    LCD.Clear();
    LCD.WriteLine("Waiting...");
    waitForTouch();
    waitForLight();
    moveUpRamp(wall_E6, IPS);
    guessToLine(wall_E6, IPS);
    goTillLight(wall_E6, IPS);
    guessToButton(wall_E6, IPS);
}

/**
 * @brief Moves the robot forward and backwards.
 * 
 * @param wall_E6 The robot.
 * @param time The amout of time to move for.
 */
void testForward(Robot wall_E6, float time) {
    LCD.Clear();
    LCD.WriteLine("Waiting...");
    waitForTouch();
    wall_E6.move(FRONT_ANGLE, time, SPEED);
    LCD.WriteLine("Yay! :)");
    LCD.WriteLine("Waiting...");
    waitForTouch();
    wall_E6.move(BACK_ANGLE, time, SPEED);
    LCD.WriteLine("finished");
}

/**
 * @brief Moves the robot to the left and then to the right.
 * 
 * @param wall_E6 The robot.
 * @param time The amount of time to move for.
 */
void testSideToSide(Robot wall_E6, float time) {
    LCD.Clear();
    LCD.WriteLine("Waiting...");
    waitForTouch();
    wall_E6.move(LEFT_ANGLE, time, SPEED);
    LCD.WriteLine("Yay! :)");
    LCD.WriteLine("Waiting...");
    waitForTouch();
    wall_E6.move(RIGHT_ANGLE, time, SPEED);
    LCD.WriteLine("finished");
}

/**
 * @brief Tests the full range of motion of the robot.
 * 
 * @param wall_E6 The robot.
 */
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

    testForward(wall_E6, 4.0);
    testSideToSide(wall_E6, 4.0);
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

void guessToLine(Robot wall_E6, float ips) {
    wall_E6.move(LEFT_ANGLE, 10.0/ips, SPEED);
}

void guessToButton(Robot wall_E6, float ips) {
    LCD.WriteLine("Driving to the button.");
    wall_E6.move(BACK_ANGLE, 7.0/ips, SPEED);
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
 */
void goTillLight(Robot wall_E6, float ips) {
    LCD.Clear();
    LCD.WriteLine("Seeking out light...");
    wall_E6.moveUnbounded(BACK_ANGLE, SLOW_SPEED);

    waitForLight();
    Sleep(0.1);
    wall_E6.stop();

    int light = detectLight();
    if (light == 1) {
        LCD.WriteLine("Red light!");
        wall_E6.move(LEFT_ANGLE, 3.5/ips, SPEED);
    } else {
        LCD.WriteLine("Blue light!");
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