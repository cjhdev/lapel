/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_DEBUG_H
#define LAPEL_DEBUG_H

#include "lapel_config.h"

#ifndef LAPEL_DEBUG
#   define LAPEL_DEBUG(...)
#endif

#ifndef LAPEL_ERROR
#   define LAPEL_ERROR(...)
#endif

#ifndef LAPEL_ASSERT
#   define LAPEL_ASSERT(XX)
#endif

#endif
