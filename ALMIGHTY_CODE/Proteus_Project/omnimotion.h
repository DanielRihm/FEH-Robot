#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <math.h>

#define VOLTAGE 6.0
#define PI 3.141592
#define MOTOR_FRONT_ANGLE 270.0
#define MOTOR_LEFT_ANGLE 150.0
#define MOTOR_RIGHT_ANGLE 30.0
#define FRONT_FACTOR 0.9
#define LEFT_FACTOR 1.0
#define RIGHT_FACTOR 0.9

FEHMotor frontM(FEHMotor::Motor0, VOLTAGE);
FEHMotor leftM(FEHMotor::Motor1, VOLTAGE);
FEHMotor rightM(FEHMotor::Motor2, VOLTAGE);

class Robot;

class Robot {
    private:
        int currAng;
        void setMotorPercent(float, float, float);
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
    setMotorPercent(speed, speed, speed);
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

    setMotorPercent(motorFS, motorLS, motorRS);
}

/**
 * @brief Stops the robot from moving.
 * 
 */
void Robot::stop() {
    setMotorPercent(0,0,0);
}

/**
 * @brief Sets all motors' percents to the specified amounts
 * 
 * @param fSpeed The front motor's speed
 * @param lSpeed The left motor's speed
 * @param rSpeed The right motor's speed
 */
void Robot::setMotorPercent(float fSpeed, float lSpeed, float rSpeed) {
    frontM.SetPercent(fSpeed * FRONT_FACTOR);
    leftM.SetPercent(lSpeed * LEFT_FACTOR);
    rightM.SetPercent(rSpeed * RIGHT_FACTOR);
}