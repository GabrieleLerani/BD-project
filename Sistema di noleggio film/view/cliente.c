#include <stdio.h>
#include "cliente.h"
#include "../model/db.h"
#include "../utils/io.h"
#include "../utils/validation.h"

int get_client_action(void)
{
	char options[4] = {'1','2', '3', '4'};
	char op;

	clear_screen();
	puts("*********************************");
	puts("*    MOVIE RENTAL SYSTEM    *");
	puts("*********************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) List available movies");
	puts("2) List active rentals");
	puts("3) Return movies");
	puts("4) Quit");

	op = multi_choice("Select an option", options, 4);
	return op - '1';
}


//TODO
void get_rent_info_for_return(){
	printf("Unimplemanted");
}


void print_movies(struct movies *list){
	clear_screen();
	puts("*** Available movies ***\n");
	puts("Title | Producer | Price | Copies | Year");
	for(int i=0;i< list->num_film;i++){
		printf("%s  -  %s  -  %s$  -  %s  -  %s\n",
			(list->all_film[i]).title, 
			(list->all_film[i]).producer, 
			(list->all_film[i]).price, 
			(list->all_film[i]).copies, 
			(list->all_film[i]).production_year);
	}
}

void print_active_rentals(struct rentals *list){
	/*
	clear_screen();
	puts("*** Active rentals ***\n");
	puts("");
	for(int i=0;i< list->num_film;i++){
		printf("%s  -  %s  -  %d$  -  %d  -  %s\n",
			(list->all_film[i]).title, 
			(list->all_film[i]).producer, 
			(list->all_film[i]).price, 
			(list->all_film[i]).copies, 
			(list->all_film[i]).production_year);
	}
	TODO	
	*/
}

void get_film_info(){
	printf("Unimplemanted");
}

