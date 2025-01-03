#include "Auton.hpp"

Auton::Auton() {
    autonCall = nullptr;
    alliance = Alliance::ALLIANCE_NONE;
    corner = Corner::CORNER_NONE;
    name = "";
    desc = "";
    mogo1 = 0;
    mogo2 = 0;
    alliance_ring = false;
}

Auton::Auton(
    std::function<void()> whatAutonCall
    , Alliance which_alliance
    , Corner which_corner
    , std::string what_name
    , std::string what_desc
    , int amt_scored_mogo1
    , int amt_scored_mogo2
    , bool whether_alliance_ring
) {
    autonCall = whatAutonCall;
    alliance = which_alliance;
    corner = which_corner;
    name = what_name;
    desc = what_desc;
    mogo1 = amt_scored_mogo1;
    mogo2 = amt_scored_mogo2;
    alliance_ring = whether_alliance_ring;
}

// ----

void AutonTable::auton_populate(std::vector<Auton> autons) {
    auton_table = {};
    auton_table.assign(autons.begin(), autons.end());
}