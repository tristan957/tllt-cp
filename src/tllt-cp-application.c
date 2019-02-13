#include <gtk/gtk.h>

#include "tllt-cp-application.h"
#include "tllt-cp-window.h"

struct _TlltCpApplication {
	GtkApplication parent;
};

typedef struct {
	GtkApplicationWindow *main_window;
} TlltCpApplicationPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpApplication, tllt_cp_application, GTK_TYPE_APPLICATION)

TlltCpApplication *
tllt_cp_application_new(const char *id)
{
	g_print("%s\n", id);
	return g_object_new(TLLT_CP_TYPE_APPLICATION, "application-id", id, NULL);
}

static void
tllt_cp_application_activate(GApplication *self)
{
	TlltCpApplicationPrivate *priv =
		tllt_cp_application_get_instance_private(TLLT_CP_APPLICATION(self));

	if (priv->main_window == NULL) {
		priv->main_window = GTK_APPLICATION_WINDOW(tllt_cp_window_new(self));
	}

	gtk_window_present(GTK_WINDOW(priv->main_window));
}

// static void
// tllt_cp_application_startup(GApplication *self)
// {
// 	G_APPLICATION_CLASS(tllt_cp_application_parent_class)->startup(self);
// }

// static void
// tllt_cp_application_finalize(GObject *object)
// {
// 	TlltCpApplication *self		   = TLLT_CP_APPLICATION(object);
// 	TlltCpApplicationPrivate *priv = tllt_cp_application_get_instance_private(self);

// 	G_OBJECT_CLASS(tllt_cp_application_parent_class)->finalize(object);
// }

static void
tllt_cp_application_class_init(TlltCpApplicationClass *klass)
{
	GObjectClass *object_class   = G_OBJECT_CLASS(klass);
	GApplicationClass *app_class = G_APPLICATION_CLASS(klass);

	// object_class->finalize = tllt_cp_application_finalize;
	app_class->activate = tllt_cp_application_activate;
	// app_class->startup	 = tllt_cp_application_startup;
}

static void
tllt_cp_application_init(TlltCpApplication *self)
{
	TlltCpApplicationPrivate *priv =
		tllt_cp_application_get_instance_private(TLLT_CP_APPLICATION(self));
}
