/* Sidebar display objects */
#ifndef _SIDEBAR_H_
#define _SIDEBAR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "api.h"

/* Sidebar size */
#define SIDEBAR_WIDTH 60

/* Draw sidebar (no reason to undraw it, it's always visible) */
void sidebar_init();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SIDEBAR_H */