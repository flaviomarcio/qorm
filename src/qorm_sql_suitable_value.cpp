#include "./qorm_sql_suitable_value.h"
#include "./qorm_macro.h"
#include <QMetaType>
#include <QUrl>
#include <QUuid>

Q_GLOBAL_STATIC_WITH_ARGS(QByteArray, static_null, ("null"))
//typedef QHash<int, QString> TypeVariantType;
//Q_GLOBAL_STATIC(TypeVariantType, mapFormat)

namespace QOrm {

static const QHash<QString, QString> &ChartoUtf8()
{
    static auto __return = QHash<QString, QString>(
        {{qsl("õe"), qsl("oe")}, {qsl("ão"), qsl("ao")}, {qsl("ã"), qsl("a")},
         {qsl("á"), qsl("a")},   {qsl("Á"), qsl("a")},   {qsl("ã"), qsl("a")},
         {qsl("Ã"), qsl("a")},   {qsl("â"), qsl("a")},   {qsl("Â"), qsl("a")},
         {qsl("é"), qsl("e")},   {qsl("É"), qsl("e")},   {qsl("ê"), qsl("e")},
         {qsl("Ê"), qsl("e")},   {qsl("í"), qsl("i")},   {qsl("Í"), qsl("i")},
         {qsl("õ"), qsl("o")},   {qsl("Õ"), qsl("o")},   {qsl("ô"), qsl("o")},
         {qsl("Ô"), qsl("o")},   {qsl("ó"), qsl("o")},   {qsl("Ó"), qsl("o")},
         {qsl("ú"), qsl("u")},   {qsl("Ú"), qsl("u")},   {qsl("ç"), qsl("c")},
         {qsl("Ç"), qsl("c")},   {qsl("ª"), qsl("_")},   {qsl("º"), qsl("_")},
         {qsl("°"), qsl("_")}});
    return __return;
}

static const auto &chartoUtf8 = ChartoUtf8();

class SqlSuitableValuePvt:public QObject
{
public:
    QSqlDriver::DbmsType dbType = QSqlDriver::UnknownDbms;
    QString connectionName;

public:
    explicit SqlSuitableValuePvt(QObject *parent=nullptr):QObject{parent} {}

    virtual ~SqlSuitableValuePvt() {}

    static QString parserTextLike(const QVariant &v)
    {
        auto text = v.toString().trimmed();
        Q_V_HASH_ITERATOR_STRING(chartoUtf8) { text = text.replace(i.key(), i.value()); }
        return text;
    }
};

QString SqlSuitableValue::Format::timesTamp() const
{
    return qsl("yyyy-MM-dd hh:mm:ss.zzz");
}

QString SqlSuitableValue::Format::date() const
{
    return qsl("yyyy-MM-dd");
}

QString SqlSuitableValue::Format::time() const
{
    return qsl("hh:mm:ss.zzz");
}

int SqlSuitableValue::Format::doublePrecision() const
{
    return 6;
}

int SqlSuitableValue::Format::currencyPrecision() const
{
    return 6;
}

SqlSuitableValue::SqlSuitableValue(QSqlDatabase db, QObject *parent):QObject{parent}
{
    this->p = new SqlSuitableValuePvt();
    this->setConnection(db);
}

SqlSuitableValue::SqlSuitableValue(QObject *parent):QObject{parent}
{
    this->p = new SqlSuitableValuePvt();
    this->setConnection(QSqlDatabase());
}

SqlSuitableValue::~SqlSuitableValue()
{

}

QString SqlSuitableValue::toUuid(const QVariant &v)
{
    return this->toStr(v.toUuid().toString());
}

QString SqlSuitableValue::toUuid(const QUuid &v)
{
    return this->toStr(v.toString());
}

QString SqlSuitableValue::toUrl(const QUrl &v)
{
    return this->toStr(v.toString());
}

QString SqlSuitableValue::toInt(const qlonglong &v)
{
    return QString::number(v);
}

QString SqlSuitableValue::toLng(const qlonglong &v)
{
    return QString::number(v);
}

QString SqlSuitableValue::toDbl(const double &v)
{
    return QString::number(v, 'f', this->format().doublePrecision());
}

QString SqlSuitableValue::toCur(const double &v)
{
    return QString::number(v, 'f', this->format().currencyPrecision());
}

QString SqlSuitableValue::toStr(const QString &v)
{
    if (v.isEmpty() || v.isNull() || v.startsWith(QChar('\0')))
        return qsl("''");

    auto str = QString(v).replace(qsl("'"), qsl_space).trimmed();
    return qsl("'%1'").arg(str);
}

QString SqlSuitableValue::toStr(const QByteArray &v)
{
    return QString(v).replace(qsl("'"), qsl_space);
}

QString SqlSuitableValue::toStrPure(const QString &v)
{
    return QString(v).replace(qsl("'"), qsl_space);
}

QString SqlSuitableValue::toDat(const QDateTime &v)
{
    auto dt = (v.date() >= QDate(1900, 01, 01)) ? v.date() : QDate(1900, 01, 01);
    return this->toStr(dt.toString(this->format().timesTamp()));
}

QString SqlSuitableValue::toDatMin(const QDateTime &v)
{
    return this->toDat(QDate(v.date().year(), v.date().month(), 1));
}

QString SqlSuitableValue::toDatMax(const QDateTime &v)
{
    return this->toDat(QDateTime(v.date(), QTime(23, 59, 59, 998)));
}

QString SqlSuitableValue::toDat(const QVariant &v)
{
    return this->toDat(v.toDate());
}

QString SqlSuitableValue::toDat(const QDate &v)
{
    auto dt = (v >= QDate(1900, 01, 01)) ? v : QDate(1900, 01, 01);
    return this->toStr(dt.toString(this->format().date()));
}

QString SqlSuitableValue::toTim(const QTime &v)
{
    return SqlSuitableValue::toStr(v.toString(this->format().time()));
}

QString SqlSuitableValue::toTim(const QDateTime &v)
{
    return SqlSuitableValue::toStr(v.time().toString(this->format().time()));
}

QString SqlSuitableValue::toBoo(const bool &v)
{
    auto db = this->connection();
    switch (db.driver()->dbmsType()) {
    case QSqlDriver::MSSqlServer:
    case QSqlDriver::SQLite:
        return this->toInt(v ? 1 : 0);
    default:
        return v ? qsl("true") : qsl("false");
    }
}

QString SqlSuitableValue::toVar(const QVariant &v)
{
    return this->toVar(v, qTypeId(v));
}

QString SqlSuitableValue::toVar(const QVariant &v, const int &vType)
{
    switch (vType) {
    case QMetaType_Int:
        return SqlSuitableValue::toInt(v.toInt());
    case QMetaType_UInt:
        return SqlSuitableValue::toInt(v.toInt());
    case QMetaType_LongLong:
        return SqlSuitableValue::toLng(v.toLongLong());
    case QMetaType_ULongLong:
        return SqlSuitableValue::toLng(v.toLongLong());
    case QMetaType_Double:
        return SqlSuitableValue::toDbl(v.toDouble());
    case QMetaType_Bool:
        return SqlSuitableValue::toBoo(v.toBool());
    case QMetaType_QString:
        return toStr(v.toString());
    case QMetaType_QByteArray:
        return toStr(v.toString());
    case QMetaType_QBitArray:
        return toStr(v.toString());
    case QMetaType_QChar:
        return toStr(v.toString());
    case QMetaType_QDate:
        return v.toDate().isValid() ? this->toDat(v.toDate()) : *static_null;
    case QMetaType_QTime:
        return v.toTime().isValid() ? this->toTim(v.toTime()) : *static_null;
    case QMetaType_QDateTime:
        return v.toDateTime().isValid() ? this->toDat(v.toDateTime()) : *static_null;
    case QMetaType_QUrl:
        return toStr(v.toUrl().toString());
    case QMetaType_QUuid:
        return v.toUuid().isNull() ? *static_null : toStr(v.toUuid().toByteArray());
    default:
        return *static_null;
    }
}

QString SqlSuitableValue::toLikeLR(const QVariant &v)
{
    return qsl("%") + SqlSuitableValuePvt::parserTextLike(v) + qsl("%");
}

QString SqlSuitableValue::toLikeL(const QVariant &v)
{
    return qsl("%") + SqlSuitableValuePvt::parserTextLike(v);
}

QString SqlSuitableValue::toLikeR(const QVariant &v)
{
    return SqlSuitableValuePvt::parserTextLike(v) + qsl("%");
}

QSqlDatabase SqlSuitableValue::connection()
{

    return QSqlDatabase::database(p->connectionName);
}

SqlSuitableValue &SqlSuitableValue::setConnection(const QSqlDatabase &db)
{

    p->connectionName = db.connectionName();
    return *this;
}

SqlSuitableValue::Format &SqlSuitableValue::format()
{
    static Format _format;
    return _format;
}

} // namespace QOrm
