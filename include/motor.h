/* Single motor entity */
#ifndef _MOTOR_H_
#define _MOTOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "api.h"

/* Motor entity structure */
typedef struct
{
    /* Index (starting from zero) in the system */
    uint8_t idx;
    /* Port number (-1 is invalid) */
    int8_t port;
    /* Reversed */
    uint8_t reversed;
    /* Gear set */
    motor_gearset_e_t gearset;
    /* Target speed */
    int32_t target;
} motor_t;

/* Functions to operate on motors */



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MOTOR_H */