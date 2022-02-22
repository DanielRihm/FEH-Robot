#include <FEHMotor.h>
#include <FEHIO.h>

#define VOLTAGE 9.0

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
        void move(int, float, int);
        void turn(float, int);
};

Robot::Robot() {
    currAng = 0;
}

Robot::Robot(int angle) {
    currAng = angle;
}
