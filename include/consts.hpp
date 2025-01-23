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

// arm ports
#define ARM_PORT 12
#define ARM_ROT_PORT 11 // reversed in budget code, but rotation sensor flipped on new bot, so
                        // reversed the reversion to make it normal!

// intake port
#define INTAKE_PORT 18

// pneumatics ports
#define MOGO_MECH_PORT 'A'
#define DOINKER_PORT 'B'

// ---

// dt PID
#define DT_LATERAL_P 8
#define DT_LATERAL_D 21

#define DT_ANGULAR_P 3.25
#define DT_ANGULAR_D 26

// arm PID
#define ARM_P 2
#define ARM_D 5