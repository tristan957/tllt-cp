#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define TLLT_TYPE_POWERABLE (tllt_powerable_get_type())
G_DECLARE_INTERFACE(TlltPowerable, tllt_powerable, TLLT, POWERABLE, GObject)

struct _TlltPowerableInterface
{
	GTypeInterface parent_iface;

	void (*on)(TlltPowerable *self);
	void (*off)(TlltPowerable *self);
	gboolean (*is_running)(TlltPowerable *self);
};

gboolean tllt_powerable_is_running(TlltPowerable *self);
void tllt_powerable_off(TlltPowerable *self);
void tllt_powerable_on(TlltPowerable *self);

G_END_DECLS
