#pragma once
#include "../model/db.h"

enum actions {
	REGISTER_USER,
	VIEW_WORK_SHIFTS,
	LIST_EXPIRED_RENTALS,
	INSERT_FILM,
	INSERT_RENTAL,
	QUIT,
	END_OF_ACTIONS
};

extern int get_impiegato_action(void);
extern void get_rent_info(struct rental *rental); //For rent reg
extern void get_film_for_rent(struct film *film,int num);
extern void get_user_info(struct user *user,struct credentials *cred); //For user reg
extern void print_done_insert();
extern void list_rentals(void); //For expired rentals
extern void get_film(struct film *film); //For insert film
