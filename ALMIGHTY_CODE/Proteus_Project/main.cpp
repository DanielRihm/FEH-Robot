#include <FEHLCD.h>
#include <OmniCourse.h>
#include <OmniUtility.h>

void test(Robot);
void testForward(Robot, float);
void testSideToSide(Robot, float);
void testSpin(Robot, float);
void displayRPS();

int main(void)
{
    Robot wall_E6;
    RPS.InitializeTouchMenu();
    setLCD();
    reportMessage("Waiting...");
    waitForTouch();
    waitForLight();
    moveUpRamp(wall_E6);
    moveToBurger(wall_E6);
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
        Sleep(0.1);
    }
}

/**
 * @brief Spins the robot clockwise and counterclockwise.
 * 
 * @param wall_E6 The robot.
 * @param time The amount of time to spin for.
 */
void testSpin(Robot wall_E6, float time) {
    reportMessage("Waiting...");
    waitForTouch();
    wall_E6.turn(time, SPEED);
    reportMessage("Waiting again...");
    waitForTouch();
    wall_E6.turn(time, -SPEED);
    reportMessage("Done");
}

/**
 * @brief Moves the robot forward and backwards.
 * 
 * @param wall_E6 The robot.
 * @param time The amout of time to move for.
 */
void testForward(Robot wall_E6, float time) {
    reportMessage("Waiting...");
    waitForTouch();
    wall_E6.move(FRONT_ANGLE, time, SPEED);
    reportMessage("Yay! :)");
    reportMessage("Waiting...");
    waitForTouch();
    wall_E6.move(BACK_ANGLE, time, SPEED);
    reportMessage("finished");
}

/**
 * @brief Moves the robot to the left and then to the right.
 * 
 * @param wall_E6 The robot.
 * @param time The amount of time to move for.
 */
void testSideToSide(Robot wall_E6, float time) {
    reportMessage("Waiting...");
    waitForTouch();
    wall_E6.move(LEFT_ANGLE, time, SPEED);
    reportMessage("Yay! :)");
    reportMessage("Waiting...");
    waitForTouch();
    wall_E6.move(RIGHT_ANGLE, time, SPEED);
    reportMessage("finished");
}

/**
 * @brief Tests the full range of motion of the robot.
 * 
 * @param wall_E6 The robot.
 */
void test(Robot wall_E6) {
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
    setLCD();
    reportMessage("Moving forward.");
    wall_E6.move(FRONT_ANGLE, 1.0, SPEED);
    reportMessage("Moving right.");
    wall_E6.move(RIGHT_ANGLE, 1.0, SPEED);
    reportMessage("Moving left.");
    wall_E6.move(LEFT_ANGLE, 1.0, SPEED);
    reportMessage("Moving backwards.");
    wall_E6.move(BACK_ANGLE, 1.0, SPEED);

    testForward(wall_E6, 4.0);
    testSideToSide(wall_E6, 4.0);
}
