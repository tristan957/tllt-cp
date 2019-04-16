#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "tllt-cp-config.h"

// We have to set the domain here in order to avoid a macro redefinition
#ifdef G_LOG_DOMAIN
#	undef G_LOG_DOMAIN
#	define G_LOG_DOMAIN "tllt-cp"
#endif

#include "tllt-cp-application.h"

int
main(int argc, const char *argv[])
{
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);

	g_autoptr(TlltCpApplication) app = tllt_cp_application_new("ht.sr.git.tristan957.tllt-cp");

	g_set_application_name(_("The Learning Little Toaster Control Panel"));

	return g_application_run(G_APPLICATION(app), argc, argv);
}
