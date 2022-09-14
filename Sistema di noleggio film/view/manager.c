#include <stdio.h>
#include <string.h>
#include "manager.h"
#include "../model/db.h"
#include "../utils/io.h"
#include "../utils/validation.h"

int get_manager_action(void)
{
	char options[9] = {'1', '2', '3', '4','5','6','7','8','9'};
	char op;

	clear_screen();
	puts("*********************************");
	puts("*    MOVIE RENTAL SYSTEM    *");
	puts("*********************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Register employee");
	puts("2) Move employee");
	puts("3) Set employment");
	puts("4) Get employment's id");
	puts("5) Set work shifts");
	puts("6) List employee reports ");
	puts("7) List employees ");
	puts("8) Insert new center");
	puts("9) Quit");
	

	op = multi_choice("Select an option", options, 9);
	return op - '1';
}

void get_employment_info(struct employment *emp){
	clear_screen();
	puts("*** Get employment information ***\n");
	get_input("Insert fiscal code: ", NUMBER_LEN, emp->cf, false);
	get_input("Insert employment [cassiere,commesso]: ", TYPE_OF_WORK_LEN, emp->type, false);
	get_input("Insert center id: ", NUMBER_LEN, emp->center_id, false);
	
	while(true){
		if(validate_date(get_input("Insert start date [YYYY-MM-DD]: ", DATE_LEN, emp->start_date, false)))
			break;
		fprintf(stderr, "Invalid date number!\n");
	}

	while(true){
		get_input("Insert end date [YYYY-MM-DD]/[not defined]: ", DATE_LEN, emp->end_date, false);
		if(validate_date(emp->end_date))
			break;
		fprintf(stderr, "Invalid date number!\n");
	}
	
}

void get_employee_info(struct employee *employee,struct credentials *cred){
	clear_screen();
	puts("*** Get employee information ***\n");
	get_input("Fiscal code: ", NUMBER_LEN, employee->cf, false);
	get_input("Name: ", NUMBER_LEN, employee->name , false);
	get_input("Qualification [diploma,laurea]: ",NUMBER_LEN , employee->qualification, false);
	
	while(true){
		if(validate_phone(get_input("Phone number: ", NUMBER_LEN, employee->phone_number, false)))
			break;
		fprintf(stderr, "Invalid phone number!\n");
	}
	
	get_input("Insert username: ", USERNAME_LEN, cred->username, false);
	get_input("Insert password: ", PASSWORD_LEN, cred->password, true);
}


void get_employee_cf(struct employee *employee,struct employment *emp){
	clear_screen();
	puts("*** Get employee cf ***\n");
	get_input("Insert fiscal code: ", NUMBER_LEN, employee->cf, false);
	get_input("Insert new center id: ", NUMBER_LEN, emp->center_id, false);
	
}

void get_employee_cf_for_id(struct employment *emp){
	clear_screen();
	puts("*** Get employee cf ***\n");
	get_input("Insert fiscal code: ", NUMBER_LEN, emp->cf, false);

}




void get_shift_info(struct employee *employee,struct workshift *work){
	
	clear_screen();
	puts("*** Set employee's workshift ***\n");
	get_input("Insert employee's fiscal code: ", NUMBER_LEN, employee->cf, false);
	get_input("Insert employment id: ", NUMBER_LEN, work->employment_id, false);

	while(true) {
		get_input("Insert day [lunedi,martedi,mercoledi,giovedi,venerdi,sabato]: ", NUMBER_LEN, work->day, false);
		if(validate_day(work->day))
			break;
		fprintf(stderr, "Invalid day!\n");
	}


	while(true) {
		get_input("Insert start hour [HH:MM]: ", TIME_LEN, work->start_time , false);
		if(validate_time(work->start_time))
			break;
		fprintf(stderr, "Invalid time!\n");
	}

	while(true) {
		get_input("Insert end hour [HH:MM]: ", TIME_LEN, work->end_time , false);

		if(validate_time(work->end_time))
			break;
		fprintf(stderr, "Invalid time!\n");
	}
	
}


void get_center_info(struct center *c,enum actions act){
	clear_screen();
	puts("*** Get center information ***\n");
	if(act = LIST_REPORT || act == LIST_EMPLOYEE){
		get_input("Insert center id: ", NUMBER_LEN, c->center_id, false);
	}

	else{

		get_input("Insert manager name: ", NUMBER_LEN, c->manager_name, false);
		get_input("Insert manager's fiscal code: ", NUMBER_LEN, c->manager_cf , false);
		
		while(true){
			if(validate_phone(get_input("Insert center's phone number: ",NUMBER_LEN , c->center_phone, false)))
				break;
			fprintf(stderr, "Invalid phone number!\n");
		}
		
		
		while(true){
		if(validate_email(get_input("Insert center's email: ",NUMBER_LEN , c->center_email, false)))
			break;
		fprintf(stderr, "Invalid email!\n");
	}

		get_input("Insert address: ", NUMBER_LEN, c->via, false);
		get_input("Insert number: ", NUMBER_LEN, c->number , false);
		get_input("Insert cap: ",NUMBER_LEN , c->cap, false);
	}
}



void print_done_insert_man(){
	clear_screen();
	puts("***  Done!  ***\n");
}


void show_report(struct report *report,int num){
	clear_screen();
	puts("*** Monthly report ***\n");

	printf("\n---- Center: %d ----\n\n",num);
	puts("  Employee  |  Start date  |  End date  |  Weeks  |  Day  |  Hours worked  |");
	for(int i=0;i< report->num_entries;i++){
		printf("%s  -  %s  -  %s  -  %s  -  %s  -  %s\n",
			(report->report[i]).emp_cf,
			(report->report[i]).start_date,
			(report->report[i]).end_date,
			(report->report[i]).weeks,
			(report->report[i]).day, 
			(report->report[i]).hours_worked);
	}
	
}


void print_employee(struct employees *employee){
	clear_screen();
	puts("*** All employee  ***\n");

	puts("  Cf  |  Name  |  Qualification  |  Phone number  |  Id  |");
	for(int i=0;i< employee->num_entries;i++){
		printf("%s  -  %s  -  %s  -  %s  -  %s \n",
			(employee->emp[i]).cf,
			(employee->emp[i]).name,
			(employee->emp[i]).qualification,
			(employee->emp[i]).phone_number,
			(employee->emp[i]).id);
	}
	
}

void print_id(struct employments *emp){
	clear_screen();
	puts("*** Employment's id  ***\n");

	for(int i=0;i< emp->num_entries;i++){
		printf("	%s   \n", (emp->emp[i]).employment_id);
	}
	
}
