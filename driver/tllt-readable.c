#include <glib-object.h>

#include "tllt-readable.h"

G_DEFINE_INTERFACE(TlltReadable, tllt_readable, G_TYPE_OBJECT)

static void
tllt_readable_default_init(G_GNUC_UNUSED TlltReadableInterface *iface)
{}

void
tllt_readable_read(TlltReadable *self)
{
	g_return_if_fail(TLLT_IS_READABLE(self));

	TlltReadableInterface *iface = TLLT_READABLE_GET_IFACE(self);
	g_return_if_fail(iface->read != NULL);
	iface->read(self);
}
