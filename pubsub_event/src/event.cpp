#if defined(__cpp_inline_variables) && (__cpp_inline_variables >= 201606L)
#else
#include "event.h"

detail::BaseEvent::Family detail::BaseEvent::family_counter_ = 0;
#endif