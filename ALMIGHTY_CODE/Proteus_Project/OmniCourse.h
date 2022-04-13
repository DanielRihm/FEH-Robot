#include <FEHLCD.h>
#include <FEHRPS.h>
#include <OmniMotion.h>
#include <OmniSensors.h>
#include <stdio.h>

float xSinkOffset = 0.0;
float ySinkOffset = 0.0;
float xTickOffset = 0.0;
float yTickOffset = 0.0;
float xBurgOffset = 0.0;
float yBurgOffset = 0.0;
float xJukeOffset = 0.0;
float yJukeOffset = 0.0;

float dist(float, float, float, float);
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
void goToTicket(Robot);
void slideTicket(Robot);
void pushJukeButton(Robot);

/**
 * @brief Finds the distance between two points.
 * 
 * @param x1 The x coordinate of the first point.
 * @param y1 The y coordinate of the first point.
 * @param x2 The x coordinate of the second point.
 * @param y2 The y coordinate of the second point.
 * @return The distance between the two points.
 */
float dist(float x1, float y1, float x2, float y2) {
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

/**
 * @brief Moves the robot from the bottom of the ramp to the jukebox light.
 * 
 * @param hankette The robot.
 */
void moveToJukeboxLight(Robot hankette) {
    reportMessage("Going to jukebox light.");
    hankette.move(LEFT_ANGLE, 10.0/IPS_SPEED, SPEED);

    float xDest = 9.0;
    float yDest = 14.0;
    float angleDest = 0.0;
    moveToSetPos(hankette, xDest, yDest, angleDest, 0.5);
}

void calibrate() {
    // Jukebox
    reportMessage("Touch for calibrationJ");
    waitForTouch();

    float botLeftX;
    float botLeftY;

    getRPS(&botLeftX, &botLeftY);

    xJukeOffset = DEFAULT_X_JUKEBOX - botLeftX;
    yJukeOffset = DEFAULT_Y_JUKEBOX - botLeftY;
    char buffer[27];
    snprintf(buffer, sizeof buffer, "X Offset: %.2f", xSinkOffset);
    reportMessage(buffer);
    snprintf(buffer, sizeof buffer, "Y Offset: %.2f", ySinkOffset);
    reportMessage(buffer);

    // sink
    reportMessage("Touch for calibrationS");
    waitForTouch();

    getRPS(&botLeftX, &botLeftY);

    xSinkOffset = DEFAULT_X_SINK - botLeftX;
    ySinkOffset = DEFAULT_Y_SINK - botLeftY;

    snprintf(buffer, sizeof buffer, "X Offset: %.2f", xSinkOffset);
    reportMessage(buffer);
    snprintf(buffer, sizeof buffer, "Y Offset: %.2f", ySinkOffset);
    reportMessage(buffer);

    // burger
    reportMessage("Touch for calibrationB");
    waitForTouch();

    getRPS(&botLeftX, &botLeftY);

    xBurgOffset = DEFAULT_X_BURGER - botLeftX;
    yBurgOffset = DEFAULT_Y_BURGER - botLeftY;

    snprintf(buffer, sizeof buffer, "X Offset: %.2f", xBurgOffset);
    reportMessage(buffer);
    snprintf(buffer, sizeof buffer, "Y Offset: %.2f", yBurgOffset);
    reportMessage(buffer);

    // ticket
    reportMessage("Touch for calibrationT");
    waitForTouch();

    getRPS(&botLeftX, &botLeftY);

    xTickOffset = DEFAULT_X_TICKET - botLeftX;
    yTickOffset = DEFAULT_Y_TICKET - botLeftY;

    snprintf(buffer, sizeof buffer, "X Offset: %.2f", xTickOffset);
    reportMessage(buffer);
    snprintf(buffer, sizeof buffer, "Y Offset: %.2f", yTickOffset);
    reportMessage(buffer);
}

/**
 * @brief Pushes the final button.
 * 
 * @param hankette The robot.
 */
void pushFinalButton(Robot hankette) {
    reportMessage("Moving to final button.");
    hankette.turn(90.0/DPS_SPEED, TURN_SPEED);

    float xDest = 29.0; // 28.5
    float yDest = 26.3;
    float angleDest = 0.0;

    moveToSetPos(hankette, xDest, yDest, angleDest, 2.0);
    hankette.moveUnbounded(BACK_ANGLE, SPEED);
}

/**
 * @brief Moves the robot back down the ramp.
 * 
 * @param hankette The robot.
 */
void moveDownRamp(Robot hankette) {
    reportMessage("Going down ramp.");
    hankette.move(BACK_ANGLE, 29.0/IPS_SPEED, SPEED);
}

/**
 * @brief Drives the robot back to the top of the ramp from the burger flip.
 * 
 * @param hankette The robot.
 */
void goToTopRamp(Robot hankette) {
    reportMessage("Returning to ramp.");
    hankette.turn(60.0/DPS_SPEED, SPEED);
    hankette.move(BACK_ANGLE + 30.0, 13.0/IPS_SPEED, SPEED);

    float xDest = 18.4;
    float yDest = 45.5;
    float angleDest = 0.0;
    moveToSetPos(hankette, xDest, yDest, angleDest, 1.5);
}

/**
 * @brief Moves the robot to the chocolate lever.
 * 
 * @param hankette The robot.
 */
void moveToChoco(Robot hankette) {
    reportMessage("Going to chocolate.");
    hankette.move(FRONT_ANGLE + 60.0, 7.0 / IPS_SPEED, SPEED);
    hankette.turn(105.0/DPS_SPEED, -SPEED);

    float xDest = 18.2; // 17
    float yDest = 53.4; // 54
    float angleDest = 105.0;

    moveToSetPos(hankette, xDest, yDest, angleDest, ICE_CREAM_ERROR);
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

    float xDest = 11.6; // 11
    float yDest = 47.8; // 48
    float angleDest = 105.0;

    moveToSetPos(hankette, xDest, yDest, angleDest, ICE_CREAM_ERROR);
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
    hankette.moveArm(120.0);
    Sleep(0.2);
    hankette.moveArm(30.0);
    Sleep(0.2);
    hankette.moveArm(120.0);
    Sleep(0.5);
}

/**
 * @brief Flips the burger tray back to its starting position.
 * 
 * @param hankette The robot.
 */
void fixBurger(Robot hankette) {
    hankette.moveArm(180.0);
    hankette.move(FRONT_ANGLE + 30.0, 2.5/IPS_SPEED, SPEED);
    hankette.move(LEFT_ANGLE + 30.0, 3.4/IPS_SPEED, SPEED);
    hankette.moveArm(120.0);
    Sleep(0.5);
    hankette.move(RIGHT_ANGLE + 30.0, 3.4/IPS_SPEED, SPEED);
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

    float xDest = 15.0; // 14
    float yDest = 51.2; // 51
    float angleDest = 105.0;

    moveToSetPos(hankette, xDest, yDest, angleDest, ICE_CREAM_ERROR);
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
    Sleep(1.0);
    hankette.move(BACK_ANGLE + 30.0, 1.0/IPS_SPEED, SPEED);
    Sleep(1.0);
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
 * @param angle The final angle of the robot in the counterclockwise.
 * @param error The allowed error in the final position.
 * @return Negative if failed.
 */
void moveToSetPos(Robot hankette, float x, float y, float angle, float error) {
    float sinkDist = dist(x, y, DEFAULT_X_SINK, DEFAULT_Y_SINK);
    float tickDist = dist(x, y, DEFAULT_X_TICKET, DEFAULT_Y_TICKET);
    float burgDist = dist(x, y, DEFAULT_X_BURGER, DEFAULT_Y_BURGER);
    float jukeDist = dist(x, y, DEFAULT_X_JUKEBOX, DEFAULT_Y_JUKEBOX);
    if (sinkDist < tickDist && sinkDist < burgDist && sinkDist < jukeDist) {
        x -= xSinkOffset;
        y -= ySinkOffset;
    } else if (tickDist < burgDist && tickDist < jukeDist) {
        x -= xTickOffset;
        y -= yTickOffset;
    } else if (burgDist < jukeDist) {
        x -= xBurgOffset;
        y -= yBurgOffset;
    } else {
        x -= xJukeOffset;
        y -= yJukeOffset;
    }
    
    float xCurr;
    float yCurr;
    float heading;
    int speed = 80;
    int turnSpeed = TURN_SPEED;
    bool headInError;

    Sleep(0.2);
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

        if (speed > 30) {
            speed = 30;
        } else if (speed > 20) {
            speed -= 5;
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
    Sleep(0.3); // 0.5
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
    float xDest = 26.0;
    float yDest = 60.0;
    float angleDest = 0.0;

    hankette.move(FRONT_ANGLE + 60.0, 10.0/IPS_SPEED, SPEED);
    moveToSetPos(hankette, xDest, yDest, angleDest, 0.4);
}

/**
 * @brief Slides the ticket to the side.
 * 
 * @param hankette The robot.
 */
void slideTicket(Robot hankette) {
    hankette.moveArm(65.0);
    Sleep(0.2);
    hankette.move(BACK_ANGLE + 30.0, 3.0/IPS_SPEED, SPEED);

    hankette.turn(1.0/IPS_SPEED, -SPEED);
    hankette.move(BACK_ANGLE + 30.0, 2.0/IPS_SPEED, SPEED);

    hankette.turn(150.0/DPS_SPEED, SPEED);
    hankette.turn(30.0/DPS_SPEED, -SPEED);
    hankette.move(RIGHT_ANGLE - 60.0, 2.0/IPS_SPEED, SPEED);
}

/**
 * @brief Lines the robot up for travel to the ticket.
 * 
 * @param hankette The robot.
 */
void goToTicket(Robot hankette) {
    reportMessage("Going to ticket");
    hankette.move(FRONT_ANGLE+75.0, 18.0/IPS_SPEED, SPEED);
    hankette.turn(150.0/DPS_SPEED, -SPEED);

    float xDest = 28.5;
    float yDest = 26.3;
    float angleDest = 150.0;

    moveToSetPos(hankette, xDest, yDest, angleDest, 0.5);
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
    float yDest = 46.0;
    float angleDest = 0.0;
    moveToSetPos(hankette, xDest, yDest, angleDest, 1.0);
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
    hankette.move(LEFT_ANGLE, 11.0/IPS_SPEED, SPEED);
    hankette.move(FRONT_ANGLE, 42.9/IPS_SPEED, SPEED);
}

/**
 * @brief Moves the robot forward until it detects a light and the aligns with the respective button.
 * 
 * @param hankette The robot.
 * @return returns the light value detected.
 */
void pushJukeButton(Robot hankette) {
    Sleep(0.5);
    int light = detectLight();
    if (light == 1) {
        reportMessage("Red light!");
        hankette.move(LEFT_ANGLE, BUTTON_DISTANCE/IPS_SPEED, SPEED);
    } else {
        reportMessage("Blue light!");
        hankette.move(RIGHT_ANGLE, BUTTON_DISTANCE/IPS_SPEED, SPEED);
    }

    hankette.move(BACK_ANGLE, 10.0/IPS_SPEED, SPEED);
    hankette.move(FRONT_ANGLE, 1.0/IPS_SPEED, SPEED);
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
    Sleep(0.2);
    float head = RPS.Heading();
    *x = RPS.X();
    *y = RPS.Y();
    float xSum = 0.0;
    float ySum = 0.0;
    float headSum = 0.0;
    const int limit = 5;
    const int maxIter = 150;
    int count = 0;
    int maxCount = 0;
    while (count < limit) {
        Sleep(0.01);
        head = RPS.Heading();
        *x = RPS.X();
        *y = RPS.Y();

        // char buffer[27];
        // snprintf(buffer, sizeof buffer, "RPS X: %.2f", *x);
        // reportMessage(buffer);
        // snprintf(buffer, sizeof buffer, "RPS Y: %.2f", *y);
        // reportMessage(buffer);

        if (*x > 0.0 && *y > 0.0 && head > 0.0 && 
            *x < 36.0 && *y < 72.0) { // for 0.06 seconds RPS can give a value outside the course
            xSum += *x;
            ySum += *y;
            headSum += head;
            count++;
        }

        maxCount++;
        if (maxCount > maxIter) {
            break;
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
