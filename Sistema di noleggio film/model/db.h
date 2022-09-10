#pragma once
#include <stdbool.h>
#include <stdlib.h>

extern bool init_db(void);
extern void fini_db(void);

#define DATE_LEN 11
#define TIME_LEN 6
#define DATETIME_LEN (DATE_LEN + TIME_LEN)
#define NUMBER_LEN 45
#define ID_LEN 145

#define USERNAME_LEN 45
#define PASSWORD_LEN 45
struct credentials {
	char username[USERNAME_LEN];
	char password[PASSWORD_LEN];
};

struct user{
	char cf[NUMBER_LEN];
	char name[NUMBER_LEN];
	char lastname[NUMBER_LEN];
	char birthday[DATE_LEN];
	char birthplace[NUMBER_LEN];
	char ID[NUMBER_LEN];
	//struct credentials *cred;
	char id[ID_LEN];
};	


typedef enum {
	LOGIN_ROLE,
	MANAGER,
	IMPIEGATO,
	CLIENTE,
	FAILED_LOGIN
} role_t;


enum type_film {
	CLASSICO,
	ULTIMA_USCITA
};


#define TITLE_LEN 45
#define PRODUCER_LEN 45

struct film {
	char title[TITLE_LEN];
	char producer[PRODUCER_LEN];
	char price[NUMBER_LEN];
	char copies[NUMBER_LEN];
	char center[NUMBER_LEN];
	char sector[NUMBER_LEN];
	char position[NUMBER_LEN];
	char production_year[DATE_LEN]; 
	char type[NUMBER_LEN];
};

struct movies {
	unsigned num_film;
	struct film all_film[];
};

struct rental{
	char rental_id[NUMBER_LEN];
	char user_id[NUMBER_LEN];
	char start_date[DATE_LEN];
	char end_date[DATE_LEN];
	unsigned num_rent;
	struct film **rented_movies;

};

struct rentals{
	unsigned num_rent;
	struct rental all_rental[];
};

struct employee{
	char cf[NUMBER_LEN];
	char name[NUMBER_LEN];
	char qualification[NUMBER_LEN];
	char phone_number[NUMBER_LEN];
	char id[ID_LEN];
};

#define DAY_LEN 10
struct workshift{
	char cf_emp[NUMBER_LEN];
	char day[DAY_LEN];
	char start_time[TIME_LEN];
	char end_time[TIME_LEN];
};

#define TYPE_OF_WORK_LEN 10
struct employment{
	char cf[NUMBER_LEN];
	char start_date[DATE_LEN];
	char end_date[DATE_LEN];
	char type[TYPE_OF_WORK_LEN];
	char center_id[TIME_LEN];
};

struct center{
	char manager_name[NUMBER_LEN];
	char manager_cf[NUMBER_LEN];
	char center_phone[NUMBER_LEN];
	char center_email[NUMBER_LEN];
	char via[NUMBER_LEN];
	char number[NUMBER_LEN];
	char cap[NUMBER_LEN];
};



extern bool init_db(void);
extern void fini_db(void);
static bool initialize_prepared_stmts(role_t for_role);
static void close_prepared_stmts(void);

extern void db_switch_to_login(void);
extern void db_switch_to_impiegato(void);
extern void db_switch_to_cliente(void);
extern void db_switch_to_manager(void);
extern role_t attempt_login(struct credentials *cred);
extern struct movies *get_available_movies();
extern void do_insert_film(struct film *film);
extern int do_register_user(struct user *user,struct credentials *cred);
extern int do_insert_rental(struct rental *rental,struct film *film);
extern int do_register_employee(struct employee *employee,struct credentials *cred);
extern int do_insert_workshift(struct employee *employee,struct workshift *shift);
extern int do_insert_employment(struct employment *emp);
extern int do_move_employee(struct employee *emp,struct employment *e);
extern int do_insert_employment(struct employment *emp);
extern int do_register_center(struct center *c);
extern int do_remove_employee(struct employee *emp);

extern void dispose_film_list(struct movies *list);






