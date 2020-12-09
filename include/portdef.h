#ifndef PORT_DEF_H_
#define PORT_DEF_H_

//Drive base motors - left side of chassis
#define LEFT_MOTOR_FRONT  5
#define LEFT_MOTOR_BACK  15

//Drive base motors - right side of chassis
#define RIGHT_MOTOR_FRONT  6
#define RIGHT_MOTOR_BACK  16     

// ------------ GYRO / IMU --------------------------
#define IMU_PORT 11

// ----------- Radio and Controllers ---------------
#define VEX_RADIO 21

// ------------ Vision Sensors ---------------------
#define VISION_PORT 14

// ----------- LEGACY port assignments ------------
#define QUAD_LEFT_TOP_PORT 3      // Odometer encoders left side
#define QUAD_LEFT_BOTTOM_PORT 4   // NOTICE becasue of cable length ilogical ports

#define QUAD_RIGHT_TOP_PORT 1     // Odometer Encoders right side
#define QUAD_RIGHT_BOTTOM_PORT 2

#endif
