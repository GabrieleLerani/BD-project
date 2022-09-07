#pragma once
#include "../model/db.h"

enum actions {
	LIST_FILM,
	ACTIVE_RENTALS,
	RETURN_MOVIES,
	QUIT,
	END_OF_ACTIONS
};

extern int get_client_action(void);
extern void get_rent_info_for_return();
extern void print_movies();
extern void print_active_rentals();
extern void get_film_info(void); //For return film
