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
    CONFIG_CB_PORT,
    CONFIG_CB_LEAD,
    CONFIG_CB_REVERSE,
    CONFIG_CB_GEAR,
    CONFIG_CB_MAX
};


/* Function to update a reverse icon by motor index */
static void config_update_reverse(uint8_t idx)
{
    /* Break out if motor is unconfigured (pointers will be invalid */
    if(motors[idx].port < 0)
    {
        return;
    }
    
    /* Update graphics */
    if(motors[idx].reversed)
    {
        /* Change icon to backwards */
        LV_IMG_DECLARE(mdi_restore);
        lv_img_set_src(motors[idx].config.reverse_icon,&mdi_restore);

        /* Change label */
        lv_label_set_text(motors[idx].config.reverse_label,"REV");   
    }
    else
    {
        /* Change icon to forwards */
        LV_IMG_DECLARE(mdi_reload);
        lv_img_set_src(motors[idx].config.reverse_icon,&mdi_reload);

        /* Change label */
        lv_label_set_text(motors[idx].config.reverse_label,"FWD");   
    }
}

/* Function to update a gear ratio icon by motor index */
static void config_update_gearset(uint8_t idx)
{
    /* Break out if motor is unconfigured (pointers will be invalid */
    if(motors[idx].port < 0)
    {
        return;
    }
    
    /* New icon and text */
    if(E_MOTOR_GEARSET_36 == motors[idx].gearset)
    {
        /* Change icon to slow */
        LV_IMG_DECLARE(mdi_speedometer_slow);
        lv_img_set_src(motors[idx].config.gearset_icon,&mdi_speedometer_slow);

        /* Change label */
        lv_label_set_text(motors[idx].config.gearset_label,"36:1");   
    }
    else if(E_MOTOR_GEARSET_18 == motors[idx].gearset)
    {
        /* Change icon to medium */
        LV_IMG_DECLARE(mdi_speedometer_medium);
        lv_img_set_src(motors[idx].config.gearset_icon,&mdi_speedometer_medium);

        /* Change label */
        lv_label_set_text(motors[idx].config.gearset_label,"18:1");   
    }
    else
    {
        /* Change icon to fast */
        LV_IMG_DECLARE(mdi_speedometer);
        lv_img_set_src(motors[idx].config.gearset_icon,&mdi_speedometer);

        /* Change label */
        lv_label_set_text(motors[idx].config.gearset_label," 6:1");   
    }

    /* If we are a follower, gray out the button, else set it blue */
    if(motors[idx].leader < 0)
    {
        /* Leader, set blue */
        lv_obj_set_style(motors[idx].config.gearset,&style_blu_ina);
    }
    else
    {
        /* Follower, gray out */
        lv_obj_set_style(motors[idx].config.gearset,&style_dis);

    }
}

/* Function to update follower (graphics only! does not check validity) */
static void config_update_follow(uint8_t idx)
{
    /* Break out if motor is unconfigured (pointers will be invalid */
    if(motors[idx].port < 0)
    {
        return;
    }
    
    /* Motor is a leader, set cog icon */
    if(motors[idx].leader < 0)
    {
        /* Change icon back to cog */
        LV_IMG_DECLARE(mdi_cog);
        lv_img_set_src(motors[idx].config.lead_icon,&mdi_cog);

        /* Change label back to LEAD */
        lv_label_set_text(motors[idx].config.lead_label,"LEAD");
    }
    /* Motor is not a leader, figure out who is and set that as the text */
    else
    {
        /* Change icon to two people */
        LV_IMG_DECLARE(mdi_account_multiple_plus);
        lv_img_set_src(motors[idx].config.lead_icon,&mdi_account_multiple_plus);

        /* Change label to indicate who the leader is */
        char temp[8];
        sprintf(temp,"FL %c",(motors[idx].leader + 'A'));
        lv_label_set_text(motors[idx].config.lead_label,temp);   
    }  

    /* Update the run tab accordingly */
    run_update_follow(idx);
}


/* Config callback */
static lv_res_t config_cb(lv_obj_t *obj)
{
    uint32_t key = lv_obj_get_free_num(obj);
    uint8_t idx = (key & 0xff);
    uint8_t cb = ((key >> 8) & 0xff);

    /* First check if motor number is valid and motor is configurable */
    if(idx >= NUM_MOTORS)
    {
        LOG_ERROR("Got callback for config, key is %x, motor is invalid",key);
        return LV_RES_OK;
    }
    else if(motors[idx].port < 0)
    {
        LOG_WARN("Got callback for config, key is %x, motor has no port",key);
        return LV_RES_OK;
    }

    /* Selection based on property */
    switch(cb)
    {
    case CONFIG_CB_PORT:
        LOG_WARN("Cannot change port number for motor %c, feature not implemented",('A'+idx));
        break;
    case CONFIG_CB_LEAD:
        /* If we are currently a follower, promote ourselves to leader
         * We kick our followers onto our leader when we follow
         * So we can't have any followers ourselves at this point
         */
        if(motors[idx].leader >= 0)
        {
            /* Promote ourself */
            LOG_DEBUG("Promoting motor %c to leader",('A'+idx));
            /* Turn ourself off */
            motors[idx].powered = 0;
            motors[idx].leader = -1;
            config_update_follow(idx);
            /* Also update gearset so it un-disables */
            config_update_gearset(idx);
        }
        /* The first motor cannot be assigned a leader */
        else if(idx == 0)
        {
            /* Motor does not have leader assigned, but first motor cannot be a leader */
            LOG_WARN("Trying to make motor %c a follower, but it cannot be a follower",('A'+idx));
            config_update_follow(idx);
        }
        else
        {
            /* Motor does not have a leader assigned, look forward to find its leader */
            for(int i = idx; i > 0; i--)
            {
                LOG_DEBUG("Trying to find leader for %c, checking %c",('A'+idx),('A'+i-1));
                if(motors[i-1].leader < 0)
                {
                    LOG_DEBUG("Found leader, using %c",((i-1)+'A'));
                    motors[idx].leader = i-1;
                    break;
                }
            }

            /* Update our own graphics */
            config_update_follow(idx);
            /* Take gearset from the leader and update that too */
            motors[idx].gearset = motors[motors[idx].leader].gearset;
            motors[idx].target = motors[motors[idx].leader].target;
            config_update_gearset(idx); 

            /* Look back to the end of the list to see if anyone is following us
             * If so, they need to follow our leader now
             */
            for(int i = idx + 1; i < NUM_MOTORS;i++)
            {
                if(motors[i].leader == idx)
                {
                    LOG_DEBUG("Motor %c was following us, now needs to follow %c",(i+'A'),(motors[idx].leader+'A'));
                    /* Change leader */
                    motors[i].leader = motors[idx].leader;

                    /* Update graphics */
                    config_update_follow(i);
                    /* Also update their gearset */
                    motors[i].gearset = motors[motors[i].leader].gearset;
                    motors[i].target = motors[motors[i].leader].target;
                    config_update_gearset(i); 
                }
            }       
        }
        break;
    case CONFIG_CB_REVERSE:
        /* Flip reversed flag */
        motors[idx].reversed = !motors[idx].reversed;
        LOG_DEBUG("Updating reverse on motor %c, value is now %d",(idx+'A'),motors[idx].reversed);
        /* Update graphics */
        config_update_reverse(idx);
        break;
    case CONFIG_CB_GEAR:
        /* Check if we are a leader and can update gearset */
        if(motors[idx].leader < 0)
        {
            /* Increment gearset and roll over */
            motors[idx].gearset++;
            if(motors[idx].gearset > E_MOTOR_GEARSET_06) motors[idx].gearset = 0;
            LOG_DEBUG("Updating gearset on motor %c, value is now %d",('A'+idx),motors[idx].gearset);
            /* Reset target speed to max for this gearset */
            motor_reset_max(idx);

            /* Check if we have any followers and update them too */
            for(int i = idx + 1; i < NUM_MOTORS;i++)
            {
                if(motors[i].leader == idx)
                {
                    LOG_DEBUG("Updating gearset for motor %c since it follows %c",('A'+i),('A'+idx));
                    motors[i].gearset = motors[idx].gearset;
                    motors[i].target = motors[idx].target;
                    config_update_gearset(i);
                }
            }
        }
        else
        {
            /* Followers can't update gearset */
            LOG_WARN("Can't update gearset on motor %c since it's a follower",('A'+idx));
        }
        /* Update graphics */
        config_update_gearset(idx);
        /* Check if we have any followers */
        break;
    default:
        LOG_ERROR("Got callback for config, key is %x (motor %c, property %d), property is invalid",key,('A'+idx),cb);
    }
}


/* Size of the buttons */
#define CONFIG_BTN_HEIGHT 36
/* Function to set common attributes for buttons */
static void config_button_setup(lv_obj_t * button)
{
    /* Common attributes for buttons on the config page*/
    lv_btn_set_action(button,LV_BTN_ACTION_CLICK,config_cb);
    lv_btn_set_style(button,LV_BTN_STYLE_INA,&style_blu_ina);
    lv_btn_set_style(button,LV_BTN_STYLE_PR,&style_blu_ina);
    lv_btn_set_style(button,LV_BTN_STYLE_REL,&style_blu_ina);
    lv_btn_set_layout(button,LV_LAYOUT_ROW_M);
    lv_obj_set_size(button,90,CONFIG_BTN_HEIGHT);  
}


/* Function to initialize the config page */
void config_draw(lv_obj_t * page)
{
    /* Create a title */
    lv_obj_t * label, * newpage, * button, * icon;
    label = lv_label_create(page,NULL);
    lv_label_set_text(label,"CONFIGURE");
    lv_obj_align(label,0,LV_ALIGN_IN_TOP_MID,0,0);

    /* Create one subpage per motor to hold configuration entities */
    for(int i = 0; i < 4; i++)
    {
        /* Page to encapsulate motor config */
        newpage = lv_page_create(page,NULL);
        motors[i].config.page = newpage;
        lv_obj_align(newpage,0,LV_ALIGN_IN_TOP_LEFT,i*103+4,22);
        lv_obj_set_size(newpage,100,210);
        lv_obj_set_hidden(newpage,false);
        lv_obj_set_style(newpage,&style_page);
        lv_page_set_scrl_layout(newpage,LV_LAYOUT_COL_M);

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

        /* Port number button */
        LV_IMG_DECLARE(mdi_power_plug);
        button = lv_btn_create(newpage,NULL);
        motors[i].config.port = button;
        lv_obj_set_free_num(button,(i+(CONFIG_CB_PORT<<8)));
        config_button_setup(button);
        /* Port number icon */
        icon = lv_img_create(button,NULL);
        lv_img_set_src(icon,&mdi_power_plug);
        lv_obj_set_style(icon,&style_dis);
        /* Port number text */
        char text[8];
        sprintf(text,"%d",motors[i].port);
        label = lv_label_create(button,NULL);
        motors[i].config.port_label = label;
        lv_label_set_text(label,text);

        /* Leader/Follower button */
        button = lv_btn_create(newpage,NULL);
        motors[i].config.lead = button;
        lv_obj_set_free_num(button,(i+(CONFIG_CB_LEAD<<8)));
        config_button_setup(button);
        /* Leader/Follower icon */
        icon = lv_img_create(button,NULL);
        motors[i].config.lead_icon = icon;
        lv_obj_set_style(icon,&style_dis);
        /* Leader/Follower text */
        label = lv_label_create(button,NULL);
        motors[i].config.lead_label = label;
        config_update_follow(i);

        /* Reversed button */
        button = lv_btn_create(newpage,NULL);
        motors[i].config.reverse = button;
        lv_obj_set_free_num(button,(i+(CONFIG_CB_REVERSE<<8)));
        config_button_setup(button);
        /* Reversed icon */
        icon = lv_img_create(button,NULL);
        motors[i].config.reverse_icon = icon;
        lv_obj_set_style(icon,&style_dis);
        /* Reversed text */
        label = lv_label_create(button,NULL);
        motors[i].config.reverse_label = label;
        /* Update graphics */
        config_update_reverse(i);

        /* Gear Ratio button */
        button = lv_btn_create(newpage,NULL);
        motors[i].config.gearset = button;
        lv_obj_set_free_num(button,(i+(CONFIG_CB_GEAR<<8)));
        config_button_setup(button);
        /* Gear Ratio icon */
        icon = lv_img_create(button,NULL);
        motors[i].config.gearset_icon = icon;
        lv_obj_set_style(icon,&style_dis);
        /* Gear Ratio text */
        label = lv_label_create(button,NULL);
        motors[i].config.gearset_label = label;
        /* Update icon and text */
        config_update_gearset(i);
    }

}

