#include <gtk/gtk.h>

#include "tllt-cp-application.h"
#include "tllt-cp-config.h"
#include "tllt-cp-resources.h"
#include "tllt-cp-window.h"
#include "tllt-driver.h"

struct _TlltCpApplication
{
	GtkApplication parent;
};

typedef struct TlltCpApplicationPrivate
{
	GtkApplicationWindow *main_window;
} TlltCpApplicationPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpApplication, tllt_cp_application, GTK_TYPE_APPLICATION)

TlltCpApplication *
tllt_cp_application_new(const char *id)
{
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

static void
tllt_cp_application_about(G_GNUC_UNUSED GSimpleAction *action, G_GNUC_UNUSED GVariant *param,
						  gpointer data)
{
	static const gchar *authors[] = {"Tristan Partin"};

	TlltCpApplication *self = TLLT_CP_APPLICATION(data);
	TlltCpApplicationPrivate *priv =
		tllt_cp_application_get_instance_private(TLLT_CP_APPLICATION(self));

	gtk_show_about_dialog(GTK_WINDOW(priv->main_window), "program-name", PACKAGE_NAME, "version",
						  PACKAGE_VERSION, "license-type", PACKAGE_LICENSE, "website",
						  PACKAGE_WEBSITE, "website-label", PACKAGE_WEBSITE_LABEL, "authors",
						  authors, "logo-icon-name", "trophy-gold", NULL);
}

static void
tllt_cp_application_startup(GApplication *self)
{
	g_resources_register(tllt_cp_get_resource());
	g_application_set_resource_base_path(self, "/com/gitlab/tristan957/tllt-cp");

	G_APPLICATION_CLASS(tllt_cp_application_parent_class)->startup(self);
}

static void
tllt_cp_application_class_init(TlltCpApplicationClass *klass)
{
	GApplicationClass *app_class = G_APPLICATION_CLASS(klass);

	app_class->activate = tllt_cp_application_activate;
	app_class->startup  = tllt_cp_application_startup;
}

static GActionEntry app_entries[] = {{.name = "about", .activate = tllt_cp_application_about}};

static void
tllt_cp_application_init(TlltCpApplication *self)
{
	// Initialize the driver
	tllt_driver_init();

	g_action_map_add_action_entries(G_ACTION_MAP(self), app_entries, G_N_ELEMENTS(app_entries),
									self);
}
