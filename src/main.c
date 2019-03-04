#include <glib/gi18n.h>
#include <gtk/gtk.h>

// We have to set the domain here in order to avoid a macro redefinition
#ifdef G_LOG_DOMAIN
#	undef G_LOG_DOMAIN
#	define G_LOG_DOMAIN "tllt-cp"
#endif

#include "tllt-cp-application.h"

int
main(int argc, char *argv[])
{
	g_autoptr(TlltCpApplication) app;

	g_set_application_name(_("The Learning Little Toaster Control Panel"));

	app = tllt_cp_application_new("com.gitlab.tristan957.tllt-cp");
	return g_application_run(G_APPLICATION(app), argc, argv);
}
