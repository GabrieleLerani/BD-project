#include <stdio.h>
#include "cliente.h"
#include "../model/db.h"
#include "../utils/io.h"
#include "../utils/validation.h"

int get_client_action(void)
{
	char options[6] = {'1','2', '3', '4','5','6'};
	char op;

	clear_screen();
	puts("*********************************");
	puts("*    MOVIE RENTAL SYSTEM    *");
	puts("*********************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) List available movies");
	puts("2) List actors for film");
	puts("3) List active rentals");
	puts("4) List all remake");
	puts("5) End rental");
	puts("6) Quit");

	op = multi_choice("Select an option", options, 6);
	return op - '1';
}

//Un utente non può terminare noleggi di altri clienti
void get_rent_info_for_return(struct rental *rent){
	clear_screen();
	puts("*** Get your rental id ***\n");
	get_input("Insert id: ", NUMBER_LEN, rent->rental_id, false);
}





void print_movies(struct movies *list,enum actions action){
	clear_screen();
	if(action != LIST_REMAKE){
		puts("*** Available movies ***\n");
		puts("Title | Producer 	| Price | Year");
		for(int i=0;i< list->num_film;i++){
			printf("%s  -  %s  	-  %s$   -  %s\n",
				(list->all_film[i]).title, 
				(list->all_film[i]).producer, 
				(list->all_film[i]).price, 
				(list->all_film[i]).production_year);
		}
	}

	else{
		puts("*** Available remake ***\n");
		puts("Original title | Original producer | Remake title | Remake producer ");
		for(int i=0;i< list->num_film;i++){
			printf("%s  -  %s  	-  %s$   -  %s\n",
				(list->all_film[i]).original_title, 
				(list->all_film[i]).original_producer, 
				(list->all_film[i]).title, 
				(list->all_film[i]).producer);
		}
	}

} 


void get_user_cf(struct user *user){
	clear_screen();
	puts("*** Get user's fiscal code ***\n");
	get_input("Insert your personal fc: ", NUMBER_LEN, user->cf, false);
}


void print_active_rentals(struct rentals *list){
	
	clear_screen();
	puts("*** Active rentals ***\n");
	puts("Rental id | Start date | End date | Film 		| Producer");
	for(int i=0;i< list->num_rent;i++){
		printf("%s  -  %s  -  %s  -  %s  -  %s\n",
			(list->all_rental[i]).rental_id, 
			(list->all_rental[i]).start_date, 
			(list->all_rental[i]).end_date, 
			(list->all_rental[i]).title, 
			(list->all_rental[i]).producer);
	}
	
}

void print_done_insert_user(){
	clear_screen();
	puts("***  Done!  ***\n");
}


void get_film_info(struct film *film){
	clear_screen();
	puts("*** Get film ***\n");
	get_input("Insert title: ", TITLE_LEN, film->title, false);
	get_input("Insert producer: ", PRODUCER_LEN, film->producer, false);
}

void print_actors(struct film *actors,struct film *film){
	
	clear_screen();
	puts("*** Actors ***\n");
	printf("\n-- Actors of %s --\n",film->title);
	for(int i=0;i< actors->num_act;i++){
		printf("%s  \n",(actors->act[i]).name);
	}
}
	