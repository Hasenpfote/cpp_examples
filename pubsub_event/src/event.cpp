#if (__cplusplus > 201402L) || (defined(_MSC_VER) && (_MSVC_LANG > 201402L))
#else
#include "event.h"

detail::BaseEvent::Family detail::BaseEvent::family_counter_ = 0;
#endif