/* Single motor entity */
#ifndef _MOTOR_H_
#define _MOTOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pros/apix.h"

/* Motor entity structure */
typedef struct
{
    /* Index (starting from zero) in the system */
    uint8_t idx;
    /* Port number (-1 is invalid) */
    int8_t port;
    /* Reversed */
    bool reversed;
    /* Powered */
    bool powered;
    /* Index of leader (-1 if self is leader) */
    int8_t leader;
    /* Gear set */
    motor_gearset_e_t gearset;
    /* Target speed */
    int32_t target;
    /* Config objs */
    struct
    {
        lv_obj_t * page;
        lv_obj_t * port;
        lv_obj_t * port_label;
        lv_obj_t * lead;
        lv_obj_t * lead_icon;
        lv_obj_t * lead_label;
        lv_obj_t * reverse;
        lv_obj_t * reverse_icon;
        lv_obj_t * reverse_label;
        lv_obj_t * gearset;
        lv_obj_t * gearset_icon;
        lv_obj_t * gearset_label;
    } config;
    /* Run objs */
    struct
    {
        lv_obj_t * page;
        lv_obj_t * power;
        lv_obj_t * speed_inc;
        lv_obj_t * speed_dec;
        lv_obj_t * speed_disp;
    } run;
    /* Feedback data */
    struct
    {
        float speed;
        float curr;
        float volt;
        float power;
        float accum_power;
    } data;
    
} motor_t;


/* Global instances of our motors */
#define NUM_MOTORS 4
extern motor_t motors[NUM_MOTORS];

/* Functions to operate on motors */
void motor_init();



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MOTOR_H */