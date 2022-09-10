#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "impiegato.h"
#include "../model/db.h"
#include "../utils/io.h"
#include "../utils/validation.h"

int get_impiegato_action(void)
{
	char options[6] = {'1','2', '3','4','5','6'};
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


void get_film_for_rent(struct film *film,int num){
/*
	for(int i = 0; i < rental->num_rent; i++){
		printf("\ninfo for film %d\n",i+1);
		get_input("Insert title: ",TITLE_LEN , film->title, false);
		get_input("Insert producer: ",PRODUCER_LEN , film->producer, false);
	
	}
*/	
	printf("\ninfo for film %d\n",num+1);
	get_input("Insert title: ",TITLE_LEN , film->title, false);
	get_input("Insert producer: ",PRODUCER_LEN , film->producer, false);
	

}



void get_rent_info(struct rental *rental){
	char temp_num[NUMBER_LEN];
	clear_screen();
	puts("*** Get rent information ***\n");
	get_input("Insert client id: ", NUMBER_LEN, rental->user_id, false);
	while(true) {
		if(validate_date(get_input("Insert the rental end date [YYYY-MM-DD]: ", DATE_LEN, rental->end_date, false)))
			break;
		fprintf(stderr, "Invalid date!\n");
	}

	get_input("Insert number of film: ", NUMBER_LEN, temp_num , false);
	
	rental->num_rent = atoi(temp_num);

}

void get_user_info(struct user *user,struct credentials *cred){
	clear_screen();
	puts("*** Get user information ***\n");
	get_input("Fiscal code: ", NUMBER_LEN, user->cf, false);
	get_input("Name: ", NUMBER_LEN, user->name , false);
	get_input("Lastname: ",NUMBER_LEN , user->lastname, false);
	while(true) {
		if(validate_date(get_input("Birthday [YYYY-MM-DD]: ", DATE_LEN, user->birthday, false)))
			break;
		fprintf(stderr, "Invalid date!\n");
	}
	get_input("Birthplace: ", NUMBER_LEN, user->birthplace, false);
	get_input("Insert username: ", USERNAME_LEN, cred->username, false);
	get_input("Insert password: ", PASSWORD_LEN, cred->password, true);
	
}

void print_done_insert(){
	clear_screen();
	puts("***  Done!  ***\n");
	
}

void list_rentals(){
	printf("Unimplemanted");
}

void get_film(struct film *film){
	clear_screen();
	puts("*** Get film information ***\n");
	get_input("Title: ", TITLE_LEN, film->title, false);
	get_input("Producer: ", PRODUCER_LEN, film->producer, false);
	get_input("Price: ",NUMBER_LEN , film->price , false);
	get_input("Copies: ", NUMBER_LEN, film->copies, false);
	get_input("Center: ", NUMBER_LEN, film->center, false);
	get_input("Sector: ", NUMBER_LEN, film->sector, false);
	get_input("Position: ", NUMBER_LEN, film->position, false);
	
	while(true) {
		if(validate_date(get_input("Production year [YYYY-MM-DD]: ", DATE_LEN, film->production_year, false)))
			break;
		fprintf(stderr, "Invalid date!\n");
	}

	get_input("Type [classico,nuova uscita]: ", NUMBER_LEN, film->type, false);

}
