#include <gio/gio.h>
#include <glib-object.h>
#include <glib/gi18n.h>
#include <json-glib/json-glib.h>
#ifdef TLLT_WITH_WIRINGPI
#	include <wiringPi.h>
#endif

#include "components/tllt-heating-element.h"
#include "sensors/tllt-thermistor.h"
#include "tllt-config.h"
#include "tllt-error.h"
#include "tllt-powerable.h"
#include "tllt-toaster.h"

typedef struct TlltToasterUpdateArgs
{
	TlltToaster *toaster;
	unsigned long total_seconds;
	TlltToasterUpdateFunc update;
	gpointer user_data;
} TlltToasterUpdateArgs;

typedef struct TlltControlToasterArgs
{
	TlltToaster *toaster;
	int temperature;
} TlltControlToasterArgs;

struct _TlltToaster
{
	GObject parent_instance;
};

typedef struct TlltToasterPrivate
{
	gboolean running;
	GCancellable *cancellable;
	TlltHeatingElement *top_heating_element;
	TlltHeatingElement *bottom_heating_element;
	TlltThermistor *thermistor;
	GTimer *timer;
} TlltToasterPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltToaster, tllt_toaster, G_TYPE_OBJECT)

typedef enum TlltToasterProps
{
	PROP_THERMISTOR = 1,
	PROP_TOP_HEATING_ELEMENT,
	PROP_BOTTOM_HEATING_ELEMENT,
	N_PROPS,
} TlltToasterProps;

static GParamSpec *obj_properties[N_PROPS];

typedef enum TlltToasterSignals
{
	SIGNAL_STARTED,
	SIGNAL_STOPPED,
	N_SIGNALS,
} TlltToasterSignals;

static guint obj_signals[N_SIGNALS];

static void
tllt_toaster_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltToaster *self		 = TLLT_TOASTER(obj);
	TlltToasterPrivate *priv = tllt_toaster_get_instance_private(self);

	switch (prop_id) {
	case PROP_THERMISTOR:
		g_value_set_object(val, priv->thermistor);
		break;
	case PROP_TOP_HEATING_ELEMENT:
		g_value_set_object(val, priv->top_heating_element);
		break;
	case PROP_BOTTOM_HEATING_ELEMENT:
		g_value_set_object(val, priv->bottom_heating_element);
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
	case PROP_THERMISTOR:
		priv->thermistor = g_value_dup_object(val);
		break;
	case PROP_TOP_HEATING_ELEMENT:
		priv->top_heating_element = g_value_dup_object(val);
		break;
	case PROP_BOTTOM_HEATING_ELEMENT:
		priv->bottom_heating_element = g_value_dup_object(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
tllt_toaster_finalize(GObject *obj)
{
	TlltToaster *self		 = TLLT_TOASTER(obj);
	TlltToasterPrivate *priv = tllt_toaster_get_instance_private(self);

	g_cancellable_cancel(priv->cancellable);

	g_object_unref(priv->cancellable);
	g_object_unref(priv->thermistor);
	g_object_unref(priv->top_heating_element);
	g_object_unref(priv->bottom_heating_element);
	g_timer_destroy(priv->timer);

	G_OBJECT_CLASS(tllt_toaster_parent_class)->finalize(obj);
}

static void
tllt_toaster_class_init(TlltToasterClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= tllt_toaster_finalize;
	obj_class->get_property = tllt_toaster_get_property;
	obj_class->set_property = tllt_toaster_set_property;

	obj_properties[PROP_THERMISTOR] =
		g_param_spec_object("thermistor", _("Thermister"), _("Pointer to thermistor object"),
							TLLT_TYPE_THERMISTOR, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
	obj_properties[PROP_TOP_HEATING_ELEMENT] = g_param_spec_object(
		"top-heating-element", _("Top heating element"), _("Top heating element"),
		TLLT_TYPE_HEATING_ELEMENT, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
	obj_properties[PROP_BOTTOM_HEATING_ELEMENT] = g_param_spec_object(
		"bottom-heating-element", _("Bottom heating element"), _("Bottom heating element"),
		TLLT_TYPE_HEATING_ELEMENT, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);

	obj_signals[SIGNAL_STARTED] =
		g_signal_new("started", G_TYPE_FROM_CLASS(klass), G_SIGNAL_RUN_LAST, 0, NULL, NULL, NULL,
					 G_TYPE_NONE, 0);
	obj_signals[SIGNAL_STOPPED] =
		g_signal_new("stopped", G_TYPE_FROM_CLASS(klass), G_SIGNAL_RUN_LAST, 0, NULL, NULL, NULL,
					 G_TYPE_NONE, 0);
}

static void
tllt_toaster_init(TlltToaster *self)
{
	TlltToasterPrivate *priv = tllt_toaster_get_instance_private(self);

	priv->timer = g_timer_new();
}

TlltToaster *
tllt_toaster_new_from_config_file(GError **err)
{
	g_return_val_if_fail(err == NULL || *err == NULL, NULL);

	const gchar *file_path = g_getenv("TLLT_TOASTER_CONFIG_FILE_PATH");
	if (file_path == NULL) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_ENV, "No config file path set");
	}

	char *buffer;
	if (!g_file_get_contents(file_path, &buffer, NULL, err)) {
		return NULL;
	}

	GObject *obj = json_gobject_from_data(TLLT_TYPE_TOASTER, buffer, -1, err);
	if (*err != NULL) {
		return NULL;
	}

	return TLLT_TOASTER(obj);
}

void
tllt_toaster_stop(TlltToaster *self)
{
	TlltToasterPrivate *priv = tllt_toaster_get_instance_private(self);

	g_cancellable_cancel(priv->cancellable);
}

static gboolean
update_toaster(gpointer user_data)
{
	TlltToasterUpdateArgs *args = user_data;
	TlltToasterPrivate *priv	= tllt_toaster_get_instance_private(args->toaster);

	const int delta = args->total_seconds - g_timer_elapsed(priv->timer, NULL);
	if (delta > 0 && !g_cancellable_is_cancelled(priv->cancellable)) {
		args->update(delta / 60, delta % 60, 1 - ((double) delta) / args->total_seconds,
					 args->user_data);

		return TRUE;
	}

	return FALSE;
}

static void
on_update_toaster_destroy(gpointer user_data)
{
	TlltToasterUpdateArgs *args = user_data;
	TlltToasterPrivate *priv	= tllt_toaster_get_instance_private(args->toaster);

	// Cancel other timeouts if they exist
	g_cancellable_cancel(priv->cancellable);
	g_timer_reset(priv->timer);
	g_object_unref(priv->cancellable);

	g_signal_emit(args->toaster, obj_signals[SIGNAL_STOPPED], 0);

	g_object_unref(args->toaster);
	g_free(args);
}

static gboolean
control_toaster(gpointer user_data)
{
	TlltControlToasterArgs *args = user_data;
	TlltToasterPrivate *priv	 = tllt_toaster_get_instance_private(args->toaster);

	static TlltThermistorState state = STATE_READ;

	if (g_cancellable_is_cancelled(priv->cancellable)) {
		// Reset the state before exit
		state = STATE_READ;

		tllt_powerable_off(TLLT_POWERABLE(priv->top_heating_element));
		tllt_powerable_off(TLLT_POWERABLE(priv->bottom_heating_element));

		return FALSE;
	}

	switch (state) {
	case STATE_READ: {
		const double temp =
			tllt_thermistor_reading_to_farenheit(tllt_sensor_read(TLLT_SENSOR(priv->thermistor)));
		if (temp < args->temperature) {
			tllt_powerable_on(TLLT_POWERABLE(priv->top_heating_element));
			tllt_powerable_on(TLLT_POWERABLE(priv->bottom_heating_element));

			state = STATE_NO_READ;
		}

		if (temp > args->temperature) {
			tllt_powerable_off(TLLT_POWERABLE(priv->top_heating_element));
			tllt_powerable_off(TLLT_POWERABLE(priv->bottom_heating_element));

			state = STATE_READ;
		}

		break;
	}
	case STATE_NO_READ:
		state = STATE_WAIT;
		break;
	case STATE_WAIT:
		state = STATE_READ;
		break;
	default:
		g_warn_if_reached();
	}

	return TRUE;
}

static void
on_control_toaster_destroy(gpointer user_data)
{
	TlltControlToasterArgs *args = user_data;

	g_object_unref(args->toaster);
	g_free(user_data);
}

void
tllt_toaster_start(TlltToaster *self, const unsigned int minutes, const unsigned int seconds,
				   const int temperature, const TlltToasterUpdateFunc update, gpointer user_data)
{
	TlltToasterPrivate *priv = tllt_toaster_get_instance_private(self);

	priv->cancellable = g_cancellable_new();
	// Add ref since in will be used in two timeouts
	g_object_ref(priv->cancellable);

	TlltToasterUpdateArgs *toaster_update_args = g_malloc(sizeof(TlltToasterUpdateArgs));
	g_return_if_fail(toaster_update_args != NULL);
	toaster_update_args->total_seconds = minutes * 60 + seconds;
	toaster_update_args->update		   = update;
	toaster_update_args->user_data	 = user_data;
	toaster_update_args->toaster	   = self;
	g_object_ref(self);

	TlltControlToasterArgs *control_toaster_args = g_malloc(sizeof(TlltControlToasterArgs));
	g_return_if_fail(control_toaster_args != NULL);
	control_toaster_args->toaster	 = self;
	control_toaster_args->temperature = temperature;
	g_object_ref(self);

	g_timer_start(priv->timer);
	g_timeout_add_full(G_PRIORITY_DEFAULT, 40, update_toaster, toaster_update_args,
					   on_update_toaster_destroy);
	g_timeout_add_seconds_full(G_PRIORITY_DEFAULT, 15, control_toaster, control_toaster_args,
							   on_control_toaster_destroy);

	g_signal_emit(self, obj_signals[SIGNAL_STARTED], 0);
}
