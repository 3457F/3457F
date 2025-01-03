#pragma once

#include <functional>
#include <string>
#include "liblvgl/lvgl.h"

enum Alliance {
    RED = 0,
    BLUE = 1,
    ALLIANCE_NONE = 2
};

enum Corner {
    NEGATIVE = -1,
    POSITIVE = 1,
    CORNER_NONE = 2
};

class Auton {
    public:
        std::function<void()> autonCall;

        /**
         * uniquely identifies the auton
         */

        /**
         * what alliance the auton will run for, red or blue
         * 0 - red; 1 - blue
         */
        Alliance alliance;
        // what corner the auton will run in, a negative one or a positive one
        Corner corner;
        std::string name;
        std::string desc;

        /**
         * uniquely identifies *scoring* of the auton
         */

        // how many rings the auton scores on mogo1
        int mogo1;
        // how many rings the auton scores on mogo2
        int mogo2;
        // whether or not the auton scores on the alliance ring
        bool alliance_ring;

        /**
         * constructors
         */

        /**
         * default constructor creates a dummy auton
         */
        Auton();
        Auton(
            std::function<void()> whatAutonCall
            , Alliance which_alliance
            , Corner which_corner
            , std::string what_name
            , std::string what_desc
            , int amt_scored_mogo1
            , int amt_scored_mogo2
            , bool whether_alliance_ring
        );
};

class AutonTable {
    public:
        std::vector<Auton> auton_table;
        void auton_populate(std::vector<Auton> autons);
};