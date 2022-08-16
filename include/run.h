/* Run tab functions */
#ifndef _RUN_H_
#define _RUN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "api.h"

/* Functions exposed by run */
void run_draw(lv_obj_t * page);
void run_update_run(uint8_t idx);
void run_update_follow(uint8_t idx);
void run_update_speeds(uint8_t idx);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SIDEBAR_H */