#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-scale.h"
#include "tllt-thermo.h"
#include "tllt-toaster.h"

struct _TlltToaster
{
	GObject parent_instance;
};

typedef struct TlltToasterPrivate
{
	TlltScale *scale;
	TlltThermistor *thermo;
} TlltToasterPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltToaster, tllt_toaster, G_TYPE_OBJECT)

typedef enum TlltToasterProps
{
	PROP_SCALE = 1,
	PROP_THERMISTOR,
	N_PROPS,
} TlltToasterProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_toaster_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltToaster *self		 = TLLT_TOASTER(obj);
	TlltToasterPrivate *priv = tllt_toaster_get_instance_private(self);

	switch (prop_id) {
	case PROP_SCALE:
		g_value_set_object(val, priv->scale);
		break;
	case PROP_THERMISTOR:
		g_value_set_object(val, priv->thermo);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
tllt_toaster_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	TlltToaster *self		 = TLLT_TOASTER(obj);
	TlltToasterPrivate *priv = tllt_toaster_get_instance_private(self);

	switch (prop_id) {
	case PROP_SCALE:
		priv->scale = g_value_get_object(val);
		break;
	case PROP_THERMISTOR:
		priv->thermo = g_value_get_object(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
tllt_toaster_class_init(TlltToasterClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->get_property = tllt_toaster_get_property;
	obj_class->set_property = tllt_toaster_set_property;

	obj_properties[PROP_SCALE] =
		g_param_spec_object("scale", _("Scale"), _("Pointer to scale object"), TLLT_TYPE_SCALE,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
	obj_properties[PROP_THERMISTOR] =
		g_param_spec_object("thermo", _("Thermister"), _("Pointer to thermo object"),
							TLLT_TYPE_THERMISTOR, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_toaster_init(G_GNUC_UNUSED TlltToaster *self)
{}

TlltToaster *
tllt_toaster_new(const unsigned char scale_gpio_pin, const unsigned char thermo_gpio_pin)
{
	g_return_val_if_fail(scale_gpio_pin != thermo_gpio_pin, NULL);

	return g_object_new(TLLT_TYPE_TOASTER, "scale", tllt_scale_new(scale_gpio_pin), "thermo",
						tllt_thermistor_new(thermo_gpio_pin), NULL);
}
