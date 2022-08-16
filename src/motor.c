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

    /* Set brake mode to coast */
    motor_set_brake_mode(motors[idx].port,E_MOTOR_BRAKE_COAST);

    /* Get powered and target from leader if applicable */
    bool powered = motors[idx].powered;
    int target = motors[idx].target;
    int direction = motors[idx].reversed ? -1 : 1;

    if(motors[idx].leader >= 0)
    {
        powered = motors[motors[idx].leader].powered;
        target = motors[motors[idx].leader].target;
        direction = motors[motors[idx].leader].reversed ? -1 : 1;
    }

    /* If not powered, stop and update gearset */
    if(!powered)
    {
        motor_brake(motors[idx].port);
        motor_set_gearing(motors[idx].port,motors[idx].gearset);
    }
    /* Else, set target speed */
    else
    {
        motor_move_velocity(motors[idx].port,target*direction);
    }

    /* Read data parameters */
    motors[idx].data.speed = motor_get_actual_velocity(motors[idx].port)*(float)direction;
    motors[idx].data.curr = (float)motor_get_current_draw(motors[idx].port);
    motors[idx].data.volt = motor_get_voltage(motors[idx].port);
    motors[idx].data.temp = motor_get_temperature(motors[idx].port);
    motors[idx].data.power = motor_get_power(motors[idx].port);
}