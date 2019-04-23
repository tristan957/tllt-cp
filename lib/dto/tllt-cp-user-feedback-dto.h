#pragma once

#if !defined(__TLLT_CP_HEADER_INTERNAL__) && !defined(TLLT_CP_COMPILATION)
#	error "Only <tllt-cp.h> can be included directly."
#endif

#include <glib-object.h>

#include "tllt-cp-user.h"

G_BEGIN_DECLS

#define TLLT_CP_TYPE_USER_FEEDBACK_DTO (tllt_cp_user_feedback_dto_get_type())
G_DECLARE_FINAL_TYPE(TlltCpUserFeedbackDto, tllt_cp_user_feedback_dto, TLLT_CP, USER_FEEDBACK_DTO,
					 GObject)

TlltCpUserFeedbackDto *tllt_cp_user_feedback_new(const TlltCpUserFeedback);

G_END_DECLS
