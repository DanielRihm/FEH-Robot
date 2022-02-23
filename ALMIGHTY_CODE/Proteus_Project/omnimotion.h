#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <math.h>

#define VOLTAGE 6.0
#define PI 3.141592
#define MOTOR_FRONT_ANGLE 270.0
#define MOTOR_LEFT_ANGLE 150.0
#define MOTOR_RIGHT_ANGLE 30.0

FEHMotor frontM(FEHMotor::Motor0, VOLTAGE);
FEHMotor leftM(FEHMotor::Motor1, VOLTAGE);
FEHMotor rightM(FEHMotor::Motor2, VOLTAGE);

class Robot;

class Robot {
    private:
        int currAng;
    public:
        Robot();
        Robot(int);
        void move(float, float, int);
        void turn(float, int);
        void stop();
        void moveUnbounded(float, int);
};

Robot::Robot() {
    currAng = 0;
}

Robot::Robot(int angle) {
    currAng = angle;
}

/**
 * @brief Turns the robot for the specified amount of time at the specified speed.
 * Requires time > 0 AND -50 < speed < 50.
 * 
 * @param time The time to turn for.
 * @param speed The speed at which to turn. Negative values will turn the opposite direction.
 */
void Robot::turn(float time, int speed) {
    frontM.SetPercent(speed);
    leftM.SetPercent(speed);
    rightM.SetPercent(speed);
    Sleep(time);
    stop();
}

/**
 * @brief Moves the robot in the specified direction at the specified speed.
 * Requires time > 0 AND 0 < speed < 25 AND 0 < angle < 360.
 * 
 * @param angle The angle at which the robot moves at.
 * @param time The amount of time the robot should move for.
 * @param speed The speed at which the robot should move.
 */
void Robot::move(float angle, float time, int speed) {
    moveUnbounded(angle, speed);
    Sleep(time);
    stop();
}

/**
 * @brief Moves the robot in the specified direction at the specified speed.
 * Requires 0 < speed < 25 AND 0 < angle < 360.
 * 
 * @param angle The angle at which the robot moves at.
 * @param speed The speed at which the robot should move.
 */
void Robot::moveUnbounded(float angle, int speed) {
    float motorFS = speed * cos(MOTOR_FRONT_ANGLE * PI / 180.0 - angle * PI / 180.0);
    float motorLS = speed * cos(MOTOR_LEFT_ANGLE * PI / 180.0 - angle * PI / 180.0);
    float motorRS = speed * cos(MOTOR_RIGHT_ANGLE * PI / 180.0 - angle * PI / 180.0);

    frontM.SetPercent(motorFS);
    leftM.SetPercent(motorLS);
    rightM.SetPercent(motorRS);
}

/**
 * @brief Stops the robot from moving.
 * 
 */
void Robot::stop() {
    frontM.SetPercent(0);
    leftM.SetPercent(0);
    rightM.SetPercent(0);
}