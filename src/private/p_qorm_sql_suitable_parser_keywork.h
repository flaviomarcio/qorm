#pragma once

#include "../qorm_object_db.h"
#include "../qorm_sql_suitable_types.h"

namespace QOrm {

    typedef QHash<int, QByteArray> KeywordCache;
    class ModelInfo;
    class Q_ORM_EXPORT SqlSuitableKeyWord{
    public:

        /**
         * @brief SqlSuitableKeyWord
         * @param parent
         */
        Q_INVOKABLE explicit SqlSuitableKeyWord(QObject *parent = nullptr);
        Q_INVOKABLE explicit SqlSuitableKeyWord(QSqlDriver::DbmsType driver, QObject *parent = nullptr);
        Q_INVOKABLE explicit SqlSuitableKeyWord(QList<QSqlDriver::DbmsType> drivers, QObject *parent = nullptr);
        Q_INVOKABLE virtual ~SqlSuitableKeyWord();

        SqlSuitableKeyWord&operator=(const SqlSuitableKeyWord&o);

        /**
         * @brief isValid
         * @return
         */
        virtual bool isValid();

        /**
         * @brief parserCommand
         * @param command
         * @return
         */
        virtual QString parserCommand(int command);
        virtual QStringList parserCommand(int command, const ModelInfo *modelInfo, const QVariant&value);

        /**
         * @brief commands
         * @return
         */
        virtual KeywordCache&commands();

        /**
         * @brief parser
         * @param v
         * @param scrit
         * @return
         */
        virtual bool parse(const QVariant &v, QByteArray &script);

        /**
         * @brief parser
         * @param driver
         * @return
         */
        static SqlSuitableKeyWord&parser(const QSqlDatabase&db);
        static SqlSuitableKeyWord&parser(const QSqlDriver*driver);
        static SqlSuitableKeyWord&parser(const QSqlDriver::DbmsType&driver);

        /**
         * @brief parserMaker
         * @param parser
         * @return
         */
        static SqlSuitableKeyWord *parserMaker(SqlSuitableKeyWord *parser);

        /**
         * @brief drivers
         * @return
         */
        virtual QList<QSqlDriver::DbmsType> &drivers() const;
        virtual void setDriver(QSqlDriver::DbmsType value);
        virtual void setDrivers(QList<QSqlDriver::DbmsType> value);

        /**
         * @brief formatValue
         * @param v
         * @return
         */
        virtual QString formatValue(const QVariant&v);

        /**
         * @brief formatParameter
         * @param v
         * @return
         */
        virtual QString formatParameter(const QVariant&v);

        /**
         * @brief formatValues
         * @param v
         * @return
         */
        virtual QStringList formatValues(const QStringList &field, const QVariant &values);
        virtual QStringList formatValues(const QVariant &value);

        /**
         * @brief formatValuesSet
         * @param field
         * @param values
         * @return
         */
        virtual QStringList formatValuesSet(const QStringList &field, const QVariant &value);

        /**
         * @brief parserCallProcedure
         * @param value
         * @return
         */
        virtual QStringList parserCallProcedure(const QVariant &value);

        /**
         * @brief parserCallFunction
         * @param value
         * @return
         */
        virtual QStringList parserCallFunction(const QVariant &value);

        /**
         * @brief parserCallFunctionTable
         * @param value
         * @return
         */
        virtual QStringList parserCallFunctionTable(const QVariant &value);

        /**
         * @brief init
         */
        virtual void init();

    private:
        void*p=nullptr;
    };

}
