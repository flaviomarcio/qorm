#pragma once

#include <QObject>
#include "./qorm_const.h"
#include "./qorm_macro.h"

#ifndef QT_STATIC
#  if defined(QT_BUILD_SQL_LIB)
#    define Q_ORM_EXPORT Q_DECL_EXPORT
#  else
#    define Q_ORM_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_ORM_EXPORT
#endif
