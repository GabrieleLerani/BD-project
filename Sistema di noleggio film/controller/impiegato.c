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
	int id = do_register_user(&user,&cred);
	if(id != -1)
		print_done_insert();
	return false;
}

static bool view_work_shifts(){
	return false;
}


static bool list_expired_rentals(){
	return false;
}


static bool insert_rental()
{	
	int val = 0;
	struct rental rental;
	struct film film;
	memset(&rental,0,sizeof(rental));
	memset(&film,0,sizeof(film));
	get_rent_info(&rental);
	
	//ricorda di deallocare memoria
	//film = malloc(sizeof(struct film *) * rental.num_rent);
	//rental.rented_movies = malloc(sizeof(struct film *) * rental.num_rent);
	
	
	

/*	//Alloco memoria
	for(int i=0;i<rental.num_rent;i++){
		rental.rented_movies[i] = malloc(sizeof(struct film));
		
		memset(rental.rented_movies[i],0,sizeof(rental.rented_movies[i]));

	}
*/

	for(int i = 0; i < rental.num_rent; i++){
		
		get_film_for_rent(&film,i);

		if(do_insert_rental(&rental,&film) == -1){
			val = -1;
			break;
		}
	}


	if(val == 0)
		print_done_insert();
	

/*	for(int i=0;i<rental.num_rent;i++){
		free(rental.rented_movies[i]);
	}

*/
	return false;
}

static bool insert_film()
{
	struct film film;
	memset(&film,0,sizeof(film));
	get_film(&film);
	do_insert_film(&film);
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
	{.action = VIEW_WORK_SHIFTS, .control = view_work_shifts},
	{.action = LIST_EXPIRED_RENTALS, .control = list_expired_rentals},
	{.action = INSERT_FILM, .control = insert_film},
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