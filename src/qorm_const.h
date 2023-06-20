#pragma once

static const auto driver_QMYSQL="QMYSQL";
static const auto driver_QODBC="QODBC";
static const auto driver_QOIC="QOIC";
static const auto driver_QPSQL="QPSQL";
static const auto driver_QSQLITE="QSQLITE";

#define Q_ORM_LOG true
#define Q_ORM_LOG_VERBOSE false
#define Q_ORM_LOG_SUPER_VERBOSE false

#if Q_ORM_LOG_VERBOSE
    #undef Q_ORM_LOG
    #define Q_ORM_LOG true
#endif

#if Q_ORM_LOG_SUPER_VERBOSE
    #undef Q_ORM_LOG
    #undef Q_ORM_LOG_VERBOSE

    #define Q_ORM_LOG true
    #define Q_ORM_LOG_VERBOSE true
#endif
