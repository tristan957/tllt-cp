#include <glib-object.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "tllt-cp-feedback-dialog.h"
#include "tllt-cp.h"

struct _TlltCpFeedbackDialog
{
	GtkDialog parent_instance;
};

typedef struct TlltCpFeedbackDialogPrivate
{
	TlltCpUser *user;
	TlltCpClient *client;
} TlltCpFeedbackDialogPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpFeedbackDialog, tllt_cp_feedback_dialog, GTK_TYPE_DIALOG)

typedef enum TlltCpFeedbackDialogProps
{
	PROP_USER = 1,
	PROP_CLIENT,
	N_PROPS,
} TlltCpFeedbackDialogProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_cp_feedback_dialog_finalize(GObject *obj)
{
	TlltCpFeedbackDialog *self		  = TLLT_CP_FEEDBACK_DIALOG(obj);
	TlltCpFeedbackDialogPrivate *priv = tllt_cp_feedback_dialog_get_instance_private(self);

	g_object_unref(priv->user);
	g_object_unref(priv->client);

	G_OBJECT_CLASS(tllt_cp_feedback_dialog_parent_class)->finalize(obj);
}

static void
tllt_cp_feedback_dialog_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltCpFeedbackDialog *self		  = TLLT_CP_FEEDBACK_DIALOG(obj);
	TlltCpFeedbackDialogPrivate *priv = tllt_cp_feedback_dialog_get_instance_private(self);

	switch (prop_id) {
	case PROP_USER:
		g_value_set_object(val, priv->user);
		break;
	case PROP_CLIENT:
		g_value_set_object(val, priv->client);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
tllt_cp_feedback_dialog_set_property(GObject *obj, guint prop_id, const GValue *val,
									 GParamSpec *pspec)
{
	TlltCpFeedbackDialog *self		  = TLLT_CP_FEEDBACK_DIALOG(obj);
	TlltCpFeedbackDialogPrivate *priv = tllt_cp_feedback_dialog_get_instance_private(self);

	switch (prop_id) {
	case PROP_USER:
		if (priv->user != NULL) {
			g_object_unref(priv->user);
		}
		priv->user = g_value_dup_object(val);
		break;
	case PROP_CLIENT:
		if (priv->client != NULL) {
			g_object_unref(priv->client);
		}
		priv->client = g_value_dup_object(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
on_less_toasty_clicked(G_GNUC_UNUSED GtkButton *widget, G_GNUC_UNUSED gpointer user_data)
{
	TlltCpFeedbackDialog *self		  = TLLT_CP_FEEDBACK_DIALOG(user_data);
	TlltCpFeedbackDialogPrivate *priv = tllt_cp_feedback_dialog_get_instance_private(self);

	g_autoptr(GError) err = NULL;
	tllt_cp_user_adjust_scale(priv->user, priv->client, LESS_TOASTY, &err);

	if (err != NULL) {
		g_printerr("%s\n", err->message);
	}

	gtk_window_close(GTK_WINDOW(self));
}

static void
on_the_same_clicked(G_GNUC_UNUSED GtkButton *widget, G_GNUC_UNUSED gpointer user_data)
{
	TlltCpFeedbackDialog *self		  = TLLT_CP_FEEDBACK_DIALOG(user_data);
	TlltCpFeedbackDialogPrivate *priv = tllt_cp_feedback_dialog_get_instance_private(self);

	g_autoptr(GError) err = NULL;
	tllt_cp_user_adjust_scale(priv->user, priv->client, THE_SAME, &err);

	if (err != NULL) {
		g_printerr("%s\n", err->message);
	}

	gtk_window_close(GTK_WINDOW(self));
}

static void
on_more_toasty_clicked(G_GNUC_UNUSED GtkButton *widget, G_GNUC_UNUSED gpointer user_data)
{
	TlltCpFeedbackDialog *self		  = TLLT_CP_FEEDBACK_DIALOG(user_data);
	TlltCpFeedbackDialogPrivate *priv = tllt_cp_feedback_dialog_get_instance_private(self);

	g_autoptr(GError) err = NULL;
	tllt_cp_user_adjust_scale(priv->user, priv->client, MORE_TOASTY, &err);

	if (err != NULL) {
		g_printerr("%s\n", err->message);
	}

	gtk_window_close(GTK_WINDOW(self));
}

static void
tllt_cp_feedback_dialog_class_init(TlltCpFeedbackDialogClass *klass)
{
	GObjectClass *obj_class   = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class = GTK_WIDGET_CLASS(klass);

	obj_class->finalize		= tllt_cp_feedback_dialog_finalize;
	obj_class->get_property = tllt_cp_feedback_dialog_get_property;
	obj_class->set_property = tllt_cp_feedback_dialog_set_property;

	obj_properties[PROP_USER] =
		g_param_spec_object("user", _("User"), _("User providing the feedback"), TLLT_CP_TYPE_USER,
							G_PARAM_PRIVATE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE);
	obj_properties[PROP_CLIENT] = g_param_spec_object(
		"client", _("Client"), _("Client to talk to the server"), TLLT_CP_TYPE_CLIENT,
		G_PARAM_PRIVATE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);

	gtk_widget_class_set_template_from_resource(
		wid_class, "/ht/sr/git/tristan957/tllt-cp/ui/tllt-cp-feedback-dialog.ui");
	gtk_widget_class_bind_template_callback(wid_class, on_less_toasty_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_the_same_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_more_toasty_clicked);
}

static void
tllt_cp_feedback_dialog_init(TlltCpFeedbackDialog *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));
}

TlltCpFeedbackDialog *
tllt_cp_feedback_dialog_new(GtkWindow *parent, TlltCpUser *user, TlltCpClient *client)
{
	return g_object_new(TLLT_CP_TYPE_FEEDBACK_DIALOG, "transient-for", parent, "user", user,
						"client", client, NULL);
}
