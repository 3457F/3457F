#pragma "once"

void print_robot_pos(void* chassisVoid);

// test autons
void test_auton();
void move_forward();

/** BLANK auton */
void no_auton();

//red neg
void red_neg_wp();
void red_negative_elims();

//red pos
void red_rush();
void red_pos_wp();

//blue neg
void blue_negative_wp();
void blue_negative_elims();

//blue pos
void blue_rush();
void blue_pos_wp();



//erm old autons!
/** OLD AUTON -- DON'T USE */
void red_negative_awp();
/** OLD AUTON -- ALSO DON'T USE */
void red_neg();
void red_positive_awp_nostack();
void red_negative_awp_old();
void red_pos_with_corner();
void red_pos_archived();
void blue_safe();
void blue_negative_awp();