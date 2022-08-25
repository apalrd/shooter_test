/* This code runs the configuration screen */
#include "main.h"

/* Include pros api */
#include "pros/apix.h"

/* Use pal log */
#define LOG_LEVEL_FILE LOG_LEVEL_DEBUG
#include "pal/log.h"

/* Possible values of the callback */
enum
{
    RUN_CB_RUN,
    RUN_CB_INC,
    RUN_CB_DEC,
    RUN_CB_SET,
    RUN_CB_ACT,
    RUN_CB_MAX
};

static bool run_has_init = false;


/* Function to update run button */
void run_update_run(uint8_t idx)
{
    /* Break out if we are called before we init */
    if(!run_has_init)
    {
        return;
    }

    /* Break out if motor is unconfigured (pointers will be invalid */
    if(motors[idx].port < 0)
    {
        return;
    }

    /* If we are follower, get power from master. Else, get our power */
    bool powered = (motors[idx].leader < 0) ? motors[idx].powered : motors[motors[idx].leader].powered;
    
    /* Motor is follower, gray out */
    if(motors[idx].leader >= 0)
    {
        lv_obj_set_style(motors[idx].run.power,&style_dis);
    }
    /* Motor is on, set green */
    else if(powered)
    {
        lv_obj_set_style(motors[idx].run.power,&style_grn_act);
    }
    /* Motor is off, set red */
    else
    {
        lv_obj_set_style(motors[idx].run.power,&style_red_ina); 
    }  
}

/* Function to update grayed out controls when following */
void run_update_follow(uint8_t idx)
{
    /* Break out if init isn't done */
    if(!run_has_init)
    {
        return;
    }

    /* Break out if motor is unconfigured (pointers will be invalid */
    if(motors[idx].port < 0)
    {
        return;
    }

    /* Update grayed-out status of inc, dec, and set buttons */
    lv_style_t * style = (motors[idx].leader < 0) ? &style_blu_ina : &style_dis;
    lv_obj_set_style(motors[idx].run.inc,style);
    lv_obj_set_style(motors[idx].run.dec,style);
    lv_obj_set_style(motors[idx].run.set,style);
    lv_obj_set_style(motors[idx].run.act,style);

    /* Call run update run to update it's grayed out status */
    run_update_run(idx);
}

/* Update speeds */
void run_update_speeds(uint8_t idx)
{
    /* Break out if init isn't done */
    if(!run_has_init)
    {
        return;
    }

    /* Break out if motor is unconfigured (pointers will be invalid */
    if(motors[idx].port < 0)
    {
        return;
    }   

    /* Get our target from the leader if leading */
    int target = motors[idx].target;
    if(motors[idx].leader >= 0) target = motors[motors[idx].leader].target;

    /* Set speed */
    char temp[8];
    sprintf(temp,"%4d",target);
    lv_label_set_text(motors[idx].run.set_label,temp);

    /* Act speed always comes from this motor */
    sprintf(temp,"%4d",(int)motors[idx].data.speed);
    lv_label_set_text(motors[idx].run.act_label,temp);

    /* Act speed green if within 5% of target */
    double tol = target * 0.05;
    double min = target - tol;
    double max = target + tol;
    if(motors[idx].data.speed >= min && motors[idx].data.speed <= max)
    {
        lv_obj_set_style(motors[idx].run.act,&style_grn_act);
        lv_obj_set_style(motors[idx].run.act_label,&style_grn_act);
    }
    else
    {
        lv_obj_set_style(motors[idx].run.act,&style_red_ina);
        lv_obj_set_style(motors[idx].run.act_label,&style_red_ina);
    }
}


/* Run callback */
static lv_res_t run_cb(lv_obj_t *obj)
{
    uint32_t key = lv_obj_get_free_num(obj);
    uint8_t idx = (key & 0xff);
    uint8_t cb = ((key >> 8) & 0xff);

    /* First check if motor number is valid and motor is configurable */
    if(idx >= NUM_MOTORS)
    {
        LOG_ERROR("Got callback for run, key is %x, motor is invalid",key);
        return LV_RES_OK;
    }
    else if(motors[idx].port < 0)
    {
        LOG_WARN("Got callback for run, key is %x, motor has no port",key);
        return LV_RES_OK;
    }

    /* If we are currently a follower, ignore the button */
    if(motors[idx].leader >= 0)
    {
        LOG_WARN("Attempted to change property %d of motor %c, but it's a follower",cb,('A'+idx));
        /* Update styles so it's grayed out still */
        run_update_follow(idx);
        return LV_RES_OK;
    }

    /* Selection based on property */
    switch(cb)
    {
    case RUN_CB_RUN:
        motors[idx].powered = !motors[idx].powered;
        LOG_DEBUG("Changing motor state for %c to %d",('A'+idx),motors[idx].powered);
        /* Easiest to just update powered for everyone */
        for(int i = 0; i < NUM_MOTORS;i++)
        {
            run_update_run(i);
        }
        break;
    case RUN_CB_INC:
        motor_inc(idx,1);
        break;
    case RUN_CB_DEC:
        motor_inc(idx,-1);
        break;
    }
}


/* Size of the buttons */
#define CONFIG_BTN_HEIGHT 36
/* Function to set common attributes for buttons */
static void run_button_setup(lv_obj_t * button)
{
    /* Common attributes for buttons on the config page*/
    lv_btn_set_action(button,LV_BTN_ACTION_CLICK,run_cb);
    lv_btn_set_style(button,LV_BTN_STYLE_INA,&style_blu_ina);
    lv_btn_set_style(button,LV_BTN_STYLE_PR,&style_blu_act);
    lv_btn_set_style(button,LV_BTN_STYLE_REL,&style_blu_ina);
    lv_btn_set_layout(button,LV_LAYOUT_ROW_M);
    lv_obj_set_size(button,90,CONFIG_BTN_HEIGHT);  
}


/* Function to initialize the run page */
void run_draw(lv_obj_t * page)
{
    /* Create a title */
    lv_obj_t * label, * newpage, * button, * icon;
    label = lv_label_create(page,NULL);
    lv_label_set_text(label,"RUN");
    lv_obj_align(label,0,LV_ALIGN_IN_TOP_MID,0,0);

    /* Create one subpage per motor to hold configuration entities */
    for(int i = 0; i < 4; i++)
    {
        /* Page to encapsulate motor config */
        newpage = lv_page_create(page,NULL);
        motors[i].run.page = newpage;
        lv_obj_align(newpage,0,LV_ALIGN_IN_TOP_LEFT,i*103+4,22);
        lv_obj_set_size(newpage,100,210);
        lv_obj_set_hidden(newpage,false);
        lv_obj_set_style(newpage,&style_page);
        lv_page_set_scrl_layout(newpage,LV_LAYOUT_PRETTY);

        /* Label for the motor */
        label = lv_label_create(newpage,NULL);
        char name[8];
        sprintf(name,"MOTOR %c",(i+'A'));
        lv_label_set_text(label,name);

        /* If the motor is invalid, add a big NA and don't configure the rest */
        if(motors[i].port < 0)
        {
            label = lv_label_create(newpage,NULL);
            lv_label_set_text(label,"N/A");
            lv_obj_align(label,0,LV_ALIGN_CENTER,0,100);
            continue;
        }

        /* Power button */
        LV_IMG_DECLARE(mdi_power);
        button = lv_btn_create(newpage,NULL);
        motors[i].run.power = button;
        lv_obj_set_free_num(button,(i+(RUN_CB_RUN<<8)));
        run_button_setup(button);
        /* power icon */
        icon = lv_img_create(button,NULL);
        lv_img_set_src(icon,&mdi_power);
        /* power text */
        label = lv_label_create(button,NULL);
        lv_label_set_text(label,"RUN");
        /* Update active style */
        run_update_run(i);

        /* Decrement button */
        button = lv_btn_create(newpage,NULL);
        motors[i].run.dec = button;
        lv_obj_set_free_num(button,(i+(RUN_CB_DEC<<8)));
        run_button_setup(button);
        lv_obj_set_width(button,42);
        lv_btn_set_layout(button,LV_LAYOUT_CENTER);
        /* icon */
        LV_IMG_DECLARE(mdi_arrow_down_bold);
        icon = lv_img_create(button,NULL);
        lv_img_set_src(icon,&mdi_arrow_down_bold);
        lv_obj_set_style(icon,&style_dis);

        /* Increment button */
        button = lv_btn_create(newpage,NULL);
        motors[i].run.inc = button;
        lv_obj_set_free_num(button,(i+(RUN_CB_INC<<8)));
        run_button_setup(button);
        lv_obj_set_width(button,42);
        lv_btn_set_layout(button,LV_LAYOUT_CENTER);
        /* icon */
        LV_IMG_DECLARE(mdi_arrow_up_bold);
        icon = lv_img_create(button,NULL);
        lv_img_set_src(icon,&mdi_arrow_up_bold);
        lv_obj_set_style(icon,&style_dis);

        /* Speed 'Button' */
        button = lv_btn_create(newpage,NULL);
        motors[i].run.set = button;
        lv_obj_set_free_num(button,(i+(RUN_CB_SET<<8)));
        run_button_setup(button);
        /* icon */
        LV_IMG_DECLARE(mdi_play_circle);
        icon = lv_img_create(button,NULL);
        lv_img_set_src(icon,&mdi_play_circle);
        lv_obj_set_style(icon,&style_dis);
        /* label */
        label = lv_label_create(button,NULL);
        motors[i].run.set_label = label;

        /* Act Speed 'Button' */
        button = lv_btn_create(newpage,NULL);
        motors[i].run.act = button;
        lv_obj_set_free_num(button,(i+(RUN_CB_ACT<<8)));
        run_button_setup(button);
        /* icon */
        LV_IMG_DECLARE(mdi_speedometer);
        icon = lv_img_create(button,NULL);
        lv_img_set_src(icon,&mdi_speedometer);
        lv_obj_set_style(icon,&style_dis);
        /* label */
        label = lv_label_create(button,NULL);
        motors[i].run.act_label = label;

        /* Update run for this motor */
        run_update_run(i);
    }
    run_has_init = true;
}

