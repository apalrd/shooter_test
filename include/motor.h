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
        lv_obj_t * inc;
        lv_obj_t * dec;
        lv_obj_t * set;
        lv_obj_t * set_label;
        lv_obj_t * act;
        lv_obj_t * act_label;
    } run;
    /* Feedback data */
    struct
    {
        /* RPM */
        double speed;
        /* Amps */
        double curr;
        /* Volts */
        double volt;
        /* Watts */
        double power;
        /* deg C */
        double temp;
        /* Data collected during a spinup */
        struct
        {
            bool armed;
            double energy;
            double time;
            double speed_max;
        } spinup;        
        /* Data collected during a spindown (freewheeling) */
        struct
        {
            bool armed;
            double time;
        } spindown;  
        /* Data collected during a a single shot */
        struct
        {
            bool armed;
            bool inprog;
            double energy;
            double time;
            double min_speed;
        } shot;

        /* Data collected during a run */
        struct
        {
            double energy;
            double time;
        } run;
        
    } data;
    
} motor_t;


/* Global instances of our motors */
#define NUM_MOTORS 4
extern motor_t motors[NUM_MOTORS];

/* Functions to operate on motors */
void motor_init();
void motor_inc(uint8_t idx, int8_t direction);
void motor_reset_max(uint8_t idx);
void motor_run(uint8_t idx);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MOTOR_H */