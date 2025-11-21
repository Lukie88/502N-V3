#pragma once



struct auton_data{
    short int catagory;
    short int auton_val;
    std::string func_name;
};

auton_data autolist[8][3] = {
    {0, 1,"red_1"},
    {0, 2,"red_2"},
    {0, 3,"red_3"},
    {1, 4,"blue_1"},
    {1, 5,"blue_2"},
    {1, 6,"blue_3"},
    {2, 7,"skills_auton_routine"},
    {3, 8,"skills_driving_routine"}
};

// All auton routines live in the auton namespace
namespace auton_routes {

    // Game autons
    void red_1();
    void red_2();
    void red_3();
    void blue_1();
    void blue_2();
    void blue_3();

    // Skills
    void skills_auton_routine();
    void skills_driving_routine();
}
