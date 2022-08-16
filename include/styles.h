/* Commonly used styles */
#ifndef _STYLES_H_
#define _STYLES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pros/apix.h"

/* Green button */
extern lv_style_t style_grn_ina, style_grn_act;
/* Red button */
extern lv_style_t style_red_ina, style_red_act;
/* Blue button */
extern lv_style_t style_blu_ina, style_blu_act;
/* Disabled button */
extern lv_style_t style_dis;
/* Page style */
extern lv_style_t style_page;


/* Create styles */
void style_init();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _STYLES_H */