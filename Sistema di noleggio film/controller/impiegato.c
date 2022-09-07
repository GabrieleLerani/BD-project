#include <stdio.h>
#include <string.h>
#include "impiegato.h"

#include "../model/db.h"
#include "../view/impiegato.h"
#include "../utils/io.h"

static bool register_user(){
	
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
	return false;
}

static bool insert_film()
{
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