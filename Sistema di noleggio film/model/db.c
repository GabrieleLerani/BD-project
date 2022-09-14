#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <assert.h>

#include "db.h"
#include "../utils/db.h"

static MYSQL *conn;

static MYSQL_STMT *login_procedure;
static MYSQL_STMT *list_film_procedure;
static MYSQL_STMT *get_rem;
static MYSQL_STMT *ins_film;
static MYSQL_STMT *ins_rem;
static MYSQL_STMT *ins_user;
static MYSQL_STMT *ins_rent;
static MYSQL_STMT *ins_emp;
static MYSQL_STMT *ins_shift;
static MYSQL_STMT *ins_employment;
static MYSQL_STMT *ins_cen;
static MYSQL_STMT *move_emp;
static MYSQL_STMT *list_shifts;
static MYSQL_STMT *list_rentals;
static MYSQL_STMT *list_exp_rentals;
static MYSQL_STMT *ins_email;
static MYSQL_STMT *ins_phone;
static MYSQL_STMT *end_rent;
static MYSQL_STMT *ins_act;
static MYSQL_STMT *get_act;
static MYSQL_STMT *get_pos;
static MYSQL_STMT *get_report;
static MYSQL_STMT *get_employee;
static MYSQL_STMT *get_employment_id;


static void close_prepared_stmts(void)
{
	if(login_procedure) {
		mysql_stmt_close(login_procedure);
		login_procedure = NULL;
	}
	if(ins_film) {
		mysql_stmt_close(ins_film);
		ins_film = NULL;
	}
	if(ins_rem) {
		mysql_stmt_close(ins_rem);
		ins_rem = NULL;
	}
	if(ins_act) {
		mysql_stmt_close(ins_act);
		ins_act = NULL;
	}
	if(ins_user) {
		mysql_stmt_close(ins_user);
		ins_user = NULL;
	}
	if(ins_rent) {
		mysql_stmt_close(ins_rent);
		ins_rent = NULL;
	}
	if(list_film_procedure) {
		mysql_stmt_close(list_film_procedure);
		list_film_procedure = NULL;
	}
	if(get_rem) {
		mysql_stmt_close(get_rem);
		get_rem = NULL;
	}
	if(ins_emp) {
		mysql_stmt_close(ins_emp);
		ins_emp = NULL;
	}
	if(ins_shift) {
		mysql_stmt_close(ins_shift);
		ins_shift = NULL;
	}
	if(ins_employment) {
		mysql_stmt_close(ins_employment);
		ins_employment = NULL;
	}
	if(ins_cen) {
		mysql_stmt_close(ins_cen);
		ins_cen = NULL;
	}
	if(move_emp) {
		mysql_stmt_close(move_emp);
		move_emp = NULL;
	}
	
	if(list_shifts) {
		mysql_stmt_close(list_shifts);
		list_shifts = NULL;
	}
	if(list_rentals) {
		mysql_stmt_close(list_rentals);
		list_rentals = NULL;
	}
	if(list_exp_rentals) {
		mysql_stmt_close(list_exp_rentals);
		list_exp_rentals = NULL;
	}
	if(ins_email) {
		mysql_stmt_close(ins_email);
		ins_email = NULL;
	}
	if(ins_phone) {
		mysql_stmt_close(ins_phone);
		ins_phone = NULL;
	}
	if(end_rent) {
		mysql_stmt_close(end_rent);
		end_rent = NULL;
	}
	if(get_act) {
		mysql_stmt_close(get_act);
		get_act = NULL;
	}
	if(get_pos) {
		mysql_stmt_close(get_pos);
		get_pos = NULL;
	}
	if(get_report) {
		mysql_stmt_close(get_report);
		get_report = NULL;
	}
	if(get_employee) {
		mysql_stmt_close(get_employee);
		get_employee = NULL;
	}
	if(get_employment_id) {
		mysql_stmt_close(get_employment_id);
		get_employment_id = NULL;
	}
}



static bool initialize_prepared_stmts(role_t for_role)
{
	switch(for_role) {

		case LOGIN_ROLE:
			if(!setup_prepared_stmt(&login_procedure, "call login(?, ?, ?)", conn)) {
				print_stmt_error(login_procedure, "Unable to initialize login statement\n");
				return false;
			}
			break;
		case MANAGER:
			if(!setup_prepared_stmt(&ins_emp, "call insert_employee(?, ?, ?, ?, ?, ?)", conn)) {
				print_stmt_error(ins_emp, "Unable to initialize insert employee statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&ins_shift, "call insert_workshift(?, ?, ?, ?, ?)", conn)) {
				print_stmt_error(ins_shift, "Unable to initialize insert shift statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&ins_employment, "call insert_employment(?, ?, ?, ?, ?)", conn)) {
				print_stmt_error(ins_employment, "Unable to initialize insert employment statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&move_emp, "call move_employee(?, ?)", conn)) {
				print_stmt_error(move_emp, "Unable to initialize move employee statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&ins_cen, "call register_center(?, ?, ?, ?, ?, ?, ?)", conn)) {
				print_stmt_error(ins_cen, "Unable to initialize register center statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&get_report, "call get_report(?)", conn)) {
				print_stmt_error(get_report, "Unable to initialize get report statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&get_employee, "call get_employees()", conn)) {
				print_stmt_error(get_employee, "Unable to initialize get employees statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&get_employment_id, "call get_employment_id(?)", conn)) {
				print_stmt_error(get_employment_id, "Unable to initialize get employment id statement\n");
				return false;
			}
			break;
		case IMPIEGATO:
			if(!setup_prepared_stmt(&ins_film, "call insert_film(?, ?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
				print_stmt_error(ins_film, "Unable to initialize insert film statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&ins_rem, "call insert_remake(?, ?, ?, ?)", conn)) {
				print_stmt_error(ins_rem, "Unable to initialize insert remake statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&ins_act, "call insert_actor(?, ?, ?)", conn)) {
				print_stmt_error(ins_act, "Unable to initialize insert actor statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&ins_user, "call insert_user(?, ?, ?, ?, ?, ?, ?)", conn)) {
				print_stmt_error(ins_user, "Unable to initialize insert user statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&ins_rent, "call insert_rental(?, ?, ?, ?, ?)", conn)) {
				print_stmt_error(ins_rent, "Unable to initialize insert rental statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&list_shifts, "call list_shifts(?)", conn)) {
				print_stmt_error(list_shifts, "Unable to initialize list shifts statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&ins_email, "call insert_email(?, ?)", conn)) {
				print_stmt_error(ins_email, "Unable to initialize insert email statement\n");
				return false;
			}

			if(!setup_prepared_stmt(&ins_phone, "call insert_phone(?, ?)", conn)) {
				print_stmt_error(ins_phone, "Unable to initialize insert phone statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&list_exp_rentals, "call list_expired_rentals(?)", conn)) {
				print_stmt_error(list_exp_rentals, "Unable to initialize list expired rentals statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&get_pos, "call get_positions(?)", conn)) {
				print_stmt_error(get_pos, "Unable to initialize get positions statement\n");
				return false;
			}
			break;
		case CLIENTE:
			if(!setup_prepared_stmt(&list_film_procedure, "call list_available_film()", conn)) {
				print_stmt_error(list_film_procedure, "Unable to initialize list film statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&list_rentals, "call list_active_rentals(?)", conn)) {
				print_stmt_error(list_rentals, "Unable to initialize list active rentals statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&end_rent, "call end_rental(?)", conn)) {
				print_stmt_error(end_rent, "Unable to initialize end rental statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&get_act, "call get_actors(?, ?)", conn)) {
				print_stmt_error(get_act, "Unable to initialize get actors statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&get_rem, "call get_remake()", conn)) {
				print_stmt_error(get_rem, "Unable to initialize get remake statement\n");
				return false;
			}
			break;
			
		default:
			fprintf(stderr, "[FATAL] Unexpected role to prepare statements.\n");
			exit(EXIT_FAILURE);
	}

	return true;
}

bool init_db(void)
{
	unsigned int timeout = 300;
	bool reconnect = true;

	conn = mysql_init(NULL);
	if(conn == NULL) {
		finish_with_error(conn, "mysql_init() failed (probably out of memory)\n");
	}

	if(mysql_real_connect(conn, getenv("HOST"), getenv("LOGIN_USER"), getenv("LOGIN_PASS"), getenv("DB"),
			      atoi(getenv("PORT")), NULL,
			      CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS) == NULL) {
		finish_with_error(conn, "mysql_real_connect() failed\n");
	}

	if (mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout)) {
		print_error(conn, "[mysql_options] failed.");
	}
	if(mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect)) {
		print_error(conn, "[mysql_options] failed.");
	}
#ifndef NDEBUG
	mysql_debug("d:t:O,/tmp/client.trace");
	if(mysql_dump_debug_info(conn)) {
		print_error(conn, "[debug_info] failed.");
	}
#endif

	return initialize_prepared_stmts(LOGIN_ROLE);
}


void fini_db(void)
{
	close_prepared_stmts();

	mysql_close(conn);
}


role_t attempt_login(struct credentials *cred)
{
	MYSQL_BIND param[3]; // Used both for input and output
	int role = 0;

	
	// Prepare parameters
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cred->username, strlen(cred->username));
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, cred->password, strlen(cred->password));
	set_binding_param(&param[2], MYSQL_TYPE_LONG, &role, sizeof(role));

	if(mysql_stmt_bind_param(login_procedure, param) != 0) { // Note _param
		print_stmt_error(login_procedure, "Could not bind parameters for login");
		role = FAILED_LOGIN;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(login_procedure) != 0) {
		print_stmt_error(login_procedure, "Could not execute login procedure");
		role = FAILED_LOGIN;
		goto out;
	}

	// Prepare output parameters
	set_binding_param(&param[0], MYSQL_TYPE_LONG, &role, sizeof(role));

	if(mysql_stmt_bind_result(login_procedure, param)) {
		print_stmt_error(login_procedure, "Could not retrieve output parameter");
		role = FAILED_LOGIN;
		goto out;
	}

	// Retrieve output parameter
	if(mysql_stmt_fetch(login_procedure)) {
		print_stmt_error(login_procedure, "Could not buffer results");
		role = FAILED_LOGIN;
		goto out;
	}

    out:

	mysql_stmt_free_result(login_procedure);
    while(mysql_stmt_next_result(login_procedure) != -1) {
    	mysql_stmt_free_result(login_procedure);
    }
	mysql_stmt_reset(login_procedure);
	
	return role;
}


void db_switch_to_login(void)
{
	close_prepared_stmts();

	mysql_close(conn);
	conn = mysql_init(NULL);


	if(mysql_real_connect(conn, getenv("HOST"), getenv("LOGIN_USER"), getenv("LOGIN_PASS"), getenv("DB"),
			      atoi(getenv("PORT")), NULL,
			      CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS) == NULL) {
		finish_with_error(conn, "mysql_real_connect() failed\n");
	}

	if(!initialize_prepared_stmts(LOGIN_ROLE)) {
		fprintf(stderr, "[FATAL] Cannot initialize prepared statements.\n");
		exit(EXIT_FAILURE);
	}
}


void db_switch_to_impiegato(void)
{
	close_prepared_stmts();
	mysql_close(conn);
	conn = mysql_init(NULL);
	
	if(mysql_real_connect(conn, getenv("HOST"), getenv("IMPIEGATO_USER"), getenv("IMPIEGATO_PASS"), getenv("DB"),
			      atoi(getenv("PORT")), NULL,
			      CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS) == NULL) {
		finish_with_error(conn, "mysql_real_connect() failed\n");
	}	


	if(!initialize_prepared_stmts(IMPIEGATO)) {
		fprintf(stderr, "[FATAL] Cannot initialize prepared statements.\n");
		exit(EXIT_FAILURE);
	}
}

void db_switch_to_cliente(void)
{
	close_prepared_stmts();
	mysql_close(conn);
	conn = mysql_init(NULL);


	if(mysql_real_connect(conn, getenv("HOST"), getenv("CLIENTE_USER"), getenv("CLIENTE_PASS"), getenv("DB"),
			      atoi(getenv("PORT")), NULL,
			      CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS) == NULL) {
		finish_with_error(conn, "mysql_real_connect() failed\n");
	}	

	if(!initialize_prepared_stmts(CLIENTE)) {
		fprintf(stderr, "[FATAL] Cannot initialize prepared statements.\n");
		exit(EXIT_FAILURE);
	}
}

void db_switch_to_manager(void)
{	
	close_prepared_stmts();
	mysql_close(conn);
	conn = mysql_init(NULL);

	if(mysql_real_connect(conn, getenv("HOST"), getenv("MANAGER_USER"), getenv("MANAGER_PASS"), getenv("DB"),
			      atoi(getenv("PORT")), NULL,
			      CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS) == NULL) {
		finish_with_error(conn, "mysql_real_connect() failed\n");
	}

	if(!initialize_prepared_stmts(MANAGER)) {
		fprintf(stderr, "[FATAL] Cannot initialize prepared statements.\n");
		exit(EXIT_FAILURE);
	}
}

struct movies *get_available_movies(){

	MYSQL_BIND param[4];
	MYSQL_TIME anno_uscita;
	memset(&anno_uscita,0,sizeof(MYSQL_TIME));
	char titolo[TITLE_LEN];
	char regista[PRODUCER_LEN];
	int costo;
	
	
	int status;
	size_t row = 0;
	struct movies *all_movies = NULL;


	// Run procedure
	if(mysql_stmt_execute(list_film_procedure) != 0) {
		print_stmt_error(list_film_procedure, "Could not execute list film procedure");
		goto out;
	}

	// Store result
	mysql_stmt_store_result(list_film_procedure);

	// Allocate mem for all movies
	all_movies = malloc(sizeof(*all_movies) + sizeof(struct film)*mysql_stmt_num_rows(list_film_procedure));
	if(all_movies == NULL){
		goto out;
	}
	memset(all_movies, 0, sizeof(*all_movies) + sizeof(struct film) * mysql_stmt_num_rows(list_film_procedure));
	all_movies->num_film = mysql_stmt_num_rows(list_film_procedure);


	// Get bound parameters
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, titolo, TITLE_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, regista, PRODUCER_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_LONG, &costo, sizeof(costo));
	set_binding_param(&param[3], MYSQL_TYPE_DATE, &anno_uscita, sizeof(anno_uscita));
	
	// Execute bind of result
	if(mysql_stmt_bind_result(list_film_procedure, param)) {
		print_stmt_error(list_film_procedure, "Unable to bind output parameters for list film procedure\n");
		free(list_film_procedure);
		all_movies = NULL;
		goto out;
	}

	/* assemble all movies general information */
	while (true) {
		status = mysql_stmt_fetch(list_film_procedure);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		strcpy(all_movies->all_film[row].title, titolo);
		strcpy(all_movies->all_film[row].producer,regista);
		mysql_date_to_string(&anno_uscita,(all_movies->all_film[row]).production_year);
		snprintf((all_movies->all_film[row]).price,NUMBER_LEN,"%d",costo);
		
		
		row++;
	}
    out:
    mysql_stmt_free_result(list_film_procedure);
    while(mysql_stmt_next_result(list_film_procedure) != -1) {
    	mysql_stmt_free_result(list_film_procedure);
    }
	mysql_stmt_reset(list_film_procedure);
	return all_movies;
}

int do_insert_film(struct film *film){
	
	MYSQL_BIND param[9];

	MYSQL_TIME year;
	int ret = 0;
	int copies,price,center,sector,position;
	
	//Convert parameters
	copies = atoi(film->copies);
	price = atoi(film->price);
	sector = atoi(film->sector);
	position = atoi(film->position);
	center = atoi(film->center);
	date_to_mysql_time(film->production_year,&year);
	
	//Bind param
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, film->title, TITLE_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, film->producer, PRODUCER_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_LONG, &price, sizeof(price));
	set_binding_param(&param[3], MYSQL_TYPE_DATE, &year, sizeof(year));
	set_binding_param(&param[4], MYSQL_TYPE_LONG, &copies, sizeof(copies));
	set_binding_param(&param[5], MYSQL_TYPE_VAR_STRING, film->type, NUMBER_LEN);
	set_binding_param(&param[6], MYSQL_TYPE_LONG, &center, sizeof(center));
	set_binding_param(&param[7], MYSQL_TYPE_LONG, &sector, sizeof(sector));
	set_binding_param(&param[8], MYSQL_TYPE_LONG, &position, sizeof(position));
	
	
	if(mysql_stmt_bind_param(ins_film, param) != 0) { // Note _param
		print_stmt_error(ins_film, "Could not bind parameters for insert film procedure");
		ret = -1;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(ins_film) != 0) {
		print_stmt_error(ins_film, "Could not execute insert film procedure");
		ret = -1;
		goto out;
	}

	out:
	mysql_stmt_free_result(ins_film);
	mysql_stmt_reset(ins_film);
	return ret;
}


int do_insert_remake(struct film *film){
	
	MYSQL_BIND param[4];
	int ret = 0;
	//Bind param
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, film->title, TITLE_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, film->producer, PRODUCER_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, film->original_title, TITLE_LEN);
	set_binding_param(&param[3], MYSQL_TYPE_VAR_STRING, film->original_producer, PRODUCER_LEN);
	
	
	if(mysql_stmt_bind_param(ins_rem, param) != 0) { // Note _param
		print_stmt_error(ins_rem, "Could not bind parameters for insert remake procedure");
		ret = -1;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(ins_rem) != 0) {
		print_stmt_error(ins_rem, "Could not execute insert remake procedure");
		ret = -1;
		goto out;
	}

	out:
	mysql_stmt_free_result(ins_rem);
	mysql_stmt_reset(ins_rem);
	return ret;
}



int do_register_user(struct user *user,struct credentials *cred){
	MYSQL_BIND param[7];
	MYSQL_TIME birthday;
	int ret = 0;

	date_to_mysql_time(user->birthday,&birthday);
	
	//Bind param
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, user->cf, NUMBER_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, user->name, NUMBER_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, user->lastname,NUMBER_LEN);
	set_binding_param(&param[3], MYSQL_TYPE_DATE, &birthday, sizeof(birthday));
	set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, user->birthplace,NUMBER_LEN);
	set_binding_param(&param[5], MYSQL_TYPE_VAR_STRING, cred->username,USERNAME_LEN);
	set_binding_param(&param[6], MYSQL_TYPE_VAR_STRING, cred->password,PASSWORD_LEN);
	
	
	if(mysql_stmt_bind_param(ins_user, param) != 0) { // Note _param
		print_stmt_error(ins_user, "Could not bind parameters for insert user procedure");
		ret = -1;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(ins_user) != 0) {
		print_stmt_error(ins_user, "Could not execute insert user procedure");
		ret = -1;
		goto out;
	}

	out:
	
	mysql_stmt_free_result(ins_user);
	mysql_stmt_reset(ins_user);
	return ret;

}


int do_insert_rental(struct rental *rental,struct film *film){
	MYSQL_BIND param[5];
	MYSQL_TIME end_date;
	int ret = 0,user_id,center;

	date_to_mysql_time(rental->end_date,&end_date);
	user_id = atoi(rental->user_id);
	center = atoi(film->center);

	//Bind param
	set_binding_param(&param[0], MYSQL_TYPE_LONG, &user_id, sizeof(user_id));
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, film->title, TITLE_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, film->producer,PRODUCER_LEN);
	set_binding_param(&param[3], MYSQL_TYPE_DATE, &end_date, sizeof(end_date));
	set_binding_param(&param[4], MYSQL_TYPE_LONG, &center, sizeof(center));



	if(mysql_stmt_bind_param(ins_rent, param) != 0) { // Note _param
		print_stmt_error(ins_rent, "Could not bind parameters for insert rental procedure");
		ret = -1;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(ins_rent) != 0) {
		print_stmt_error(ins_rent, "Could not execute insert rental procedure");
		ret = -1;
		goto out;
	}

	out:
	
	mysql_stmt_free_result(ins_rent);
	mysql_stmt_reset(ins_rent);
	
	return ret;
}


int do_register_employee(struct employee *employee,struct credentials *cred){
	MYSQL_BIND param[6];
	
	int ret = 0;


	//Bind param
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, employee->cf, NUMBER_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, employee->name, NUMBER_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, employee->qualification,NUMBER_LEN);
	set_binding_param(&param[3], MYSQL_TYPE_VAR_STRING, employee->phone_number, NUMBER_LEN);
	set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, cred->username, USERNAME_LEN);
	set_binding_param(&param[5], MYSQL_TYPE_VAR_STRING, cred->password, PASSWORD_LEN);
	
	
	if(mysql_stmt_bind_param(ins_emp, param) != 0) { // Note _param
		print_stmt_error(ins_emp, "Could not bind parameters for insert employee procedure");
		ret = -1;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(ins_emp) != 0) {
		print_stmt_error(ins_emp, "Could not execute insert employee procedure");
		ret = -1;
		goto out;
	}

	out:
	
	mysql_stmt_free_result(ins_emp);
	mysql_stmt_reset(ins_emp);
	return ret;

}


int do_insert_workshift(struct employee *employee,struct workshift *shift){
	
	MYSQL_BIND param[5];
	MYSQL_TIME start,end;
	int ret = 0;
	int employment_id = atoi(shift->employment_id);

	time_to_mysql_time(shift->start_time,&start);
	time_to_mysql_time(shift->end_time, &end);

	
	//Bind param
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, employee->cf, NUMBER_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, shift->day, DAY_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_TIME, &start, sizeof(start));
	set_binding_param(&param[3], MYSQL_TYPE_TIME, &end, sizeof(end));
	set_binding_param(&param[4], MYSQL_TYPE_LONG, &employment_id, sizeof(employment_id));
	

	if(mysql_stmt_bind_param(ins_shift, param) != 0) { // Note _param
		print_stmt_error(ins_shift, "Could not bind parameters for insert workshift procedure");
		ret = -1;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(ins_shift) != 0) {
		print_stmt_error(ins_shift, "Could not execute insert workshift procedure");
		ret = -1;
		goto out;
	}

	out:
	
	mysql_stmt_free_result(ins_shift);
	mysql_stmt_reset(ins_shift);
	return ret;

}

int do_insert_employment(struct employment *emp){
	MYSQL_BIND param[5];
	MYSQL_TIME start,end;
	int center;
	int ret = 0;
	
	date_to_mysql_time(emp->start_date,&start);
	date_to_mysql_time(emp->end_date, &end);
	center = atoi(emp->center_id);
	
	//Bind param
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, emp->cf, NUMBER_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_DATE, &start, sizeof(start));
	set_binding_param(&param[2], MYSQL_TYPE_DATE, &end, sizeof(end));
	set_binding_param(&param[3], MYSQL_TYPE_VAR_STRING, emp->type, TYPE_OF_WORK_LEN);
	set_binding_param(&param[4], MYSQL_TYPE_LONG, &center, sizeof(center));
	
	

	if(mysql_stmt_bind_param(ins_employment, param) != 0) { // Note _param
		print_stmt_error(ins_shift, "Could not bind parameters for insert employment procedure");
		ret = -1;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(ins_employment) != 0) {
		print_stmt_error(ins_employment, "Could not execute insert employment procedure");
		ret = -1;
		goto out;
	}

	out:
	
	mysql_stmt_free_result(ins_employment);
	mysql_stmt_reset(ins_employment);
	return ret;
}


int do_move_employee(struct employee *emp,struct employment *e){
	
	MYSQL_BIND param[2];
	
	int ret = 0;
	int id = atoi(e->center_id);
	
	//Bind param
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, emp->cf, NUMBER_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_LONG, &id, sizeof(id));
	
	if(mysql_stmt_bind_param(move_emp, param) != 0) { // Note _param
		print_stmt_error(move_emp, "Could not bind parameters for move employee procedure");
		ret = -1;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(move_emp) != 0) {
		print_stmt_error(move_emp, "Could not execute move employee procedure");
		ret = -1;
		goto out;
	}

	out:
	
	mysql_stmt_free_result(move_emp);
	mysql_stmt_reset(move_emp);
	return ret;
}


int do_register_center(struct center *c){
	
	MYSQL_BIND param[7];
	int ret = 0;
	int number,cap;
	
	number = atoi(c->number);
	cap = atoi(c->cap);
	
	//Bind param
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, c->manager_name, NUMBER_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, c->manager_cf, NUMBER_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, c->center_phone, NUMBER_LEN);
	set_binding_param(&param[3], MYSQL_TYPE_VAR_STRING, c->center_email, NUMBER_LEN);
	set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, c->via, NUMBER_LEN);
	set_binding_param(&param[5], MYSQL_TYPE_LONG, &number, sizeof(number));
	set_binding_param(&param[6], MYSQL_TYPE_LONG, &cap, sizeof(cap));
	
	
	if(mysql_stmt_bind_param(ins_cen, param) != 0) { // Note _param
		print_stmt_error(ins_cen, "Could not bind parameters for register center procedure");
		ret = -1;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(ins_cen) != 0) {
		print_stmt_error(ins_cen, "Could not execute register center procedure");
		ret = -1;
		goto out;
	}

	out:
	
	mysql_stmt_free_result(ins_cen);
	mysql_stmt_reset(ins_cen);
	return ret;
}



struct workshifts *get_shifts(struct employee *emp)
{	
	MYSQL_BIND in_param[1];
	MYSQL_BIND out_param[3];
	MYSQL_TIME start,end;
	memset(&start,0,sizeof(MYSQL_TIME));
	memset(&end,0,sizeof(MYSQL_TIME));
	char day[DAY_LEN];
	
	
	int status;
	size_t row = 0;
	struct workshifts *all_shifts = NULL;


	//Bind input param
	set_binding_param(&in_param[0], MYSQL_TYPE_VAR_STRING, emp->cf, NUMBER_LEN);
	
	

	if(mysql_stmt_bind_param(list_shifts, in_param) != 0) { 
		print_stmt_error(list_shifts, "Could not bind parameters for list shifts procedure");
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(list_shifts) != 0) {
		print_stmt_error(list_shifts, "Could not execute list shifts procedure");
		goto out;
	}

	// Store result
	mysql_stmt_store_result(list_shifts);

	// Allocate mem for all movies
	all_shifts = malloc(sizeof(*all_shifts) + sizeof(struct workshift)*mysql_stmt_num_rows(list_shifts));
	if(all_shifts == NULL){
		goto out;
	}
	memset(all_shifts, 0, sizeof(*all_shifts) + sizeof(struct workshift) * mysql_stmt_num_rows(list_shifts));
	all_shifts->num_shift = mysql_stmt_num_rows(list_shifts);


	// Get bound parameters
	set_binding_param(&out_param[0], MYSQL_TYPE_VAR_STRING, day, DAY_LEN);
	set_binding_param(&out_param[1], MYSQL_TYPE_DATE, &start, sizeof(start));
	set_binding_param(&out_param[2], MYSQL_TYPE_DATE, &end, sizeof(end));
	
	

	// Execute bind of result
	if(mysql_stmt_bind_result(list_shifts, out_param)) {
		print_stmt_error(list_shifts, "Unable to bind output parameters for list shifts procedure\n");
		free(list_shifts);
		all_shifts = NULL;
		goto out;
	}

	/* assemble all movies general information */
	while (true) {
		status = mysql_stmt_fetch(list_shifts);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		strcpy((all_shifts->all_shifts[row]).day, day);
		mysql_time_to_string(&start,(all_shifts->all_shifts[row]).start_time);
		mysql_time_to_string(&end,(all_shifts->all_shifts[row]).end_time);
		
		row++;
	}
    out:
    mysql_stmt_free_result(list_shifts);
    while(mysql_stmt_next_result(list_shifts) != -1) {
    	mysql_stmt_free_result(list_shifts);
    }
	//mysql_stmt_free_result(list_shifts);
	mysql_stmt_reset(list_shifts);
	return all_shifts;
}

int do_register_user_email(struct user *user){
	
	MYSQL_BIND param[2];
	int ret = 0;

	//Bind param
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, user->cf, NUMBER_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, user->email, EMAIL_LEN);
	
	
	
	if(mysql_stmt_bind_param(ins_email, param) != 0) { // Note _param
		print_stmt_error(ins_email, "Could not bind parameters for insert email procedure");
		ret = -1;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(ins_email) != 0) {
		print_stmt_error(ins_email, "Could not execute insert email procedure");
		ret = -1;
		goto out;
	}

	out:
	
	mysql_stmt_free_result(ins_email);
	mysql_stmt_reset(ins_email);
	return ret;
}

int do_register_user_phone(struct user *user){
	
	MYSQL_BIND param[2];
	int ret = 0;

	//Bind param
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, user->cf, NUMBER_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, user->phone, NUMBER_LEN);
	
	
	
	if(mysql_stmt_bind_param(ins_phone, param) != 0) { // Note _param
		print_stmt_error(ins_phone, "Could not bind parameters for insert phone procedure");
		ret = -1;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(ins_phone) != 0) {
		print_stmt_error(ins_phone, "Could not execute insert phone procedure");
		ret = -1;
		goto out;
	}

	out:
	
	mysql_stmt_free_result(ins_phone);
	mysql_stmt_reset(ins_phone);
	return ret;
}

struct rentals *get_user_active_rental(struct user *user){
	MYSQL_BIND in_param[1];
	MYSQL_BIND out_param[5];
	MYSQL_TIME start,end;
	int temp_rent_id;
	char temp_title[NUMBER_LEN];
	char temp_producer[NUMBER_LEN];

	memset(&start,0,sizeof(MYSQL_TIME));
	memset(&end,0,sizeof(MYSQL_TIME));
	
	
	int status;
	size_t row = 0;
	struct rentals *rentals = NULL;


	//Bind input param
	set_binding_param(&in_param[0], MYSQL_TYPE_VAR_STRING, user->cf, NUMBER_LEN);
	
	

	if(mysql_stmt_bind_param(list_rentals, in_param) != 0) { 
		print_stmt_error(list_rentals, "Could not bind parameters for list rentals procedure");
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(list_rentals) != 0) {
		print_stmt_error(list_rentals, "Could not execute list rentals procedure");
		goto out;
	}

	// Store result
	mysql_stmt_store_result(list_rentals);

	// Allocate mem for all movies
	rentals = malloc(sizeof(*rentals) + sizeof(struct rental)*mysql_stmt_num_rows(list_rentals));
	if(rentals == NULL){
		goto out;
	}
	memset(rentals, 0, sizeof(*rentals) + sizeof(struct rental) * mysql_stmt_num_rows(list_rentals));
	rentals->num_rent = mysql_stmt_num_rows(list_rentals);


	// Get bound parameters
	set_binding_param(&out_param[0], MYSQL_TYPE_LONG, &temp_rent_id, sizeof(temp_rent_id));
	set_binding_param(&out_param[1], MYSQL_TYPE_DATE, &start, sizeof(start));
	set_binding_param(&out_param[2], MYSQL_TYPE_DATE, &end, sizeof(end));
	set_binding_param(&out_param[3], MYSQL_TYPE_VAR_STRING, temp_title, TITLE_LEN);
	set_binding_param(&out_param[4], MYSQL_TYPE_VAR_STRING, temp_producer, PRODUCER_LEN);
	
	

	// Execute bind of result
	if(mysql_stmt_bind_result(list_rentals, out_param)) {
		print_stmt_error(list_rentals, "Unable to bind output parameters for list rentals procedure\n");
		free(list_rentals);
		rentals = NULL;
		goto out;
	}

	/* assemble all movies general information */
	while (true) {
		status = mysql_stmt_fetch(list_rentals);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		strcpy((rentals->all_rental[row]).title, temp_title);
		strcpy((rentals->all_rental[row]).producer, temp_producer);
		snprintf((rentals->all_rental[row]).rental_id,NUMBER_LEN,"%d",temp_rent_id);

		mysql_date_to_string(&start,(rentals->all_rental[row]).start_date);
		mysql_date_to_string(&end,(rentals->all_rental[row]).end_date);
		
		row++;
	}
    out:
    mysql_stmt_free_result(list_rentals);
    while(mysql_stmt_next_result(list_rentals) != -1) {
    	mysql_stmt_free_result(list_rentals);
    }
	mysql_stmt_reset(list_rentals);
	return rentals;
}


struct rentals *get_expired_rental(struct center *center)
{
	MYSQL_BIND in_param[1];
	MYSQL_BIND out_param[7];
	MYSQL_TIME start,end;
	int center_id;
	int temp_rent_id;
	char temp_cf[NUMBER_LEN];
	char temp_title[TITLE_LEN];
	char temp_producer[PRODUCER_LEN];
	char temp_username[NUMBER_LEN];
	

	center_id = atoi(center->center_id);
	memset(&start,0,sizeof(MYSQL_TIME));
	memset(&end,0,sizeof(MYSQL_TIME));
	
	
	int status;
	size_t row = 0;
	struct rentals *rentals = NULL;


	//Bind input param
	set_binding_param(&in_param[0], MYSQL_TYPE_LONG, &center_id, sizeof(center));
	
	

	if(mysql_stmt_bind_param(list_exp_rentals, in_param) != 0) { 
		print_stmt_error(list_exp_rentals, "Could not bind parameters for list expired rentals procedure");
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(list_exp_rentals) != 0) {
		print_stmt_error(list_exp_rentals, "Could not execute list expired rentals procedure");
		goto out;
	}

	// Store result
	mysql_stmt_store_result(list_exp_rentals);

	// Allocate mem for all movies
	rentals = malloc(sizeof(*rentals) + sizeof(struct rental)*mysql_stmt_num_rows(list_exp_rentals));
	if(rentals == NULL){
		goto out;
	}
	memset(rentals, 0, sizeof(*rentals) + sizeof(struct rental) * mysql_stmt_num_rows(list_exp_rentals));
	rentals->num_rent = mysql_stmt_num_rows(list_exp_rentals);


	// Get bound parameters
	set_binding_param(&out_param[0], MYSQL_TYPE_LONG, &temp_rent_id, sizeof(temp_rent_id));
	set_binding_param(&out_param[1], MYSQL_TYPE_DATE, &start, sizeof(start));
	set_binding_param(&out_param[2], MYSQL_TYPE_DATE, &end, sizeof(end));
	set_binding_param(&out_param[3], MYSQL_TYPE_VAR_STRING, temp_title, TITLE_LEN);
	set_binding_param(&out_param[4], MYSQL_TYPE_VAR_STRING, temp_producer, PRODUCER_LEN);
	set_binding_param(&out_param[5], MYSQL_TYPE_VAR_STRING, temp_cf, NUMBER_LEN);
	set_binding_param(&out_param[6], MYSQL_TYPE_VAR_STRING, temp_username, NUMBER_LEN);
	
	

	// Execute bind of result
	if(mysql_stmt_bind_result(list_exp_rentals, out_param)) {
		print_stmt_error(list_exp_rentals, "Unable to bind output parameters for list expired rentals procedure\n");
		free(list_exp_rentals);
		rentals = NULL;
		goto out;
	}

	/* assemble all movies general information */
	while (true) {
		status = mysql_stmt_fetch(list_exp_rentals);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		strcpy((rentals->all_rental[row]).title, temp_title);
		strcpy((rentals->all_rental[row]).producer, temp_producer);
		strcpy((rentals->all_rental[row]).user_name, temp_username);
		strcpy((rentals->all_rental[row]).user_cf, temp_cf);

		snprintf((rentals->all_rental[row]).rental_id,NUMBER_LEN,"%d",temp_rent_id);

		mysql_date_to_string(&start,(rentals->all_rental[row]).start_date);
		mysql_date_to_string(&end,(rentals->all_rental[row]).end_date);
		
		row++;
	}
    out:
    mysql_stmt_free_result(list_exp_rentals);
    while(mysql_stmt_next_result(list_exp_rentals) != -1) {
    	mysql_stmt_free_result(list_exp_rentals);
    }
	
	mysql_stmt_reset(list_exp_rentals);
	return rentals;
}


int do_end_rental(struct rental *rental){
	MYSQL_BIND param[1];
	int ret = 0;
	int id;
	
	id = atoi(rental->rental_id);
	
	//Bind param
	set_binding_param(&param[0], MYSQL_TYPE_LONG, &id, sizeof(id));
	
	
	if(mysql_stmt_bind_param(end_rent, param) != 0) { // Note _param
		print_stmt_error(end_rent, "Could not bind parameters for end rental procedure");
		ret = -1;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(end_rent) != 0) {
		print_stmt_error(end_rent, "Could not execute end rental procedure");
		ret = -1;
		goto out;
	}

	out:
	
	mysql_stmt_free_result(end_rent);
	mysql_stmt_reset(end_rent);
	return ret;
}


int do_insert_actor(struct film *film,struct actor *act){
	MYSQL_BIND param[3];

	int ret = 0;

	//Bind param
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, act->name, NUMBER_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, film->title, TITLE_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, film->producer,PRODUCER_LEN);



	if(mysql_stmt_bind_param(ins_act, param) != 0) { // Note _param
		print_stmt_error(ins_act, "Could not bind parameters for insert actor procedure");
		ret = -1;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(ins_act) != 0) {
		print_stmt_error(ins_act, "Could not execute insert actor procedure");
		ret = -1;
		goto out;
	}

	out:
	
	mysql_stmt_free_result(ins_act);
	mysql_stmt_reset(ins_act);
	
	// memset(act,0,sizeof(act));
	
	return ret;
}


//TODO
struct film *get_actors(struct film *film){
	
	MYSQL_BIND in_param[2];
	MYSQL_BIND out_param[1];
	
	char temp_name[NUMBER_LEN];

	
	int status;
	size_t row = 0;
	struct film *act = NULL;


	//Bind input param
	set_binding_param(&in_param[0], MYSQL_TYPE_VAR_STRING, film->title, TITLE_LEN);
	set_binding_param(&in_param[1], MYSQL_TYPE_VAR_STRING, film->producer, PRODUCER_LEN);
	

	if(mysql_stmt_bind_param(get_act, in_param) != 0) { 
		print_stmt_error(get_act, "Could not bind parameters for get actors procedure");
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(get_act) != 0) {
		print_stmt_error(get_act, "Could not execute get actors procedure");
		goto out;
	}

	// Store result
	mysql_stmt_store_result(get_act);

	// Allocate mem for all movies
	act = malloc(sizeof(*act) + sizeof(struct film)*mysql_stmt_num_rows(get_act));
	if(act == NULL){
		goto out;
	}
	memset(act, 0, sizeof(*film) + sizeof(struct film) * mysql_stmt_num_rows(get_act));
	act->num_act = mysql_stmt_num_rows(get_act);


	// Get bound parameters
	
	set_binding_param(&out_param[0], MYSQL_TYPE_VAR_STRING, temp_name, NUMBER_LEN);
	
	

	// Execute bind of result
	if(mysql_stmt_bind_result(get_act, out_param)) {
		print_stmt_error(get_act, "Unable to bind output parameters for get actors procedure\n");
		free(get_act);
		act = NULL;
		goto out;
	}

	/* assemble all movies general information */
	while (true) {
		status = mysql_stmt_fetch(get_act);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		strcpy((act->act[row]).name, temp_name);
		
		row++;
	}
    out:
    mysql_stmt_free_result(get_act);
    while(mysql_stmt_next_result(get_act) != -1) {
    	mysql_stmt_free_result(get_act);
    }
	
	mysql_stmt_reset(get_act);
	return act;
}


struct movies *get_remake(){

	MYSQL_BIND param[4];
	
	char titolo_originale[TITLE_LEN];
	char regista_originale[PRODUCER_LEN];
	char titolo_remake[TITLE_LEN];
	char regista_remake[PRODUCER_LEN];
	
	
	int status;
	size_t row = 0;
	struct movies *all_movies = NULL;


	// Run procedure
	if(mysql_stmt_execute(get_rem) != 0) {
		print_stmt_error(get_rem, "Could not execute list remake procedure");
		goto out;
	}

	// Store result
	mysql_stmt_store_result(get_rem);

	// Allocate mem for all movies
	all_movies = malloc(sizeof(*all_movies) + sizeof(struct film)*mysql_stmt_num_rows(get_rem));
	if(all_movies == NULL){
		goto out;
	}
	memset(all_movies, 0, sizeof(*all_movies) + sizeof(struct film) * mysql_stmt_num_rows(get_rem));
	all_movies->num_film = mysql_stmt_num_rows(get_rem);


	// Get bound parameters
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, titolo_remake, TITLE_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, regista_remake, PRODUCER_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, titolo_originale, TITLE_LEN);
	set_binding_param(&param[3], MYSQL_TYPE_VAR_STRING, regista_originale, PRODUCER_LEN);
	
	// Execute bind of result
	if(mysql_stmt_bind_result(get_rem, param)) {
		print_stmt_error(get_rem, "Unable to bind output parameters for list remake procedure\n");
		free(get_rem);
		all_movies = NULL;
		goto out;
	}

	/* assemble all movies general information */
	while (true) {
		status = mysql_stmt_fetch(get_rem);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		strcpy(all_movies->all_film[row].original_title, titolo_originale);
		strcpy(all_movies->all_film[row].original_producer,regista_originale);
		strcpy(all_movies->all_film[row].title, titolo_remake);
		strcpy(all_movies->all_film[row].producer,regista_remake);
		
		
		row++;
	}
    out:
    mysql_stmt_free_result(get_rem);
    while(mysql_stmt_next_result(get_rem) != -1) {
    	mysql_stmt_free_result(get_rem);
    }
	mysql_stmt_reset(get_rem);
	return all_movies;
}

struct all_positions *get_sector(struct center *center){

	MYSQL_BIND in_param[1];
	MYSQL_BIND out_param[2];
	
	int id,status,temp_sector,temp_pos;

	size_t row = 0;
	struct all_positions *pos = NULL;

	id = atoi(center->center_id);

	//Bind input param
	set_binding_param(&in_param[0], MYSQL_TYPE_LONG, &id, sizeof(id));
	
	

	if(mysql_stmt_bind_param(get_pos, in_param) != 0) { 
		print_stmt_error(get_pos, "Could not bind parameters for get positions procedure");
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(get_pos) != 0) {
		print_stmt_error(get_pos, "Could not execute get positions procedure");
		goto out;
	}

	// Store result
	mysql_stmt_store_result(get_pos);

	
	pos = malloc(sizeof(*pos) + sizeof(struct position)*mysql_stmt_num_rows(get_pos));
	if(pos == NULL){
		goto out;
	}
	memset(pos, 0, sizeof(*pos) + sizeof(struct position) * mysql_stmt_num_rows(get_pos));
	pos->num_pos = mysql_stmt_num_rows(get_pos);


	// Get bound parameters
	set_binding_param(&out_param[0], MYSQL_TYPE_LONG, &temp_sector, sizeof(temp_sector));
	set_binding_param(&out_param[1], MYSQL_TYPE_LONG, &temp_pos, sizeof(temp_pos));
	
	
	// Execute bind of result
	if(mysql_stmt_bind_result(get_pos, out_param)) {
		print_stmt_error(get_pos, "Unable to bind output parameters for get position procedure\n");
		free(get_pos);
		pos = NULL;
		goto out;
	}

	
	while (true) {
		status = mysql_stmt_fetch(get_pos);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		snprintf((pos->pos[row]).sector,NUMBER_LEN,"%d",temp_sector);
		snprintf((pos->pos[row]).position,NUMBER_LEN,"%d",temp_pos);

		row++;
	}

    out:
    mysql_stmt_free_result(get_pos);
    while(mysql_stmt_next_result(get_pos) != -1) {
    	mysql_stmt_free_result(get_pos);
    }
	
	mysql_stmt_reset(get_pos);
	return pos;
}

struct report *do_get_report(struct center *center){
	
	MYSQL_BIND in_param[1];
	MYSQL_BIND out_param[6];
	MYSQL_TIME start_date,end_date,hours_worked;
	
	int id,status,temp_weeks;
	char temp_day[DAY_LEN];
	char temp_emp_cf[NUMBER_LEN];

	size_t row = 0;
	struct report *rep = NULL;

	id = atoi(center->center_id);

	memset(&start_date,0,sizeof(MYSQL_TIME));
	memset(&end_date,0,sizeof(MYSQL_TIME));
	memset(&hours_worked,0,sizeof(MYSQL_TIME));


	//Bind input param
	set_binding_param(&in_param[0], MYSQL_TYPE_LONG, &id, sizeof(id));
	
	

	if(mysql_stmt_bind_param(get_report, in_param) != 0) { 
		print_stmt_error(get_report, "Could not bind parameters for get report procedure");
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(get_report) != 0) {
		print_stmt_error(get_report, "Could not execute get report procedure");
		goto out;
	}

	// Store result
	mysql_stmt_store_result(get_report);

	
	rep = malloc(sizeof(*rep) + sizeof(struct report_entry)*mysql_stmt_num_rows(get_report));
	if(rep == NULL){
		goto out;
	}
	memset(rep, 0, sizeof(*rep) + sizeof(struct report_entry)*mysql_stmt_num_rows(get_report));
	rep->num_entries = mysql_stmt_num_rows(get_report);


	// Get bound parameters
	set_binding_param(&out_param[0], MYSQL_TYPE_VAR_STRING, temp_emp_cf, NUMBER_LEN);
	set_binding_param(&out_param[1], MYSQL_TYPE_DATE, &start_date, sizeof(start_date));
	set_binding_param(&out_param[2], MYSQL_TYPE_DATE, &end_date, sizeof(end_date));
	set_binding_param(&out_param[3], MYSQL_TYPE_LONG, &temp_weeks, sizeof(temp_weeks));
	set_binding_param(&out_param[4], MYSQL_TYPE_VAR_STRING, temp_day, DAY_LEN);
	set_binding_param(&out_param[5], MYSQL_TYPE_TIME, &hours_worked, sizeof(hours_worked));
	
	
	
	// Execute bind of result
	if(mysql_stmt_bind_result(get_report, out_param)) {
		print_stmt_error(get_report, "Unable to bind output parameters for get report procedure\n");
		free(get_report);
		rep = NULL;
		goto out;
	}

	
	while (true) {
		status = mysql_stmt_fetch(get_report);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		strcpy((rep->report[row]).emp_cf, temp_emp_cf);

		mysql_time_to_string(&hours_worked,(rep->report[row]).hours_worked);
		
		strcpy((rep->report[row]).day, temp_day);

		snprintf((rep->report[row]).weeks,NUMBER_LEN,"%d",temp_weeks);

		mysql_date_to_string(&start_date,(rep->report[row]).start_date);
		mysql_date_to_string(&end_date,(rep->report[row]).end_date);



		row++;
	}

    out:
    mysql_stmt_free_result(get_report);
    while(mysql_stmt_next_result(get_report) != -1) {
    	mysql_stmt_free_result(get_report);
    }
	
	mysql_stmt_reset(get_report);
	return rep;
}

struct employees *do_get_employees(struct center *center){
	
	
	MYSQL_BIND out_param[5];
	
	
	char temp_cf[NUMBER_LEN];
	char temp_name[NUMBER_LEN];
	char temp_qualification[NUMBER_LEN];
	char temp_phone_number[NUMBER_LEN];
	int temp_id,status;

	size_t row = 0;
	struct employees *employees = NULL;


	// Run procedure
	if(mysql_stmt_execute(get_employee) != 0) {
		print_stmt_error(get_employee, "Could not execute get employees procedure");
		goto out;
	}

	// Store result
	mysql_stmt_store_result(get_employee);

	
	employees = malloc(sizeof(*employees) + sizeof(struct employee)*mysql_stmt_num_rows(get_employee));
	if(employees == NULL){
		goto out;
	}
	memset(employees,0,sizeof(*employees) + sizeof(struct employee)*mysql_stmt_num_rows(get_employee));
	employees->num_entries = mysql_stmt_num_rows(get_employee);


	// Get bound parameters
	set_binding_param(&out_param[0], MYSQL_TYPE_VAR_STRING, temp_cf, NUMBER_LEN);
	set_binding_param(&out_param[1], MYSQL_TYPE_VAR_STRING, temp_name, NUMBER_LEN);
	set_binding_param(&out_param[2], MYSQL_TYPE_VAR_STRING, temp_qualification, NUMBER_LEN);
	set_binding_param(&out_param[3], MYSQL_TYPE_VAR_STRING, temp_phone_number, NUMBER_LEN);
	set_binding_param(&out_param[4], MYSQL_TYPE_LONG, &temp_id, sizeof(temp_id));	

	// Execute bind of result
	if(mysql_stmt_bind_result(get_employee, out_param)) {
		print_stmt_error(get_employee, "Unable to bind output parameters for get report procedure\n");
		free(get_employee);
		employees = NULL;
		goto out;
	}

	
	while (true) {
		status = mysql_stmt_fetch(get_employee);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		printf("\n\nprova: %s\n\n",temp_cf);
		strcpy((employees->emp[row]).cf, temp_cf);
		strcpy((employees->emp[row]).name, temp_name);
		strcpy((employees->emp[row]).qualification, temp_qualification);
		strcpy((employees->emp[row]).phone_number, temp_phone_number);
		
		snprintf((employees->emp[row]).id,ID_LEN,"%d",temp_id);

		row++;
	}

    out:
    mysql_stmt_free_result(get_employee);
    while(mysql_stmt_next_result(get_employee) != -1) {
    	mysql_stmt_free_result(get_employee);
    }
	
	mysql_stmt_reset(get_employee);
	return employees;
}



struct employments *do_get_employment_id(struct employment *emp_cf){

	MYSQL_BIND in_param[1];
	MYSQL_BIND out_param[1];
	
	int status;
	int temp_id;

	size_t row = 0;
	struct employments *emp = NULL;


	//Bind input param
	set_binding_param(&in_param[0], MYSQL_TYPE_VAR_STRING, emp_cf->cf, NUMBER_LEN);
	
	
	if(mysql_stmt_bind_param(get_employment_id, in_param) != 0) { 
		print_stmt_error(get_employment_id, "Could not bind parameters for get employment id procedure");
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(get_employment_id) != 0) {
		print_stmt_error(get_employment_id, "Could not execute get employment id procedure");
		goto out;
	}

	// Store result
	mysql_stmt_store_result(get_employment_id);

	
	emp = malloc(sizeof(*emp) + sizeof(struct employment)*mysql_stmt_num_rows(get_employment_id));
	if(emp == NULL){
		goto out;
	}
	memset(emp, 0, sizeof(*emp) + sizeof(struct employment)*mysql_stmt_num_rows(get_employment_id));
	emp->num_entries = mysql_stmt_num_rows(get_employment_id);


	// Get bound parameters
	set_binding_param(&out_param[0], MYSQL_TYPE_LONG, &temp_id, sizeof(temp_id));
	
	
	// Execute bind of result
	if(mysql_stmt_bind_result(get_employment_id, out_param)) {
		print_stmt_error(get_employment_id, "Unable to bind output parameters for get employment id procedure\n");
		free(get_employment_id);
		emp = NULL;
		goto out;
	}

	
	while (true) {
		status = mysql_stmt_fetch(get_employment_id);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		
		snprintf((emp->emp[row]).employment_id,NUMBER_LEN,"%d",temp_id);

		row++;
	}

    out:
    mysql_stmt_free_result(get_employment_id);
    while(mysql_stmt_next_result(get_employment_id) != -1) {
    	mysql_stmt_free_result(get_employment_id);
    }
	
	mysql_stmt_reset(get_employment_id);
	return emp;
}




void dispose_film_list(struct movies *list)
{
	free(list);
}

void dispose_shifts(struct workshifts *shifts)
{
	free(shifts);
}

void dispose_rentals(struct rentals *rentals)
{
	free(rentals);
}

void dispose_actors(struct film *film)
{
	free(film);
}

void dispose_positions(struct all_positions *pos)
{
	free(pos);
}

void dispose_report(struct report *report)
{
	free(report);
}


void dispose_employee(struct employees *employee)
{
	free(employee);
}

void dispose_emp_id(struct employments *emp)
{
	free(emp);
}


