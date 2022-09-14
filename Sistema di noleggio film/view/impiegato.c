#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "impiegato.h"
#include "../model/db.h"
#include "../utils/io.h"
#include "../utils/validation.h"

int get_impiegato_action(void)
{
	char options[10] = {'1','2', '3','4','5','6','7','8','9','0'};
	char op;

	clear_screen();
	puts("*********************************");
	puts("*    MOVIE RENTAL SYSTEM    *");
	puts("*********************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Register user");
	puts("2) Register user email");
	puts("3) Register user phone");
	puts("4) View work shifts");
	puts("5) List expired rentals");
	puts("6) List available sector for insert film");
	puts("7) Insert film");
	puts("8) Insert actor for film");
	puts("9) Insert rental");
	
	puts("0) Quit");
	

	op = multi_choice("Select an option", options, 10);
	return op - '1';
}

void get_user_email(struct user *user){
	clear_screen();
	puts("*** Get user email ***\n");
	
	get_input("Insert user fc: ", NUMBER_LEN, user->cf, false);
	
	//Manca validate_email
	while(true){
		if(validate_email(get_input("Insert user email: ", EMAIL_LEN, user->email, false)))
			break;
		fprintf(stderr, "Invalid email!\n");
	}
}

void get_user_phone(struct user *user){
	clear_screen();
	puts("*** Get user phone ***\n");
	get_input("Insert user fc: ", NUMBER_LEN, user->cf, false);
	
	while(true){
		if(validate_phone(get_input("Insert user phone: ", NUMBER_LEN, user->phone, false)))
			break;
		fprintf(stderr, "Invalid phone number!\n");
	}
}


void get_film_for_rent(struct film *film,int num){

	printf("\ninfo for film %d\n",num+1);
	get_input("Insert title: ",TITLE_LEN , film->title, false);
	get_input("Insert producer: ",PRODUCER_LEN , film->producer, false);
	get_input("Insert center: ", NUMBER_LEN , film->center, false);
}

void get_cf(struct employee *emp){
	clear_screen();
	puts("*** Get employee's fiscal code ***\n");
	get_input("Insert your fc: ", NUMBER_LEN, emp->cf, false);

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

void print_expired_rentals(struct rentals *list){
	
	clear_screen();
	puts("*** Expired rentals ***\n");
	puts("Rental id | Start date | End date | Film | Producer | Client fc | Client name ");
	for(int i=0;i< list->num_rent;i++){
		printf("%s  -  %s  -  %s  -  %s  -  %s  -  %s  -  %s\n",
			(list->all_rental[i]).rental_id, 
			(list->all_rental[i]).start_date, 
			(list->all_rental[i]).end_date, 
			(list->all_rental[i]).title, 
			(list->all_rental[i]).producer,
			(list->all_rental[i]).user_cf,
			(list->all_rental[i]).user_name);
	}
	
}

void get_center_id(struct center *center){
	clear_screen();
	puts("*** Get center identifier ***\n");
	get_input("Insert id: ", NUMBER_LEN, center->center_id, false);
}


void get_film(struct film *film){
	clear_screen();
	puts("*** Get film information ***\n");
	get_input("Insert title: ", TITLE_LEN, film->title, false);

	if(yes_or_no("This film is a remake?", 'y', 'n', false, true)){
		film->is_remake = true;
		get_input("Insert original title: ", TITLE_LEN, film->original_title, false);
		get_input("Insert original producer: ", PRODUCER_LEN, film->original_producer, false);
	}

	get_input("Insert producer: ", PRODUCER_LEN, film->producer, false);
	get_input("Insert price: ",NUMBER_LEN , film->price , false);
	get_input("Insert copies: ", NUMBER_LEN, film->copies, false);
	get_input("Insert center: ", NUMBER_LEN, film->center, false);
	get_input("Insert sector: ", NUMBER_LEN, film->sector, false);
	get_input("Insert position: ", NUMBER_LEN, film->position, false);
	
	while(true) {
		if(validate_date(get_input("Production year [YYYY-MM-DD]: ", DATE_LEN, film->production_year, false)))
			break;
		fprintf(stderr, "Invalid date!\n");
	}

	get_input("Type [classico,ultima uscita]: ", NUMBER_LEN, film->type, false);
	
}


void print_work_shifts(struct workshifts *shifts){
	clear_screen();
	puts("*** Your work shifts ***\n");
	puts(" Day | Start hour | End hour");
	for(int i=0;i< shifts->num_shift;i++){
		printf("%s  -  %s   -  %s\n",
			(shifts->all_shifts[i]).day, 
			(shifts->all_shifts[i]).start_time, 
			(shifts->all_shifts[i]).end_time);
	}
}


void print_available_sector(struct all_positions *pos,int num){
	clear_screen();
	puts("*** Free position for insert film ***\n");
	printf("\n----  For center %d  ----\n\n",num);
	puts("  Sector | Position ");
	for(int i=0;i< pos->num_pos;i++){
		printf("	%s  -  %s   \n",
			(pos->pos[i]).sector, 
			(pos->pos[i]).position);
	}
}


void get_film_info_for_add_actor(struct film *film){
	
	char temp_num[NUMBER_LEN];

	clear_screen();
	puts("*** Get actor for film ***\n");
	get_input("Insert film title: ", TITLE_LEN, film->title, false);
	get_input("Insert film producer: ", PRODUCER_LEN, film->producer, false);
	get_input("Insert number of actor: ", NUMBER_LEN, temp_num, false);
	
	film->num_act = atoi(temp_num);
}

void get_actor(struct actor *act,int num){
	
	printf("\ninfo for actor %d\n",num+1);
	get_input("Insert actor name: ", NUMBER_LEN, act->name, false);	
}