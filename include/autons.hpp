#pragma "once"

void print_robot_pos(void* chassisVoid);

// test autons
void test_auton();
void move_forward();

/** BLANK auton */
void no_auton();

// red negative
void red_neg_2();
void red_negative_elims();

/** OLD AUTON -- DON'T USE */
void red_negative_awp();
/** OLD AUTON -- ALSO DON'T USE */
void red_neg();


// red positive
void red_rush();

void red_pos();
void red_pos_with_corner();
void red_positive_awp_nostack();

// blue negative
void blue_negative_awp();

void blue_safe();

// blue positive
void blue_rush();

void blue_pos_wp();