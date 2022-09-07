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
extern void get_rent_info(void); //For rent reg
extern void get_user_info(void); //For user reg
extern void list_rentals(void); //For expired rentals
extern void get_film(void); //For insert film
