set(qorm_SRCS
    # cmake-format: sortable
    private/p_qorm_connection_manager.h
    private/p_qorm_connection_notify.h
    private/p_qorm_connection_notify_subscribe.h
    private/p_qorm_const.h
    private/p_qorm_model_action_method.cpp
    private/p_qorm_model_controls.cpp
    private/p_qorm_model_crud.cpp
    private/p_qorm_model_crud_body.cpp
    private/p_qorm_model_dao.cpp
    private/p_qorm_model_dto_crud.h
    private/p_qorm_model_dto_filter.h
    private/p_qorm_model_dto_header.h
    private/p_qorm_model_dto_items.h
    private/p_qorm_model_dto_link.h
    private/p_qorm_model_dto_link_base.cpp
    private/p_qorm_model_dto_types.h
    private/p_qorm_model_info.cpp
    private/p_qorm_model_report.cpp
    private/p_qorm_model_report_body.cpp
    private/p_qorm_qt_include.h
    private/p_qorm_query.h
    private/p_qorm_report_runner.h
    private/p_qorm_sql_suitable_builder.h
    private/p_qorm_sql_suitable_parameter.cpp
    private/p_qorm_sql_suitable_parser_keywork.cpp
    private/p_qorm_sql_suitable_parser_keywork_ansi.h
    private/p_qorm_sql_suitable_parser_keywork_mysql.h
    private/p_qorm_sql_suitable_parser_keywork_oracle.h
    private/p_qorm_sql_suitable_parser_keywork_psq.h
    private/p_qorm_sql_suitable_parser_keywork_sqlite.h
    private/p_qorm_sql_suitable_parser_keywork_tsql.h
    private/p_qorm_sql_suitable_parser_strategy.cpp
    private/p_qorm_sql_suitable_parser_strategy_options.h
    private/p_qorm_task_pool.h
    private/p_qorm_task_slot.cpp
    qorm_connection_manager.cpp
    qorm_connection_notify.cpp
    qorm_connection_pool.cpp
    qorm_connection_setting.cpp
    qorm_const.h
    qorm_controller.cpp
    qorm_dao.cpp
    qorm_global.h
    qorm_macro.h
    qorm_model.cpp
    qorm_model_crud.h
    qorm_model_crud_block.cpp
    qorm_model_crud_request.cpp
    qorm_model_crud_types.h
    qorm_model_dao.h
    qorm_model_descriptor.cpp
    qorm_model_dto.cpp
    qorm_model_dto_options.cpp
    qorm_model_report.h
    qorm_object.h
    qorm_object_db.cpp
    qorm_query.cpp
    qorm_report_engine.cpp
    qorm_report_item.cpp
    qorm_report_runner.cpp
    qorm_report_strategy.cpp
    qorm_script_exec.cpp
    qorm_sql_suitable_builder.cpp
    qorm_sql_suitable_types.cpp
    qorm_sql_suitable_value.cpp
    qorm_task_runner.cpp
    qorm_transaction.cpp
    qorm_transaction_scope.cpp
    qorm_types.h
    qorm_wrapper.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/src)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    add_library(qorm STATIC ${qorm_SRCS})
else()
    add_library(qorm SHARED ${qorm_SRCS})
endif()

target_link_libraries(
    qorm # cmake-format: sortable
)

install(TARGETS qorm
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/qt/plugins/qorm
)
