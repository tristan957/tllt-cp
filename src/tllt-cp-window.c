#include <gtk/gtk.h>

#include "tllt-cp-window.h"

struct _TlltCpWindow {
	GtkApplicationWindow parent_instance;
};

typedef struct {
	GtkButton *login_button;
	GtkButton *logout_button;
	GtkButton *user_details_button;
} TlltCpWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpWindow, tllt_cp_window, GTK_TYPE_APPLICATION_WINDOW)

static void
on_login_button_clicked(GtkButton *button, G_GNUC_UNUSED gpointer user_data)
{
	g_print("%s\n", gtk_button_get_label(button));
}

static void
on_logout_button_clicked(GtkButton *button, G_GNUC_UNUSED gpointer user_data)
{
	g_print("%s\n", gtk_button_get_label(button));
}

static void
on_user_details_button_clicked(GtkButton *button, G_GNUC_UNUSED gpointer user_data)
{
	g_print("%s\n", gtk_button_get_label(button));
}

TlltCpWindow *
tllt_cp_window_new(GApplication *app)
{
	g_print("Creating new window\n");
	return g_object_new(TLLT_CP_TYPE_WINDOW, "application", app, NULL);
}

static void
tllt_cp_window_finalize(GObject *object)
{
	G_OBJECT_CLASS(tllt_cp_window_parent_class)->finalize(object);
}

static void
tllt_cp_window_class_init(TlltCpWindowClass *klass)
{
	g_print("Initializing class\n");
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class  = GTK_WIDGET_CLASS(klass);

	object_class->finalize = tllt_cp_window_finalize;

	gtk_widget_class_set_template_from_resource(wid_class,
												"/com/gitlab/tristan957/TlltCp/tllt-cp-window.ui");
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, login_button);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, logout_button);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, user_details_button);
	gtk_widget_class_bind_template_callback(wid_class, on_login_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_logout_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_user_details_button_clicked);
}

static void
tllt_cp_window_init(TlltCpWindow *self)
{
	g_print("Initializing object\n");
	gtk_widget_init_template(GTK_WIDGET(self));
}
