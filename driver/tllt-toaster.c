#include <gio/gio.h>
#include <glib-object.h>
#include <glib/gi18n.h>

#include "sensors/tllt-scale.h"
#include "sensors/tllt-thermistor.h"
#include "tllt-toaster.h"

typedef struct TlltToasterStartArgs
{
	TlltToaster *toaster;
	unsigned long total_seconds;
	TlltToasterUpdateFunc update;
	gpointer user_data;
} TlltToasterStartArgs;

struct _TlltToaster
{
	GObject parent_instance;
};

typedef struct TlltToasterPrivate
{
	gboolean running;
	GCancellable *cancellable;
	TlltScale *scale;
	TlltThermistor *thermo;
	GTimer *timer;
} TlltToasterPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltToaster, tllt_toaster, G_TYPE_OBJECT)

typedef enum TlltToasterProps
{
	PROP_SCALE = 1,
	PROP_THERMISTOR,
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
tllt_toaster_finalize(GObject *obj)
{
	g_print("FINALIZING\n");
	TlltToaster *self		 = TLLT_TOASTER(obj);
	TlltToasterPrivate *priv = tllt_toaster_get_instance_private(self);

	g_cancellable_cancel(priv->cancellable);

	g_clear_object(&priv->cancellable);
	g_clear_object(&priv->scale);
	g_clear_object(&priv->thermo);
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

	obj_properties[PROP_SCALE] =
		g_param_spec_object("scale", _("Scale"), _("Pointer to scale object"), TLLT_TYPE_SCALE,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
	obj_properties[PROP_THERMISTOR] =
		g_param_spec_object("thermistor", _("Thermister"), _("Pointer to thermo object"),
							TLLT_TYPE_THERMISTOR, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

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
tllt_toaster_new(const unsigned char scale_gpio_pin, const unsigned char thermo_gpio_pin)
{
	g_return_val_if_fail(scale_gpio_pin != thermo_gpio_pin, NULL);

	return g_object_new(TLLT_TYPE_TOASTER, "scale", tllt_scale_new(scale_gpio_pin), "thermistor",
						tllt_thermistor_new(thermo_gpio_pin), NULL);
}

void
tllt_toaster_stop(TlltToaster *self)
{
	TlltToasterPrivate *priv = tllt_toaster_get_instance_private(self);

	g_cancellable_cancel(priv->cancellable);
}

static gboolean
start_toaster(gpointer user_data)
{
	TlltToasterStartArgs *args = user_data;
	TlltToasterPrivate *priv   = tllt_toaster_get_instance_private(args->toaster);

	g_object_ref(args->toaster);

	const int delta = args->total_seconds - g_timer_elapsed(priv->timer, NULL);
	if (delta > 0 && !g_cancellable_is_cancelled(priv->cancellable)) {
		args->update(delta / 60, delta % 60, 1 - ((double) delta) / args->total_seconds,
					 args->user_data);
		return TRUE;
	}

	g_timer_reset(priv->timer);
	g_object_unref(priv->cancellable);

	g_signal_emit(args->toaster, obj_signals[SIGNAL_STOPPED], 0);

	g_object_unref(args->toaster);
	g_free(args);

	return FALSE;
}

void
tllt_toaster_start_with_time(TlltToaster *self, const unsigned int minutes,
							 const unsigned int seconds, const TlltToasterUpdateFunc update,
							 gpointer user_data)
{
	TlltToasterPrivate *priv = tllt_toaster_get_instance_private(self);

	priv->cancellable = g_cancellable_new();

	TlltToasterStartArgs *args = g_malloc(sizeof(TlltToasterStartArgs));
	g_warn_if_fail(args != NULL);
	args->total_seconds = minutes * 60 + seconds;
	args->update		= update;
	args->user_data		= user_data;
	args->toaster		= self;

	g_timer_start(priv->timer);
	g_timeout_add(40, start_toaster, args);

	g_signal_emit(self, obj_signals[SIGNAL_STARTED], 0);
}
