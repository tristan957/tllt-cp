#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum TlltCpErrorCode
{
	ERROR_CURL,
	ERROR_JSON,
} TlltCpErrorCode;

#define TLLT_CP_TYPE_ERROR (tllt_cp_error_get_type())
G_DECLARE_FINAL_TYPE(TlltCpError, tllt_cp_error, TLLT_CP, ERROR, GObject)

G_END_DECLS
