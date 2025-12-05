
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


//lvgl state definitions because it doesn't work for us
#define LV_STATE_DEFAULT   0x0000
#define LV_STATE_CHECKED   0x0001
#define LV_STATE_FOCUSED   0x0002
#define LV_STATE_FOCUS_KEY 0x0004
#define LV_STATE_EDITED    0x0008
#define LV_STATE_HOVER     0x0010
#define LV_STATE_PRESSED   0x0020
#define LV_STATE_SCROLLED  0x0040
#define LV_STATE_DISABLED  0x0080
#define LV_STATE_ANY       0xFFFF


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
//   cat| id  |    name                |   function pointer
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
lv_obj_t *screen_stats;
lv_obj_t *screen_debug;

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
typedef struct {
    lv_coord_t width;
    lv_coord_t height;
    lv_coord_t radius;
    lv_color_t base_color;
    lv_color_t glow_color;
    lv_coord_t glow_size;
} rect_config_t;namespace custom_graphics {
static void rect_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED) {
        // Print a message to the console to confirm input is working
        LV_LOG_USER("Button Clicked: Input Handler is Active!");
        
        // Example action: Toggle the background color on click
        lv_color_t current_color = lv_obj_get_style_bg_color(obj, LV_PART_MAIN);
        if (current_color == lv_color_hex(0x8B0000)) {
            // If it's red (clicked state), revert to blue
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3B82F6), LV_STATE_DEFAULT);
        } else {
            // Otherwise, set to red for visual feedback
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x8B0000), LV_STATE_DEFAULT);
        }
    }
}

lv_obj_t *create_glowing_rounded_rect(lv_obj_t *parent, const rect_config_t *cfg)
{
    lv_obj_t *obj = lv_obj_create(parent);
    
    // 1. Basic Geometry and Layout
    lv_obj_set_size(obj, cfg->width, cfg->height);
    lv_obj_center(obj);

    // 2. Define the appearance for the default state
    static lv_style_t style_default;
    lv_style_init(&style_default);
    
    // Apply customizable corner rounding (LV_PART_MAIN)
    lv_style_set_radius(&style_default, cfg->radius);
    
    // Set base background color
    lv_style_set_bg_color(&style_default, cfg->base_color);
    lv_style_set_bg_opa(&style_default, LV_OPA_COVER);
    
    // Remove default border and padding so the drawing fits the object area
    lv_style_set_border_width(&style_default, 0);
    lv_style_set_pad_all(&style_default, 0); 
    
    lv_obj_add_style(obj, &style_default, LV_STATE_DEFAULT);


    // 3. Define the glow effect for the hover state
    static lv_style_t style_hover;
    lv_style_init(&style_hover);
    
    // Configure the Shadow to act as a "glow" (LV_PART_MAIN, LV_STATE_HOVER)
    lv_style_set_shadow_color(&style_hover, cfg->glow_color);
    lv_style_set_shadow_width(&style_hover, cfg->glow_size);
    // Setting spread to 0 makes the shadow blur away from the object edge.
    lv_style_set_shadow_spread(&style_hover, 0); 
    // Center the shadow (no offset)
    lv_style_set_shadow_offset_x(&style_hover, 0);
    lv_style_set_shadow_offset_y(&style_hover, 0);
    
    // Optional: Add a transition for a smooth fade-in/out glow
    // LV_TRANSFORM_PROP_SHADOW_COLOR and LV_TRANSFORM_PROP_SHADOW_WIDTH define what properties to transition
    static const lv_style_prop_t trans_props[] = {
        LV_STYLE_SHADOW_COLOR, LV_STYLE_SHADOW_WIDTH, 0 
    };
    static lv_style_transition_dsc_t trans_dsc;
    lv_style_transition_dsc_init(&trans_dsc, trans_props, lv_anim_path_ease_out, 300, 0, NULL);
    lv_style_set_transition(&style_hover, &trans_dsc);
    
    lv_obj_add_style(obj, &style_hover, 0x0010);

    return obj;
}


// --- Demonstration Example ---

void lv_example_custom_glow(void)
{
    // Ensure you have a screen to work on
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1F2937), LV_STATE_DEFAULT);

    // --- Example 1: Large Blue Glow Button ---
    rect_config_t cfg1 = {
        .width = 200,
        .height = 100,
        .radius = 20, // Medium rounding
        .base_color = lv_color_hex(0x3B82F6), // Blue background
        .glow_color = lv_color_hex(0x93C5FD), // Light blue glow
        .glow_size = 25 // Large glow radius
    };
    lv_obj_t *rect1 = create_glowing_rounded_rect(scr, &cfg1);
    lv_obj_set_align(rect1, LV_ALIGN_CENTER);
    
    // Add a label
    lv_obj_t *label1 = lv_label_create(rect1);
    lv_label_set_text(label1, "Hover Me (Blue)");
    lv_obj_set_style_text_color(label1, lv_color_white(), LV_STATE_DEFAULT);
    lv_obj_center(label1);
    

    // --- Example 2: Small Rounded Green Glow ---
    rect_config_t cfg2 = {
        .width = 150,
        .height = 70,
        .radius = 999, // Max rounding for pill shape
        .base_color = lv_color_hex(0x10B981), // Green background
        .glow_color = lv_color_hex(0xA7F3D0), // Light green glow
        .glow_size = 15 // Smaller glow radius
    };
    lv_obj_t *rect2 = create_glowing_rounded_rect(scr, &cfg2);
    
    // Add a label
    lv_obj_t *label2 = lv_label_create(rect2);
    lv_label_set_text(label2, "Pill Shape (Green)");
    lv_obj_set_style_text_color(label2, lv_color_white(), LV_STATE_DEFAULT);
    lv_obj_center(label2);
}

void draw_background(){}
}
void screen_debugging(){}
void screen_autons(){}
void screen_statistics(){}




rect_config_t cfg1 = {
    .width = 200,
    .height = 100,
    .radius = 20, 
    .base_color = lv_color_hex(0x3B82F6), // Blue
    .glow_color = lv_color_hex(0x93C5FD), // Light blue glow
    .glow_size = 25 
};
lv_obj_t *scr = lv_screen_active();
void brain_menu(void){
    lv_obj_t *rect1 = custom_graphics::create_glowing_rounded_rect(scr, &cfg1);} 