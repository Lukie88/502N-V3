
// _______   _______  _______  __  .__   __.  __  .___________. __    ______   .__   __.      _______.
//|       \ |   ____||   ____||  | |  \ |  | |  | |           ||  |  /  __  \  |  \ |  |     /       |
//|  .--.  ||  |__   |  |__   |  | |   \|  | |  | `---|  |----`|  | |  |  |  | |   \|  |    |   (----`
//|  |  |  ||   __|  |   __|  |  | |  . `  | |  |     |  |     |  | |  |  |  | |  . `  |     \   \    
//|  '--'  ||  |____ |  |     |  | |  |\   | |  |     |  |     |  | |  `--'  | |  |\   | .----)   |   
//|_______/ |_______||__|     |__| |__| \__| |__|     |__|     |__|  \______/  |__| \__| |_______/    
     
// src/auton_utils.cpp
#include "auton_select.hpp"
#include "lemlib/api.hpp"
#include "auton_routines.hpp"
#include "portconfig.hpp"
#include "pros/screen.hpp"
#include "pros/rtos.hpp"
#include "api.h"
#include "robot_afunc.hpp"

void lvgl_task(void* param) {
    while (true) {
        lv_tick_inc(15);
        lv_timer_handler();
        pros::delay(15);}
}

short int selected_auto = 1;      // which specific auton inside a color menu (to be used later)
short int selected_section = 0;   // 0 = red, 1 = blue, 2 = skills, 3 = driving skills
std::string auton_names = "";
bool show_selected_auto = false;


using FunctionPointer = std::function<void()>;
struct auton_data{
    short int catagory;
    short int auton_val;
    std::string func_name;
    FunctionPointer func;
};

const auton_data autolist[8] = {
//   id | cat |    name                |   function pointer
    {0  , 1   ,"red left"              ,auton_routes::red_1},
    {0  , 2   ,"red right"             ,auton_routes::red_2},
    {0  , 3   ,"red 3"                 ,auton_routes::red_3},
    {1  , 4   ,"blue left"             ,auton_routes::blue_1},
    {1  , 5   ,"blue right"            ,auton_routes::blue_2},
    {1  , 6   ,"blue 3"                ,auton_routes::blue_3},
    {2  , 7   ,"skills auton routine"  ,auton_routes::skills_auton_routine},
    {3  , 8   ,"skills driving routine",auton_routes::skills_driving_routine}
};
//HOW TO USE THIS
//first column is the section (red:0, blue:1, skills:2, driving skills:3)
//second column is the auton ID (just add 1 from the previous)
//third column is the name that shows up on the brain menu
//fourth column is the function pointer to the auton routine (just add the function name from auton_routines.hpp)

const int AUTOLIST_SIZE = sizeof(autolist) / sizeof(auton_data);

lv_obj_t *screen_auton;
lv_obj_t *screen_statistics;
lv_obj_t *screen_debugging;

static lv_style_t button_style;



static lv_obj_t * create_text(lv_obj_t * parent,
                              const char * icon,
                              const char * txt,
                              lv_menu_builder_variant_t builder_variant){

    lv_obj_t * obj = lv_menu_cont_create(parent);
    lv_obj_t * img = NULL;
    lv_obj_t * label = NULL;
    if(icon) {
        img = lv_image_create(obj);
        lv_image_set_src(img, icon);
    }

    if(txt) {

        label = lv_label_create(obj);
        lv_label_set_text(label, txt);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_flex_grow(label, 1);
    }

    if(builder_variant == LV_MENU_ITEM_BUILDER_VARIANT_2 && icon && txt) {
        lv_obj_add_flag(img, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
        lv_obj_swap(img, label);
    }
    return obj;
}




void draw_top_bar(){}

// ___________    ____  _______ .__   __. .___________.    __    __       ___      .__   __.  _______   __       _______ .______      
//|   ____\   \  /   / |   ____||  \ |  | |           |   |  |  |  |     /   \     |  \ |  | |       \ |  |     |   ____||   _  \     
//|  |__   \   \/   /  |  |__   |   \|  | `---|  |----`   |  |__|  |    /  ^  \    |   \|  | |  .--.  ||  |     |  |__   |  |_)  |    
//|   __|   \      /   |   __|  |  . `  |     |  |        |   __   |   /  /_\  \   |  . `  | |  |  |  ||  |     |   __|  |      /     
//|  |____   \    /    |  |____ |  |\   |     |  |        |  |  |  |  /  _____  \  |  |\   | |  '--'  ||  `----.|  |____ |  |\  \----.
//|_______|   \__/     |_______||__| \__|     |__|        |__|  |__| /__/     \__\ |__| \__| |_______/ |_______||_______|| _| `._____|
                                                                                                                                    
void run_selected_auton() {
    if (selected_auto >= 1 && selected_auto <= AUTOLIST_SIZE) {
        if (selected_section == 0){set_sorter_alliance(SorterAlliance::Red);}
        else if (selected_section == 1){set_sorter_alliance(SorterAlliance::Blue);}
        color_selected = true;
        FunctionPointer runfunc = autolist[selected_auto-1].func;
        runfunc();
    }else {
        color_selected = false;
        auton_routes::unselected_auton_routine();
    }}


std::vector<int> get_auto_ids(short int section){
    std::vector<int> list_of_ids;
    for (int i = 0; i < 8; i++){
        if (autolist[i].catagory == section){
            list_of_ids.push_back(autolist[i].auton_val);}}
    return list_of_ids;
}

const char *get_auton_name(int group_param, int routine_param) {
    // Iterate through every element in the autolist array
    for (int i = 0; i < AUTOLIST_SIZE; i++) {
        if (autolist[i].catagory == group_param && autolist[i].auton_val == routine_param) {
            return autolist[i].func_name.c_str();
        }
    }
    return "UNKNOWN_AUTON_ROUTINE"; //panik
}

//     _______.  ______ .______       _______  _______ .__   __.     __          ___   ____    ____  ______    __    __  .___________.    _______.
//    /       | /      ||   _  \     |   ____||   ____||  \ |  |    |  |        /   \  \   \  /   / /  __  \  |  |  |  | |           |   /       |
//   |   (----`|  ,----'|  |_)  |    |  |__   |  |__   |   \|  |    |  |       /  ^  \  \   \/   / |  |  |  | |  |  |  | `---|  |----`  |   (----`
//    \   \    |  |     |      /     |   __|  |   __|  |  . `  |    |  |      /  /_\  \  \_    _/  |  |  |  | |  |  |  |     |  |        \   \    
//.----)   |   |  `----.|  |\  \----.|  |____ |  |____ |  |\   |    |  `----./  _____  \   |  |    |  `--'  | |  `--'  |     |  |    .----)   |   
//|_______/     \______|| _| `._____||_______||_______||__| \__|    |_______/__/     \__\  |__|     \______/   \______/      |__|    |_______/    
                                                                                                                                                
void screen_debugging(){}
void screen_autons(){}
void screen_statistics(){}




void brain_menu(void){

}