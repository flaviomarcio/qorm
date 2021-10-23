#pragma once

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
