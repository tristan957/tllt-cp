#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TLLT_CP_TYPE_HEADER_BAR (tllt_cp_header_bar_get_type())
G_DECLARE_FINAL_TYPE(TlltCpHeaderBar, tllt_cp_header_bar, TLLT_CP, HEADER_BAR, GtkHeaderBar)

TlltCpHeaderBar *tllt_cp_header_bar_new();

G_END_DECLS
