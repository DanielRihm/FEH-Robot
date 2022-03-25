#include <FEHLCD.h>
#include <FEHRPS.h>
#include <OmniMotion.h>
#include <OmniSensors.h>
#include <stdio.h>

float xOffset = 0.0;
float yOffset = 0.0;

void calibrate();
void moveToJukeboxLight(Robot);
void pushFinalButton(Robot);
void moveDownRamp(Robot);
void goToTopRamp(Robot);
void unFlipLever(Robot);
float angleDifference(float, float);
bool checkHeading(float, float);
void iceCreamFlip(Robot);
void moveToTwist(Robot);
void moveToVanil(Robot);
void moveToChoco(Robot);
void moveToSetPos(Robot, float, float, float, float);
float getRPS(float*, float*);
void waitForTouch();
int waitForLight();
void moveUpRamp(Robot);
void fixBurger(Robot);
void moveToBurger(Robot);
void burgerSetup(Robot);
void flipBurger(Robot);
void goToSink(Robot);
void dropTray(Robot);
void sinkToBurger(Robot); // not needed for test 3
void lineUpToTicket(Robot); // not needed for test 3
void slideTicket(Robot); // not needed for test 3
int goTillLight(Robot, float); // not needed for test 2
void guessToButton(Robot, float); // not needed for test 2

/**
 * @brief Moves the robot from the bottom of the ramp to the jukebox light.
 * 
 * @param hankette The robot.
 */
void moveToJukeboxLight(Robot hankette) {
    hankette.move(LEFT_ANGLE, 7.0/IPS_SPEED, SPEED);

    float xDest = 9.0;
    float yDest = 14.0;
    float angleDest = 0.0;
    moveToSetPos(hankette, xDest, yDest, angleDest, 0.5);
}

void calibrate() {
    reportMessage("Touch for calibration");
    waitForTouch();

    float botLeftX;
    float botLeftY;

    getRPS(&botLeftX, &botLeftY);

    xOffset = DEFAULT_BOT_X - botLeftX;
    yOffset = DEFAULT_BOT_Y - botLeftY;
    char buffer[64];
    snprintf(buffer, sizeof buffer, "X Offset: %f", xOffset);
    reportMessage(buffer);
    snprintf(buffer, sizeof buffer, "Y Offset: %f", yOffset);
    reportMessage(buffer);
}

/**
 * @brief Pushes the final button.
 * 
 * @param hankette The robot.
 */
void pushFinalButton(Robot hankette) {
    hankette.turn(40.0/DPS_SPEED, SPEED);
    hankette.move(BACK_ANGLE, 20.0/IPS_SPEED, SPEED);
}

/**
 * @brief Moves the robot back down the ramp.
 * 
 * @param hankette The robot.
 */
void moveDownRamp(Robot hankette) {
    reportMessage("Going down ramp.");
    hankette.move(BACK_ANGLE, 25.0/IPS_SPEED, SPEED);
}

/**
 * @brief Drives the robot back to the top of the ramp from the burger flip.
 * 
 * @param hankette The robot.
 */
void goToTopRamp(Robot hankette) {
    reportMessage("Returning to ramp.");
    hankette.turn(60.0/DPS_SPEED, SPEED);
    hankette.move(BACK_ANGLE + 30.0, 10.0/IPS_SPEED, SPEED);

    float xDest = 18.4;
    float yDest = 45.5;
    float angleDest = 0.0;
    moveToSetPos(hankette, xDest, yDest, angleDest, 0.5);
}

/**
 * @brief Moves the robot to the chocolate lever.
 * 
 * @param hankette 
 */
void moveToChoco(Robot hankette) {
    reportMessage("Going to chocolate.");
    hankette.move(FRONT_ANGLE + 60.0, 7.0 / IPS_SPEED, SPEED);
    hankette.turn(105.0/DPS_SPEED, -SPEED);

    float xDest = 17.0;
    float yDest = 54.0;
    float angleDest = 105.0;

    moveToSetPos(hankette, xDest, yDest, angleDest, 0.2);
}

/**
 * @brief Moves the robot to the vanilla lever.
 * 
 * @param hankette 
 */
void moveToVanil(Robot hankette) {
    reportMessage("Going to vanilla.");
    hankette.move(FRONT_ANGLE + 60.0, 7.0 / IPS_SPEED, SPEED);
    hankette.turn(105.0/DPS_SPEED, -SPEED);

    float xDest = 11.0;
    float yDest = 48.0;
    float angleDest = 105.0;

    moveToSetPos(hankette, xDest, yDest, angleDest, 0.2);
}

/**
 * @brief Unflip the ice cream lever after 7 seconds.
 * 
 * @param hankette The robot.
 */
void unFlipLever(Robot hankette) {
    hankette.move(FRONT_ANGLE + 30.0, 2.0/IPS_SPEED, SPEED);
    hankette.moveArm(30.0);
    Sleep(7.0);
    hankette.move(BACK_ANGLE + 30.0, 2.0/IPS_SPEED, SPEED);
    hankette.moveArm(100.0);
    Sleep(0.5);
}

/**
 * @brief Flips the burger tray back to its starting position.
 * 
 * @param hankette The robot.
 */
void fixBurger(Robot hankette) {
    hankette.moveArm(180.0);
    hankette.move(FRONT_ANGLE + 30.0, 2.0/IPS_SPEED, SPEED);
    hankette.move(LEFT_ANGLE + 30.0, 1.0, SPEED);
    hankette.moveArm(120.0);
    Sleep(1.0);
    hankette.move(RIGHT_ANGLE + 30.0, 1.66/IPS_SPEED, SPEED);
}

/**
 * @brief Moves the robot up to the lever for the twist and flips it.
 * 
 * @param hankette The robot.
 */
void iceCreamFlip(Robot hankette) {
    hankette.move(BACK_ANGLE + 30.0, 9.0/IPS_SPEED, SPEED);
    hankette.turn(5.0/DPS_SPEED, SPEED);
    hankette.moveArm(70.0);
    Sleep(1.0);
    hankette.moveArm(150.0);
    Sleep(0.5);
}

/**
 * @brief Moves the robot in position for that epic twist ice cream.
 * 
 * @param hankette The robot.
 */
void moveToTwist(Robot hankette) {
    reportMessage("Going to twist.");
    hankette.move(FRONT_ANGLE + 60.0, 7.0 / IPS_SPEED, SPEED);
    hankette.turn(105.0/DPS_SPEED, -SPEED);

    float xDest = 14.2; // 14
    float yDest = 50.8; // 51
    float angleDest = 105.0;

    moveToSetPos(hankette, xDest, yDest, angleDest, 0.2);
}

/**
 * @brief Moves forward and flips the burger.
 * 
 * @param hankette The robot.
 */
void flipBurger(Robot hankette) {
    reportMessage("Fliping tray back.");
    hankette.move(LEFT_ANGLE + 30.0, 2.0/IPS_SPEED, SPEED);
    Sleep(0.5);
    hankette.moveArm(160.0);
    Sleep(2.0);
    hankette.move(RIGHT_ANGLE + 30.0, 2.0/IPS_SPEED, SPEED);
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
    } else if (dAngle >= 180.0) {
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
 * @param hankette The robot.
 * @param x The x position.
 * @param y The y position.
 * @param angle The final angle of the robot.
 * @param error The allowed error in the final position.
 * @return Negative if failed.
 */
void moveToSetPos(Robot hankette, float x, float y, float angle, float error) {
    x -= xOffset;
    y -= yOffset;
    float xCurr;
    float yCurr;
    float heading;
    int speed = SPEED;
    bool headInError;

    do {
        // makes heading angle in the same frame of reference as all other angles in the code.
        heading = getRPS(&xCurr, &yCurr);
        if (xCurr < 0 || yCurr < 0 || heading < 0) {
            break;
        }

        heading = 360.0 - (heading - RPS_FRONT_ANGLE);
        float dX = x - xCurr;
        float dY = y - yCurr;
        float dist = sqrt(dX*dX + dY*dY);
        float moveAngle = atan(dY/dX) * 180.0 / PI;
        moveAngle = 90.0 - moveAngle;
        if (dX < 0.0) {
            moveAngle += 180.0;
        }

        char buffer[27];
        snprintf(buffer, sizeof buffer, "Moving dist: %.2f", dist);
        reportMessage(buffer);
        snprintf(buffer, sizeof buffer, "   At angle: %.2f", moveAngle);
        reportMessage(buffer);
        snprintf(buffer, sizeof buffer, "   From: (%.1f, %.1f)", xCurr, yCurr);
        reportMessage(buffer);
        snprintf(buffer, sizeof buffer, "   To: (%.1f, %.1f)", x, y);
        reportMessage(buffer);
        hankette.move(angleDifference(heading, moveAngle), dist/IPS_SPEED, speed);

        // robot now needs to fix any drift/go to desired angle.
        heading = getRPS(&xCurr, &yCurr);
        if (xCurr < 0 || yCurr < 0 || heading < 0) {
            break;
        }
        
        heading = 360.0 - (heading - RPS_FRONT_ANGLE);
        // makes sure that it turns in an optimal direction.
        float dHeading = angleDifference(heading, angle);

        if (dHeading > 0) {
            hankette.turn(dHeading/DPS_SPEED, -speed);
        } else {
            hankette.turn((-dHeading)/DPS_SPEED, speed);
        }

        heading = getRPS(&xCurr, &yCurr);
        heading = 360.0 - (heading - RPS_FRONT_ANGLE);

        headInError = checkHeading(heading, angle, error);

        if (speed > 40) {
            speed = 40;
        } else if (speed > 20) {
            speed -= 2;
        }
    } while ((xCurr < x - error || xCurr > x + error ||
        yCurr  < y - error || yCurr > y + error ||
        !headInError)/* && speed > 10 */);
}

/**
 * @brief Moves the robot into position to flip the burger.
 * 
 * @param hankette The robot.
 */
void burgerSetup(Robot hankette) {
    hankette.turn(60.0/DPS_SPEED, -SPEED);
    hankette.moveArm(25.0);
    Sleep(0.5);
}

/**
 * @brief Moves the robot from the ice cream levers to the burger flip.
 * 
 * @param hankette The robot.
 */
void moveToBurger(Robot hankette) {
    reportMessage("Moving to burger.");
    hankette.move(FRONT_ANGLE + 30.0, 10.0/IPS_SPEED, SPEED);
    hankette.turn(105.0/DPS_SPEED, SPEED);
    float xDest = 26.5;
    float yDest = 60.0;
    float angleDest = 0.0;

    hankette.move(FRONT_ANGLE + 60.0, 12.0/IPS_SPEED, SPEED);
    moveToSetPos(hankette, xDest, yDest, angleDest, 0.1);
}

/**
 * @brief Slides the ticket to the side.
 * 
 * @param hankette The robot.
 */
void slideTicket(Robot hankette) {
    hankette.move(RIGHT_ANGLE+60.0, 5.0/IPS_SPEED, SPEED);
    hankette.turn(30.0/DPS_SPEED, -SPEED);
}

/**
 * @brief Lines the robot up for travel to the ticket.
 * 
 * @param hankette The robot.
 */
void lineUpToTicket(Robot hankette) {
    reportMessage("Lining up to the ticket.");
    hankette.move(BACK_ANGLE, 1.0, SPEED);
    hankette.turn(150.0/DPS_SPEED, -SPEED);
    hankette.move(FRONT_ANGLE-60.0, 2.5, SPEED);
    reportMessage("back to ticket angle.");
    hankette.move(RIGHT_ANGLE+30.0, 0.5, SPEED);
    hankette.turn(38.0/DPS_SPEED, SPEED);
    reportMessage("Going to ticket.");
    hankette.move(FRONT_ANGLE+60.0, 20.0/IPS_SPEED, SPEED);
}

/**
 * @brief Drives the robot straight from the sink to the robot.
 * 
 * @param hankette The robot.
 */
void sinkToBurger(Robot hankette) {
    reportMessage("Going to burger.");
    hankette.move(FRONT_ANGLE+60.0, 22.0/IPS_SPEED, SPEED);
    hankette.move(FRONT_ANGLE, 1.5, SPEED);
}

/**
 * @brief Drops the tray into the sink.
 * 
 * @param hankette The robot.
 */
void dropTray(Robot hankette) {
    reportMessage("Dropping the tray into the sink.");
    hankette.move(BACK_ANGLE, 2.0/IPS_SPEED, SPEED);
    hankette.toggleRamp();
    Sleep(0.5);
}

/**
 * @brief Lines up to the sink from the top of the ramp.
 * 
 * @param hankette The robot.
 */
void goToSink(Robot hankette) {
    reportMessage("Lining up to sink.");
    hankette.move(LEFT_ANGLE, 11.0/IPS_SPEED, SPEED);

    float xDest = 7.0;
    float yDest = 45.5;
    float angleDest = 0.0;
    moveToSetPos(hankette, xDest, yDest, angleDest, 0.5);
}

/**
 * @brief The robot moves a set distance to where the button should be.
 * 
 * @param hankette The robot.
 * @param ips Inches per second.
 */
void guessToButton(Robot hankette, float ips) {
    reportMessage("Driving to the button.");
    hankette.move(BACK_ANGLE, 10.0/ips, SPEED);
}

/**
 * @brief Moves the robot from the start, up, and then back down the ramp.
 * Requires ips > 0.0
 * 
 * @param hankette The robot's motor configuration.
 * @param ips Inches per second.
 */
void moveUpRamp(Robot hankette) {
    reportMessage("Moving up the ramp.");
    hankette.move(LEFT_ANGLE, 8.5/IPS_SPEED, SPEED);
    hankette.move(FRONT_ANGLE, 42.0/IPS_SPEED, SPEED);
}

/**
 * @brief Moves the robot forward until it detects a light and the aligns with the respective button.
 * 
 * @param hankette The robot.
 * @param ips Inches per second.
 * @return returns the light value detected.
 */
int goTillLight(Robot hankette, float ips) {
    reportMessage("Seeking out light...");
    hankette.moveUnbounded(BACK_ANGLE, SLOW_SPEED);

    waitForLight();
    Sleep(0.2);
    hankette.stop();

    int light = detectLight();
    if (light == 1) {
        reportMessage("Red light!");
        hankette.move(LEFT_ANGLE, BUTTON_DISTANCE/ips, SPEED);
    } else {
        reportMessage("Blue light!");
        hankette.move(RIGHT_ANGLE, BUTTON_DISTANCE/ips, SPEED);
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
    Sleep(0.5);
    float head = RPS.Heading();
    *x = RPS.X();
    *y = RPS.Y();
    float xSum = 0.0;
    float ySum = 0.0;
    float headSum = 0.0;
    const int limit = 10;
    int count = 0;
    for (int i = 0; i < limit; i++) {
        Sleep(0.01);
        head = RPS.Heading();
        *x = RPS.X();
        *y = RPS.Y();
        if (*x > 0.0 && *y > 0.0 && head > 0.0 || 
            *x < 36.0 && *y < 72.0) { // for 0.06 seconds RPS can give a value outside the course
            // char buffer[27];
            // snprintf(buffer, sizeof buffer, "RPS X: %.2f", mx);
            // reportMessage(buffer);
            // snprintf(buffer, sizeof buffer, "RPS Y: %.2f", my);
            // reportMessage(buffer);
            xSum += *x;
            ySum += *y;
            headSum += head;
            count++;
        }
    }

    if (count != 0) {
        *x = xSum / count;
        *y = ySum / count;
        head = headSum / count;
    } else {
        *x = -2.0;
        *y = -2.0;
        head = -2.0;
    }
    

    return head;
}
