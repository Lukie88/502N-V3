#pragma once
#include <vector>
#include "liblvgl/lvgl.h"

typedef enum {
    LV_MENU_ITEM_BUILDER_VARIANT_1,
    LV_MENU_ITEM_BUILDER_VARIANT_2
} lv_menu_builder_variant_t;

inline bool color_selected;
void run_selected_auton();

std::vector<int> get_auto_ids(short int section);

static lv_obj_t * create_text(lv_obj_t * parent, 
                              const char * icon, 
                              const char * txt,
                              lv_menu_builder_variant_t builder_variant);

void lvgl_task(void* param);
void run_selected_auton();
void brain_menu(void);
