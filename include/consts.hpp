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
#define HORIZ_TRACK_WHEEL 9

// arm ports
#define ARM_PORT 12
#define ARM_ROT_PORT 11 // reversed in budget code, but rotation sensor flipped on new bot, so
                        // reversed the reversion to make it normal!

// color sensor ports
#define COLOR_PORT 15

// intake ports
#define INTAKE_PORT 18
#define FLOATING_PORT 1

// distance sensor ports
#define DIST_LEFT 1
#define DIST_RIGHT 1
#define DIST_FRONT 1
#define DIST_BACK 1

// pneumatics ports
#define MOGO_MECH_PORT 'A'
#define DOINKER_PORT 'B'
#define INTAKE_LIFT_PORT 'C'
#define RUSH_MECH_PORT 'D'
#define INTAKE_LIM_SWITCH_PORT 'E'
// ---

// dt PID
#define DT_LATERAL_P 10
#define DT_LATERAL_D 3

#define DT_ANGULAR_P 7
#define DT_ANGULAR_D 63

// arm PID
// 1 15 before
// 2 20 too high
#define ARM_P 1.5 // 2
#define ARM_D 1 // 5; 15 works too slow though