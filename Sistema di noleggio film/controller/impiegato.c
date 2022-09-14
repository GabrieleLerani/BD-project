#include <stdio.h>
#include <string.h>
#include "impiegato.h"

#include "../model/db.h"
#include "../view/impiegato.h"
#include "../utils/io.h"

static bool register_user(){
	struct user user;
	struct credentials cred;
	memset(&user,0,sizeof(user));
	memset(&cred,0,sizeof(cred));
	get_user_info(&user,&cred);
	if(do_register_user(&user,&cred) != -1)
		print_done_insert();
	return false;
}

static bool register_user_email()
{
	struct user user;
	memset(&user,0,sizeof(user));
	get_user_email(&user);
	if(do_register_user_email(&user) != -1)
		print_done_insert();
	return false;
}

static bool register_user_phone()
{
	struct user user;
	memset(&user,0,sizeof(user));
	get_user_phone(&user);
	if(do_register_user_phone(&user) != -1)
		print_done_insert();
	return false;
}


static bool view_work_shifts(){
	struct workshifts *all_shifts;
	struct employee emp;
	memset(&emp,0,sizeof(emp));
	get_cf(&emp);
	all_shifts = get_shifts(&emp);
	if(all_shifts != NULL){
		print_work_shifts(all_shifts);
		dispose_shifts(all_shifts);
	}
	return false;
}


static bool list_expired_rentals(){
	struct rentals *rentals;
	struct center center;
	memset(&center,0,sizeof(center));
	get_center_id(&center);
	rentals = get_expired_rental(&center);
	if(rentals != NULL){
		print_expired_rentals(rentals);
		dispose_rentals(rentals);
	}
	return false;
}


static bool insert_rental()
{	
	int val = 0,ret;
	struct rental rental;
	struct film film;
	memset(&rental,0,sizeof(rental));
	memset(&film,0,sizeof(film));
	get_rent_info(&rental);
	
	for(int i = 0; i < rental.num_rent; i++){
		
		get_film_for_rent(&film,i);
		ret = do_insert_rental(&rental,&film);
		memset(&film,0,sizeof(film)); 
		
		if(ret == -1){
			val = -1;
			break;
		}
	}

	if(val == 0)
		print_done_insert();
	
	return false;
}

static bool insert_actor(){
	int val = 0,num;
	struct film film;
	struct actor act;
	memset(&act,0,sizeof(act));
	memset(&film,0,sizeof(film));
	get_film_info_for_add_actor(&film);
	
	for(int i = 0; i < film.num_act; i++){
		
		get_actor(&act,i);
		num = do_insert_actor(&film,&act);
		memset(&act,0,sizeof(act));
		if(num == -1){//do_insert_actor(&film,&act) == -1){
			val = -1;
			break;
		}
	}

	if(val == 0)
		print_done_insert();
	
	return false;
}


static bool insert_film()
{
	struct film film;
	memset(&film,0,sizeof(film));
	get_film(&film);
	
	if(film.is_remake){
		do_insert_remake(&film);
	}
	if(do_insert_film(&film) != -1)
		print_done_insert();
	return false;
}

static bool list_available_sector(){
	
	struct center center;
	struct all_positions *all_pos;
	memset(&center,0,sizeof(center));
	get_center_id(&center);
	all_pos = get_sector(&center);
	if(all_pos != NULL){
		print_available_sector(all_pos,atoi(center.center_id));
		dispose_positions(all_pos);
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
	{.action = REGISTER_USER, .control = register_user},
	{.action = REGISTER_USER_EMAIL, .control = register_user_email},
	{.action = REGISTER_USER_PHONE, .control = register_user_phone},
	{.action = VIEW_WORK_SHIFTS, .control = view_work_shifts},
	{.action = LIST_EXPIRED_RENTALS, .control = list_expired_rentals},
	{.action = LIST_AVAILABLE_SECTOR, .control = list_available_sector},
	{.action = INSERT_FILM, .control = insert_film},
	{.action = INSERT_ACTOR, .control = insert_actor},
	{.action = INSERT_RENTAL, .control = insert_rental},
	{.action = QUIT, .control = quit}
};

void impiegato_controller(){
	db_switch_to_impiegato();

	while(true) {
		int action = get_impiegato_action();
		if(action >= END_OF_ACTIONS) {
			fprintf(stderr, "Error: unknown action\n");
			continue;
		}
		if (controls[action].control())
			break;

		press_anykey();
	}
}