#include <gtk/gtk.h>

#include "tllt-cp-header-bar.h"

struct _TlltCpHeaderBar {
	GtkHeaderBar parent_instance;
};

typedef struct {
	GtkButton *login_button;
} TlltCpHeaderBarPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpHeaderBar, tllt_cp_header_bar, GTK_TYPE_HEADER_BAR)

static void
tllt_cp_header_bar_finalize(GObject *object)
{
	G_OBJECT_CLASS(tllt_cp_header_bar_parent_class)->finalize(object);
}

static void
tllt_cp_header_bar_class_init(TlltCpHeaderBarClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);

	object_class->finalize = tllt_cp_header_bar_finalize;
}

static void
tllt_cp_header_bar_init(TlltCpHeaderBar *self)
{
}

TlltCpHeaderBar *
tllt_cp_header_bar_new()
{
	return g_object_new(TLLT_CP_TYPE_HEADER_BAR, "show-close-button", TRUE);
}
