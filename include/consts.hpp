/**
 * PORTS
 */

// dt ports
#define LEFT_BACK_PORT -19
#define LEFT_MID_PORT -20
#define LEFT_FRONT_PORT -1

#define RIGHT_BACK_PORT 2
#define RIGHT_MID_PORT 3
#define RIGHT_FRONT_PORT 10

// tracking wheel port
#define HORIZ_TRACK_WHEEL_PORT 9
#define VERTICAL_TRACK_WHEEL_PORT -4 

// arm ports
#define ARM_PORT 12
#define ARM_ROT_PORT 11 // reversed in budget code, but rotation sensor flipped on new bot, so
                        // reversed the reversion to make it normal!

// color sensor ports
// #define COLOR_PORT 15

// intake ports
#define INTAKE_COLOR_SENSOR_PORT 15
#define INTAKE_DIST_SENSOR_PORT 5
#define INTAKE_PORT -7
#define FLOATING_PORT 1

// distance sensor ports -- ALL REMOVED
#define DIST_LEFT 8
#define DIST_RIGHT 14
#define DIST_FRONT 7
#define DIST_BACK 13

// pneumatics ports
#define MOGO_MECH_PORT 'A'
// TODO: probs need to swap so in intake perspective
#define LEFT_DOINKER_PORT 'B'
#define RIGHT_DOINKER_PORT 'C'
#define INTAKE_LIFT_PORT 'D'
// ---

// dt PID -- 600rpm dt (mecha)
// #define DT_LATERAL_P 10 // 10
// #define DT_LATERAL_D 3 // 3.5

// #define DT_ANGULAR_P 2 // 7
// #define DT_ANGULAR_D 10 // 62

// dt PID -- 450rpm dt (iowa -- taken from budget bot)
#define DT_LATERAL_P 8
#define DT_LATERAL_D 21

#define DT_ANGULAR_P 3.25
#define DT_ANGULAR_D 26

// arm PID
// 1 15 before
// 2 20 too high
#define ARM_P 1.5 // 2
#define ARM_D 1 // 5; 15 works too slow though