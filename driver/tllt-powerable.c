#include <glib-object.h>

#include "tllt-powerable.h"

G_DEFINE_INTERFACE(TlltPowerable, tllt_powerable, G_TYPE_OBJECT)

static void
tllt_powerable_default_init(TlltPowerableInterface *iface)
{
	iface->running = FALSE;
}

void
tllt_powerable_off(TlltPowerable *self)
{
	g_return_if_fail(TLLT_IS_POWERABLE(self));

	TlltPowerableInterface *iface = TLLT_POWERABLE_GET_IFACE(self);
	iface->running				  = FALSE;
	g_return_if_fail(iface->off != NULL);
	iface->off(self);
}

void
tllt_powerable_on(TlltPowerable *self)
{
	g_return_if_fail(TLLT_IS_POWERABLE(self));

	TlltPowerableInterface *iface = TLLT_POWERABLE_GET_IFACE(self);
	iface->running				  = TRUE;
	g_return_if_fail(iface->on != NULL);
	iface->on(self);
}

gboolean
tllt_powerable_running(TlltPowerable *self)
{
	TlltPowerableInterface *iface = TLLT_POWERABLE_GET_IFACE(self);

	return iface->running;
}
