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
#define EMAIL_LEN 100

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
	char id[ID_LEN];
	char email[EMAIL_LEN];
	char phone[NUMBER_LEN];
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

struct actor{
	char name[NUMBER_LEN];
};

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
	char original_title[TITLE_LEN];
	char original_producer[PRODUCER_LEN];
	bool is_remake;
	unsigned num_act;
	struct actor act[];
};


struct position{
	char center[NUMBER_LEN];
	char sector[NUMBER_LEN];
	char position[NUMBER_LEN];
};

struct all_positions{
	unsigned num_pos;
	struct position pos[];
};

struct movies {
	unsigned num_film;
	struct film all_film[];
};

struct rental{
	char rental_id[NUMBER_LEN];
	char user_cf[NUMBER_LEN];
	char user_id[NUMBER_LEN];
	char user_name[NUMBER_LEN];
	char start_date[DATE_LEN];
	char end_date[DATE_LEN];
	char title[TITLE_LEN];
	char producer[PRODUCER_LEN];
	unsigned num_rent;
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

struct employees{
	unsigned num_entries;
	struct employee emp[];
};

#define DAY_LEN 10
struct workshift{
	char cf_emp[NUMBER_LEN];
	char day[DAY_LEN];
	char start_time[TIME_LEN];
	char end_time[TIME_LEN];
	char employment_id[NUMBER_LEN];
};

struct workshifts {
	unsigned num_shift;
	struct workshift all_shifts[];
};


#define TYPE_OF_WORK_LEN 10
struct employment{
	char cf[NUMBER_LEN];
	char start_date[DATE_LEN];
	char end_date[DATE_LEN];
	char type[TYPE_OF_WORK_LEN];
	char center_id[TIME_LEN];
	char employment_id[NUMBER_LEN];
};


struct employments{
	unsigned num_entries;
	struct employment emp[];
};

struct center{
	char center_id[NUMBER_LEN];
	char manager_name[NUMBER_LEN];
	char manager_cf[NUMBER_LEN];
	char center_phone[NUMBER_LEN];
	char center_email[NUMBER_LEN];
	char via[NUMBER_LEN];
	char number[NUMBER_LEN];
	char cap[NUMBER_LEN];
};


struct report_entry{
	char emp_cf[NUMBER_LEN];
	char hours_worked[NUMBER_LEN];
	char start_date[DATE_LEN];
	char end_date[DATE_LEN];
	char weeks[NUMBER_LEN]; 
	char day[DAY_LEN];
	
	
};

struct report{
	unsigned num_entries;
	struct report_entry report[];
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
extern struct movies *get_remake();
extern int do_insert_film(struct film *film);
extern int do_insert_remake(struct film *film);
extern int do_insert_actor(struct film *film,struct actor *actor);
extern int do_register_user(struct user *user,struct credentials *cred);
extern int do_insert_rental(struct rental *rental,struct film *film);
extern int do_register_employee(struct employee *employee,struct credentials *cred);
extern int do_insert_workshift(struct employee *employee,struct workshift *shift);
extern int do_insert_employment(struct employment *emp);
extern int do_move_employee(struct employee *emp,struct employment *e);
extern int do_insert_employment(struct employment *emp);
extern int do_register_center(struct center *c);

extern struct workshifts *get_shifts(struct employee *emp);
extern int do_register_user_email(struct user *user);
extern int do_register_user_phone(struct user *user);
extern struct rentals *get_user_active_rental(struct user *user);
extern struct rentals *get_expired_rental(struct center *center);
extern int do_end_rental(struct rental *rental);
extern struct film *get_actors(struct film *film);
extern struct all_positions *get_sector(struct center *center);
extern struct report *do_get_report(struct center *center);
extern struct employees *do_get_employees();
extern struct employments *do_get_employment_id(struct employment *emp);

extern void dispose_shifts(struct workshifts *shifts);
extern void dispose_film_list(struct movies *list);
extern void dispose_rentals(struct rentals *rentals);
extern void dispose_actors(struct film *film);
extern void dispose_positions(struct all_positions *pos);
extern void dispose_report(struct report *report);
extern void dispose_employee(struct employees *employee);
extern void dispose_emp_id(struct employments *employments);


