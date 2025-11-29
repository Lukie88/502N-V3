#pragma once
#include <vector>
#include "liblvgl/lvgl.h"

typedef enum {
    LV_MENU_ITEM_BUILDER_VARIANT_1,
    LV_MENU_ITEM_BUILDER_VARIANT_2
} lv_menu_builder_variant_t;

<<<<<<< HEAD


=======
>>>>>>> 663526d3b5d059167ff347d8f979bfc0748f6d2f
std::vector<int> get_auto_ids(short int section);
static void switch_event_handler(lv_event_t * e);
static void button_event_handler(lv_event_t * e);

static lv_obj_t * create_text(lv_obj_t * parent, 
                              const char * icon, 
                              const char * txt,
                              lv_menu_builder_variant_t builder_variant);


static lv_obj_t * create_slider(lv_obj_t * parent,
                                const char * icon,
                                const char * txt,
                                int32_t min, 
                                int32_t max, 
                                int32_t val);

static lv_obj_t * create_switch(lv_obj_t * parent,
                                const char * icon,
                                const char * txt,
                                bool * external_state_ptr);
lv_obj_t * create_button(lv_obj_t * parent, const char * icon, const char * txt, lv_event_cb_t event_cb, uint32_t button_id);
void lvgl_task(void* param);
<<<<<<< HEAD
void lv_example_menu_5(void);
void run_selected_auton();
=======
void brain_menu(void);
void run_selected_auton();