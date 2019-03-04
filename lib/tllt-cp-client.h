#pragma once

#include <glib-object.h>

#define TLLT_CP_TYPE_CLIENT (tllt_cp_client_get_type())
G_DECLARE_FINAL_TYPE(TlltCpClient, tllt_cp_client, TLLT_CP, CLIENT, GObject)

TlltCpClient *tllt_cp_client_new_from_environment();
