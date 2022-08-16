/* File to manage operation of a single motor */
#include "main.h"


/* Use pal log */
#define LOG_LEVEL_FILE LOG_LEVEL_DEBUG
#include "pal/log.h"


/* Global instances of our motors */
motor_t motors[NUM_MOTORS];

/* Table of standard increments per gear ratio */
static const int inc_amt[] = {5, 5, 10};
static const int max_spd[] = {100, 200, 600};

/* Function to update a motor to its max speed when gear ratio is changed */
void motor_reset_max(uint8_t idx)
{
    motors[idx].target = max_spd[motors[idx].gearset];
}

/* Function to inc/dec a motor speed with checks */
void motor_inc(uint8_t idx, int8_t direction)
{

    /* Add/sub new speed */
    motors[idx].target += inc_amt[motors[idx].gearset] * direction;

    /* Check underflow */
    if(motors[idx].target < 0) motors[idx].target = 0;

    /* Check overflow */
    if(motors[idx].target > max_spd[motors[idx].gearset]) motors[idx].target = max_spd[motors[idx].gearset];
}

/* Functions to operate on motors */
void motor_init()
{
    LOG_DEBUG("Init Motors");
    int nextAlloc = 0;

    /* Iterate through all V5 ports to find the first 4 motors (A,B,C,D) */
    for(int i = 0; i < 21; i++)
	{
		v5_device_e_t type = registry_get_plugged_type(i);
		LOG_DEBUG("Port %02d has device class %03d",(i+1),type);

        /* If it's a motor, set it as the next alloc motor */
        if(E_DEVICE_MOTOR == type)
        {
            /* Configure this motor with defaults */
            motors[nextAlloc].idx = nextAlloc;
            motors[nextAlloc].port = i+1; /* i is zero-indexed */
            motors[nextAlloc].gearset = E_MOTOR_GEARSET_06;
            motors[nextAlloc].leader = -1;
            motors[nextAlloc].reversed = false;
            motors[nextAlloc].powered = false;
            motors[nextAlloc].target = 600; /* Max for gearset 06 */
            LOG_ALWAYS("Found motor on port %02d, allocating as motor %c",(i+1),(nextAlloc+'A'));
            nextAlloc++;
        }

        /* If we have allocated too many motors, break out early */
        if(nextAlloc >= NUM_MOTORS)
        {
            break;
        }

	}

    /* If we haven't allocated all motors, fill in the rest with -1 so they are unused */
    for(int i = nextAlloc; i < NUM_MOTORS; i++)
    {
        motors[i].idx = i;
        motors[i].port = -1;
        motors[i].leader = -1;
        motors[i].gearset = E_MOTOR_GEARSET_06;
        LOG_ALWAYS("No more motors found for motor %c",(i+'A'));
    }
}

/* Function for runtime motor control */
void motor_run(uint8_t idx)
{   
    if(motors[idx].port < 0)
    {
        /* Break out, invalid motor */
        return;
    }

    /* Get a pointer to our motor to make it easier */
    motor_t * mine = &motors[idx];

    /* Set brake mode to coast */
    motor_set_brake_mode(mine->port,E_MOTOR_BRAKE_COAST);

    /* Get powered and target from leader if applicable */
    bool powered = mine->powered;
    int target = mine->target;
    int direction = mine->reversed ? -1 : 1;

    if(mine->leader >= 0)
    {
        powered = motors[mine->leader].powered;
        target = motors[mine->leader].target;
        /* Reversed does not come from the leader */
    }

    /* If not powered, stop and update gearset */
    if(!powered)
    {
        motor_brake(mine->port);
        motor_set_gearing(mine->port,mine->gearset);
    }
    /* Else, set target speed */
    else
    {
        motor_move_velocity(mine->port,target*direction);
    }

    /* Read data parameters */
    mine->data.speed = motor_get_actual_velocity(mine->port)*(float)direction;
    mine->data.curr = (float)motor_get_current_draw(mine->port);
    mine->data.volt = motor_get_voltage(mine->port);
    mine->data.temp = motor_get_temperature(mine->port);
    mine->data.power = motor_get_power(mine->port);

    /* Spinup detector */
    if(!mine->data.spinup.armed && !powered)
    {
        /* Arm spinup if we are not powered and below 5 RPM */
        if(fabs(mine->data.speed) < 5.0)
        {
            mine->data.spinup.armed = true;
            /* Reset accum data */
            mine->data.spinup.speed_max = 0.0;
            mine->data.spinup.energy = 0.0;
            mine->data.spinup.time = 0.0;
        }
    }
    /* If we are powered and armed, run the spinup detector */
    else if(mine->data.spinup.armed && powered)
    {
        /* Accumulate energy and time */
        mine->data.spinup.energy += dt*mine->data.power;
        mine->data.spinup.time += dt;

        /* If we reached 66%, 95%, 99%, report */

        
        if((mine->data.speed >= ((double)mine->target*0.66)) &&             /* We have crossed 66% */
           (mine->data.spinup.speed_max < ((double)mine->target*0.66)))     /* Last speed was below 66% */
        {
            /* Report 66% trip */
            LOG_ALWAYS("MOTOR %c: SPINUP Reached 66%% in %f sec (%f J)",(mine->idx+'A'),mine->data.spinup.time,mine->data.spinup.energy);
        }
        else if((mine->data.speed >= ((double)mine->target*0.95)) &&        /* We have crossed 95% */
           (mine->data.spinup.speed_max < ((double)mine->target*0.95)))     /* Last speed was below 95% */
        {
            /* Report 95% trip */
            LOG_ALWAYS("MOTOR %c: SPINUP Reached 95%% in %f sec (%f J)",(mine->idx+'A'),mine->data.spinup.time,mine->data.spinup.energy);
        }
        else if((mine->data.speed >= ((double)mine->target*0.99)) &&        /* We have crossed 99% */
           (mine->data.spinup.speed_max < ((double)mine->target*0.99)))     /* Last speed was below 99% */
        {
            /* Report 95% trip */
            LOG_ALWAYS("MOTOR %c: SPINUP Reached 99%% in %f sec (%f J)",(mine->idx+'A'),mine->data.spinup.time,mine->data.spinup.energy);
            /* De-arm spinup detect, must spindown to re-run test */
            mine->data.spinup.armed = false;
        }

    }
    /* Store last speed for spinup detector only if new speed is higher than last speed */
    if(mine->data.spinup.speed_max < mine->data.speed)
    {
        mine->data.spinup.speed_max = mine->data.speed;
    }
}