#include "./qorm_sql_suitable_value.h"
#include "../../qstm/src/qstm_macro.h"
#include <QMetaType>
#include <QUrl>
#include <QUuid>
#include <QSqlDriver>
#include <QVariant>
#include <QDateTime>

Q_GLOBAL_STATIC_WITH_ARGS(QByteArray, static_null, ("null"))
//typedef QHash<int, QString> TypeVariantType;
//Q_GLOBAL_STATIC(TypeVariantType, mapFormat)

namespace QOrm {

static const QHash<QString, QString> &ChartoUtf8()
{
    static auto __return = QHash<QString, QString>(
        {{QStringLiteral("õe"), QStringLiteral("oe")}, {QStringLiteral("ão"), QStringLiteral("ao")}, {QStringLiteral("ã"), QStringLiteral("a")},
         {QStringLiteral("á"), QStringLiteral("a")},   {QStringLiteral("Á"), QStringLiteral("a")},   {QStringLiteral("ã"), QStringLiteral("a")},
         {QStringLiteral("Ã"), QStringLiteral("a")},   {QStringLiteral("â"), QStringLiteral("a")},   {QStringLiteral("Â"), QStringLiteral("a")},
         {QStringLiteral("é"), QStringLiteral("e")},   {QStringLiteral("É"), QStringLiteral("e")},   {QStringLiteral("ê"), QStringLiteral("e")},
         {QStringLiteral("Ê"), QStringLiteral("e")},   {QStringLiteral("í"), QStringLiteral("i")},   {QStringLiteral("Í"), QStringLiteral("i")},
         {QStringLiteral("õ"), QStringLiteral("o")},   {QStringLiteral("Õ"), QStringLiteral("o")},   {QStringLiteral("ô"), QStringLiteral("o")},
         {QStringLiteral("Ô"), QStringLiteral("o")},   {QStringLiteral("ó"), QStringLiteral("o")},   {QStringLiteral("Ó"), QStringLiteral("o")},
         {QStringLiteral("ú"), QStringLiteral("u")},   {QStringLiteral("Ú"), QStringLiteral("u")},   {QStringLiteral("ç"), QStringLiteral("c")},
         {QStringLiteral("Ç"), QStringLiteral("c")},   {QStringLiteral("ª"), QStringLiteral("_")},   {QStringLiteral("º"), QStringLiteral("_")},
         {QStringLiteral("°"), QStringLiteral("_")}});
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
    return QStringLiteral("yyyy-MM-dd hh:mm:ss.zzz");
}

QString SqlSuitableValue::Format::date() const
{
    return QStringLiteral("yyyy-MM-dd");
}

QString SqlSuitableValue::Format::time() const
{
    return QStringLiteral("hh:mm:ss.zzz");
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
    this->p = new SqlSuitableValuePvt(this);
    this->setConnection(db);
}

SqlSuitableValue::SqlSuitableValue(QObject *parent):QObject{parent}
{
    this->p = new SqlSuitableValuePvt(this);
    this->setConnection({});
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
        return QStringLiteral("''");

    auto str = QString(v).replace(QStringLiteral("'"), QStringLiteral(" ").trimmed());
    return QStringLiteral("'%1'").arg(str);
}

QString SqlSuitableValue::toStr(const QByteArray &v)
{
    return QString(v).replace(QStringLiteral("'"), QStringLiteral(" "));
}

QString SqlSuitableValue::toStrPure(const QString &v)
{
    return QString(v).replace(QStringLiteral("'"), QStringLiteral(" "));
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
        return v ? QStringLiteral("true") : QStringLiteral("false");
    }
}

QString SqlSuitableValue::toVar(const QVariant &v)
{
    return this->toVar(v, v.typeId());
}

QString SqlSuitableValue::toVar(const QVariant &v, const int &vType)
{
    switch (vType) {
    case QMetaType::Int:
        return SqlSuitableValue::toInt(v.toInt());
    case QMetaType::UInt:
        return SqlSuitableValue::toInt(v.toInt());
    case QMetaType::LongLong:
        return SqlSuitableValue::toLng(v.toLongLong());
    case QMetaType::ULongLong:
        return SqlSuitableValue::toLng(v.toLongLong());
    case QMetaType::Double:
        return SqlSuitableValue::toDbl(v.toDouble());
    case QMetaType::Bool:
        return SqlSuitableValue::toBoo(v.toBool());
    case QMetaType::QString:
        return toStr(v.toString());
    case QMetaType::QByteArray:
        return toStr(v.toString());
    case QMetaType::QBitArray:
        return toStr(v.toString());
    case QMetaType::QChar:
        return toStr(v.toString());
    case QMetaType::QDate:
        return v.toDate().isValid() ? this->toDat(v.toDate()) : *static_null;
    case QMetaType::QTime:
        return v.toTime().isValid() ? this->toTim(v.toTime()) : *static_null;
    case QMetaType::QDateTime:
        return v.toDateTime().isValid() ? this->toDat(v.toDateTime()) : *static_null;
    case QMetaType::QUrl:
        return toStr(v.toUrl().toString());
    case QMetaType::QUuid:
        return v.toUuid().isNull() ? *static_null : toStr(v.toUuid().toByteArray());
    default:
        return *static_null;
    }
}

QString SqlSuitableValue::toLikeLR(const QVariant &v)
{
    return QStringLiteral("%") + SqlSuitableValuePvt::parserTextLike(v) + QStringLiteral("%");
}

QString SqlSuitableValue::toLikeL(const QVariant &v)
{
    return QStringLiteral("%") + SqlSuitableValuePvt::parserTextLike(v);
}

QString SqlSuitableValue::toLikeR(const QVariant &v)
{
    return SqlSuitableValuePvt::parserTextLike(v) + QStringLiteral("%");
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
