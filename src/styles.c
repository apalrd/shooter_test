/* Styles shared across modules */
#include "main.h"

/* Use pal log */
#define LOG_LEVEL_FILE LOG_LEVEL_DEBUG
#include "pal/log.h"

/* Green button */
lv_style_t style_grn_ina, style_grn_act;
/* Red button */
lv_style_t style_red_ina, style_red_act;
/* Blue button */
lv_style_t style_blu_ina, style_blu_act;
/* Disabled button */
lv_style_t style_dis;
/* Page style */
lv_style_t style_page;


/* Function to configure the styles */
void style_init()
{
    LOG_DEBUG("Styles Init");
    
    /* Green */
    lv_style_copy(&style_grn_ina,&lv_style_btn_ina);
    style_grn_ina.body.radius=0;
    style_grn_ina.body.border.width=5;
    style_grn_ina.body.padding.hor = 2;
    style_grn_ina.body.padding.ver = 2;
    style_grn_ina.body.padding.inner = 2;
    style_grn_ina.body.border.color=LV_COLOR_BLACK;
    style_grn_ina.body.main_color=LV_COLOR_GREEN;
    style_grn_ina.body.grad_color=LV_COLOR_GREEN;
    style_grn_ina.text.color=LV_COLOR_BLACK;
    style_grn_ina.image.color=LV_COLOR_BLACK;
    lv_style_copy(&style_grn_act,&style_grn_ina);
    style_grn_act.body.border.color=LV_COLOR_WHITE;
    style_grn_act.body.main_color=LV_COLOR_LIME;
    style_grn_act.body.grad_color=LV_COLOR_LIME;
    style_grn_act.image.color=LV_COLOR_WHITE;
    style_grn_act.text.color=LV_COLOR_WHITE;

    /* Red */
    lv_style_copy(&style_red_ina,&style_grn_ina);
    style_red_ina.body.main_color=LV_COLOR_MAROON;
    style_red_ina.body.grad_color=LV_COLOR_MAROON;
    lv_style_copy(&style_red_act,&style_grn_act);
    style_red_act.body.main_color=LV_COLOR_RED;
    style_red_act.body.grad_color=LV_COLOR_RED;

    /* Blue */
    lv_style_copy(&style_blu_ina,&style_grn_ina);
    style_blu_ina.body.main_color=LV_COLOR_TEAL;
    style_blu_ina.body.grad_color=LV_COLOR_TEAL;
    lv_style_copy(&style_blu_act,&style_grn_act);
    style_blu_act.body.main_color=LV_COLOR_BLUE;
    style_blu_act.body.grad_color=LV_COLOR_BLUE;

    /* Disabled */
    lv_style_copy(&style_dis,&style_grn_ina);
    style_dis.body.main_color=LV_COLOR_GRAY;
    style_dis.body.grad_color=LV_COLOR_GRAY;

    /* Page style has a huge gap, need to fix */
    lv_style_copy(&style_page,&lv_style_plain);
    style_page.body.main_color=LV_COLOR_GRAY;
    style_page.body.grad_color=LV_COLOR_GRAY;
    style_page.body.border.color=LV_COLOR_SILVER;
    style_page.body.border.width = 2;
    style_page.body.padding.hor = 2;
    style_page.body.padding.ver = 2;
    style_page.body.padding.inner = 2;
}