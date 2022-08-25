#include "main.h"
#include "stdio.h"
#include "pros/apix.h"

/* Use pal log */
#define LOG_LEVEL_FILE LOG_LEVEL_DEBUG
#include "pal/log.h"

/* Include all of our files */
#include "motor.h"

/* Controller code */
void controller()
{
	/* Check if we actually have a controller, and leave if we don't */
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

	/* Initialize styles */
	style_init();

	/* Initiailze device allocations */
	motor_init();
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
double dt;
void opcontrol() 
{
	/* Let them know we are in opcontrol */
	LOG_ALWAYS("In Opcontrol");

	/* Initialize sidebar, which will initialize tab pages as well */
	sidebar_init();

	/* Set dt at 20ms initially and assume it is */
	dt = 0.02;


	while(1)
	{
		/* Process each motor */
		for(int i = 0; i < NUM_MOTORS; i++)
		{
			/* Set speeds and data log */
			motor_run(i);

			/* Update graphics */
			run_update_speeds(i);
		}
		delay(dt*1000);
	}

}
