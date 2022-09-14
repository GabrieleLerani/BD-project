#pragma once
#include "../model/db.h"

enum actions {
	REGISTER_USER,
	REGISTER_USER_EMAIL,
	REGISTER_USER_PHONE,
	VIEW_WORK_SHIFTS,
	LIST_EXPIRED_RENTALS,
	LIST_AVAILABLE_SECTOR,
	INSERT_FILM,
	INSERT_ACTOR,
	INSERT_RENTAL,
	QUIT,
	END_OF_ACTIONS
};

extern int get_impiegato_action(void);
extern void get_rent_info(struct rental *rental); //For rent reg
extern void get_film_for_rent(struct film *film,int num);
extern void get_user_info(struct user *user,struct credentials *cred); //For user reg
extern void print_done_insert();
extern void print_expired_rentals(struct rentals *rentals); //For expired rentals
extern void get_film(struct film *film); //For insert film
extern void print_work_shifts(struct workshifts *shifts);
extern void get_cf(struct employee *emp);
extern void get_user_email(struct user *user);
extern void get_user_phone(struct user *user);
extern void get_center_id(struct center *center);
extern void get_film_info_for_add_actor(struct film *film);
extern void get_actor(struct actor *actor,int num);
extern void print_available_sector(struct all_positions *pos,int num);