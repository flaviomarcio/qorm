cmake_minimum_required(VERSION 3.14)

project(QOrm LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


if(MSVC)
    add_compile_options(/W3 /WX /Zc:lambda)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

#add_subdirectory(${CMAKE_SOURCE_DIR}/src)

#if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../qstm/src/src.cmake")
  include(${CMAKE_CURRENT_SOURCE_DIR}/../qstm/src/src.cmake)
#endif()

#include(${CMAKE_CURRENT_SOURCE_DIR}/src/src.cmake)

set(QOrm_SRCS
    includes/QOrm
    src/private/p_qorm_connection_manager.cpp
    src/private/p_qorm_connection_manager.h
    src/private/p_qorm_connection_notify.cpp
    src/private/p_qorm_connection_notify.h
    src/private/p_qorm_connection_notify_subscribe.cpp
    src/private/p_qorm_connection_notify_subscribe.h
    src/private/p_qorm_const.h
    src/private/p_qorm_model_action_method.cpp
    src/private/p_qorm_model_action_method.h
    src/private/p_qorm_model_controls.cpp
    src/private/p_qorm_model_controls.h
    src/private/p_qorm_model_crud.cpp
    src/private/p_qorm_model_crud.h
    src/private/p_qorm_model_crud_body.cpp
    src/private/p_qorm_model_crud_body.h
    src/private/p_qorm_model_dao.cpp
    src/private/p_qorm_model_dao.h
    src/private/p_qorm_model_dto_endpoint.cpp
    src/private/p_qorm_model_dto_endpoint.h
    src/private/p_qorm_model_dto_endpoints.cpp
    src/private/p_qorm_model_dto_endpoints.h
    #src/private/p_qorm_model_dto_filter.h
    #src/private/p_qorm_model_dto_header.h
    src/private/p_qorm_model_dto_host.cpp
    src/private/p_qorm_model_dto_host.h
    #src/private/p_qorm_model_dto_items.h
    src/private/p_qorm_model_dto_types.h
    src/private/p_qorm_model_info.cpp
    src/private/p_qorm_model_info.h
    src/private/p_qorm_model_report.cpp
    src/private/p_qorm_model_report.h
    src/private/p_qorm_model_report_body.cpp
    src/private/p_qorm_model_report_body.h
    src/private/p_qorm_query.cpp
    src/private/p_qorm_query.h
    src/private/p_qorm_sql_suitable_builder.cpp
    src/private/p_qorm_sql_suitable_builder.h
    src/private/p_qorm_sql_suitable_parameter.cpp
    src/private/p_qorm_sql_suitable_parameter.h
    src/private/p_qorm_sql_suitable_parser_command.cpp
    src/private/p_qorm_sql_suitable_parser_command.h
    src/private/p_qorm_sql_suitable_parser_field.cpp
    src/private/p_qorm_sql_suitable_parser_field.h
    src/private/p_qorm_sql_suitable_parser_item.cpp
    src/private/p_qorm_sql_suitable_parser_item.h
    src/private/p_qorm_sql_suitable_parser_keywork.cpp
    src/private/p_qorm_sql_suitable_parser_keywork.h
    src/private/p_qorm_sql_suitable_parser_keywork_ansi.cpp
    src/private/p_qorm_sql_suitable_parser_keywork_ansi.h
    src/private/p_qorm_sql_suitable_parser_keywork_mysql.cpp
    src/private/p_qorm_sql_suitable_parser_keywork_mysql.h
    src/private/p_qorm_sql_suitable_parser_keywork_oracle.cpp
    src/private/p_qorm_sql_suitable_parser_keywork_oracle.h
    src/private/p_qorm_sql_suitable_parser_keywork_psq.cpp
    src/private/p_qorm_sql_suitable_parser_keywork_psq.h
    src/private/p_qorm_sql_suitable_parser_keywork_sqlite.cpp
    src/private/p_qorm_sql_suitable_parser_keywork_sqlite.h
    src/private/p_qorm_sql_suitable_parser_keywork_tsql.cpp
    src/private/p_qorm_sql_suitable_parser_keywork_tsql.h
    src/private/p_qorm_sql_suitable_parser_object.cpp
    src/private/p_qorm_sql_suitable_parser_object.h
    src/private/p_qorm_sql_suitable_parser_strategy.cpp
    src/private/p_qorm_sql_suitable_parser_strategy.h
    src/private/p_qorm_sql_suitable_parser_strategy_options.h
    src/private/p_qorm_sql_suitable_parser_value.cpp
    src/private/p_qorm_sql_suitable_parser_value.h
    src/private/p_qorm_task_pool.cpp
    src/private/p_qorm_task_pool.h
    src/private/p_qorm_task_slot.cpp
    src/private/p_qorm_task_slot.h
    src/qorm.h
    src/qorm_connection_manager.cpp
    src/qorm_connection_manager.h
    src/qorm_connection_notify.cpp
    src/qorm_connection_notify.h
    src/qorm_connection_pool.cpp
    src/qorm_connection_pool.h
    src/qorm_connection_setting.cpp
    src/qorm_connection_setting.h
    src/qorm_const.h
    src/qorm_controller.cpp
    src/qorm_controller.h
    src/qorm_dao.cpp
    src/qorm_dao.h
    src/qorm_global.h
    src/qorm_macro.h
    src/qorm_model.cpp
    src/qorm_model.h
    src/qorm_model_crud.h
    src/qorm_model_crud_block.cpp
    src/qorm_model_crud_block.h
#    src/qorm_model_crud_request.cpp
#    src/qorm_model_crud_request.h
    src/qorm_model_crud_types.h
    src/qorm_model_dao.h
#    src/qorm_model_descriptor.cpp
#    src/qorm_model_descriptor.h
    src/qorm_model_dto.cpp
    src/qorm_model_dto.h
    src/qorm_model_dto_options.cpp
    src/qorm_model_dto_options.h
    src/qorm_model_macro.h
    src/qorm_model_report.h
    src/qorm_object.h
    src/qorm_object_db.cpp
    src/qorm_object_db.h
    src/qorm_query.cpp
    src/qorm_query.h
    src/qorm_script_exec.cpp
    src/qorm_script_exec.h
    src/qorm_sql_suitable_builder.cpp
    src/qorm_sql_suitable_builder.h
    src/qorm_sql_suitable_types.cpp
    src/qorm_sql_suitable_types.h
    src/qorm_sql_suitable_value.cpp
    src/qorm_sql_suitable_value.h
    src/qorm_startup.cpp
    src/qorm_startup.h
    src/qorm_task_runner.cpp
    src/qorm_task_runner.h
    src/qorm_transaction.cpp
    src/qorm_transaction.h
    src/qorm_transaction_scope.cpp
    src/qorm_transaction_scope.h
    src/qorm_types.cpp
    src/qorm_types.h
    src/qorm_wrapper.cpp
    src/qorm_wrapper.h
)


if(${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    add_library(QOrm STATIC ${QOrm_SRCS})
else()
    add_library(QOrm SHARED ${QOrm_SRCS})
endif()


target_link_libraries(QOrm PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(QOrm PRIVATE Qt${QT_VERSION_MAJOR}::Sql)
target_link_libraries(QOrm PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(QOrm PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(QOrm PRIVATE QOrm_LIBRARY)

target_link_libraries(
    QOrm # cmake-format: sortable
)

target_include_directories(QOrm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(QOrm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../qstm/src/src/src.cmake")
  target_include_directories(QOrm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../qstm/src)
  target_include_directories(QOrm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../qstm/includes)
endif()

install(TARGETS QOrm
    DESTINATION ${HOME}/qtreforce.sdk/lib
)
