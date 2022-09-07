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

static void close_prepared_stmts(void)
{
	if(login_procedure) {
		mysql_stmt_close(login_procedure);
		login_procedure = NULL;
	}
	//TODO
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
			//TODO
			break;
		case IMPIEGATO:
			//TODO
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

	// Allocate mem
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
		all_movies->all_film[row].price = costo;
		all_movies->all_film[row].copies = copie;
		
		row++;
	}
    out:
	mysql_stmt_free_result(list_film_procedure);
	mysql_stmt_reset(list_film_procedure);
	return all_movies;
}



void dispose_film_list(struct movies *list)
{
	free(list);
}
