#include <FEHLCD.h>
#include <FEHRPS.h>
#include <OmniMotion.h>
#include <OmniSensors.h>

float angleDifference(float, float);
void fixBurger(Robot);
bool checkHeading(float, float);
void twistFlip(Robot);
void moveToTwist(Robot);
void moveToSetPos(Robot, float, float, float);
float getRPS(float*, float*);
void waitForTouch();
int waitForLight();
void moveUpRamp(Robot);
void moveToBurger(Robot);
void burgerSetup(Robot);
void flipBurger(Robot);
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
 * @brief Flips the burger tray back to its starting position.
 * 
 * @param wall_E6 The robot.
 */
void fixBurger(Robot wall_E6) {
    wall_E6.moveArm(180.0);
    wall_E6.move(FRONT_ANGLE + 30.0, 2.0/IPS_SPEED, SPEED);
    wall_E6.move(LEFT_ANGLE + 30.0, 0.7, SPEED);
    wall_E6.moveArm(120.0);
    Sleep(1.0);
    wall_E6.move(RIGHT_ANGLE + 30.0, 1.66/IPS_SPEED, SPEED);
}

/**
 * @brief Moves the robot up to the lever for the twist and flips it.
 * 
 * @param wall_E6 The robot.
 */
void twistFlip(Robot wall_E6) {
    wall_E6.move(BACK_ANGLE + 30.0, 7.32/IPS_SPEED, SPEED);
    while (true) {
        wall_E6.turn(10.0/DPS_SPEED, SPEED);
        wall_E6.moveArm(70.0);
        Sleep(1.0);
        wall_E6.moveArm(150.0);
        Sleep(0.5);
    }
}

/**
 * @brief Moves the robot in position for that epic twist ice cream.
 * 
 * @param wall_E6 The robot.
 */
void moveToTwist(Robot wall_E6) {
    reportMessage("Going to ice cream.");
    wall_E6.move(BACK_ANGLE - 30.0, 6.0 / IPS_SPEED, SPEED);
    wall_E6.turn(45.0/DPS_SPEED, -SPEED);

    float xDest = 13.0;
    float yDest = 51.0;
    float angleDest = 105.0;

    moveToSetPos(wall_E6, xDest, yDest, angleDest);
}

/**
 * @brief Moves forward and flips the burger.
 * 
 * @param wall_E6 The robot.
 */
void flipBurger(Robot wall_E6) {
    reportMessage("Fliping tray back.");
    wall_E6.move(LEFT_ANGLE + 30.0, 1.6/IPS_SPEED, SPEED);
    Sleep(0.5);
    wall_E6.moveArm(150.0);
    Sleep(2.0);
    wall_E6.move(BACK_ANGLE + 30.0, 1.66/IPS_SPEED, SPEED);
    wall_E6.moveArm(160.0);
    Sleep(1.5);
    wall_E6.move(RIGHT_ANGLE + 30.0, 1.66/IPS_SPEED, SPEED);
}

/**
 * @brief Finds the difference between two angles.
 * 
 * @param angle The angle.
 * @param destAngle The other angle.
 * @return The difference (destAngle - angle)
 */
float angleDifference(float angle, float destAngle) {
    double dAngle = fmod(destAngle - angle, 360.0);
    if (dAngle < -180.0) {
        dAngle += 360.0;
    }else if (dAngle >= 180.0) {
        dAngle -= 360.0;
    }
    return dAngle;
}

/**
 * @brief Checks whether the given angle is within the given error from the destination angle.
 * 
 * @param angle The current angle.
 * @param destAngle The desired angle.
 * @param error The allowed error.
 * @return true if the given angle is within the allowed error, false otherwise.
 */
bool checkHeading(float angle, float destAngle, float error) {
    double dAngle = angleDifference(angle, destAngle);

    if (dAngle < error && dAngle > -error) {
        return true;
    }
    return false;
}

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
    int speed = SPEED;
    const float error = 0.1;
    bool headInError;

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
        wall_E6.move(angleDifference(heading, moveAngle), dist/IPS_SPEED, speed); // I'm sure this is correct /s

        // robot now needs to fix any drift/go to desired angle.
        heading = getRPS(&xCurr, &yCurr);
        heading = 360.0 - (heading - RPS_FRONT_ANGLE);
        // makes sure that it turns in an optimal direction.
        float dHeading = angleDifference(heading, angle);

        if (dHeading > 0) {
            wall_E6.turn(dHeading/DPS_SPEED, -speed);
        } else {
            wall_E6.turn((-dHeading)/DPS_SPEED, speed);
        }

        heading = getRPS(&xCurr, &yCurr);
        heading = 360.0 - (heading - RPS_FRONT_ANGLE);

        headInError = checkHeading(heading, angle, error);

        speed -= 2;
    } while ((xCurr < x - error || xCurr > x + error ||
        yCurr  < y - error || yCurr > y + error ||
        !headInError) && speed > 10);
}

/**
 * @brief Moves the robot into position to flip the burger.
 * 
 * @param wall_E6 The robot.
 */
void burgerSetup(Robot wall_E6) {
    wall_E6.turn(60.0/DPS_SPEED, -SPEED);
    wall_E6.moveArm(25.0);
    Sleep(0.5);
}

/**
 * @brief Moves the robot from the top of the ramp to the burger flip.
 * 
 * @param wall_E6 The robot.
 */
void moveToBurger(Robot wall_E6) {
    reportMessage("Moving to burger.");
    float xDest = 27.3;
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
    LCD.ClearBuffer();
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
    Sleep(0.4);
    float head = RPS.Heading();
    *x = RPS.X();
    *y = RPS.Y();
    float xSum = 0.0;
    float ySum = 0.0;
    float headSum = 0.0;
    const int limit = 10;
    int count = 0;
    if (*x > -1.5 && *y > -1.5 && head > -1.5) {
        for (int i = 0; i < limit; i++) {
            Sleep(0.01);
            head = RPS.Heading();
            *x = RPS.X();
            *y = RPS.Y();
            if (*x > -0.1 && *y > -0.1 && head > -0.1) {
                xSum += *x;
                ySum += *y;
                headSum += head;
                count++;
            }
        }
    }

    *x = xSum / count;
    *y = ySum / count;
    head = headSum / count;

    return head;
}
