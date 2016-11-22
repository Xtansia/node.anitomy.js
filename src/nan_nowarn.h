#pragma once
#ifndef ANITOMY_LIB_NAN_NOWARN_H
#define ANITOMY_LIB_NAN_NOWARN_H

#if defined(__GNUG__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
#elif defined(_MSC_VER)
# pragma warning( push )
# pragma warning( disable : 4100 )
#endif

#include <nan.h>

#if defined(__GNUG__)
# pragma GCC diagnostic pop
#elif defined(_MSC_VER)
# pragma warning( pop )
#endif

#endif // !ANITOMY_LIB_NAN_NOWARN_H
