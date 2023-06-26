#include "./qorm_script_exec.h"
#include "./qorm_macro.h"
#include <QFile>
#include <QMetaMethod>
#include <QMetaObject>
#include <QMetaProperty>
#include <QUrl>
#include <QSqlError>
#include <QSqlQuery>

namespace QOrm {

class ScriptExecPvt:public QObject
{
public:
    ScriptExec *parent = nullptr;
    QVariantList scriptValues;
    QStringList scriptedValues;
    explicit ScriptExecPvt(ScriptExec *parent):QObject{parent}, parent{parent} {}



    void scriptedClear() { this->scriptedValues.clear(); }

    void scriptAppend(const QVariant &v) { this->scriptValues.append(v); }

    QStringList scriptParser(const QVariant &v)
    {
        switch (v.typeId()) {
        case QMetaType::QString:
        case QMetaType::QByteArray:
        case QMetaType::QUrl: {
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

            auto lines = bytes.split(QStringLiteral("\n"));
            for (auto &v : lines) {
                auto vLines = v.split(QStringLiteral(";"));
                for (auto line : vLines) {
                    line = line.trimmed();

                    if (line.isEmpty())
                        continue;

                    if (line.startsWith(QStringLiteral("--")))
                        continue;

                    if (line.startsWith(QStringLiteral("//")))
                        continue;

                    if (line.startsWith(QStringLiteral("##")))
                        continue;

                    __return.append(line.trimmed());
                }
            }
            return __return;
        }
        case QMetaType::QStringList:
            return v.toStringList();
            break;
        default:
            return QStringList{v.toString()};
        }
        return {};
    }

    const QStringList &scriptedMaker()
    {
        if (this->scriptedValues.isEmpty()) {
            for (auto &v : this->scriptValues) {
                auto lines = this->scriptParser(v);
                for (auto &line : lines)
                    this->scriptedValues.append(line);
            }
        }
        return this->scriptedValues;
    }

    bool scriptExec(const QString &scommand)
    {
        if (scommand.isEmpty() || scommand.startsWith(QStringLiteral("--")))
            return true;

        auto db = this->parent->connection();
        QSqlQuery query(db);

        if (!query.exec(scommand))
            return false;

        query.finish();
        query.clear();

        return true;
    }

    ResultValue &scriptExec()
    {
        QVariantList __return;
        const auto &script = this->scriptedMaker();
        auto db = this->parent->connection();
        if (!db.isValid() || !db.isOpen()) {
            oWarning() << QStringLiteral("connection is not valid");
            return this->parent->lr(__return) = __return.isEmpty();
        }

        for (auto &command : script) {
            auto scommand = command.trimmed();
            QSqlQuery query(db);
            QSqlError sqlError;
            if (scommand.isEmpty() || scommand.startsWith(QStringLiteral("--")))
                continue;

            if (query.exec(command)) {
                query.finish();
                query.clear();
                continue;
            }

            sqlError = query.lastError();
            __return.append(QVariantHash{{QT_STRINGIFY(nativeErrorCode), sqlError.nativeErrorCode()}, {QT_STRINGIFY(text), sqlError.text()}, {QT_STRINGIFY(command), command}});
        }
        return this->parent->lr(__return) = __return.isEmpty();
    }
};

ScriptExec::ScriptExec(QObject *parent) : QOrm::ObjectDb{parent}, p{new ScriptExecPvt{this}}
{

}

ScriptExec &ScriptExec::operator=(const QVariant &v)
{

    p->scriptValues.clear();
    p->scriptAppend(v);
    return *this;
}

ScriptExec &ScriptExec::operator=(const QFileInfoList &v)
{
    p->scriptedClear();
    p->scriptValues.clear();
    return this->append(v);
}

ScriptExec &ScriptExec::operator<<(const QFileInfoList &v)
{
    return this->append(v);
}

ScriptExec &ScriptExec::operator<<(const QVariant &v)
{
    return this->append(v);
}

ScriptExec &ScriptExec::append(const QFileInfoList &v)
{
    for (auto &f : v)
        p->scriptAppend(QUrl::fromLocalFile(f.filePath()));
    return *this;
}

ScriptExec &ScriptExec::append(const QVariant &v)
{
    p->scriptAppend(v);
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

bool ScriptExec::exec(const QString &command)
{
    return p->scriptExec(command);
}

} // namespace QOrm
