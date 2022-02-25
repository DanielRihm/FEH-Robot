#include <FEHLCD.h>
#include <OmniMotion.h>
#include <OmniSensors.h>
#include <OmniCourse.h>
#include <OmniUtility.h>

// organization is vague and needs to be improved
// utility, motion, and sensors are fine, however OmniCourse
// is strange and what methods fall under it vs motion and sensors is
// vague and needs to be clarified in the future

void test(Robot);
void testForward(Robot, float);
void testSideToSide(Robot, float);

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
