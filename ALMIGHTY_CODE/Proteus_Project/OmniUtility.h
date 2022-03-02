#include <FEHLCD.h>

#define RIGHT_ANGLE 90.0
#define FRONT_ANGLE 0.0
#define LEFT_ANGLE 270.0
#define BACK_ANGLE 180.0
#define SPEED 45
#define SLOW_SPEED 25
#define FAST_SPEED 60
#define IPS_SPEED 3.9375
#define BUTTON_DISTANCE 2.0

// OmniMotor constants
#define VOLTAGE 6.0
#define PI 3.141592
#define MOTOR_FRONT_ANGLE 270.0
#define MOTOR_LEFT_ANGLE 150.0
#define MOTOR_RIGHT_ANGLE 30.0
#define NEG_FRONT_FACTOR 0.9
#define NEG_LEFT_FACTOR 1.0
#define NEG_RIGHT_FACTOR 1.0
#define POS_FRONT_FACTOR 0.9
#define POS_LEFT_FACTOR 0.9
#define POS_RIGHT_FACTOR 0.9

//OmniSensors constants
#define RED_LIGHT_CUTOFF 0.5
#define BLUE_LIGHT_CUTOFF 1.0
#define RED_LIGHT 1
#define BLUE_LIGHT 2
#define NO_LIGHT 0
#define LINE_CUTOFF 2.0

//Utility constants
#define LCD_LEFT_MOTOR 0
#define LCD_RIGHT_MOTOR 1
#define LCD_FRONT_MOTOR 2

// variable storing the current column that the actions are at.
int currentColumn = 4;

void setLCD();
void reportSpeed(int, float);
void reportMessage(char[]);

/**
 * @brief Sets the inital screen.
 * 
 */
void setLCD() {
    LCD.Clear();
    LCD.WriteRC(" Left Motor:",0,0);
    LCD.WriteRC("Right Motor:",0,1);
    LCD.WriteRC("Front Motor:",0,2);
    LCD.WriteRC("Messages:",0,3);
}

/**
 * @brief Reports the specified speed for the specified motor to the screen.
 * 
 * @param motor The motor: 0 is left, 1 is right, 2 is front.
 * @param speed The speed at which the motor is running.
 */
void reportSpeed(int motor, float speed) {
    LCD.WriteRC("             ", 13, motor);
    LCD.WriteRC(speed, 13, motor);
}

/**
 * @brief Reports the specified message to the screen.
 * 
 * @param message the message to be reported.
 */
void reportMessage(char message[]) {
    if (currentColumn < 27) {
        LCD.WriteRC("                          ", 0, currentColumn);
        LCD.WriteRC(message, 0, currentColumn);
        currentColumn++;
    } else {
        currentColumn = 4;
        reportMessage(message);
    }
}
