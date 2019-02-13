#include <gtk/gtk.h>

#include "tllt-cp-application.h"

int
main(int argc, char *argv[])
{
	g_autoptr(TlltCpApplication) app;

	g_set_application_name("The Learning Little Toaster");

	app = tllt_cp_application_new("com.gitlab.tristan957.TlltCp");
	return g_application_run(G_APPLICATION(app), argc, argv);
}
