#include "./qorm_script_exec.h"
#include <QFile>
#include <QMetaMethod>
#include <QMetaObject>
#include <QMetaProperty>
#include <QUrl>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

namespace QOrm {

class ScriptExecPvt:public QObject
{
public:
    QOrm::ObjectDb *parent = nullptr;
    QVariantList scriptValues;
    QStringList scriptedValues;
    explicit ScriptExecPvt(QOrm::ObjectDb *parent):QObject{parent} { this->parent = parent; }

    virtual ~ScriptExecPvt() {}

    void scriptedClear() { this->scriptedValues.clear(); }

    void scriptAppend(const QVariant &v) { this->scriptValues << v; }

    QStringList scriptParser(const QVariant &v)
    {
        switch (qTypeId(v)) {
        case QMetaType_QString:
        case QMetaType_QByteArray:
        case QMetaType_QUrl: {
            QStringList __return;
            QFile file(v.toString());

            if (!file.exists())
                return {};

            if (!file.open(file.ReadOnly))
                return {};

            QString bytes = file.readAll().trimmed();
            file.close();
            if (bytes.isEmpty())
                return {};

            auto lines = bytes.split(qsl("\n"));
            for (auto &v : lines) {
                auto vLines = v.split(qsl(";"));
                for (auto line : vLines) {
                    line = line.trimmed();

                    if (line.isEmpty())
                        continue;

                    if (line.startsWith(qsl("--")))
                        continue;

                    if (line.startsWith(qsl("//")))
                        continue;

                    if (line.startsWith(qsl("##")))
                        continue;

                    __return << line.trimmed();
                }
            }
            return __return;
        }
        case QMetaType_QStringList:
            return v.toStringList();
            break;
        default:
            return qvsl{v.toString()};
        }
        return {};
    }

    const QStringList &scriptedMaker()
    {
        if (this->scriptedValues.isEmpty()) {
            for (auto &v : this->scriptValues) {
                auto lines = this->scriptParser(v);
                for (auto &line : lines)
                    this->scriptedValues << line;
            }
        }
        return this->scriptedValues;
    }

    ResultValue &scriptExec()
    {
        QVariantList __return;
        const auto &script = this->scriptedMaker();
        auto db = this->parent->connection();
        if (!db.isValid() || !db.isOpen()) {
            sWarning() << qsl("connection is not valid");
            return this->parent->lr(__return) = __return.isEmpty();
        }

        for (auto &command : script) {
            auto scommand = command.trimmed();
            QSqlQuery query(db);
            QSqlError sqlError;
            if (scommand.isEmpty() || scommand.startsWith(qsl("--")))
                continue;

            if (query.exec(command)) {
                query.finish();
                query.clear();
                continue;
            }

            sqlError = query.lastError();
            __return << qvh{{qsl_fy(nativeErrorCode), sqlError.nativeErrorCode()},
                            {qsl_fy(text), sqlError.text()},
                            {qsl_fy(command), command}};
        }
        return this->parent->lr(__return) = __return.isEmpty();
    }
};

ScriptExec::ScriptExec(QObject *parent) : QOrm::ObjectDb{parent}
{
    this->p = new ScriptExecPvt{this};
}

ScriptExec::~ScriptExec() {}

ScriptExec &ScriptExec::operator=(const QVariant &v)
{

    p->scriptValues.clear();
    p->scriptAppend(v);
    return *this;
}

ScriptExec &ScriptExec::operator=(const QFileInfoList &entryInfoList)
{

    p->scriptedClear();
    p->scriptValues.clear();
    for (auto &f : entryInfoList) {
        p->scriptAppend(QUrl::fromLocalFile(f.filePath()));
    }
    return *this;
}

ScriptExec &ScriptExec::operator<<(const QVariant &v)
{

    p->scriptAppend(v);
    return *this;
}

ScriptExec &ScriptExec::operator<<(const QFileInfoList &entryInfoList)
{

    for (auto &f : entryInfoList) {
        auto fileName = f.filePath();
        if (!QFile::exists(fileName)) {
            sWarning() << tr("Invalid filename: %1").arg(fileName);
            continue;
        }

        p->scriptAppend(fileName);
    }
    return *this;
}

QVariantList ScriptExec::scriptValues() const
{

    return p->scriptValues;
}

const QStringList &ScriptExec::scriptedValues()
{

    return p->scriptedMaker();
}

ResultValue &ScriptExec::exec()
{

    p->scriptedClear();
    return p->scriptExec();
}

} // namespace QOrm
