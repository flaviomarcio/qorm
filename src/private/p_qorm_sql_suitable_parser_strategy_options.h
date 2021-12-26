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
#include "../qorm_sql_suitable_types.h"
#include "./p_qorm_model_info.h"
#include "./p_qorm_sql_suitable_parser_keywork.h"
#include "./p_qorm_sql_suitable_parameter.h"

namespace QOrm {

static qlonglong sequence_zzzz=1;
class SqlParserCommand;
typedef QMap<QString, SqlParserCommand*> SqlParserPointerMap;

class SqlSuitableKeyWord;
class Q_ORM_EXPORT SqlParserCommand:public QVariant{
    friend class SqlParserItem;
public:
    explicit SqlParserCommand(const QVariant&v):QVariant(v){

    }

    explicit SqlParserCommand():QVariant(){
    }

    virtual ~SqlParserCommand(){
        this->___clear();
    }

    virtual bool ignorePrepare(){
        return false;
    }

    QVariantMap appendMapStartsWith(const QVariant&vKey, const QVariantMap&mapSrc){
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

    QVariantMap unionMapStartsWith(const QVariant&vKey, const QVariantMap&mapSrc){

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

    QVariant getVariantStartsWith(const QVariant&vKey, const QVariantMap&mapSrc){
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


    QString suuid(const QString&delim=QString())const{
        return _____zzzzz_uuid==0?qsl_null:delim+QString::number(this->_____zzzzz_uuid).rightJustified(11,'0');
    }

    QVariant makeUuid(){
        this->_____zzzzz_uuid=++sequence_zzzz;
        return this->_____zzzzz_uuid;
    }

    virtual QStringList toScript(SqlSuitableKeyWord&parser){
        Q_UNUSED(parser)
        return {};
    }

    virtual void setPropertys(){
    }

    virtual void clear(){
        this->___clear();
    }

    virtual QVariant vMap(const QString&key)const{
        return this->toMap().value(key);
    }


    void insert(const QString&key, const QVariant&v){
        auto map=this->toMap();
        map.insert(key, v);
        this->setValue(map);
    }


    template<class TObject=QVariant>
    TObject oPointer(const QString&name){
        auto r=static_cast<TObject>(mapPointer.value(name));
        return r;
    }

    virtual QMap<QString,SqlParserCommand*> mPointer()const{
        return this->mapPointer;
    }


    virtual void setPointer(const QString&key, SqlParserCommand*p){
        auto pp=this->mapPointer.value(key);
        if(pp!=p && pp!=nullptr){
            this->mapPointer.remove(key);
            ::operator delete(pp);
            pp=nullptr;
        }
        if(p!=nullptr)
            this->mapPointer.insert(key, p);
    }

    virtual bool isChanged()const{
        return !this->mapPointer.isEmpty();
    }

    virtual void m(){
        this->make();
    }

    virtual void m(const QVariant&v){
        this->make(v);
    }

    virtual void make(){
        this->make(QVariant());
    }

    virtual void make(const QVariant&){
    }

    virtual bool makeObject(){
        bool __return=false;
        if(!this->mapPointer.isEmpty()){
            VariantUtil u;
            auto vThis=this->toMap();
            QMapIterator<QString, SqlParserCommand*> i(this->mapPointer);
            int seq=0;
            while (i.hasNext()) {
                i.next();
                auto key=qsl("%1.%2").arg(i.key()).arg(++seq);

                if(i.value()!=nullptr){
                    auto v=i.value();
                    if(v==nullptr)
                        continue;
                    if(v!=this)v->makeObject();
                    auto typeId=qTypeId(*v);
                    if(QStmTypesVariantDictionary.contains(typeId)){
                        auto map=v->toHash();
                        vThis.insert(key, map);
                        __return=true;
                    }

                    if(typeId==QMetaType_User){
                        vThis.insert(key, qv(*v));
                        __return=true;
                    }
                }
            }
            this->setValue(vThis);
        }
        return __return;
    }



private:
    SqlParserPointerMap mapPointer;
    qlonglong _____zzzzz_uuid=0;

    void ___clear(){
        {
            auto values=this->mapPointer.values();
            for(auto&v:values){
                v->setValue(QVariant());
                delete v;
            }
            this->mapPointer.clear();
        }
        this->setValue(qv_null);
    }
};

class Q_ORM_EXPORT SqlParserItem: public SqlParserCommand{
public:
    friend class SqlParserValue;
    friend class SqlParserObject;
    friend class SqlParserField;
public:
    explicit SqlParserItem():SqlParserCommand(){
        auto map=this->toHash();
        map.insert(qsl("info"),KeywordObjectInfo::koiObject);
        this->init(map);
    }
    explicit SqlParserItem(const QVariant&value):SqlParserCommand(){
        auto map=this->toHash();
        map.insert(qsl("info"),KeywordObjectInfo::koiObject);
        map.insert(qsl("value"),value);
        this->init(map);
    }

    explicit SqlParserItem(const QVariant&value, const QVariant&title, const KeywordObjectInfo&info):SqlParserCommand(){
        auto map=this->toHash();
        map.insert(qsl("info"),info);
        map.insert(qsl("value"),value);
        if(title.isValid() && !title.isNull())
            map.insert(qsl("title"),title);
        this->init(map);
    }

    void init(QVariantHash&map){
        this->_____zzzzz_uuid=++sequence_zzzz;//QUuid::createUuidV5(QUuid::createUuid(),QString::number(++staticInit).toUtf8());
        map.insert(qsl("uuid"), this->_____zzzzz_uuid);
        this->setValue(map);
    }

    auto setDefaultValue(const QVariant&defaultValue)const{//nao deve passar referencia[&] ou vai alterar objeto em memoria
        auto map=this->toMap();
        map.insert(qsl("defaultValue"), defaultValue);
        auto v=QVariant(map);
        auto item=SqlParserItem::from(v);
        return item;
    }

    auto defVal(const QVariant&defaultValue){//nao deve passar referencia[&] ou vai alterar objeto em memoria
        return this->setDefaultValue(defaultValue);
    }

    QVariantMap toMap()const{
        auto map=QVariant::toMap();
        if(this->_____zzzzz_uuid>0)
            map.insert(qsl("uuid"),QString::number(this->_____zzzzz_uuid).rightJustified(11,'0'));
        return map;
    }

    virtual QString toFormatParameter(SqlSuitableKeyWord &parser)const{
        auto map=this->toMap();
        auto v=this->value();
        auto defValue=map.value(qsl("defaultValue"));
        if(this->info()==KeywordObjectInfo::koiValue){
            if(defValue.isValid() && !defValue.isNull() && v.isNull())
                return parser.formatValue(defValue);
            return parser.formatValue(v);
        }

        auto name=v.toString();
        if(defValue.isValid() && !defValue.isNull()){
            auto command =parser.parserCommand(KeywordGenericCommand::kgcIsNullCheckValue);
            if(command.contains(qsl("%1")) && command.contains(qsl("%2"))){
                auto defValueFormated=parser.formatValue(defValue);
                name=command.arg(name, defValueFormated);
            }
        }
        return name;
    }

    virtual QString toFormat(SqlSuitableKeyWord &parser)const{
        auto v=this->value();
        if(this->info()==KeywordObjectInfo::koiValue)
            return parser.formatValue(v);
        return v.toString();
    }

    static const SqlParserItem from(const QVariant&v){
        SqlParserItem r;
        Q_DECLARE_VU;
        auto vValue=vu.toVariant(v);
        auto rMap=r.toMap();
        auto typeId=qTypeId(vValue);
        if(QStmTypesVariantDictionary.contains(typeId)){
            auto map=vValue.toMap();
            typeId=map.value(qsl("typeId")).toInt();
            auto&value=map[qsl("value")];
            map.insert(qsl("uuid"), rMap.value(qsl("uuid")));
            value=vu.convertTo(value, typeId);
            r.setValue(map);
            return r;
        }

        vValue=vu.convertTo(vValue, typeId);
        QVariantMap map;
        map.insert(qsl("uuid"), rMap.value(qsl("uuid")));
        map.insert(qsl("info"), KeywordObjectInfo::koiValue);
        map.insert(qsl("value"), vValue);
        map.insert(qsl("typeId"), typeId);
        r.setValue(map);
        return r;

    }

    static auto createObject(const QVariant&v){
        SqlParserItem r;
        Q_DECLARE_VU;
        auto vValue=vu.toVariant(v);
        auto rMap=r.toHash();
        QVariantHash map;

        auto typeId=qTypeId(vValue);
        if(QStmTypesVariantDictionary.contains(typeId)){
            auto map=vValue.toHash();
            if(!map.contains(qsl("uuid"))){
                map.insert(qsl("uuid"),rMap.value(qsl("uuid")));
            }
            map.insert(qsl("info"),KeywordObjectInfo::koiObject);
            return r;
        }

        map.insert(qsl("uuid"), rMap.value(qsl("uuid")));
        map.insert(qsl("info"), KeywordObjectInfo::koiObject);
        map.insert(qsl("value"), vValue);
        map.insert(qsl("typeId"), qTypeId(vValue));
        r.setValue(map);
        return r;
    }

    static auto createValue(const QVariant&v){
        SqlParserItem r;
        Q_DECLARE_VU;
        auto vValue=vu.toVariant(v);
        auto rMap=r.toHash();
        QVariantMap map;
        map.insert(qsl("uuid"),rMap.value(qsl("uuid")));
        map.insert(qsl("info"),KeywordObjectInfo::koiValue);
        map.insert(qsl("value"),vValue);
        map.insert(qsl("typeId"),qTypeId(vValue));
        r.setValue(map);
        return r;
    }

    /**
     * @brief operator =
     * @param value
     */
    void operator=(const QVariant&value){
        QVariant::setValue(value);
    }
    KeywordObjectInfo info()const{
        auto v=this->toHash().value(qsl("info"));
        return KeywordObjectInfo(v.toInt());
    }
    QVariant value()const{
        auto vHash=this->toHash();
        auto v=vHash.value(qsl("value"));
        auto d=vHash.value(qsl("defaultValue"));
        v=v.isValid() && !v.isNull()?v:d;
        return v;
    }

    QVariant valueTypeId()const{
        auto vHash=this->toHash();
        auto v=vHash.value(qsl("value"));
        auto d=vHash.value(qsl("defaultValue"));
        v=v.isValid() && !v.isNull()?v:d;
        return qTypeId(v);
    }

    bool isList()const{
        const auto&v=this->value();
        return qTypeId(v)==QMetaType_QVariantList || qTypeId(v)==QMetaType_QStringList;
    }

    bool isMap()const{
        const auto&v=this->value();
        return qTypeId(v)==QMetaType_QVariantMap || qTypeId(v)==QMetaType_QVariantHash;
    }

    QVariant name()const{
        if(this->info()==KeywordObjectInfo::koiObject){
            auto v=this->toHash().value(qsl("value"));
            return v;
        }
        return {};
    }

    bool isObject()const{
        return this->info()==KeywordObjectInfo::koiObject;
    }

    bool isValue()const{
        return this->info()==KeywordObjectInfo::koiValue;
    }

    QVariant title()const{
        if(this->info()==KeywordObjectInfo::koiObject){
            auto v=this->toHash().value(qsl("title"));
            return v;
        }
        return {};
    }

};

class Q_ORM_EXPORT SqlParserValue: public SqlParserItem{
public:
    explicit SqlParserValue():SqlParserItem(QVariant(), QVariant(), KeywordObjectInfo::koiValue){
    }
    explicit SqlParserValue(const QVariant&v):SqlParserItem(v, QVariant(), KeywordObjectInfo::koiValue){
    }
};


class Q_ORM_EXPORT SqlParserObject: public SqlParserItem{
public:
    explicit SqlParserObject():SqlParserItem(QVariant(), QVariant(), KeywordObjectInfo::koiValue){
    }
    explicit SqlParserObject(const QVariant&v):SqlParserItem(v, QVariant(), KeywordObjectInfo::koiValue){
    }
};

class Q_ORM_EXPORT SqlParserField: public SqlParserItem{
public:
    explicit SqlParserField():SqlParserItem(){
    }

    explicit SqlParserField(const QVariant&v):SqlParserItem(v){
    }

    explicit SqlParserField(const KeywordGrouping&grouping, const QVariant&name, const QVariant&nameAs, const QVariant&value, const QVariant&defaultValue):SqlParserItem(value){
        auto map=this->toHash();
        map.insert(qsl("grouping"),grouping);
        map.insert(qsl("name"),name);
        map.insert(qsl("nameAs"),nameAs);
        map.insert(qsl("value"),value);
        map.insert(qsl("defaultValue"),defaultValue);
        this->setValue(map);
    }

    ~SqlParserField(){
    }

    virtual QString toFormat(SqlSuitableKeyWord &parser)const{
        auto typeId=qTypeId(qTypeId(*this));
        if(QStmTypesVariantDictionary.contains(typeId))
            return qsl_null;
        return SqlParserItem::toFormat(parser);
    }
};

template<class TemplateParent>
class SqlParserFields: public SqlParserCommand{
public:
    TemplateParent*parent=nullptr;
    explicit SqlParserFields(const QVariant&v=QVariant()):SqlParserCommand(v){
    }
    explicit SqlParserFields(TemplateParent*parent, const QVariant&v=QVariant()):SqlParserCommand(v){
        this->parent=parent;
    }
    auto&f(const QVariant&v){
        this->insert(qsl("f.")+QUuid::createUuid().toString(),v);
        return*this;
    }
    auto&as(const QVariant&name, const QVariant&nameAs){
        return this->f(SqlParserField(KeywordGrouping::kgNone, name, nameAs, QVariant(), QVariant()));
    }
    auto&as(const QVariant&name, const QVariant&nameAs, const QVariant&defaultValue){
        return this->f(SqlParserField(KeywordGrouping::kgNone, name, nameAs, QVariant(), defaultValue));
    }
    auto&sum(const QVariant&name, const QVariant&nameAs, const QVariant&defaultValue){
        return this->f(SqlParserField(KeywordGrouping::kgSumAs, name, nameAs, QVariant(), defaultValue));
    }
    auto&max(const QVariant&name, const QVariant&nameAs, const QVariant&defaultValue){
        return this->f(SqlParserField(KeywordGrouping::kgMaxAs, name, nameAs, QVariant(), defaultValue));
    }
    auto&min(const QVariant&name, const QVariant&nameAs, const QVariant&defaultValue){
        return this->f(SqlParserField(KeywordGrouping::kgMinAs, name, nameAs, QVariant(), defaultValue));
    }
    auto&avg(const QVariant&name, const QVariant&nameAs, const QVariant&defaultValue){
        return this->f(SqlParserField(KeywordGrouping::kgAvgAs, name, nameAs, QVariant(), defaultValue));
    }
    auto&count(){
        return this->f(SqlParserField(KeywordGrouping::kgCountAs, 1, qsl("rows"), 1, 0));
    }
    auto&count(const QVariant&name, const QVariant&nameAs, const QVariant&defaultValue){
        auto _name=name;
        if(name.isNull())
            _name=1;

        auto _nameAs=nameAs.toString();
        if(_nameAs.isNull())
            _nameAs=qsl("rows");
        return this->f(SqlParserField(KeywordGrouping::kgCountAs, _name, _nameAs, QVariant(), defaultValue));
    }

};

class SqlParserValues:public SqlParserCommand{
public:
    explicit SqlParserValues(const QVariant&v=QVariant()):SqlParserCommand(v){
    }

    SqlParserValues&value(const QVariant&v){
        SqlParserItem*c=nullptr;
        QMapIterator<QString, SqlParserCommand*> i(this->mPointer());
        while (i.hasNext()) {
            i.next();
            if(i.key().startsWith(__func__))
                c=static_cast<SqlParserItem*>(i.value());
        }
        if(c==nullptr){
            c = new SqlParserItem(v);
            this->setPointer(c->makeUuid().toString(), c);
        }

        auto typeId=qTypeId(v);
        if(QStmTypesVariantList.contains(typeId)){
            for(auto&i:v.toList())
                this->value(i);
            return*this;
        }

        if(QStmTypesVariantDictionary.contains(typeId)){
            QHashIterator<QString, QVariant> i(v.toHash());
            while (i.hasNext()) {
                i.next();
                this->value(i.value());
            }
            return*this;
        }

        this->value(v);
        return*this;
    }

    SqlParserValues&v(const QVariant&v){
        return this->value(v);
    }

    QStringList toScript(SqlSuitableKeyWord &parser){
        QStringList output;
        auto map=this->toMap();
        QMapIterator<QString, QVariant> i(map);
        while (i.hasNext()) {
            i.next();
            output<<parser.formatValue(i.value());
        }
        return output;
    }
};

template<class TemplateParent>
class SqlParserConditions:public SqlParserCommand{
public:
    class SqlParserCondition: public SqlParserCommand{
    public:
        QString funcName;
        QString sName()const{
            return this->funcName+this->suuid(qsl("."));
        }
        explicit SqlParserCondition(const QVariant&v = QVariant()):SqlParserCommand(v){
        }


        explicit SqlParserCondition(QString funcName, const QVariant&field, const QVariant&valueA, const QVariant&valueB, const KeywordOperator&keywordOperator, const KeywordLogical&keywordLogical):SqlParserCommand(){
            auto map=this->toHash();
            this->funcName=funcName;
            map.insert(qsl("funcName"),funcName);
            map.insert(qsl("field"),field);
            map.insert(qsl("valueA"),valueA);
            map.insert(qsl("valueB"),valueB);
            map.insert(qsl("keywordOperator"),keywordOperator);
            map.insert(qsl("keywordLogical"),keywordLogical);
            this->setValue(map);
        }

        explicit SqlParserCondition(QString funcName, const QVariant&valueA, const QVariant&valueB, const KeywordOperator&keywordOperator, const KeywordLogical&keywordLogical):SqlParserCommand(){
            auto map=this->toHash();
            this->funcName=funcName;
            map.insert(qsl("funcName"),funcName);
            map.insert(qsl("valueA"),valueA);
            map.insert(qsl("valueB"),valueB);
            map.insert(qsl("keywordOperator"),keywordOperator);
            map.insert(qsl("keywordLogical"),keywordLogical);
            this->setValue(map);
        }

        QStringList toScript(SqlSuitableKeyWord &parser){
            auto map=this->toHash();
            auto first = map.value(qsl("first")).toBool();
            auto keywordOperator = KeywordOperator( map.contains(qsl("keywordOperator"))?map.value(qsl("keywordOperator")).toInt(): -1 );

            if(first)
                map.insert(qsl("keywordLogical"), KeywordLogical::klNone);


            auto field = map.value(qsl("field"));
            auto valueA = map.value(qsl("valueA"));
            auto valueATypeId=qTypeId(valueA);
            auto valueAval = valueA.toHash().value(qsl("value"));
            auto valueAvalTypeId=qTypeId(valueAval);

            auto valueB = map.value(qsl("valueB"));
            auto valueBTypeId=qTypeId(valueB);

            auto valueBval = valueB.toHash().value(qsl("value"));
            auto valueBvalTypeId=qTypeId(valueBval);

            if(QStmTypesVariantDictionary.contains(valueBvalTypeId))
                valueBval=valueBval.toHash().values();

            {//checkOperatorIn
                auto checkOperator=[&valueA, &valueB, &valueBTypeId, &valueAvalTypeId, &valueBvalTypeId, &keywordOperator](){
                    if((!valueA.isValid()) || (!valueB.isValid()) || valueA.isNull() || valueB.isNull()){
                        if(keywordOperator!=KeywordOperator::koIsNull && keywordOperator!=KeywordOperator::koIsNotNull)
                            return KeywordOperator::koIsNull;
                    }

                    if(
                        (keywordOperator != KeywordOperator::koIn && keywordOperator != KeywordOperator::koInOut)
                        &&
                        (QStmTypesVariantList.contains(valueAvalTypeId) || QStmTypesVariantList.contains(valueBvalTypeId))
                        ){
                        return KeywordOperator::koIn;
                    }

                    return keywordOperator;
                };

                keywordOperator=checkOperator();//CheckOperatorIn

                //fix values for parameter In/OutIn
                if(keywordOperator == KeywordOperator::koIn && keywordOperator == KeywordOperator::koInOut){
                    if(QStmTypesVariantList.contains(valueAvalTypeId) && !QStmTypesVariantList.contains(valueBvalTypeId)){
                        QORM_VARIABLE_INVERTER(valueA, valueB)
                        QORM_VARIABLE_INVERTER(valueATypeId, valueBTypeId)
                        QORM_VARIABLE_INVERTER(valueAvalTypeId, valueBvalTypeId)
                    }
                }
            }

            auto vField = SqlParserItem::from(field).toFormat(parser);
            auto vValueA = SqlParserItem::from(valueA).toFormatParameter(parser);
            auto vValueB = SqlParserItem::from(valueB).toFormatParameter(parser);
            auto keywordLogical = KeywordLogical ( map.contains(qsl("keywordLogical"))?map.value(qsl("keywordLogical") ).toInt(): -1 );

            auto kOperator = parser.parserCommand(keywordOperator );
            auto kLogical = parser.parserCommand(keywordLogical );

            static const auto vLikeNulls=QVector<QString>{qsl_null, qsl("''"), qsl("'%'"), qsl("'%%'")};

            if((keywordOperator==koLike && vLikeNulls.contains(vValueB)) || (keywordOperator==koLike && vLikeNulls.contains(vValueA)))
                return {};

            if(keywordOperator==koIsNull || keywordOperator==koIsNotNull)
                return QStringList{qsl(" %1 %2").arg(kLogical, kOperator.arg(vValueA)).trimmed()};

            if(keywordOperator==koBetween || keywordOperator==koNotBetween)
                return QStringList{qsl(" %1 %2").arg(kLogical, kOperator.isEmpty()?kOperator:(kOperator.arg(vField, vValueA, vValueB))).trimmed()};

            return QStringList{qsl(" %1 %2").arg(kLogical, kOperator.isEmpty()?kOperator:(kOperator.arg(vValueA, vValueB))).trimmed()};
        }

        virtual void defineFirst(){
            auto map=this->toHash();
            map.insert(qsl("first"), true);
            this->setValue(map);
        }
    };

public:
    void*parent=nullptr;
    explicit SqlParserConditions(const QVariant&v):SqlParserCommand(v){
    }
    explicit SqlParserConditions(void*parent, const QString&name, const QVariant&v=QVariant()):SqlParserCommand(v){
        this->parent=parent;
        Q_UNUSED(name)
    }

    auto&condition(const QVVM&v){
        SearchParameters searchParameters;
        searchParameters+=v;
        auto value=searchParameters.buildVariant();
        return this->condition(value);

    }
    auto&condition(const QVariant&v){
        SearchParameters searchParameters(v);
        for(auto&param:searchParameters.build()){
            auto valueA=SqlParserItem::from(param.valueA());
            auto valueB=SqlParserItem::from(param.valueB());
            auto valueC=SqlParserItem::from(param.valueC());
            auto keywordOperator=param.keywordOperator();
            auto keywordLogical=param.keywordLogical();
            if(valueB.isList() || valueA.isList()){
                this->in(valueA, valueB);
                continue;
            }

            switch (keywordOperator) {
            case koEqual:
                this->equal(valueA, valueB, keywordLogical);
                break;
            case koNotEqual:
                this->notEqual(valueA, valueB, keywordLogical);
                break;
            case koEqualMinor:
                this->equalMinor(valueA, valueB, keywordLogical);
                break;
            case koEqualBigger:
                this->equalBigger(valueA, valueB, keywordLogical);
                break;
            case koBigger:
                this->bigger(valueA, valueB, keywordLogical);
                break;
            case koMinor:
                this->minor(valueA, valueB, keywordLogical);
                break;
            case koIn:
                this->in(valueA, valueB, keywordLogical);
                break;
            case koInOut:
                this->inOut(valueA, valueB, keywordLogical);
                break;
            case koLike:
                this->like(valueA, valueB, keywordLogical);
                break;
            case koNotNull:
                this->isNotNull(valueA, valueB, keywordLogical);
                break;
            case koBetween:
                if(param.valueC().isValid())
                    this->between(valueA, valueB, valueC, keywordLogical);
                else
                    this->between(valueA, valueB, keywordLogical);
                break;
            case koIsNull:
                this->isNull(param.valueA(), param.valueB(), keywordLogical);
                break;
            case koIsNotNull:
                this->isNotNull(param.valueA(), param.valueB(), keywordLogical);
                break;
            default:
                break;
            }
        }
        return*this;
    }
    auto&equal(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koEqual, keywordLogical));
    }
    auto&notEqual(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koNotEqual, keywordLogical));
    }
    auto&equalBigger(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koEqualBigger, keywordLogical));
    }
    auto&equalMinor(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koEqualMinor, keywordLogical));
    }
    auto&bigger(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koBigger, keywordLogical));
    }
    auto&minor(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koMinor, keywordLogical));
    }
    auto&like(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koLike, keywordLogical));
    }
    auto&soundex(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koSoundexEqual, keywordLogical));
    }
    auto&in(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koIn, keywordLogical));
    }
    auto&in(const QVariant&valueA, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        auto valueB=SqlParserObject(qsl(":%1").arg(valueA.toString()));
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koIn, keywordLogical));
    }
    auto&inOut(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koInOut, keywordLogical));
    }
    auto&inOut(const QVariant&valueA,const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        auto valueB=SqlParserObject(qsl(":%1").arg(valueA.toString()));
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koInOut, keywordLogical));
    }
    auto&isNull(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koIsNull, keywordLogical));
    }
    auto&isNull(const QVariant&valueA, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        auto valueB=SqlParserObject(qsl(":%1").arg(valueA.toString()));
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koIsNull, keywordLogical));
    }
    auto&isNotNull(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koIsNotNull, keywordLogical));
    }
    auto&isNotNull(const QVariant&valueA, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        auto valueB=SqlParserObject(qsl(":%1").arg(valueA.toString()));
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koIsNotNull, keywordLogical));
    }
    auto&between(const QVariant&field, const QVariant&valueA, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        if(qTypeId(valueA)==QMetaType_QDateTime || qTypeId(valueA)==QMetaType_QDate || qTypeId(valueA)==QMetaType_QTime){
            auto valueB=QDateTime(valueA.toDateTime().date(), QTime(23,59,59,998));
            return this->addCondition(new SqlParserCondition(__func__, field, valueA, valueB, KeywordOperator::koBetween, keywordLogical));
        }
        return this->equal(field, valueA, keywordLogical);
    }
    auto&between(const QVariant&field, const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
        return this->addCondition(new SqlParserCondition(__func__, field, valueA, valueB, KeywordOperator::koBetween, keywordLogical));
    }

    QStringList toScript(SqlSuitableKeyWord &parser){

        QStringList output;

        auto list = this->toList();

        if(!list.isEmpty()){
            output<<parser.parserCommand(KeywordCombine::kcWhere);
            auto first=true;
            for(auto&v:list){
                auto map=v.toHash();
                if(first){
                    map.remove(qsl("keywordLogical"));
                    first=false;
                }
                SqlParserCondition condition(map);
                output<<condition.toScript(parser);
            }
        }
        return output;
    }
private:
    auto&addCondition(SqlParserCondition*v){
        v->makeUuid();
        this->setPointer(v->sName(),v);
        return*this;
    }
};

template <class T>
class SqlParserCombinations:public SqlParserCommand{
public:
    class SqlParserCombination:public SqlParserCommand{
    public:
        QString funcName;
        QString sName(){
            return this->funcName+this->suuid(qsl("."));
        }
        SqlParserCombinations<T> * parent=nullptr;
        explicit SqlParserCombination(const QVariant&v=QVariant()):SqlParserCommand(v){
            this->parent=nullptr;
            this->makeUuid();
        }
        explicit SqlParserCombination(const QString&funcName, SqlParserCombinations<T> * parent, const KeywordCombine&combine, const QVariant&condition):SqlParserCommand(){
            this->makeUuid();
            this->funcName=funcName.toLower();
            this->parent=parent;

            auto map=this->toMap();
            map.insert(qsl("combine"),combine);
            if(combine==KeywordCombine::kcWhere){
                this->setValue(map);
                this->c().condition(condition);
                return;
            }

            map.insert(qsl("on"),condition);
            this->setValue(map);
        }

        auto&makeOperator(const KeywordOperator&keywordOperator=KeywordOperator::koEqual){
            auto map=this->toHash();
            map.insert(qsl("keywordOperator"),keywordOperator);
            this->setValue(map);
            return*this;
        }

        auto&makeLogical(const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
            auto map=this->toHash();
            map.insert(qsl("keywordLogical"),keywordLogical);
            this->setValue(map);
            return*this;
        }

        auto &equal(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
            this->c().equal(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &equalBigger(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
            this->c().equalBigger(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &equalMinor(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
            this->c().equalMinor(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &bigger(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
            this->c().bigger(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &minor(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
            this->c().minor(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &like(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical){
            this->c().like(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &in(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
            this->c().in(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &in(const QVariant&valueA, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
            this->c().in(valueA, keywordLogical);
            return*this;
        }
        auto &inOut(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
            this->c().inOut(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &inOut(const QVariant&valueA,const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
            this->c().inOut(valueA, keywordLogical);
            return*this;
        }
        auto &isNull(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
            this->c().isNull(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &isNull(const QVariant&valueA, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
            this->c().isNull(valueA, keywordLogical);
            return*this;
        }
        auto &notNull(const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
            this->c().isNotNull(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &notNull(const QVariant&valueA, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
            this->c().isNotNull(valueA, keywordLogical);
            return*this;
        }
        auto &between(const QVariant&field, const QVariant&valueA, const QVariant&valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
            this->c().between(field, valueA, valueB, keywordLogical);
            return*this;
        }

        SqlParserConditions<SqlParserCombinations<T>>&c(){
            return this->condition();
        }

        SqlParserConditions<SqlParserCombinations<T>>&condition(){
            auto cond=(this->oPointer<SqlParserConditions<SqlParserCombinations<T>>*>(__func__));
            if(cond==nullptr){
                cond=new SqlParserConditions<SqlParserCombinations<T>>(this->vMap(__func__));
                cond->parent=this;
                this->setPointer(__func__, cond);
            }
            return*cond;
        }

        void make(){
        }

        virtual QStringList toScript(SqlSuitableKeyWord&parser){

            QVariantMap map;
            {//convercao para mapa
                auto typeId=qTypeId(*this);
                switch (typeId) {
                case QMetaType_QVariantList:
                {
                    auto l=this->toList();
                    map=l.isEmpty()?map:l.first().toMap();
                    break;
                }
                default:
                    map=this->toMap();
                }
            }

            if(map.isEmpty())
                return {};

            auto kCombine=KeywordCombine(map.value(qsl("combine")).toInt());
            auto kCombineAlias=map.value(qsl("alias")).toString();
            auto kCombineConditions=getVariantStartsWith(qsl("condition"), map).toMap();

            auto kCombineOn=map.value(qsl("on"));
            const auto&modelInfo=QOrm::ModelInfo::modelInfo(kCombineOn);
            auto tableName=modelInfo.tableNameFull();
            auto connection=parser.parserCommand(kCombine);

            static const auto kCombineCheckList=QVector<int>{
                KeywordCombine::kcJoin,
                KeywordCombine::kcJoinLeft,
                KeywordCombine::kcJoinInner,
                KeywordCombine::kcJoinOuter,
                KeywordCombine::kcJoinCross,
                KeywordCombine::kcJoinFull,
                KeywordCombine::kcWhere
            };

            switch (kCombine) {
            case KeywordCombine::kcFrom:
                if(kCombineAlias.isEmpty())
                    return QStringList{qsl("%1 %2").arg(connection, tableName)};
                return QStringList{qsl("%1 %2 as %3").arg(connection, tableName, kCombineAlias)};
            default:
                if (kCombineCheckList.contains(kCombine)){
                    if(kCombine!=KeywordCombine::kcWhere){
                        if(kCombineAlias.isEmpty())
                            connection=qsl("%1 %2").arg(connection, tableName);
                        else
                            connection=qsl("%1 %2 as %3").arg(connection, tableName, kCombineAlias);
                    }

                    QStringList cond;
                    QStringList lstCond;

                    QMapIterator<QString, QVariant> i(kCombineConditions);
                    bool first=true;
                    while (i.hasNext()) {
                        i.next();
                        const auto&map=i.value();
                        SqlParserConditions<SqlParserCommand>::SqlParserCondition cond(map);
                        if(first){
                            first=false;
                            cond.defineFirst();
                        }
                        auto line=cond.toScript(parser);
                        if(line.isEmpty()){
                            if(first)
                                first=false;
                            continue;
                        }
                        lstCond<<line;
                    }

                    if(kCombine==KeywordCombine::kcJoinCross){
                        return QStringList{connection};
                    }

                    if(lstCond.isEmpty())
                        return {};

                    QStringList output;
                    if(kCombine==KeywordCombine::kcWhere)
                        output<<connection;
                    else
                        output<<(connection+qsl(" on "));
                    for(auto&v:lstCond)
                        output<<v;
                    return output;
                }
            }
            return {};
        }
    };


    explicit SqlParserCombinations(const QVariant&v):SqlParserCommand(v){
    }

    auto&limit(int limit){
        if(limit>0){
            auto map=this->toMap();
            map.insert(__func__, limit);
            this->setValue(map);
        }
        return*this;
    }

    auto&from(){
        return this->from(QVariant());
    }

    auto&from(const QVariant&fromObject){
        SqlParserCombination*c=nullptr;
        QMapIterator<QString, SqlParserCommand*> i(this->mPointer());
        while (i.hasNext()) {
            i.next();
            if(i.key().startsWith(__func__)){
                c=static_cast<SqlParserCombination*>(i.value());
            }
        }
        if(c==nullptr){
            c = new SqlParserCombination(__func__, this, KeywordCombine::kcFrom, fromObject);
            this->setPointer(c->sName(), c);
        }
        return*this;
    }

    auto&where(){
        return this->where(QVariant());
    }
    auto&where(const QVariant&whereObject){
        SqlParserCombination*c=nullptr;
        QMapIterator<QString, SqlParserCommand*> i(this->mPointer());
        while (i.hasNext()) {
            i.next();
            if(i.key().startsWith(__func__)){
                c=static_cast<SqlParserCombination*>(i.value());
            }
        }
        if(c==nullptr){
            c= new SqlParserCombination(__func__, this, KeywordCombine::kcWhere, whereObject);
            this->setPointer(c->sName(), c);
        }
        return c->condition();
    }

    SqlParserCombination&joinInner(const QVariant&on){
        auto c=new SqlParserCombination(qsl("join"), this, KeywordCombine::kcJoinInner, on);
        this->setPointer(c->sName(), c);
        return*c;
    }
    SqlParserCombination&joinLeft(const QVariant&on){
        auto c=new SqlParserCombination(qsl("join"), this, KeywordCombine::kcJoinLeft, on);
        this->setPointer(c->sName(), c);
        return*c;
    }
    SqlParserCombination&joinOuter(const QVariant&on){
        auto c=new SqlParserCombination(qsl("join"), this, KeywordCombine::kcJoinOuter, on);
        this->setPointer(c->sName(), c);
        return*c;
    }
    SqlParserCombination&joinFull(const QVariant&on){
        auto c=new SqlParserCombination(qsl("join"), this, KeywordCombine::kcJoinFull, on);
        this->setPointer(c->sName(), c);
        return*c;
    }
    SqlParserCombination&joinCross(const QVariant&on){
        auto c=new SqlParserCombination(qsl("join"), this, KeywordCombine::kcJoinCross, on);
        this->setPointer(c->sName(), c);
        return*c;
    }
    SqlParserCombination&workTable(const QVariant&workTable){
        auto c=new SqlParserCombination(__func__, this, KeywordCombine::kcWorkTable, workTable);
        this->setPointer(c->sName(), c);
        return*c;
    }
    SqlParserCombination&cte(const QVariant&cte){
        auto c=new SqlParserCombination(__func__, this, KeywordCombine::kcCte, cte);
        this->setPointer(c->sName(), c);
        return*c;
    }

    SqlParserCombination&combination(){
        auto v=(this->oPointer<SqlParserCombination*>(__func__));
        if(v==nullptr){
            v=new SqlParserCombination();
            this->setPointer(__func__, v);
        }
        return*v;
    }

    void m(){
        this->make();
    }

    void make(){
        auto map=this->toHash();
        map.insert(qsl("combination"), this->combination());
        this->setValue(map);
    }

    QStringList toScript(SqlSuitableKeyWord &parser) {
        auto map=this->toMap();
        if(map.isEmpty())
            return {};
        QVariantList lstFrom;
        QVariantList lstJoin;
        QVariantList lstWhere;

        static const auto kCombineCheckList=QVector<int>{
            KeywordCombine::kcJoin,
            KeywordCombine::kcJoinLeft,
            KeywordCombine::kcJoinInner,
            KeywordCombine::kcJoinOuter,
            KeywordCombine::kcJoinCross,
            KeywordCombine::kcJoinFull
        };

        QMapIterator<QString, QVariant> i(map);
        while (i.hasNext()) {
            i.next();
            QVariantList*lst=nullptr;
            auto map=i.value().toMap();
            auto kCombine=KeywordCombine(map.value(qsl("combine")).toInt());
            switch (kCombine) {
            case KeywordCombine::kcFrom:
                lst=&lstFrom;
                break;
            case KeywordCombine::kcWhere:
                lst=&lstWhere;
                break;
            default:
                if (kCombineCheckList.contains(kCombine))
                    lst=&lstJoin;
            }
            if(lst==nullptr)
                continue;
            lst->append(map);
        }

        QVariantList lst;
        for(auto&v:lstFrom)
            lst<<v;
        for(auto&v:lstJoin)
            lst<<v;
        for(auto&v:lstWhere)
            lst<<v;

        QStringList output;
        for(auto&vCombine:lst){
            auto combine=SqlParserCombinations::SqlParserCombination(vCombine);
            output<<combine.toScript(parser);
        }

        return output;
    }
};




template <class T>
class Q_ORM_EXPORT SqlParserFrom:public SqlParserCombinations<T>{
public:

    explicit SqlParserFrom(const QVariant&v=QVariant()):SqlParserCombinations<T>(v){
    }
    /**
     * @brief distinct
     * @return
     */
    auto&distinct(){
        auto map=this->toMap();
        map.insert(qsl("distinct"), true);
        this->setValue(map);
        return*dynamic_cast<T*>(this);
    }
    /**
     * @brief distinct
     * @return
     */
    auto&distinct(bool value){
        auto map=this->toMap();
        map.insert(qsl("distinct"), value);
        this->setValue(map);
        return*dynamic_cast<T*>(this);
    }
};


class SqlParserCallObject:public SqlParserCommand{
public:

    auto&database(const QVariant&v){
        auto object=this->oPointer<SqlParserCommand*>(__func__);
        if(object==nullptr){
            object=new SqlParserCommand(v);
            this->setPointer(__func__,object);
        }
        return*this;
    }

    auto&schema(const QVariant&v){
        auto object=this->oPointer<SqlParserCommand*>(__func__);
        if(object==nullptr){
            object=new SqlParserCommand(v);
            this->setPointer(__func__,object);
        }
        return*this;
    }

    auto&object(const QVariant&v){
        auto object=this->oPointer<SqlParserCommand*>(__func__);
        if(object==nullptr){
            object=new SqlParserCommand(v);
            this->setPointer(__func__,object);
        }
        return*this;
    }

    SqlParserValues&values(const QVariant&values){
        auto object=this->oPointer<SqlParserValues*>(__func__);
        if(object==nullptr){
            object=new SqlParserValues(QVariant());
            this->setPointer(__func__,object);
        }
        return object->v(values);
    }

    SqlParserValues&values(){
        return this->values(QVariant());
    }

    SqlParserValues&v(){
        return this->values();
    }

    explicit SqlParserCallObject():SqlParserCommand(){
    }

    explicit SqlParserCallObject(const QVariant&v=QVariant()):SqlParserCommand(v){
    }
};

}

Q_DECLARE_METATYPE(QOrm::SqlParserCommand)
Q_DECLARE_METATYPE(QOrm::SqlParserItem)
Q_DECLARE_METATYPE(QOrm::SqlParserValue)
Q_DECLARE_METATYPE(QOrm::SqlParserField)
Q_DECLARE_METATYPE(QOrm::SqlParserValues)

Q_DECLARE_METATYPE(QOrm::SqlParserCommand*)
Q_DECLARE_METATYPE(QOrm::SqlParserItem*)
Q_DECLARE_METATYPE(QOrm::SqlParserValue*)
Q_DECLARE_METATYPE(QOrm::SqlParserField*)
Q_DECLARE_METATYPE(QOrm::SqlParserValues*)

