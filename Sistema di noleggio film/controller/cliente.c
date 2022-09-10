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
		print_movies(all_movies);
		dispose_film_list(all_movies);
	}
	return false;
}

static bool list_active_rentals(){
	return false;
}


static bool return_movies(){
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
	{.action = ACTIVE_RENTALS, .control = list_active_rentals},
	{.action = RETURN_MOVIES, .control = return_movies},
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