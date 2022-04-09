#include <FEHLCD.h>
#include <OmniCourse.h>
#include <OmniUtility.h>

void test(Robot);
void testForward(Robot, float);
void testSideToSide(Robot, float);
void testSpin(Robot, float);
void displayRPS();
void displayLight();

int main(void)
{
    Robot hankette;
    RPS.InitializeTouchMenu();
    beginDebug();
    setLCD();
    calibrate();
    reportMessage("Waiting...");
    waitForTouch();
    waitForLight();
    hankette.moveArm(150.0);
    moveUpRamp(hankette);
    goToSink(hankette);
    dropTray(hankette);
    int iceCream = RPS.GetIceCream();
    if (iceCream == 0) {
        moveToVanil(hankette);
    } else if (iceCream == 1) {
        moveToTwist(hankette);
    } else {
        moveToChoco(hankette);
    }

    iceCreamFlip(hankette);
    unFlipLever(hankette);
    moveToBurger(hankette);
    burgerSetup(hankette);
    flipBurger(hankette);
    fixBurger(hankette);
    goToTopRamp(hankette);
    moveDownRamp(hankette);
    moveToJukeboxLight(hankette);
    pushJukeButton(hankette);
    goToTicket(hankette);
    slideTicket(hankette);
    pushFinalButton(hankette);
    ceaseDebug();
}

/**
 * @brief Displays the currently detected light.
 * 
 */
void displayLight() {
    LCD.Clear();
    while (true) {
        int light = detectLight();
        if (light == RED_LIGHT) {
            LCD.WriteRC("RED LIGHT ", 0, 0);
        } else if (light == BLUE_LIGHT) {
            LCD.WriteRC("BLUE LIGHT", 0, 0);
        } else {
            LCD.WriteRC("NOTHING   ", 0, 0);
        }

        LCD.WriteRC(cds.Value(), 1, 0);
    }
}

/**
 * @brief Constantly writes the current position to the screen.
 * 
 */
void displayRPS() {
    LCD.Clear();
    while(true) {
        LCD.WriteRC(RPS.X(), 0, 0);
        LCD.WriteRC(RPS.Y(), 1, 0);
        LCD.WriteRC(RPS.Heading(), 2, 0);
        Sleep(0.1);
    }
}

/**
 * @brief Spins the robot clockwise and counterclockwise.
 * 
 * @param hankette The robot.
 * @param time The amount of time to spin for.
 */
void testSpin(Robot hankette, float time) {
    reportMessage("Waiting...");
    waitForTouch();
    hankette.turn(time, TURN_SPEED);
    reportMessage("Waiting again...");
    waitForTouch();
    hankette.turn(time, -TURN_SPEED);
    reportMessage("Done");
}

/**
 * @brief Moves the robot forward and backwards.
 * 
 * @param hankette The robot.
 * @param time The amout of time to move for.
 */
void testForward(Robot hankette, float time) {
    reportMessage("Waiting...");
    waitForTouch();
    hankette.move(FRONT_ANGLE, time, SPEED);
    reportMessage("Yay! :)");
    reportMessage("Waiting...");
    waitForTouch();
    hankette.move(BACK_ANGLE, time, SPEED);
    reportMessage("finished");
}

/**
 * @brief Moves the robot to the left and then to the right.
 * 
 * @param hankette The robot.
 * @param time The amount of time to move for.
 */
void testSideToSide(Robot hankette, float time) {
    reportMessage("Waiting...");
    waitForTouch();
    hankette.move(LEFT_ANGLE, time, SPEED);
    reportMessage("Yay! :)");
    reportMessage("Waiting...");
    waitForTouch();
    hankette.move(RIGHT_ANGLE, time, SPEED);
    reportMessage("finished");
}

/**
 * @brief Tests the full range of motion of the robot.
 * 
 * @param hankette The robot.
 */
void test(Robot hankette) {
    hankette.turn(5.0, TURN_SPEED);
    for (int i = 0; i < 360; i++) {
        hankette.moveUnbounded(i, SPEED);
        if (i % 5 == 0) {
            LCD.Clear();
            LCD.Write("Moving at angle ");
            LCD.WriteLine(i);
        }
        Sleep(0.01);
    }

    Sleep(1.0);
    setLCD();
    reportMessage("Moving forward.");
    hankette.move(FRONT_ANGLE, 1.0, SPEED);
    reportMessage("Moving right.");
    hankette.move(RIGHT_ANGLE, 1.0, SPEED);
    reportMessage("Moving left.");
    hankette.move(LEFT_ANGLE, 1.0, SPEED);
    reportMessage("Moving backwards.");
    hankette.move(BACK_ANGLE, 1.0, SPEED);

    testForward(hankette, 4.0);
    testSideToSide(hankette, 4.0);
}
