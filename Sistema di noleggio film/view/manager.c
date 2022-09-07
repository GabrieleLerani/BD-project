#include <stdio.h>
#include "manager.h"
#include "../model/db.h"
#include "../utils/io.h"
#include "../utils/validation.h"

int get_manager_action(void)
{
	char options[7] = {'1','2', '3', '4','5','6','7'};
	char op;

	clear_screen();
	puts("*********************************");
	puts("*    MOVIE RENTAL SYSTEM    *");
	puts("*********************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Register employee");
	puts("2) Move employee");
	puts("3) Delete employee");
	puts("4) Set work shifts");
	puts("5) List monthly employee reports ");
	puts("6) List annual employee reports");
	puts("7) Quit");
	

	op = multi_choice("Select an option", options, 7);
	return op - '1';
}

void get_employee_info(){
	printf("Unimplemanted");
}

void get_employee_id(){
	printf("Unimplemanted");
}

void get_shift_option(){
	printf("Unimplemanted");
}

void show_monthly_report(){
	printf("Unimplemanted");
}

void show_annual_report(){
	printf("Unimplemanted");
}
