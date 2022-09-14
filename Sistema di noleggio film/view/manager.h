#pragma once
#include "../model/db.h"

enum actions {
	REGISTER_EMPLOYEE,
	MOVE_EMPLOYEE,
	DELETE_EMPLOYEE,
	SET_EMPLOYMENT,
	GET_EMPLOYMENT_ID,
	SET_WORK_SHIFTS,
	LIST_REPORT,
	LIST_EMPLOYEE,
	REGISTER_CENTER,
	QUIT,
	END_OF_ACTIONS
};

extern int get_manager_action(void);
extern void get_employee_info(struct employee *employee,struct credentials *cred);
extern void get_employee_cf(struct employee *employee,struct employment *emp);
extern void get_employment_info(struct employment *emp);
extern void get_shift_info(struct employee *employee,struct workshift *work);
extern void show_report(struct report *report,int num); 
extern void get_center_info(struct center *cen,enum actions act);
extern void print_employee(struct employees *employee);
extern void get_employee_cf_for_id(struct employment *emp);
extern void print_id(struct employments *emp);

extern void print_done_insert_man(void);