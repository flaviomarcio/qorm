QT += core
QT += network
QT += sql

QTREFORCE_QORM=true
DEFINES+=QTREFORCE_QORM

INCLUDEPATH += $$PWD/includes

HEADERS += \
    $$PWD/src/private/p_qorm_connection_manager.h \
    $$PWD/src/private/p_qorm_connection_notify.h \
    $$PWD/src/private/p_qorm_connection_notify_subscribe.h \
    $$PWD/src/private/p_qorm_model_action_method.h \
    $$PWD/src/private/p_qorm_const.h \
    $$PWD/src/private/p_qorm_model_controls.h \
    $$PWD/src/private/p_qorm_model_crud_body.h \
    $$PWD/src/private/p_qorm_model_report_body.h \
    $$PWD/src/private/p_qorm_model_crud.h \
    $$PWD/src/private/p_qorm_model_report.h \
    $$PWD/src/private/p_qorm_model_dao.h \
    $$PWD/src/private/p_qorm_model_dto_crud.h \
    $$PWD/src/private/p_qorm_model_dto_link_base.h \
    $$PWD/src/private/p_qorm_model_dto_link.h \
    $$PWD/src/private/p_qorm_model_dto_filter.h \
    $$PWD/src/private/p_qorm_model_dto_header.h \
    $$PWD/src/private/p_qorm_model_dto_items.h \
    $$PWD/src/private/p_qorm_model_dto_types.h \
    $$PWD/src/private/p_qorm_model_info.h \
    $$PWD/src/private/p_qorm_qt_include.h \
    $$PWD/src/private/p_qorm_query.h \
    #$$PWD/src/private/p_qorm_report_runner.h \
    $$PWD/src/private/p_qorm_sql_suitable_builder.h \
    $$PWD/src/private/p_qorm_sql_suitable_parser_command.h \
    $$PWD/src/private/p_qorm_sql_suitable_parser_field.h \
    $$PWD/src/private/p_qorm_sql_suitable_parser_item.h \
    $$PWD/src/private/p_qorm_sql_suitable_parser_keywork.h \
    $$PWD/src/private/p_qorm_sql_suitable_parser_keywork_ansi.h \
    $$PWD/src/private/p_qorm_sql_suitable_parser_keywork_mysql.h \
    $$PWD/src/private/p_qorm_sql_suitable_parser_keywork_oracle.h \
    $$PWD/src/private/p_qorm_sql_suitable_parser_keywork_psq.h \
    $$PWD/src/private/p_qorm_sql_suitable_parser_keywork_sqlite.h \
    $$PWD/src/private/p_qorm_sql_suitable_parser_keywork_tsql.h \
    $$PWD/src/private/p_qorm_sql_suitable_parser_object.h \
    $$PWD/src/private/p_qorm_sql_suitable_parser_strategy.h \
    $$PWD/src/private/p_qorm_sql_suitable_parser_strategy_options.h \
    $$PWD/src/private/p_qorm_sql_suitable_parameter.h \
    $$PWD/src/private/p_qorm_sql_suitable_parser_value.h \
    $$PWD/src/private/p_qorm_task_pool.h \
    $$PWD/src/private/p_qorm_task_slot.h \
    $$PWD/src/qorm_macro.h \
    $$PWD/src/qorm_connection_manager.h \
    $$PWD/src/qorm_connection_notify.h \
    $$PWD/src/qorm_connection_pool.h \
    $$PWD/src/qorm_connection_setting.h \
    $$PWD/src/qorm_model_dto_options.h \
    $$PWD/src/qorm.h \
    $$PWD/src/qorm_const.h \
    $$PWD/src/qorm_controller.h \
    $$PWD/src/qorm_dao.h \
    $$PWD/src/qorm_global.h \
    $$PWD/src/qorm_model_descriptor.h \
    $$PWD/src/qorm_model.h \
    $$PWD/src/qorm_model_crud_request.h \
    $$PWD/src/qorm_model_crud_block.h \
    $$PWD/src/qorm_model_crud_types.h \
    $$PWD/src/qorm_model_crud.h \
    $$PWD/src/qorm_model_report.h \
    $$PWD/src/qorm_model_dao.h \
    $$PWD/src/qorm_model_dto.h \
    $$PWD/src/qorm_object.h \
    $$PWD/src/qorm_object_db.h \
    $$PWD/src/qorm_query.h \
    $$PWD/src/qorm_script_exec.h \
    $$PWD/src/qorm_sql_suitable_builder.h \
    $$PWD/src/qorm_sql_suitable_types.h \
    $$PWD/src/qorm_sql_suitable_value.h \
    $$PWD/src/qorm_task_runner.h \
    $$PWD/src/qorm_transaction.h \
    $$PWD/src/qorm_transaction_scope.h \
    $$PWD/src/qorm_types.h \
    $$PWD/src/qorm_wrapper.h \

SOURCES += \
    $$PWD/src/private/p_qorm_connection_manager.cpp \
    $$PWD/src/private/p_qorm_connection_notify.cpp \
    $$PWD/src/private/p_qorm_connection_notify_subscribe.cpp \
    $$PWD/src/private/p_qorm_model_action_method.cpp \
    $$PWD/src/private/p_qorm_model_controls.cpp \
    $$PWD/src/private/p_qorm_model_crud_body.cpp \
    $$PWD/src/private/p_qorm_model_report_body.cpp \
    $$PWD/src/private/p_qorm_model_dto_link_base.cpp \
    $$PWD/src/private/p_qorm_model_dao.cpp \
    $$PWD/src/private/p_qorm_model_crud.cpp \
    $$PWD/src/private/p_qorm_model_report.cpp \
    $$PWD/src/private/p_qorm_model_info.cpp \
    $$PWD/src/private/p_qorm_query.cpp \
    $$PWD/src/private/p_qorm_sql_suitable_builder.cpp \
    $$PWD/src/private/p_qorm_sql_suitable_parser_command.cpp \
    $$PWD/src/private/p_qorm_sql_suitable_parser_field.cpp \
    $$PWD/src/private/p_qorm_sql_suitable_parser_item.cpp \
    $$PWD/src/private/p_qorm_sql_suitable_parser_keywork.cpp \
    $$PWD/src/private/p_qorm_sql_suitable_parser_keywork_ansi.cpp \
    $$PWD/src/private/p_qorm_sql_suitable_parser_keywork_mysql.cpp \
    $$PWD/src/private/p_qorm_sql_suitable_parser_keywork_oracle.cpp \
    $$PWD/src/private/p_qorm_sql_suitable_parser_keywork_psq.cpp \
    $$PWD/src/private/p_qorm_sql_suitable_parser_keywork_sqlite.cpp \
    $$PWD/src/private/p_qorm_sql_suitable_parser_keywork_tsql.cpp \
    $$PWD/src/private/p_qorm_sql_suitable_parser_object.cpp \
    $$PWD/src/private/p_qorm_sql_suitable_parser_strategy.cpp \
    $$PWD/src/private/p_qorm_sql_suitable_parameter.cpp \
    $$PWD/src/private/p_qorm_sql_suitable_parser_value.cpp \
    $$PWD/src/private/p_qorm_task_pool.cpp \
    $$PWD/src/private/p_qorm_task_slot.cpp \
    $$PWD/src/qorm_connection_manager.cpp \
    $$PWD/src/qorm_connection_notify.cpp \
    $$PWD/src/qorm_connection_pool.cpp \
    $$PWD/src/qorm_connection_setting.cpp \
    $$PWD/src/qorm_model_dto_options.cpp \
    $$PWD/src/qorm_controller.cpp \
    $$PWD/src/qorm_dao.cpp \
    $$PWD/src/qorm_model_descriptor.cpp \
    $$PWD/src/qorm_model_crud_request.cpp \
    $$PWD/src/qorm_model_crud_block.cpp \
    $$PWD/src/qorm_model.cpp \
    $$PWD/src/qorm_model_dto.cpp \
    $$PWD/src/qorm_object_db.cpp \
    $$PWD/src/qorm_query.cpp \
    #$$PWD/src/qorm_report_engine.cpp \
    #$$PWD/src/qorm_report_item.cpp \
    #$$PWD/src/qorm_report_runner.cpp \
    #$$PWD/src/qorm_report_strategy.cpp \
    $$PWD/src/qorm_script_exec.cpp \
    $$PWD/src/qorm_sql_suitable_builder.cpp \
    $$PWD/src/qorm_sql_suitable_types.cpp \
    $$PWD/src/qorm_sql_suitable_value.cpp \
    $$PWD/src/qorm_task_runner.cpp \
    $$PWD/src/qorm_transaction.cpp \
    $$PWD/src/qorm_transaction_scope.cpp \
    $$PWD/src/qorm_types.cpp \
    $$PWD/src/qorm_wrapper.cpp \

INCLUDEPATH+=$$PWD
