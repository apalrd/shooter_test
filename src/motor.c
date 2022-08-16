/* File to manage operation of a single motor */
#include "main.h"


/* Use pal log */
#define LOG_LEVEL_FILE LOG_LEVEL_DEBUG
#include "pal/log.h"


/* Global instances of our motors */
motor_t motors[NUM_MOTORS];

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

    /* Set motors for testing */
    motors[2].port = 17;
    motors[3].port = 14;
}