#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-sensor.h"

typedef struct TlltSensorPrivate
{
	gboolean running;
} TlltSensorPrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(TlltSensor, tllt_sensor, G_TYPE_OBJECT)

typedef enum TlltSensorProps
{
	PROP_RUNNING = 1,
	N_PROPS,
} TlltSensorProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_sensor_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltSensor *self		= TLLT_SENSOR(obj);
	TlltSensorPrivate *priv = tllt_sensor_get_instance_private(self);

	switch (prop_id) {
	case PROP_RUNNING:
		g_value_set_boolean(val, priv->running);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_sensor_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	TlltSensor *self		= TLLT_SENSOR(obj);
	TlltSensorPrivate *priv = tllt_sensor_get_instance_private(self);

	switch (prop_id) {
	case PROP_RUNNING:
		priv->running = g_value_get_boolean(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_sensor_class_init(TlltSensorClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->get_property = tllt_sensor_get_property;
	obj_class->set_property = tllt_sensor_set_property;

	obj_properties[PROP_RUNNING] =
		g_param_spec_boolean("running", _("Running"), _("Whether the sensor is running"), FALSE,
							 G_PARAM_READWRITE | G_PARAM_PRIVATE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_sensor_init(G_GNUC_UNUSED TlltSensor *self)
{}

void
tllt_sensor_off(TlltSensor *self)
{
	TlltSensorPrivate *priv = tllt_sensor_get_instance_private(self);

	priv->running = FALSE;

	TlltSensorClass *klass = TLLT_SENSOR_GET_CLASS(self);
	klass->off(self);
}

void
tllt_sensor_on(TlltSensor *self)
{
	TlltSensorPrivate *priv = tllt_sensor_get_instance_private(self);

	priv->running = TRUE;

	TlltSensorClass *klass = TLLT_SENSOR_GET_CLASS(self);
	klass->on(self);
}

double
tllt_sensor_read(TlltSensor *self)
{
	TlltSensorClass *klass = TLLT_SENSOR_GET_CLASS(self);
	return klass->read(self);
}
