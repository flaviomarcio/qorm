#pragma once

#include <QJsonDocument>
#include <QMetaType>
#include <QUuid>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QDateTime>
#include <QList>
#include <QVariantList>
#include <QVariantMap>
#include "../../../qstm/src/qstm_types.h"
#include "./p_qorm_sql_suitable_parser_keywork.h"
#include "../qorm_sql_suitable_types.h"
#include "./p_qorm_model_info.h"

namespace QOrm {

class SqlParserCommand;
typedef QMap<QString, SqlParserCommand*> SqlParserPointerMap;

class SqlSuitableKeyWord;

class Q_ORM_EXPORT SqlParserCommand:public QVariant{
    friend class SqlParserItem;
public:
    explicit SqlParserCommand(const QVariant &v);

    explicit SqlParserCommand();

    virtual ~SqlParserCommand();

    static qlonglong sequence_zzzz();

    virtual bool ignorePrepare();

    QVariantMap appendMapStartsWith(const QVariant &vKey, const QVariantMap&mapSrc);

    QVariantMap unionMapStartsWith(const QVariant &vKey, const QVariantMap&mapSrc);

    QVariant getVariantStartsWith(const QVariant &vKey, const QVariantMap&mapSrc);

    QString suuid(const QString &delim=QString())const;

    QVariant makeUuid();

    virtual QStringList toScript(SqlSuitableKeyWord&parser);

    virtual void setPropertys();

    virtual void clear();

    virtual QVariant vMap(const QString &key)const;

    void insert(const QString &key, const QVariant &v);


    template<class TObject=QVariant>
    TObject oPointer(const QString &name)
    {
        auto r=static_cast<TObject>(mapPointer.value(name));
        return r;
    }

    virtual QMap<QString,SqlParserCommand*> mPointer()const;

    virtual void setPointer(const QString &key, SqlParserCommand*p);

    virtual bool isChanged()const;

    virtual void m();

    virtual void m(const QVariant &v);

    virtual void make();

    virtual void make(const QVariant &);

    virtual bool makeObject();

private:
    SqlParserPointerMap mapPointer;
    qlonglong _____zzzzz_uuid=0;
    void ___clear();
};

}

Q_DECLARE_METATYPE(QOrm::SqlParserCommand)
Q_DECLARE_METATYPE(QOrm::SqlParserCommand*)
