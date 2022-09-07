#include <stdio.h>
#include "impiegato.h"
#include "../model/db.h"
#include "../utils/io.h"
#include "../utils/validation.h"

int get_impiegato_action(void)
{
	char options[6] = {'1','2', '3', '4','5','6'};
	char op;

	clear_screen();
	puts("*********************************");
	puts("*    MOVIE RENTAL SYSTEM    *");
	puts("*********************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Register user");
	puts("2) View work shifts");
	puts("3) List expired rentals");
	puts("4) Insert film");
	puts("5) Insert rental");
	puts("6) Quit");
	

	op = multi_choice("Select an option", options, 6);
	return op - '1';
}

void get_rent_info(){
	printf("Unimplemanted");
}

void get_user_info(){
	printf("Unimplemanted");
}

void list_rentals(){
	printf("Unimplemanted");
}

void get_film(){
	printf("Unimplemanted");	
}
