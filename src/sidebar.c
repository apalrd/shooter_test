/* This code runs the sidebar psudo-tab control */
#include "main.h"
#include "pros/apix.h"

/* Use pal log */
#define LOG_LEVEL_FILE LOG_LEVEL_DEBUG
#include "pal/log.h"

/* Sidebar objects */
typedef enum
{
    SIDEBAR_OBJ_CONFIG,
    SIDEBAR_OBJ_RUN,
    SIDEBAR_OBJ_LOG,
    SIDEBAR_OBJ_TEST,
    /* Must be last */
    SIDEBAR_OBJ_COUNT
} sidebar_obj_t;

/* Sidebar entities list */
static lv_obj_t * sidebar_objs[SIDEBAR_OBJ_COUNT];
/* Sidebar icon entities list */
static lv_obj_t * sidebar_icons[SIDEBAR_OBJ_COUNT];
/* Pages list */
static lv_obj_t * sidebar_pages[SIDEBAR_OBJ_COUNT];


/* Sidebar button callback routine */
static lv_res_t sidebar_cb(lv_obj_t *obj)
{
    /* Find which index we are */
    uint32_t idx = lv_obj_get_free_num(obj);

    if(idx < SIDEBAR_OBJ_COUNT)
    {
        /* Valid idx, change visible page by hiding all but this one */
        LOG_DEBUG("Page flipping to %d",idx);
        for(int i = 0; i < SIDEBAR_OBJ_COUNT; i++)
        {
            lv_obj_set_hidden(sidebar_pages[i],true);
            lv_obj_set_style(sidebar_objs[i],&style_grn_ina);
        }
        lv_obj_set_hidden(sidebar_pages[idx],false);
        lv_obj_set_style(sidebar_objs[idx],&style_grn_act);
    }
    else
    {
        LOG_ERROR("Received invalid cb idx: %d",idx);
    }
    return LV_RES_OK;
}

/* Initialize our data */
void sidebar_init()
{
    LOG_ALWAYS("Sidebar Init");

    /* Declare all of the button icons we need */
    LV_IMG_DECLARE(mdi_cog_48px);
    LV_IMG_DECLARE(mdi_power_48px);
    LV_IMG_DECLARE(mdi_sine_wave_48px);
    LV_IMG_DECLARE(mdi_clipboard_pulse_outline_48px);

    for(int i = 0; i < SIDEBAR_OBJ_COUNT; i++)
    {
        sidebar_objs[i] = lv_btn_create(lv_scr_act(),NULL);
        lv_btn_set_action(sidebar_objs[i],LV_BTN_ACTION_CLICK,sidebar_cb);
        lv_obj_set_free_num(sidebar_objs[i],i);
        lv_btn_set_style(sidebar_objs[i],LV_BTN_STYLE_INA,&style_grn_ina);
        lv_btn_set_style(sidebar_objs[i],LV_BTN_STYLE_PR,&style_grn_ina);
        lv_btn_set_style(sidebar_objs[i],LV_BTN_STYLE_REL,&style_grn_ina);
        lv_obj_set_size(sidebar_objs[i],60,60);
        lv_obj_align(sidebar_objs[i],lv_scr_act(),LV_ALIGN_IN_TOP_RIGHT,0,(i)*60);
        lv_obj_set_hidden(sidebar_objs[i],false);

        /* Choose image depending on button id */

        /* Add an image of a gear to the config button */
        sidebar_icons[i] = lv_img_create(sidebar_objs[i],0);
        lv_img_set_style(sidebar_icons[i],&style_dis);
        lv_obj_align(sidebar_icons[i],0,LV_ALIGN_CENTER,0,0);
        switch(i)
        {
        case SIDEBAR_OBJ_CONFIG:
            lv_img_set_src(sidebar_icons[i],&mdi_cog_48px);
            break;
        case SIDEBAR_OBJ_RUN:
            lv_img_set_src(sidebar_icons[i],&mdi_power_48px);
            break;
        case SIDEBAR_OBJ_LOG:
            lv_img_set_src(sidebar_icons[i],&mdi_sine_wave_48px);
            break;
        case SIDEBAR_OBJ_TEST:
            lv_img_set_src(sidebar_icons[i],&mdi_clipboard_pulse_outline_48px);
            break;
        }

        /* Create a page corresponding to this button, currently empty */
        sidebar_pages[i] = lv_page_create(lv_scr_act(),NULL);
        lv_obj_set_style(sidebar_pages[i],&style_page);
        lv_obj_align(sidebar_pages[i],0,LV_ALIGN_IN_TOP_LEFT,0,0);
        lv_obj_set_size(sidebar_pages[i],lv_obj_get_width(lv_scr_act())-60,lv_obj_get_height(lv_scr_act()));
        lv_obj_set_hidden(sidebar_pages[i],true);
    }

    /* Call all of the other functions to draw their pages */
    config_draw(sidebar_pages[SIDEBAR_OBJ_CONFIG]);

    /* Select the config page by default */
    LOG_DEBUG("Flipping to page 0 by default");
    sidebar_cb(sidebar_objs[SIDEBAR_OBJ_CONFIG]);
    //lv_obj_set_hidden(sidebar_pages[SIDEBAR_OBJ_CONFIG],false);
    //lv_obj_set_style(sidebar_objs[SIDEBAR_OBJ_CONFIG],&style_grn_act);

    /* Create a label on each page */
    lv_obj_t * label;
    label = lv_label_create(sidebar_pages[1],NULL);
    lv_label_set_text(label,"RUN");
    label = lv_label_create(sidebar_pages[2],NULL);
    lv_label_set_text(label,"LOG");
    label = lv_label_create(sidebar_pages[3],NULL);
    lv_label_set_text(label,"TEST");
}