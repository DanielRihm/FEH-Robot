#include <FEHLCD.h>
#include <FEHRPS.h>
#include <exception>
#include <iostream>
#include <OmniMotion.h>
#include <OmniSensors.h>
using namespace std;

void moveToSetPos(Robot, float, float, float);
float getRPS(float*, float*);
void waitForTouch();
int waitForLight();
void moveUpRamp(Robot);
void moveToBurger(Robot);
void burgerSetup(Robot);
void leftToSink(Robot); // not needed for test 3
void flushWithSink(Robot); // not needed for test 3
void sinkToBurger(Robot); // not needed for test 3
void lineUpToTicket(Robot); // not needed for test 3
void slideTicket(Robot); // not needed for test 3
int goTillLight(Robot, float); // not needed for test 2
void guessToButton(Robot, float); // not needed for test 2
void goToLineFirst(Robot, float); // not needed for test 2
void buttonToRamp(Robot, float, int); // not needed for test 2

/**
 * @brief Moves the robot to the specified position.
 * 
 * @param wall_E6 The robot.
 * @param x The x position.
 * @param y The y position.
 * @param angle The final angle of the robot.
 * @return Negative if failed.
 */
void moveToSetPos(Robot wall_E6, float x, float y, float angle) {
    float xCurr;
    float yCurr;
    float heading;
    const float error = 1.0;

    do {
        // makes heading angle in the same frame of reference as all other angles in the code.
        heading = getRPS(&xCurr, &yCurr);
        heading = 360.0 - (heading - RPS_FRONT_ANGLE);
        float dX = x - xCurr;
        float dY = y - yCurr;
        float dist = sqrt(dX*dX + dY*dY);
        float moveAngle = atan(dY/dX) * 180.0 / PI;
        moveAngle = 90.0 - moveAngle;
        if (dX < 0.0) {
            moveAngle += 180.0;
        }
        wall_E6.move(heading + moveAngle, dist/IPS_SPEED, SPEED); // I'm sure this is correct /s

        // robot now needs to fix any drift/go to desired angle.
        heading = getRPS(&xCurr, &yCurr);
        heading = 360.0 - (heading - RPS_FRONT_ANGLE);
        // makes sure that it turns in an optimal direction.
        float dHeading = abs(heading - angle);
        if (dHeading >= 360.0) {
            dHeading -= 360.0;
        }

        if (dHeading > 0 && dHeading < 180.0) {
            wall_E6.turn(dHeading/DPS_SPEED, SPEED);
        } else {
            wall_E6.turn((360-dHeading)/DPS_SPEED, -SPEED);
        }

        heading = getRPS(&xCurr, &yCurr);
        heading = 360.0 - (heading - RPS_FRONT_ANGLE);

    } while (xCurr < x - error || xCurr > x + error ||
        yCurr  < y - error || yCurr > y + error /*||
        heading < angle - error || heading > angle + error*/);
}

/**
 * @brief Moves the robot into position to flip the burger.
 * 
 * @param wall_E6 The robot.
 */
void burgerSetup(Robot wall_E6) {
    wall_E6.turn(60.0/DPS_SPEED, SPEED);
    wall_E6.moveArm(45.0);
}

/**
 * @brief Moves the robot from the top of the ramp to the burger flip.
 * 
 * @param wall_E6 The robot.
 */
void moveToBurger(Robot wall_E6) {
    reportMessage("Moving to burger.");
    float xDest = 28.9;
    float yDest = 60.0;
    float angleDest = 0.0;

    wall_E6.move(FRONT_ANGLE + 30.0, 16.0/IPS_SPEED, SPEED);
    moveToSetPos(wall_E6, xDest, yDest, angleDest);
}

/**
 * @brief Slides the ticket to the side.
 * 
 * @param wall_E6 The robot.
 */
void slideTicket(Robot wall_E6) {
    wall_E6.move(RIGHT_ANGLE+60.0, 5.0/IPS_SPEED, SPEED);
    wall_E6.turn(30.0/DPS_SPEED, -SPEED);
}

/**
 * @brief Lines the robot up for travel to the ticket.
 * 
 * @param wall_E6 The robot.
 */
void lineUpToTicket(Robot wall_E6) {
    reportMessage("Lining up to the ticket.");
    wall_E6.move(BACK_ANGLE, 1.0, SPEED);
    wall_E6.turn(150.0/DPS_SPEED, -SPEED);
    wall_E6.move(FRONT_ANGLE-60.0, 2.5, SPEED);
    reportMessage("back to ticket angle.");
    wall_E6.move(RIGHT_ANGLE+30.0, 0.5, SPEED);
    wall_E6.turn(38.0/DPS_SPEED, SPEED);
    reportMessage("Going to ticket.");
    wall_E6.move(FRONT_ANGLE+60.0, 20.0/IPS_SPEED, SPEED);
}

/**
 * @brief Drives the robot straight from the sink to the robot.
 * 
 * @param wall_E6 The robot.
 */
void sinkToBurger(Robot wall_E6) {
    reportMessage("Going to burger.");
    wall_E6.move(FRONT_ANGLE+60.0, 22.0/IPS_SPEED, SPEED);
    wall_E6.move(FRONT_ANGLE, 1.5, SPEED);
}

/**
 * @brief Lines the robot up with the sink.
 * 
 * @param wall_E6 The robot.
 */
void flushWithSink(Robot wall_E6) {
    reportMessage("Lining up to sink v2.");
    wall_E6.move(RIGHT_ANGLE+30.0, 0.5, SPEED);
    wall_E6.turn(30.0/DPS_SPEED, -SPEED);
    wall_E6.move(BACK_ANGLE, 1.0, SPEED);
}

/**
 * @brief Lines up against the left wall of the sink.
 * 
 * @param wall_E6 The robot.
 */
void leftToSink(Robot wall_E6) {
    reportMessage("Lining up to sink.");
    wall_E6.turn(30.0/DPS_SPEED, SPEED);
    wall_E6.move(LEFT_ANGLE+30.0, 12.0/IPS_SPEED, SPEED);
    wall_E6.move(LEFT_ANGLE+30.0, 0.5, SPEED);
}

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
    wall_E6.move(LEFT_ANGLE, 8.5/IPS_SPEED, SPEED);
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

/**
 * @brief Returns the recorded RPS values.
 * 
 * @param x The x value of the robot's position,
 * @param y The y value of the robot's position,
 * @return The heading of the robot.
 */
float getRPS(float *x, float *y) {
    Sleep(0.5);
    float head = RPS.Heading();
    *x = RPS.X();
    *y = RPS.Y();
    int counter = 0;
    const int limit = 20;
    if (*x > -1.5 && *y > -1.5 && head > -1.5) {
        while ((*x < 0.0 || *y < 0.0 || head < 0.0) && counter < limit) {
            Sleep(0.5);
            *x = RPS.X();
            *y = RPS.Y();
            head = RPS.Heading();
            counter++;
        }
    }
    return head;
}
