#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-powerable.h"

G_DEFINE_INTERFACE(TlltPowerable, tllt_powerable, G_TYPE_OBJECT)

static void
tllt_powerable_default_init(TlltPowerableInterface *iface)
{
	g_object_interface_install_property(
		iface, g_param_spec_boolean("running", _("Running"),
									_("Whether or not the powerable object is running"), FALSE,
									G_PARAM_READWRITE));
}

void
tllt_powerable_off(TlltPowerable *self)
{
	g_return_if_fail(TLLT_IS_POWERABLE(self));
	g_return_if_fail(tllt_powerable_is_running(self));

	TlltPowerableInterface *iface = TLLT_POWERABLE_GET_IFACE(self);
	g_return_if_fail(iface->off != NULL);
	iface->off(self);
}

void
tllt_powerable_on(TlltPowerable *self)
{
	g_return_if_fail(TLLT_IS_POWERABLE(self));
	g_return_if_fail(!tllt_powerable_is_running(self));

	TlltPowerableInterface *iface = TLLT_POWERABLE_GET_IFACE(self);
	g_return_if_fail(iface->on != NULL);
	iface->on(self);
}

gboolean
tllt_powerable_is_running(TlltPowerable *self)
{
	g_return_val_if_fail(TLLT_IS_POWERABLE(self), FALSE);

	TlltPowerableInterface *iface = TLLT_POWERABLE_GET_IFACE(self);
	g_return_val_if_fail(iface->is_running != NULL, FALSE);
	return iface->is_running(self);
}
