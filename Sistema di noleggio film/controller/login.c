#include <stdbool.h>
#include <stdio.h>

#include "login.h"
#include "manager.h"
#include "impiegato.h"
#include "cliente.h"

#include "../view/login.h"
#include "../model/db.h"


bool login(void)
{
	struct credentials cred;
	view_login(&cred);
	role_t role = attempt_login(&cred);

	switch(role) {
		case MANAGER:
			manager_controller();
			break;
		case IMPIEGATO:
			impiegato_controller();
			break;
		case CLIENTE:
			cliente_controller();
			break;

		default:
			return false;
	}

	return true;
}
