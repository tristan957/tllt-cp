#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-cp-error.h"

struct _TlltCpError
{
	GObject parent_instance;
};

typedef struct TlltCpErrorPrivate
{
	unsigned int status_code;
	char *message;
} TlltCpErrorPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpError, tllt_cp_error, G_TYPE_OBJECT)

typedef enum TlltCpErrorProps
{
	PROP_STATUS_CODE = 1,
	PROP_MESSAGE,
	N_PROPS,
} TlltCpErrorProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_cp_error_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltCpError *self		 = TLLT_CP_ERROR(obj);
	TlltCpErrorPrivate *priv = tllt_cp_error_get_instance_private(self);

	switch (prop_id) {
	case PROP_STATUS_CODE:
		g_value_set_uint(val, priv->status_code);
		break;
	case PROP_MESSAGE:
		g_value_set_string(val, priv->message);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_cp_error_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	TlltCpError *self		 = TLLT_CP_ERROR(obj);
	TlltCpErrorPrivate *priv = tllt_cp_error_get_instance_private(self);

	switch (prop_id) {
	case PROP_STATUS_CODE:
		priv->status_code = g_value_get_uint(val);
		break;
	case PROP_MESSAGE:
		g_free(priv->message);
		priv->message = g_value_dup_string(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_cp_error_finalize(GObject *obj)
{
	TlltCpError *self		 = TLLT_CP_ERROR(obj);
	TlltCpErrorPrivate *priv = tllt_cp_error_get_instance_private(self);

	g_free(priv->message);

	G_OBJECT_CLASS(tllt_cp_error_parent_class)->finalize(obj);
}

static void
tllt_cp_error_class_init(TlltCpErrorClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= tllt_cp_error_finalize;
	obj_class->get_property = tllt_cp_error_get_property;
	obj_class->set_property = tllt_cp_error_set_property;

	obj_properties[PROP_STATUS_CODE] = g_param_spec_uint(
		"statusCode", _("HTTP status code"), _("HTTP status code from server"), 100, 530, 200,
		G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_PRIVATE);
	obj_properties[PROP_MESSAGE] =
		g_param_spec_string("message", _("Message"), _("Message pertaining to error"), NULL,
							G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_PRIVATE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_cp_error_init(G_GNUC_UNUSED TlltCpError *self)
{}
