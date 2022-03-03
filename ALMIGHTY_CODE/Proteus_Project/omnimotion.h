#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <math.h>
#include <FEHLCD.h>
#include <OmniUtility.h>
#include <FEHServo.h>

FEHMotor frontM(FEHMotor::Motor0, VOLTAGE);
FEHMotor leftM(FEHMotor::Motor1, VOLTAGE);
FEHMotor rightM(FEHMotor::Motor2, VOLTAGE);
FEHServo rampDoor(FEHServo::Servo0);

class Robot;

class Robot {
    private:
        void setMotorPercent(float, float, float);
        bool rampClosed;
    public:
        Robot();
        void move(float, float, int);
        void turn(float, int);
        void stop();
        void moveUnbounded(float, int);
        void toggleRamp();
        bool isRampClosed();
};

Robot::Robot() {
    rampDoor.SetMin(RAMP_DOOR_MIN);
    rampDoor.SetMax(RAMP_DOOR_MAX);
    rampDoor.SetDegree(90.0);
    rampClosed = true;
}

/**
 * @brief Toggles the state of the ramp.
 * 
 */
void Robot::toggleRamp() {
    if (rampClosed) {
        rampDoor.SetDegree(0.0);
        rampClosed = false;
    } else {
        rampDoor.SetDegree(90.0);
        rampClosed = true;
    }
}

/**
 * @brief Tells whether the ramp is closed or not.
 * 
 * @return true if the ramp is closed. false otherwise.
 */
bool Robot::isRampClosed() {
    return rampClosed;
}

/**
 * @brief Turns the robot for the specified amount of time at the specified speed.
 * Requires time > 0 AND -50 < speed < 50. Turns counterclockwise.
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
    if (fSpeed < 0.0) {
        frontM.SetPercent(fSpeed * NEG_FRONT_FACTOR);
        reportSpeed(LCD_FRONT_MOTOR, fSpeed * NEG_FRONT_FACTOR);
    } else {
        frontM.SetPercent(fSpeed * POS_FRONT_FACTOR);
        reportSpeed(LCD_FRONT_MOTOR, fSpeed * POS_FRONT_FACTOR);
    }

    if (lSpeed < 0.0) {
        leftM.SetPercent(lSpeed * NEG_LEFT_FACTOR);
        reportSpeed(LCD_LEFT_MOTOR, lSpeed * NEG_LEFT_FACTOR);
    } else {
        leftM.SetPercent(lSpeed * POS_LEFT_FACTOR);
        reportSpeed(LCD_LEFT_MOTOR, lSpeed * POS_LEFT_FACTOR);
    }

    if (rSpeed < 0.0) {
        rightM.SetPercent(rSpeed * NEG_RIGHT_FACTOR);
        reportSpeed(LCD_RIGHT_MOTOR, rSpeed * NEG_RIGHT_FACTOR);
    } else {
        rightM.SetPercent(rSpeed * POS_RIGHT_FACTOR);
        reportSpeed(LCD_RIGHT_MOTOR, rSpeed * POS_RIGHT_FACTOR);
    }
}