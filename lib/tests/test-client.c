#include <glib-object.h>

#include "tllt-cp-client.h"

int
main(G_GNUC_UNUSED int argc, G_GNUC_UNUSED char *argv[])
{
	g_autoptr(TlltCpClient) client = tllt_cp_client_new_from_environment();

	return 0;
}
