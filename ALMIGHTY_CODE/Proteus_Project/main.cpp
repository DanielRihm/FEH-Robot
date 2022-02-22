#include <FEHLCD.h>
#include <OmniMotion.h>
#include <OmniSensors.h>

#define RIGHT_ANGLE 90.0
#define FRONT_ANGLE 0.0
#define LEFT_ANGLE 270.0
#define BACK_ANGLE 180.0

void test(Robot);

int main(void)
{
    Robot wall_E6(0);
    test(wall_E6);
}

void test(Robot wall_E6) {
    LCD.Clear();
    wall_E6.turn(5.0, 10);
    for (int i = 0; i < 360; i++) {
        wall_E6.move(i, 0.02, 10);
        LCD.WriteLine("Moving at angle " + i);
    }

    LCD.Clear();
    wall_E6.move(FRONT_ANGLE, 1.0, 10);
    LCD.WriteLine("Moving forward.");
    wall_E6.move(RIGHT_ANGLE, 1.0, 10);
    LCD.WriteLine("Moving right.");
    wall_E6.move(LEFT_ANGLE, 1.0, 10);
    LCD.WriteLine("Moving left.");
    wall_E6.move(BACK_ANGLE, 1.0, 10);
    LCD.WriteLine("Moving backwards.");

    LCD.WriteLine("Waiting...");
    float x, y;
    while (!LCD.Touch(&x,&y));
    wall_E6.move(FRONT_ANGLE, 1.0, 10);
}