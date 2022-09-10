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
static MYSQL_STMT *ins_film;
static MYSQL_STMT *ins_user;
static MYSQL_STMT *ins_rent;
static MYSQL_STMT *ins_emp;
static MYSQL_STMT *ins_shift;
static MYSQL_STMT *ins_employment;
static MYSQL_STMT *ins_cen;
static MYSQL_STMT *move_emp;
static MYSQL_STMT *rem_emp;

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
	if(rem_emp) {
		mysql_stmt_close(rem_emp);
		rem_emp = NULL;
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
			if(!setup_prepared_stmt(&rem_emp, "call delete_employee(?)", conn)) {
				print_stmt_error(rem_emp, "Unable to initialize remove employee statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&ins_shift, "call insert_workshift(?, ?, ?, ?)", conn)) {
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
			break;
		case IMPIEGATO:
			if(!setup_prepared_stmt(&ins_film, "call insert_film(?, ?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
				print_stmt_error(ins_film, "Unable to initialize insert film statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&ins_user, "call insert_user(?, ?, ?, ?, ?, ?, ?)", conn)) {
				print_stmt_error(ins_user, "Unable to initialize insert user statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&ins_rent, "call insert_rental(?, ?, ?, ?)", conn)) {
				print_stmt_error(ins_rent, "Unable to initialize insert rental statement\n");
				return false;
			}
			break;
		case CLIENTE:
			if(!setup_prepared_stmt(&list_film_procedure, "call list_available_film()", conn)) {
				print_stmt_error(list_film_procedure, "Unable to initialize list film statement\n");
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
	// Consume the possibly-returned table for the output parameter
	//while(mysql_stmt_next_result(login_procedure) != -1) {}

	mysql_stmt_free_result(login_procedure);
	mysql_stmt_reset(login_procedure);
	
	return role;
}


void db_switch_to_login(void)
{
	close_prepared_stmts();
	//Try
	mysql_close(conn);
	conn = mysql_init(NULL);
/*
	if(mysql_change_user(conn, getenv("LOGIN_USER"), getenv("LOGIN_PASS"), getenv("DB"))) {
		fprintf(stderr, "mysql_change_user() failed: %s\n", mysql_error(conn));
		exit(EXIT_FAILURE);
	}
	*/
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
	/*
	if(mysql_change_user(conn, getenv("IMPIEGATO_USER"), getenv("IMPIEGATO_PASS"), getenv("DB"))) {
		fprintf(stderr, "mysql_change_user() failed: %s\n", mysql_error(conn));
		exit(EXIT_FAILURE);
	}
	*/
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
/*
	if(mysql_change_user(conn, getenv("CLIENTE_USER"), getenv("CLIENTE_PASS"), getenv("DB"))) {
		fprintf(stderr, "mysql_change_user() failed: %s\n", mysql_error(conn));
		exit(EXIT_FAILURE);
	}
*/
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

	
/*
	if(mysql_change_user(conn, getenv("MANAGER_USER"), getenv("MANAGER_PASS"), getenv("DB"))) {
		fprintf(stderr, "mysql_change_user() failed: %s\n", mysql_error(conn));
		exit(EXIT_FAILURE);
	}
*/
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

	MYSQL_BIND param[5];
	MYSQL_TIME anno_uscita;
	memset(&anno_uscita,0,sizeof(MYSQL_TIME));
	char titolo[TITLE_LEN];
	char regista[PRODUCER_LEN];
	int costo;
	int copie;
	
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
	set_binding_param(&param[3], MYSQL_TYPE_LONG, &copie, sizeof(copie));
	set_binding_param(&param[4], MYSQL_TYPE_DATE, &anno_uscita, sizeof(anno_uscita));
	
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
		snprintf((all_movies->all_film[row]).copies,NUMBER_LEN,"%d",copie);
		
		row++;
	}
    out:
	mysql_stmt_free_result(list_film_procedure);
	mysql_stmt_reset(list_film_procedure);
	return all_movies;
}

void do_insert_film(struct film *film){
	
	MYSQL_BIND param[9];
	MYSQL_TIME year;
	
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
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(ins_film) != 0) {
		print_stmt_error(ins_film, "Could not execute insert film procedure");
		return;
	}

	mysql_stmt_free_result(ins_film);
	mysql_stmt_reset(ins_film);
	return;
}

int do_register_user(struct user *user,struct credentials *cred){
	MYSQL_BIND param[7];
	MYSQL_TIME birthday;
	int temp_id = 0;

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
		temp_id = -1;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(ins_user) != 0) {
		print_stmt_error(ins_user, "Could not execute insert user procedure");
		temp_id = -1;
		goto out;
	}

	out:
	
	mysql_stmt_free_result(ins_user);
	mysql_stmt_reset(ins_user);
	return temp_id;

}


int do_insert_rental(struct rental *rental,struct film *film){
	MYSQL_BIND param[4];
	MYSQL_TIME end_date;
	int ret = 0,user_id;

	date_to_mysql_time(rental->end_date,&end_date);
	user_id = atoi(rental->user_id);


	//Bind param
	set_binding_param(&param[0], MYSQL_TYPE_LONG, &user_id, sizeof(user_id));
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, film->title, TITLE_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, film->producer,PRODUCER_LEN);
		

	//set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, rental->rented_movies[current_film]->title, TITLE_LEN);
	//set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, rental->rented_movies[current_film]->producer,PRODUCER_LEN);
	set_binding_param(&param[3], MYSQL_TYPE_DATE, &end_date, sizeof(end_date));
	



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
	memset(film,0,sizeof(film));
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
	
	MYSQL_BIND param[4];
	MYSQL_TIME start,end;
	int ret = 0;
	
	time_to_mysql_time(shift->start_time,&start);
	time_to_mysql_time(shift->end_time, &end);

	
	//Bind param
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, employee->cf, NUMBER_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, shift->day, DAY_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_TIME, &start, sizeof(start));
	set_binding_param(&param[3], MYSQL_TYPE_TIME, &end, sizeof(end));
	
	printf("Day: %s",shift->day);

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


int do_remove_employee(struct employee *emp){
	
	MYSQL_BIND param[1];
	int ret = 0;
	
	
	//Bind param
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, emp->cf, NUMBER_LEN);
	
	

	if(mysql_stmt_bind_param(rem_emp, param) != 0) { 
		print_stmt_error(ins_cen, "Could not bind parameters for remove employee procedure");
		ret = -1;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(rem_emp) != 0) {
		print_stmt_error(rem_emp, "Could not execute remove employee procedure");
		ret = -1;
		goto out;
	}

	out:
	
	mysql_stmt_free_result(rem_emp);
	mysql_stmt_reset(rem_emp);
	return ret;
}



void dispose_film_list(struct movies *list)
{
	free(list);
}
