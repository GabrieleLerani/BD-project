#include <stdio.h>
#include <string.h>
#include "manager.h"


#include "../model/db.h"
#include "../view/manager.h"
#include "../utils/io.h"

static bool register_center(){
	struct center cen;
	memset(&cen,0,sizeof(cen));
	get_center_info(&cen,REGISTER_CENTER);
	if(do_register_center(&cen) != -1){
		print_done_insert_man();
	}
	return false;
}

static bool register_employee(){
	struct employee emp;
	struct credentials cred;
	memset(&emp,0,sizeof(emp));
	memset(&cred,0,sizeof(cred));
	get_employee_info(&emp,&cred);
	if(do_register_employee(&emp,&cred) != -1){
		print_done_insert_man();
	}
	return false;
}

static bool move_employee(){
	struct employee emp;
	struct employment e;
	memset(&e,0,sizeof(e));
	memset(&emp,0,sizeof(emp));
	get_employee_cf(&emp,&e);
	if(do_move_employee(&emp,&e) != -1){
		print_done_insert_man();
	}
	
	return false;
}




static bool set_employment(){
	
	struct employment emp;
	memset(&emp,0,sizeof(emp));
	get_employment_info(&emp);
	if(do_insert_employment(&emp) != -1){
		print_done_insert_man();
	}
	
	return false;
}



static bool set_work_shifts()
{
	struct workshift work;
	struct employee emp;
	memset(&emp,0,sizeof(emp));
	memset(&work,0,sizeof(work));
	get_shift_info(&emp,&work);
	if(do_insert_workshift(&emp,&work) != -1){
		print_done_insert_man();
	}
	return false;
}

static bool list_report()
{
	struct center center;
	struct report *report;
	memset(&center,0,sizeof(center));
	get_center_info(&center,LIST_REPORT);
	report = do_get_report(&center);
	if(report != NULL){
		show_report(report,atoi(center.center_id));
		dispose_report(report);
	}
	return false;
}

static bool list_employee()
{
	
	struct employees *emp;
	emp = do_get_employees();
	if(emp != NULL){
		print_employee(emp);
		dispose_employee(emp);
	}
	return false;
}

static bool get_employment_id()
{
	
	struct employment emp;
	struct employments *id;
	memset(&emp,0,sizeof(emp));
	get_employee_cf_for_id(&emp);
	id = do_get_employment_id(&emp);
	if(id != NULL){
		print_id(id);
		dispose_emp_id(id);
	}
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
	{.action = SET_EMPLOYMENT, .control = set_employment},
	{.action = GET_EMPLOYMENT_ID, .control = get_employment_id},
	{.action = SET_WORK_SHIFTS, .control = set_work_shifts},
	{.action = LIST_REPORT, .control = list_report},
	{.action = LIST_EMPLOYEE, .control = list_employee},
	{.action = REGISTER_CENTER, .control = register_center},
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