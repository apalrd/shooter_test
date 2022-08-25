/* Report tab functions */
#ifndef _REPORT_H_
#define _REPORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "api.h"

/* Functions exposed by report */
void report_draw(lv_obj_t * page);

/* Appearance of a printf-style function */
#define REPORT(...) do{char temp[64];snprintf(temp,64,__VA_ARGS__);report_print(temp);}while(0)

/* Report print buffer - note string is truncated at null or 44 printable chars */
void report_print(const char * str);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _REPORT_H */