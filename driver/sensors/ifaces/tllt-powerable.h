#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define TLLT_TYPE_POWERABLE (tllt_powerable_get_type())
G_DECLARE_INTERFACE(TlltPowerable, tllt_powerable, TLLT, POWERABLE, GObject)

struct _TlltPowerableInterface
{
	GTypeInterface parent_iface;

	gboolean running;

	void (*on)(TlltPowerable *self);
	void (*off)(TlltPowerable *self);
};

void tllt_powerable_on(TlltPowerable *self);
void tllt_powerable_off(TlltPowerable *self);

G_END_DECLS
