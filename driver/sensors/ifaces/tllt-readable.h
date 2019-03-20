#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define TLLT_TYPE_READABLE (tllt_powerable_get_type())
G_DECLARE_INTERFACE(TlltReadable, tllt_powerable, TLLT, READABLE, GObject)

struct _TlltReadableInterface
{
	GTypeInterface parent_iface;

	double (*read)(TlltReadable *self);
};

void tllt_readable_read(TlltReadable *self);

G_END_DECLS
