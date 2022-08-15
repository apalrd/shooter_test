#include "main.h"
#include "stdio.h"
#include "pros/apix.h"

/* Use pal log */
#define LOG_LEVEL_FILE LOG_LEVEL_DEBUG
#include "pal/log.h"

/* Include all of our files */
#include "motor.h"


/* Global instances of our motors */
motor_t g_motor[NUM_MOTORS];

/* Function to print the attached device classes per port */
void print_devices()
{
    for(int i = 0; i < 21; i++)
	{
		v5_device_e_t type = registry_get_plugged_type(i);
		LOG_ALWAYS("Port %02d has device class %03d",(i+1),type);
	}

	/* Determine screen size */
	int bwidth = lv_obj_get_width(lv_scr_act());
	int bheight = lv_obj_get_height(lv_scr_act());
	LOG_ALWAYS("Screen is %dx%d px",bwidth,bheight);
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() 
{
	/* Initiailze logger */
	log_init();

	/* Let them know we are in initialize */
	LOG_ALWAYS("In Initialize");

	print_devices();
	sidebar_init();
	
	/* Draw Sidebar */
	sidebar_draw();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() 
{
	/* Let them know we are in disable */
	LOG_ALWAYS("In Disable");
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() 
{
	/* Let them know we are in comp initialize */
	LOG_ALWAYS("In Comp Initialize");
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous()
{
	/* Let them know we are in auton */
	LOG_ALWAYS("In Autonomous");

}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() 
{
	/* Let them know we are in opcontrol */
	LOG_ALWAYS("In Opcontrol");


	while(1)
	{
		delay(1000);
		LOG_DEBUG("Spinning");
	}

}
