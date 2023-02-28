#pragma once

#if !defined(QFRAMELESS_EXPORT) && !defined(QFRAMELESS_IMPORT)
#define QFRAMELESS_EXPORT
#elif defined(QFRAMELESS_IMPORT)
#if defined(QFRAMELESS_EXPORT)
#undef QFRAMELESS_EXPORT
#endif
#define QFRAMELESS_EXPORT __declspec(dllimport)
#elif defined(QFRAMELESS_EXPORT)
#undef QFRAMELESS_EXPORT
#define QFRAMELESS_EXPORT __declspec(dllexport)
#endif