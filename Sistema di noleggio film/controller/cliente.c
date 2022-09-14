#include <stdio.h>
#include <string.h>
#include "cliente.h"

#include "../model/db.h"
#include "../view/cliente.h"
#include "../utils/io.h"

static bool list_film(){
	struct movies *all_movies;
	all_movies = get_available_movies();
	if(all_movies!=NULL){
		print_movies(all_movies,LIST_FILM);
		dispose_film_list(all_movies);
	}
	return false;
}

static bool end_rental(){
	
	struct rental rental;
	memset(&rental,0,sizeof(rental));
	get_rent_info_for_return(&rental);
	if(do_end_rental(&rental) != -1)
		print_done_insert_user();
	return false;
}


static bool list_active_rentals(){
	struct rentals *rentals;
	struct user user;
	memset(&user,0,sizeof(user));
	get_user_cf(&user);
	rentals = get_user_active_rental(&user);
	if(rentals != NULL){
		print_active_rentals(rentals);
		dispose_rentals(rentals);
	}
	return false;
}

static bool list_actors(){
	struct film film;
	struct film *actors;
	memset(&film,0,sizeof(film));
	get_film_info(&film);
	actors = get_actors(&film);
	if(actors != NULL){
		print_actors(actors,&film);
		dispose_actors(actors);
	}
	return false;
}

static bool list_remake(){
	struct movies *all_movies;
	all_movies = get_remake();
	if(all_movies!=NULL){
		print_movies(all_movies,LIST_REMAKE);
		dispose_film_list(all_movies);
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
	{.action = LIST_FILM, .control = list_film},
	{.action = LIST_ACTORS, .control = list_actors},
	{.action = ACTIVE_RENTALS, .control = list_active_rentals},
	{.action = LIST_REMAKE, .control = list_remake},
	{.action = END_RENTAL, .control = end_rental},
	{.action = QUIT, .control = quit}
};


void cliente_controller(void)
{
	db_switch_to_cliente();

	while(true) {
		int action = get_client_action();
		if(action >= END_OF_ACTIONS) {
			fprintf(stderr, "Error: unknown action\n");
			continue;
		}
		if (controls[action].control())
			break;

		press_anykey();
	}
}