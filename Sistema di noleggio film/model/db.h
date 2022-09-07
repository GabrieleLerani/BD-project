#pragma once
#include <stdbool.h>
#include <stdlib.h>

extern bool init_db(void);
extern void fini_db(void);

#define DATE_LEN 11
#define TIME_LEN 6
#define DATETIME_LEN (DATE_LEN + TIME_LEN)

#define USERNAME_LEN 45
#define PASSWORD_LEN 45
struct credentials {
	char username[USERNAME_LEN];
	char password[PASSWORD_LEN];
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
	int price;
	int copies;
	int center;
	int sector;
	int position;
	char production_year[DATE_LEN]; 
	enum type_film type;
};

struct movies {
	unsigned num_film;
	struct film all_film[];
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



extern void dispose_film_list(struct movies *list);






