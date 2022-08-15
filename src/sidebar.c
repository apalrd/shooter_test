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
    SIDEBAR_OBJ_CTRL,
    /* Must be last */
    SIDEBAR_OBJ_COUNT
} sidebar_obj_t;

/* Sidebar entities list */
static lv_obj_t * sidebar_objs[SIDEBAR_OBJ_COUNT];
/* Sidebar icon entities list */
static lv_obj_t * sidebar_icons[SIDEBAR_OBJ_COUNT];

/* Styles */
static lv_style_t style_ina, style_act, style_dis;


/* Sidebar button callback routine */
static lv_res_t sidebar_cb(lv_obj_t *obj)
{
    /* Find which index we are */
    uint32_t idx = lv_obj_get_free_num(obj);
    /* Button was pressed */
    LOG_DEBUG("Received cb, idx %d",idx);
}

/* Initialize our data */
void sidebar_init()
{
    LOG_ALWAYS("Sidebar Init");
    /* Create themes */
    lv_style_copy(&style_ina,&lv_style_btn_ina);
    style_ina.body.radius=3;
    style_ina.body.border.width=5;
    style_ina.body.border.color=LV_COLOR_BLACK;
    style_ina.body.main_color=LV_COLOR_GREEN;
    style_ina.body.grad_color=LV_COLOR_GREEN;
    style_ina.text.color=LV_COLOR_BLACK;
    lv_style_copy(&style_act,&style_ina);
    style_act.body.border.color=LV_COLOR_WHITE;
    style_act.body.main_color=LV_COLOR_LIME;
    style_act.body.grad_color=LV_COLOR_LIME;
    lv_style_copy(&style_dis,&style_ina);
    style_dis.body.main_color=LV_COLOR_GRAY;
    style_dis.body.grad_color=LV_COLOR_GRAY;

    LOG_DEBUG("Sidebar Obj Count is %d",SIDEBAR_OBJ_COUNT);
}


void sidebar_draw()
{
    LOG_ALWAYS("Sidebar Draw");
    /* Create a button for each sidebar object */
    lv_obj_t * label;

    /* Config button */
    sidebar_objs[SIDEBAR_OBJ_CONFIG] = lv_btn_create(lv_scr_act(),NULL);
    lv_btn_set_action(sidebar_objs[SIDEBAR_OBJ_CONFIG],LV_BTN_ACTION_CLICK,sidebar_cb);
    lv_obj_set_free_num(sidebar_objs[SIDEBAR_OBJ_CONFIG],SIDEBAR_OBJ_CONFIG);
    lv_btn_set_style(sidebar_objs[SIDEBAR_OBJ_CONFIG],LV_BTN_STYLE_INA,&style_ina);
    lv_btn_set_style(sidebar_objs[SIDEBAR_OBJ_CONFIG],LV_BTN_STYLE_PR,&style_ina);
    lv_btn_set_style(sidebar_objs[SIDEBAR_OBJ_CONFIG],LV_BTN_STYLE_REL,&style_ina);
    lv_obj_set_size(sidebar_objs[SIDEBAR_OBJ_CONFIG],60,60);
    lv_obj_align(sidebar_objs[SIDEBAR_OBJ_CONFIG],lv_scr_act(),LV_ALIGN_CENTER,0,0);

    /* Add text on top */
    label = lv_label_create(sidebar_objs[SIDEBAR_OBJ_CONFIG],NULL);
    lv_label_set_text(label,"CONF");

    lv_area_t coords;
    lv_obj_get_coords(sidebar_objs[SIDEBAR_OBJ_CONFIG],&coords);
    LOG_DEBUG("Object is at %d %d %d %d",coords.x1,coords.x2,coords.y1,coords.y2);

    /* Add an image of a gear */
    LV_IMG_DECLARE(mdi_cog);
    lv_obj_t * temp = lv_img_create(lv_scr_act(),0);
    lv_img_set_src(temp,&mdi_cog);
    lv_obj_align(temp,0,LV_ALIGN_IN_TOP_LEFT,0,0);
    lv_obj_set_size(temp,24,24);

}

void sidebar_update()
{

}