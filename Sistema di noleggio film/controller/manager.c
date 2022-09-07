#include <stdio.h>
#include <string.h>
#include "manager.h"

#include "../model/db.h"
#include "../view/manager.h"
#include "../utils/io.h"


static bool register_employee(){
	
	return false;
}

static bool move_employee(){
	return false;
}


static bool delete_employee(){
	return false;
}


static bool set_work_shifts()
{
	return false;
}

static bool list_monthly_report()
{
	return false;
}

static bool list_annual_report()
{
	return false;
}


static bool quit(void)
{
	return true;
}


static struct {
	enum actions action;
	bool (*control)(void);
} controls[END_OF_ACTIONS] = {
	{.action = REGISTER_EMPLOYEE, .control = register_employee},
	{.action = MOVE_EMPLOYEE, .control = move_employee},
	{.action = DELETE_EMPLOYEE, .control = delete_employee},
	{.action = SET_WORK_SHIFTS, .control = set_work_shifts},
	{.action = LIST_MONTHLY_REPORT, .control = list_monthly_report},
	{.action = LIST_ANNUAL_REPORT, .control = list_annual_report},
	{.action = QUIT, .control = quit}
};

void manager_controller(){
	db_switch_to_manager();

	while(true) {
		int action = get_manager_action();
		if(action >= END_OF_ACTIONS) {
			fprintf(stderr, "Error: unknown action\n");
			continue;
		}
		if (controls[action].control())
			break;

		press_anykey();
	}
}