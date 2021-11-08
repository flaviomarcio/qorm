#include "./qorm_script_exec.h"
#include <QMetaProperty>
#include <QMetaMethod>
#include <QMetaObject>
#include <QFile>
#include <QUrl>
#include <QSqlQuery>
#include <QSqlError>

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ScriptExecPvt*>(this->p)\

class ScriptExecPvt{
public:
    QOrm::ObjectDb*parent=nullptr;
    QVariantList scriptValues;
    QStringList scriptedValues;
    explicit ScriptExecPvt(QOrm::ObjectDb*parent){
        this->parent=parent;
    }

    virtual ~ScriptExecPvt(){
    }

    void scriptedClear(){
        this->scriptedValues.clear();
    }

    void scriptAppend(const QVariant&v){
        this->scriptedClear();
        this->scriptValues<<v;
    }

    QStringList scriptParser(const QVariant&v){
        QStringList __return;
        if(v.typeId()==QMetaType::QString || v.typeId()==QMetaType::QByteArray || v.typeId()==QMetaType::QUrl){
            auto url=v.typeId()==QMetaType::QUrl?v.toUrl():QUrl::fromLocalFile(v.toString());
            if(url.isLocalFile()){
                QFile file(url.toLocalFile());
                if(file.exists() && file.open(file.ReadOnly)){
                    QString bytes=file.readAll().trimmed();
                    file.close();
                    if(!bytes.isEmpty()){
                        auto lines=bytes.split(qsl("\n"));
                        for(auto&v:lines){
                            auto vLines=v.split(qsl(";"));
                            for(auto line:vLines){
                                line=line.trimmed();
                                if(line.isEmpty())
                                    continue;
                                else
                                    __return<<line.trimmed();
                            }
                        }
                    }
                }
            }
            else if(v.typeId()==QMetaType::QStringList){
                __return=v.toStringList();
            }
            else{
                __return<<v.toString();
            }
        }
        return __return;
    }

    const QStringList&scriptedMaker(){
        if(this->scriptedValues.isEmpty()){
            for(auto&v:this->scriptValues){
                auto lines=this->scriptParser(v);
                for(auto&line:lines)
                    this->scriptedValues<<line;
            }
        }
        return this->scriptedValues;
    }


    ResultValue&scriptExec(){
        QVariantList __return;
        const auto&script=this->scriptedMaker();
        auto db=this->parent->connection();
        if(!db.isValid() || !db.isOpen()){
            sWarning()<<qsl("connection is not valid");
        }
        else{
            for(auto&command:script){
                auto scommand=command.trimmed();
                QSqlQuery query(db);
                QSqlError sqlError;
                if(scommand.isEmpty() || scommand.startsWith(qsl("--")))
                    continue;
                else if(!query.exec(command))
                    sqlError=query.lastError();
                else{
                    query.finish();
                    query.clear();
                    continue;
                }
                __return<<qvh({{qsl_fy(nativeErrorCode), sqlError.nativeErrorCode()}, {qsl_fy(text), sqlError.text()}, {qsl_fy(command), command}});
            }
        }
        return this->parent->lr(__return)=__return.isEmpty();
    }
};

ScriptExec::ScriptExec(QObject *parent):QOrm::ObjectDb(parent)
{
    this->p = new ScriptExecPvt(this);
}

ScriptExec::~ScriptExec()
{
}

ScriptExec &ScriptExec::operator=(const QVariant &v)
{
    dPvt();
    p.scriptValues.clear();
    p.scriptAppend(v);
    return*this;
}

ScriptExec &ScriptExec::operator=(const QFileInfoList &entryInfoList)
{
    dPvt();
    p.scriptValues.clear();
    for(auto&f:entryInfoList){
        p.scriptAppend(QUrl::fromLocalFile(f.filePath()));
    }
    return*this;
}

ScriptExec &ScriptExec::operator<<(const QVariant &v)
{
    dPvt();
    p.scriptAppend(v);
    return*this;
}

ScriptExec &ScriptExec::operator<<(const QFileInfoList &entryInfoList)
{
    dPvt();
    for(auto&f:entryInfoList){
        p.scriptAppend(QUrl::fromLocalFile(f.filePath()));
    }
    return*this;
}

QVariantList ScriptExec::scriptValues() const
{
    dPvt();
    return p.scriptValues;
}

const QStringList &ScriptExec::scriptedValues()
{
    dPvt();
    return p.scriptedMaker();
}

ResultValue &ScriptExec::exec()
{
    dPvt();
    p.scriptedClear();
    return p.scriptExec();
}

}
