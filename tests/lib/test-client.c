#include <glib-object.h>

#include "tllt-cp-client.h"
#include "tllt-cp-user.h"

int
main(G_GNUC_UNUSED int argc, G_GNUC_UNUSED char *argv[])
{
	g_autoptr(GError) err		   = NULL;
	g_autoptr(TlltCpClient) client = tllt_cp_client_new_from_environment();
	g_autoptr(TlltCpUser) user =
		tllt_cp_user_authenticate(client, "tristan.partin@gmail.com", "your-mom", &err);
	// g_autoptr(TlltCpUser) user = tllt_cp_user_get_by_id(client, 1, &err);

	if (err != NULL) {
		g_printerr("%s\n", err->message);
	} else {
		g_print("%s\n", tllt_cp_user_get_name(user));
	}

	return 0;
}
