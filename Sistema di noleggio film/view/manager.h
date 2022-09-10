#pragma once
#include "../model/db.h"

enum actions {
	REGISTER_EMPLOYEE,
	MOVE_EMPLOYEE,
	DELETE_EMPLOYEE,
	SET_EMPLOYMENT,
	SET_WORK_SHIFTS,
	LIST_MONTHLY_REPORT,
	LIST_ANNUAL_REPORT,
	REGISTER_CENTER,
	QUIT,
	END_OF_ACTIONS
};

extern int get_manager_action(void);
extern void get_employee_info(struct employee *employee,struct credentials *cred);//For emp reg
extern void get_employee_cf(struct employee *employee,struct employment *emp,enum actions act);
extern void get_employment_info(struct employment *emp);
extern void get_shift_info(struct employee *employee,struct workshift *work);
extern void show_monthly_report(void); //For expired rentals
extern void show_annual_report(void);
extern void get_center_info(struct center *cen);


extern void print_done_insert_man(void);