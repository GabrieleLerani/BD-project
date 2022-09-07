#pragma once
#include "../model/db.h"

enum actions {
	REGISTER_EMPLOYEE,
	MOVE_EMPLOYEE,
	DELETE_EMPLOYEE,
	SET_WORK_SHIFTS,
	LIST_MONTHLY_REPORT,
	LIST_ANNUAL_REPORT,
	QUIT,
	END_OF_ACTIONS
};

extern int get_manager_action(void);
extern void get_employee_info(void);//For emp reg
extern void get_employee_id(void);//For shifts,move and delete
extern void get_shift_option(void);
extern void show_monthly_report(void); //For expired rentals
extern void show_annual_report(void);
//TODO