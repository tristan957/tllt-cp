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

typedef struct TlltToasterOperationArgs
{
	TlltToaster *toaster;
	int temperature;
	int total_time_seconds;
	unsigned int ref_count;
	TlltToasterUpdateFunc update;
	gpointer user_data;
} TlltToasterOperationArgs;

typedef struct TlltToasterPrivate
{
	gboolean running;
	GCancellable *cancellable;
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
	SIGNAL_PREPARING,
	SIGNAL_STARTED,
	SIGNAL_STOPPED,
	N_SIGNALS,
} TlltToasterSignals;

static guint obj_signals[N_SIGNALS];

static void
tllt_toaster_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltToaster *self = TLLT_TOASTER(obj);

	switch (prop_id) {
	case PROP_THERMISTOR:
		g_value_set_object(val, self->thermistor);
		break;
	case PROP_TOP_HEATING_ELEMENT:
		g_value_set_object(val, self->top_heating_element);
		break;
	case PROP_BOTTOM_HEATING_ELEMENT:
		g_value_set_object(val, self->bottom_heating_element);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
tllt_toaster_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	TlltToaster *self = TLLT_TOASTER(obj);

	switch (prop_id) {
	case PROP_THERMISTOR:
		self->thermistor = g_value_dup_object(val);
		break;
	case PROP_TOP_HEATING_ELEMENT:
		self->top_heating_element = g_value_dup_object(val);
		break;
	case PROP_BOTTOM_HEATING_ELEMENT:
		self->bottom_heating_element = g_value_dup_object(val);
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
	g_object_unref(self->thermistor);
	g_object_unref(self->top_heating_element);
	g_object_unref(self->bottom_heating_element);
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

	obj_signals[SIGNAL_PREPARING] =
		g_signal_new("preparing", G_TYPE_FROM_CLASS(klass), G_SIGNAL_RUN_LAST, 0, NULL, NULL, NULL,
					 G_TYPE_NONE, 0);
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
tllt_toaster_new_from_config_file(const char *file_path, GError **err)
{
	g_return_val_if_fail(err == NULL || *err == NULL, NULL);

	if (file_path == NULL) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_ENV, "No config file path set");
		return NULL;
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

static void
tllt_toaster_operations_args_unref(TlltToasterOperationArgs *args)
{
	args->ref_count--;
	if (args->ref_count == 0) {
		g_free(args);
	}
}

static gboolean
update_toaster(gpointer user_data)
{
	TlltToasterOperationArgs *args = user_data;
	TlltToasterPrivate *priv	   = tllt_toaster_get_instance_private(args->toaster);

	const int delta = args->total_time_seconds - g_timer_elapsed(priv->timer, NULL);
	if (delta > 0 && !g_cancellable_is_cancelled(priv->cancellable)) {
		args->update(delta / 60, delta % 60, 1 - ((double) delta) / args->total_time_seconds,
					 args->user_data);

		return TRUE;
	}

	return FALSE;
}

static void
update_toaster_destroy(gpointer user_data)
{
	TlltToasterOperationArgs *args = user_data;
	TlltToasterPrivate *priv	   = tllt_toaster_get_instance_private(args->toaster);

	g_signal_emit(args->toaster, obj_signals[SIGNAL_STOPPED], 0);

	// Cancel other timeouts if they exist
	g_cancellable_cancel(priv->cancellable);

	g_timer_reset(priv->timer);
	g_object_unref(priv->cancellable);
	g_object_unref(args->toaster);
	tllt_toaster_operations_args_unref(args);
}

/**
 * This is the function for controlling how the toaster will stay at a consistant temperature.
 * It is essentially a state machine. There are 3 states, which can be found in TlltThermistor.h.
 * Starting in a read state, the value is read from the thermistor. If it is higher than the
 * necessary temperature, the heating elements are turned off, and the state is set to read again,
 * so we always recheck every X seconds if the heating elements are off. In the case where the
 * sensor reading is lower than what we want, the heating elements are powered on and the state is
 * set to no read. On the no read state, the state then moves to a waiting state, so that the
 * heating elements can stay on for 2X seconds. The waiting state, then sets the state to the read
 * state. In the waiting state, the heating elements are turned off for X seconds to allow for a
 * thermistor reading.
 */
static gboolean
control_toaster(gpointer user_data)
{
	TlltToasterOperationArgs *args = user_data;
	TlltToasterPrivate *priv	   = tllt_toaster_get_instance_private(args->toaster);

	// Start in wait state because we are starting every operation with heating elements on
	static TlltThermistorState state = STATE_WAIT;

	if (g_cancellable_is_cancelled(priv->cancellable)) {
		// Reset the state before exit
		state = STATE_READ;

		tllt_powerable_off(TLLT_POWERABLE(args->toaster->top_heating_element));
		tllt_powerable_off(TLLT_POWERABLE(args->toaster->bottom_heating_element));

		return G_SOURCE_REMOVE;
	}

	switch (state) {
	case STATE_READ: {
		const double temp = tllt_sensor_read(TLLT_SENSOR(args->toaster->thermistor));
		if (temp < args->temperature) {
			tllt_powerable_on(TLLT_POWERABLE(args->toaster->top_heating_element));
			tllt_powerable_on(TLLT_POWERABLE(args->toaster->bottom_heating_element));

			state = STATE_NO_READ;
		}

		if (temp > args->temperature) {
			tllt_powerable_off(TLLT_POWERABLE(args->toaster->top_heating_element));
			tllt_powerable_off(TLLT_POWERABLE(args->toaster->bottom_heating_element));

			state = STATE_READ;
		}

		break;
	}
	case STATE_NO_READ:
		state = STATE_WAIT;
		break;
	case STATE_WAIT:
		state = STATE_READ;
		tllt_powerable_off(TLLT_POWERABLE(args->toaster->top_heating_element));
		tllt_powerable_off(TLLT_POWERABLE(args->toaster->bottom_heating_element));
		break;
	default:
		g_warn_if_reached();
	}

	return G_SOURCE_CONTINUE;
}

static void
toaster_operation_destroy(gpointer user_data)
{
	TlltToasterOperationArgs *args = user_data;
	TlltToasterPrivate *priv	   = tllt_toaster_get_instance_private(args->toaster);

	g_object_unref(priv->cancellable);
	g_object_unref(args->toaster);

	tllt_toaster_operations_args_unref(args);
}

static gboolean
preheat_toaster(gpointer user_data)
{
	TlltToasterOperationArgs *args = user_data;
	TlltToasterPrivate *priv	   = tllt_toaster_get_instance_private(args->toaster);

	const double temp = tllt_sensor_read(TLLT_SENSOR(args->toaster->thermistor));
	if (!(temp < args->temperature + 10 && temp > args->temperature - 10) &&
		!g_cancellable_is_cancelled(priv->cancellable)) {
		return G_SOURCE_CONTINUE;
	}

	if (g_cancellable_is_cancelled(priv->cancellable)) {
		toaster_operation_destroy(args);
	} else {
		args->ref_count++;
		g_object_ref(args->toaster);
		g_object_ref(priv->cancellable);
		g_timer_reset(priv->timer);

		g_signal_emit(args->toaster, obj_signals[SIGNAL_STARTED], 0);
		g_timer_start(priv->timer);
		g_timeout_add_full(G_PRIORITY_DEFAULT, 40, update_toaster, args, update_toaster_destroy);
		g_timeout_add_seconds_full(G_PRIORITY_DEFAULT, 15, control_toaster, args,
								   toaster_operation_destroy);
	}

	return G_SOURCE_REMOVE;
}

void
tllt_toaster_start(TlltToaster *self, const unsigned int minutes, const unsigned int seconds,
				   const int temperature, const TlltToasterUpdateFunc update, gpointer user_data)
{
	TlltToasterPrivate *priv = tllt_toaster_get_instance_private(self);

	priv->cancellable = g_cancellable_new();

	TlltToasterOperationArgs *toaster_op_args = g_malloc(sizeof(TlltToasterOperationArgs));
	g_return_if_fail(toaster_op_args != NULL);
	toaster_op_args->total_time_seconds = minutes * 60 + seconds;
	toaster_op_args->update				= update;
	toaster_op_args->user_data			= user_data;
	toaster_op_args->toaster			= self;
	toaster_op_args->temperature		= temperature;
	toaster_op_args->ref_count			= 1;
	g_object_ref(self);

	const double temp = tllt_sensor_read(TLLT_SENSOR(self->thermistor));
	if (temp < temperature) {
		tllt_powerable_on(TLLT_POWERABLE(self->top_heating_element));
		tllt_powerable_on(TLLT_POWERABLE(self->bottom_heating_element));
	} else {
		tllt_powerable_off(TLLT_POWERABLE(self->top_heating_element));
		tllt_powerable_off(TLLT_POWERABLE(self->bottom_heating_element));
	}

	g_timeout_add(500, preheat_toaster, toaster_op_args);
	g_signal_emit(self, obj_signals[SIGNAL_PREPARING], 0);
}
