#include "p_qorm_sql_suitable_parser_command.h"
#include <QCoreApplication>

namespace QOrm {

static qlonglong __sequence_zzzz=0;

static void init()
{
    __sequence_zzzz=0;
}

Q_COREAPP_STARTUP_FUNCTION(init)

SqlParserCommand::SqlParserCommand(const QVariant &v):QVariant{v}
{
}

SqlParserCommand::SqlParserCommand():QVariant{}
{
}

SqlParserCommand::~SqlParserCommand()
{
    this->___clear();
}

qlonglong SqlParserCommand::sequence_zzzz()
{
    return ++__sequence_zzzz;
}

bool SqlParserCommand::ignorePrepare()
{
    return false;
}

QVariantMap SqlParserCommand::appendMapStartsWith(const QVariant &vKey, const QVariantMap &mapSrc)
{
    QVariantMap mapDst;
    QStringList lKey=qTypeId(vKey)==QMetaType_QStringList?vKey.toStringList():QStringList{vKey.toString()};
    QMapIterator<QString, QVariant> i(mapSrc);
    while (i.hasNext()) {
        i.next();
        auto key=i.key().toLower();
        auto&value=i.value();
        for(auto&s:lKey){
            if(key.startsWith(s)){
                mapDst.insert(key, value);
            }
        }
    }
    return mapDst;
}

QVariantMap SqlParserCommand::unionMapStartsWith(const QVariant &vKey, const QVariantMap &mapSrc)
{
    QStringList lKey=qTypeId(vKey)==QMetaType_QStringList?vKey.toStringList():QStringList{vKey.toString()};
    QVariantMap mapDst;
    QVariantMap mapUni;
    {
        QMapIterator<QString, QVariant> i(mapSrc);
        while (i.hasNext()) {
            i.next();
            auto key=i.key().toLower();
            auto&value=i.value();
            for(auto&s:lKey){
                if(key.startsWith(s)){
                    mapUni.insert(key, value);
                }
            }
        }
    }

    {
        QMapIterator<QString, QVariant> i(mapUni);
        while (i.hasNext()) {
            i.next();
            QMapIterator<QString, QVariant> ii(i.value().toMap());
            while (ii.hasNext()) {
                ii.next();
                mapDst.insert(ii.key(), ii.value());
            }
        }
    }
    return mapDst;
}

QVariant SqlParserCommand::getVariantStartsWith(const QVariant &vKey, const QVariantMap &mapSrc)
{
    QStringList lKey=qTypeId(vKey)==QMetaType_QStringList?vKey.toStringList():QStringList{vKey.toString()};
    QMapIterator<QString, QVariant> i(mapSrc);
    while (i.hasNext()) {
        i.next();
        auto __key=i.key().trimmed();
        for(auto&s:lKey){
            if(__key.startsWith(s)){
                return i.value();
            }
        }

    }
    return {};
}

QString SqlParserCommand::suuid(const QString &delim) const
{
    return _____zzzzz_uuid==0?qsl_null:delim+QString::number(this->_____zzzzz_uuid).rightJustified(11,'0');
}

QVariant SqlParserCommand::makeUuid()
{
    this->_____zzzzz_uuid=this->sequence_zzzz();
    return this->_____zzzzz_uuid;
}

QStringList SqlParserCommand::toScript(SqlSuitableKeyWord &parser)
{
    Q_UNUSED(parser)
    return {};
}

void SqlParserCommand::setPropertys()
{
}

void SqlParserCommand::clear(){
    this->___clear();
}

QVariant SqlParserCommand::vMap(const QString &key) const
{
    return this->toMap().value(key);
}

void SqlParserCommand::insert(const QString &key, const QVariant &v)
{
    auto map=this->toMap();
    map.insert(key, v);
    this->setValue(map);
}

QMap<QString, QOrm::SqlParserCommand *> SqlParserCommand::mPointer() const
{
    return this->mapPointer;
}

void SqlParserCommand::setPointer(const QString &key, SqlParserCommand *p)
{
    auto pp=this->mapPointer.value(key);
    if(pp!=p && pp!=nullptr){
        this->mapPointer.remove(key);
        ::operator delete(pp);
        pp=nullptr;
    }
    if(p!=nullptr)
        this->mapPointer.insert(key, p);
}

bool SqlParserCommand::isChanged() const
{
    return !this->mapPointer.isEmpty();
}

void SqlParserCommand::m(){
    this->make();
}

void SqlParserCommand::m(const QVariant &v)
{
    this->make(v);
}

void SqlParserCommand::make()
{
    this->make(QVariant());
}

void SqlParserCommand::make(const QVariant &)
{
}

bool SqlParserCommand::makeObject()
{
    if(this->mapPointer.isEmpty())
        return false;
    bool __return=false;
    Q_DECLARE_VU;
    auto vThis=this->toMap();
    QMapIterator<QString, SqlParserCommand*> i(this->mapPointer);
    int seq=0;
    while (i.hasNext()) {
        i.next();
        auto key=qsl("%1.%2").arg(i.key()).arg(++seq);

        if(i.value()==nullptr)
            continue;

        auto v=i.value();
        if(v==nullptr)
            continue;

        if(v!=this)
            v->makeObject();
        auto typeId=qTypeId(*v);
        if(QMetaTypeUtilVariantDictionary.contains(typeId)){
            auto map=v->toHash();
            vThis.insert(key, map);
            __return=true;
        }

        if(typeId==QMetaType_User){
            vThis.insert(key, qv(*v));
            __return=true;
        }
    }
    this->setValue(vThis);
    return __return;
}

void SqlParserCommand::___clear()
{
    auto values=this->mapPointer.values();
    for(auto&v:values){
        v->setValue(QVariant());
        delete v;
    }
    this->mapPointer.clear();
    this->setValue(qv_null);
}

}



