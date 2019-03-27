#include <time.h>

#include <gio/gio.h>
#include <glib-object.h>
#include <glib/gi18n.h>

#include "sensors/tllt-scale.h"
#include "sensors/tllt-thermistor.h"
#include "tllt-toaster.h"

typedef struct TlltToasterStartArgs
{
	unsigned int minutes;
	unsigned int seconds;
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
	SIGNAL_START,
	SIGNAL_STOP,
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
	TlltToaster *self		 = TLLT_TOASTER(obj);
	TlltToasterPrivate *priv = tllt_toaster_get_instance_private(self);

	g_object_unref(priv->cancellable);

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

	obj_signals[SIGNAL_START] = g_signal_new("start", G_TYPE_FROM_CLASS(klass), G_SIGNAL_RUN_LAST,
											 0, NULL, NULL, NULL, G_TYPE_NONE, 0);
	obj_signals[SIGNAL_STOP]  = g_signal_new("stop", G_TYPE_FROM_CLASS(klass), G_SIGNAL_RUN_LAST, 0,
											 NULL, NULL, NULL, G_TYPE_NONE, 0);
}

static void
tllt_toaster_init(G_GNUC_UNUSED TlltToaster *self)
{
	TlltToasterPrivate *priv = tllt_toaster_get_instance_private(self);

	priv->cancellable = g_cancellable_new();
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

	g_cancellable_reset(priv->cancellable);

	g_signal_emit(self, obj_signals[SIGNAL_STOP], 0);
}

static void
start_toaster_async(G_GNUC_UNUSED GTask *task, G_GNUC_UNUSED gpointer source_object,
					gpointer task_data, G_GNUC_UNUSED GCancellable *cancellable)
{
	TlltToasterStartArgs *args = task_data;

	const clock_t total_cycles = (args->minutes * 60 + args->seconds) * CLOCKS_PER_SEC;

	// send signal to heating elements

	const clock_t start = clock();
	clock_t t			= clock();
	while (t - start < total_cycles) {
		t = clock();
	}
}

static void
start_toaster_cb(GObject *source_object, G_GNUC_UNUSED GAsyncResult *res,
				 G_GNUC_UNUSED gpointer user_data)
{
	TlltToaster *self = TLLT_TOASTER(source_object);
	tllt_toaster_stop(self);
}

static void
start_toaster_destroy_notify(gpointer data)
{
	g_free(data);
}

void
tllt_toaster_start_with_time(TlltToaster *self, const unsigned int minutes,
							 const unsigned int seconds)
{
	TlltToasterPrivate *priv = tllt_toaster_get_instance_private(self);

	TlltToasterStartArgs *args = g_malloc(sizeof(TlltToasterStartArgs));
	args->minutes			   = minutes;
	args->seconds			   = seconds;

	GTask *toast = g_task_new(self, priv->cancellable, start_toaster_cb, NULL);
	g_task_set_return_on_cancel(toast, TRUE);
	g_task_set_task_data(toast, args, start_toaster_destroy_notify);
	g_task_run_in_thread(toast, start_toaster_async);
	g_object_unref(toast);
}
