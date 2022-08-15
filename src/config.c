/* This code runs the configuration screen */
#include "main.h"

/* Include pros api */
#include "pros/apix.h"

static void config_single(motor_t entity)
{

}


/* Main function called when configure page */
void config()
{
    /* Draw our motor configuration widgets */
    for(int i = 0; i < NUM_MOTORS;i++)
    {
        config_single(g_motor[i]);
    }

}

