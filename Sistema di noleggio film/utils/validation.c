#include <stdbool.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "validation.h"

regex_t regex_date;
regex_t regex_time;


static void print_regerror(int errcode, size_t length, regex_t *compiled)
{
	char buffer[length];
	(void) regerror(errcode, compiled, buffer, length);
	fprintf(stderr, "Regex match failed: %s\n", buffer);
}

bool init_validation(void)
{
	int ret1;
	int ret2;

	ret1 = regcomp(&regex_date, "^([12][0-9]{3}-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01]))", REG_EXTENDED);
	if(ret1) {
		if(ret1 == REG_ESPACE) {
			fprintf(stderr, "%s\n", strerror(ENOMEM));
		} else {
			fprintf(stderr, "Fatal error while setting up date validation regex.\n");
		}
	}

	ret2 = regcomp(&regex_time, "^([0-1]?[0-9]|2?[0-3]|[0-9]):([0-5][0-9]|[0-9])$", REG_EXTENDED);
	if(ret2) {
		if(ret2 == REG_ESPACE) {
			fprintf(stderr, "%s\n", strerror(ENOMEM));
		} else {
			fprintf(stderr, "Fatal error while setting up time validation regex.\n");
		}
	}

	return ret1 == 0 && ret2 == 0;
}


void fini_validation(void)
{
	regfree(&regex_time);
	regfree(&regex_date);
}


bool validate_date(char *str)
{
	int ret = regexec(&regex_date, str, 0, NULL, REG_NOTEOL);

	if(ret != 0 && ret != REG_NOMATCH) {
		size_t length = regerror(ret, &regex_date, NULL, 0);
		print_regerror(ret, length, &regex_date);
		return false;
	}
	return ret == 0;
}


bool validate_time(char *str)
{
	int ret = regexec(&regex_time, str, 0, NULL, 0);

	if(ret != 0 && ret != REG_NOMATCH) {
		size_t length = regerror(ret, &regex_time, NULL, 0);
		print_regerror(ret, length, &regex_time);
		return false;
	}
	return ret == 0;
}

bool validate_phone(char *str){
	if(strlen(str)>= 11 || strlen(str) == 0)
		return false;
	while(*str){
		if(isdigit(*str++) == 0) return false;
	}
	return true;
}

bool validate_email(char *email){
	
	bool valid = false;
	int special_char = 0;
	int count_char_after_special = 0;
	int count_char_after_dot = 0;
	for(int i = 0 ;i<=strlen(email);i++){
		if(i == strlen(email))
			return valid;
		if(email[i] == '@')
			special_char ++;
		if(special_char > 2)
			return false;
		if(special_char == 1 && isalpha(email[i]))
            count_char_after_special++;
        if(count_char_after_special > 4 && email[i] == '.')
			special_char++;
		if(special_char == 2 && isalpha(email[i]))
			count_char_after_dot++;
		if(count_char_after_dot >= 2)
			valid = true;
	}
}


bool validate_day(char *str){
	return (strcmp(str,"lunedi") == 0 || strcmp(str,"martedi")==0 || strcmp(str,"mercoledi")==0
		|| strcmp(str,"giovedi")==0 || strcmp(str,"venerdi")==0 || strcmp(str,"sabato")==0);


}