#pragma once
#include "../model/db.h"

enum actions {
	LIST_FILM,
	LIST_ACTORS,
	ACTIVE_RENTALS,
	LIST_REMAKE,
	END_RENTAL,
	QUIT,
	END_OF_ACTIONS
};

extern int get_client_action(void);
extern void get_rent_info_for_return(struct rental *rent);
extern void print_movies(struct movies *list,enum actions act);
extern void print_active_rentals(struct rentals *list);
extern void print_actors(struct film *actors,struct film *film);
extern void get_film_info(struct film *film); //For return film
extern void get_user_cf(struct user *user);
extern void print_done_insert_user();
