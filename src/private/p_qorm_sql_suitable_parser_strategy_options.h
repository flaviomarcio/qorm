#pragma once

//#include <QStm>
//#include "../qorm_macro.h"
#include "./p_qorm_model_info.h"
#include "./p_qorm_sql_suitable_parameter.h"
#include "./p_qorm_sql_suitable_parser_command.h"
#include "./p_qorm_sql_suitable_parser_item.h"
#include "./p_qorm_sql_suitable_parser_object.h"
#include "./p_qorm_sql_suitable_parser_value.h"
#include "../../../qstm/src/qstm_vvm.h"

#define __QORM_VARIABLE_INVERTER(valueA, valueB)\
{\
    auto aux=valueA;\
    valueA=valueB;  \
    valueB=aux;\
}


namespace QOrm {

template<class TemplateParent>
class SqlParserConditions:public SqlParserCommand{
public:
    class SqlParserCondition: public SqlParserCommand{
    public:
        QString funcName;
        QString sName()const
        {
            return this->funcName+this->suuid(QStringLiteral("."));
        }
        explicit SqlParserCondition(const QVariant &v = {}):SqlParserCommand{v}
        {
        }
        explicit SqlParserCondition(QString funcName, const QVariant &field, const QVariant &valueA, const QVariant &valueB, const KeywordOperator&keywordOperator, const KeywordLogical &keywordLogical)
            :SqlParserCommand{}
        {
            auto map=this->toMap();
            this->funcName=funcName;
            map.insert(QStringLiteral("funcName"),funcName);
            map.insert(QStringLiteral("field"),field);
            map.insert(QStringLiteral("valueA"),valueA);
            map.insert(QStringLiteral("valueB"),valueB);
            map.insert(QStringLiteral("keywordOperator"),keywordOperator);
            map.insert(QStringLiteral("keywordLogical"),keywordLogical);
            this->setValue(map);
        }

        explicit SqlParserCondition(QString funcName, const QVariant &valueA, const QVariant &valueB, const KeywordOperator&keywordOperator, const KeywordLogical &keywordLogical)
            :SqlParserCommand{}
        {
            auto map=this->toMap();
            this->funcName=funcName;
            map.insert(QStringLiteral("funcName"),funcName);
            map.insert(QStringLiteral("valueA"),valueA);
            map.insert(QStringLiteral("valueB"),valueB);
            map.insert(QStringLiteral("keywordOperator"),keywordOperator);
            map.insert(QStringLiteral("keywordLogical"),keywordLogical);
            this->setValue(map);
        }

        QStringList toScript(SqlSuitableKeyWord &parser)
        {
            auto map=this->toMap();
            auto first = map.value(QStringLiteral("first")).toBool();
            auto keywordOperator = KeywordOperator( map.contains(QStringLiteral("keywordOperator"))?map.value(QStringLiteral("keywordOperator")).toInt(): -1 );

            if(first)
                map.insert(QStringLiteral("keywordLogical"), KeywordLogical::klNone);


            auto field = map.value(QStringLiteral("field"));
            auto valueA = map.value(QStringLiteral("valueA"));
            auto valueATypeId=valueA.typeId();
            auto valueAval = valueA.toHash().value(QStringLiteral("value"));
            auto valueAvalTypeId = valueAval.typeId();

            auto valueB = map.value(QStringLiteral("valueB"));
            auto valueBTypeId=valueB.typeId();

            QVariant valueBval;
            switch (valueBTypeId) {
            case QMetaType::QVariantHash:
            case QMetaType::QVariantMap:
            {
                auto v=valueB.toHash();
                if(v.contains(QStringLiteral("value"))){
                    valueBval=valueB.toHash().value(QStringLiteral("value"));
                    break;
                }
                valueBval=v.values();
                break;
            }
            default:
                valueBval=valueB;
                break;
            }

            auto valueBvalTypeId=valueBval.typeId();

            switch (valueBvalTypeId) {
            case QMetaType::QVariantHash:
            case QMetaType::QVariantMap:
                valueBval=valueBval.toHash().values();
                valueBvalTypeId=valueBval.typeId();
                break;
            default:
                break;
            }

            {//checkOperatorIn
                auto checkOperator=[&valueA, &valueB, &valueBTypeId, &valueAvalTypeId, &valueBvalTypeId, &keywordOperator](){
                    if((!valueA.isValid()) || (!valueB.isValid()) || valueA.isNull() || valueB.isNull()){
                        switch (keywordOperator) {
                        case KeywordOperator::koIsNull:
                        case KeywordOperator::koIsNotNull:
                            return keywordOperator;
                        default:
                            return KeywordOperator::koIsNull;
                        }
                    }

                    switch (keywordOperator) {
                    case KeywordOperator::koIn:
                    case KeywordOperator::koInOut:
                        return keywordOperator;
                    default:
                        switch (valueAvalTypeId) {
                        case QMetaType::QVariantList:
                        case QMetaType::QStringList:
                            return KeywordOperator::koIn;
                        default:
                            break;
                        }

                        switch (valueBvalTypeId) {
                        case QMetaType::QVariantList:
                        case QMetaType::QStringList:
                            return KeywordOperator::koIn;
                        default:
                            break;
                        }

                        return keywordOperator;
                    }
                };

                keywordOperator=checkOperator();//CheckOperatorIn

                switch (keywordOperator) {
                case KeywordOperator::koIn:
                case KeywordOperator::koInOut:
                {
                    switch (valueBvalTypeId) {
                    case QMetaType::QVariantList:
                    case QMetaType::QStringList:
                    {

                        switch (valueBvalTypeId) {
                        case QMetaType::QVariantList:
                        case QMetaType::QStringList:
                            break;
                        default:
                            __QORM_VARIABLE_INVERTER(valueA, valueB)
                            __QORM_VARIABLE_INVERTER(valueATypeId, valueBTypeId)
                            __QORM_VARIABLE_INVERTER(valueAvalTypeId, valueBvalTypeId)
                            break;
                        }
                        break;
                    }
                    default:
                        break;
                    }
                    break;
                }
                default:
                    break;
                }
            }

            auto vField = SqlParserItem::from(field).toFormat(parser);
            auto vValueA = SqlParserItem::from(valueA).toFormatParameter(parser);
            auto vValueB = SqlParserItem::from(valueB).toFormatParameter(parser);
            auto keywordLogical = KeywordLogical ( map.contains(QStringLiteral("keywordLogical"))?map.value(QStringLiteral("keywordLogical") ).toInt(): -1 );

            auto kOperator = parser.parserCommand(keywordOperator );
            auto kLogical = parser.parserCommand(keywordLogical );

            static const auto vLikeNulls=QVector<QString>{{}, QStringLiteral("''"), QStringLiteral("'%'"), QStringLiteral("'%%'")};

            switch (keywordOperator) {
            case KeywordOperator::koLike:
            {
                if(vLikeNulls.contains(vValueB) || vLikeNulls.contains(vValueA))
                    return {};
                break;
            }
            case koIsNull:
            case koIsNotNull:
                return QStringList{QStringLiteral(" %1 %2").arg(kLogical, kOperator.arg(vValueA)).trimmed()};
            case koBetween:
            case koNotBetween:
                return QStringList{QStringLiteral(" %1 %2").arg(kLogical, kOperator.isEmpty()?kOperator:(kOperator.arg(vField, vValueA, vValueB))).trimmed()};
            default:
                break;
            }
            return QStringList{QStringLiteral(" %1 %2").arg(kLogical, kOperator.isEmpty()?kOperator:(kOperator.arg(vValueA, vValueB))).trimmed()};
        }

        virtual void defineFirst()
        {
            auto map=this->toHash();
            map.insert(QStringLiteral("first"), true);
            this->setValue(map);
        }
    };

public:
    void *parent=nullptr;
    explicit SqlParserConditions(const QVariant &v):SqlParserCommand{v}
    {
    }
    explicit SqlParserConditions(void *parent, const QString &name, const QVariant &v={}):SqlParserCommand{v}
    {
        this->parent=parent;
        Q_UNUSED(name)
    }

    auto &condition(const QVVM &v)
    {
        SearchParameters searchParameters;
        searchParameters+=v;
        auto value=searchParameters.buildVariant();
        return this->condition(value);

    }
    auto &condition(const QVariant &v)
    {
        SearchParameters searchParameters(v);
        for(auto &param:searchParameters.build()){
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
        return *this;
    }
    auto &equal(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        KeywordOperator keywordOperator;
        switch (valueA.typeId()) {
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
            keywordOperator=KeywordOperator::koIn;
            break;
        default:
            switch (valueB.typeId()) {
            case QMetaType::QVariantList:
            case QMetaType::QStringList:
                keywordOperator=KeywordOperator::koIn;
                break;
            default:
                keywordOperator=KeywordOperator::koEqual;
            }
        }
        return this->addCondition(new SqlParserCondition{__func__, valueA, valueB, keywordOperator, keywordLogical});
    }
    auto &notEqual(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        KeywordOperator keywordOperator;
        switch (valueA.typeId()) {
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
            keywordOperator=KeywordOperator::koInOut;
            break;
        default:
            switch (valueB.typeId()) {
            case QMetaType::QVariantList:
            case QMetaType::QStringList:
                keywordOperator=KeywordOperator::koInOut;
                break;
            default:
                keywordOperator=KeywordOperator::koNotEqual;
            }
        }
        return this->addCondition(new SqlParserCondition{__func__, valueA, valueB, keywordOperator, keywordLogical});
    }
    auto &equalBigger(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition{__func__, valueA, valueB, KeywordOperator::koEqualBigger, keywordLogical});
    }
    auto &equalMinor(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition{__func__, valueA, valueB, KeywordOperator::koEqualMinor, keywordLogical});
    }
    auto &bigger(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition{__func__, valueA, valueB, KeywordOperator::koBigger, keywordLogical});
    }
    auto &minor(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition{__func__, valueA, valueB, KeywordOperator::koMinor, keywordLogical});
    }
    auto &like(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition{__func__, valueA, valueB, KeywordOperator::koLike, keywordLogical});
    }
    auto &soundex(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition{__func__, valueA, valueB, KeywordOperator::koSoundexEqual, keywordLogical});
    }
    auto &in(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition{__func__, valueA, valueB, KeywordOperator::koIn, keywordLogical});
    }
    auto &in(const QVariant &valueA, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        auto valueB=SqlParserObject(QStringLiteral(":%1").arg(valueA.toString()));
        return this->addCondition(new SqlParserCondition{__func__, valueA, valueB, KeywordOperator::koIn, keywordLogical});
    }
    auto &inOut(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition{__func__, valueA, valueB, KeywordOperator::koInOut, keywordLogical});
    }
    auto &inOut(const QVariant &valueA,const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        auto valueB=SqlParserObject(QStringLiteral(":%1").arg(valueA.toString()));
        return this->addCondition(new SqlParserCondition{__func__, valueA, valueB, KeywordOperator::koInOut, keywordLogical});
    }
    auto &isNull(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition{__func__, valueA, valueB, KeywordOperator::koIsNull, keywordLogical});
    }
    auto &isNull(const QVariant &valueA, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        auto valueB=SqlParserObject(QStringLiteral(":%1").arg(valueA.toString()));
        return this->addCondition(new SqlParserCondition{__func__, valueA, valueB, KeywordOperator::koIsNull, keywordLogical});
    }
    auto &isNotNull(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition{__func__, valueA, valueB, KeywordOperator::koIsNotNull, keywordLogical});
    }
    auto &isNotNull(const QVariant &valueA, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        auto valueB=SqlParserObject(QStringLiteral(":%1").arg(valueA.toString()));
        return this->addCondition(new SqlParserCondition{__func__, valueA, valueB, KeywordOperator::koIsNotNull, keywordLogical});
    }
    SqlParserConditions<TemplateParent>&between(const QVariant &field, const QVariant &valueA, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        switch (valueA.typeId()) {
        case QMetaType::QDateTime:
        case QMetaType::QDate:
        case QMetaType::QTime:
        {
            auto valueB=QDateTime{valueA.toDateTime().date(), QTime(23,59,59,998)};
            return this->addCondition(new SqlParserCondition{__func__, field, valueA, valueB, KeywordOperator::koBetween, keywordLogical});
        }
        default:
            return this->equal(field, valueA, keywordLogical);
        }
    }
    auto &between(const QVariant &field, const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition{__func__, field, valueA, valueB, KeywordOperator::koBetween, keywordLogical});
    }

    QStringList toScript(SqlSuitableKeyWord &parser)
    {
        auto list = this->toList();
        if(list.isEmpty())
            return {};
        QStringList output;
        output.append(parser.parserCommand(KeywordCombine::kcWhere));
        auto first=true;
        for(auto &v:list){
            auto map=v.toHash();
            if(first){
                map.remove(QStringLiteral("keywordLogical"));
                first=false;
            }
            SqlParserCondition condition(map);
            output.append(condition.toScript(parser));
        }
        return output;
    }
private:
    auto &addCondition(SqlParserCondition *v)
    {
        v->makeUuid();
        this->setPointer(v->sName(),v);
        return *this;
    }
};

template <class T>
class SqlParserCombinations:public SqlParserCommand{
public:
    class SqlParserCombination:public SqlParserCommand{
    public:
        QString funcName;
        QString sName()
        {
            return this->funcName+this->suuid(QStringLiteral("."));
        }

        SqlParserCombinations<T> * parent=nullptr;
        explicit SqlParserCombination(const QVariant &v={}):SqlParserCommand{v}
        {
            this->parent=nullptr;
            this->makeUuid();
        }

        explicit SqlParserCombination(const QString &funcName, SqlParserCombinations<T> * parent, const KeywordCombine&combine, const QVariant &condition, const QVariant &alias={}):SqlParserCommand{}
        {
            this->makeUuid();
            this->funcName=funcName.toLower();
            this->parent=parent;

            auto map=this->toMap();
            map.insert(QStringLiteral("combine"),combine);
            if(combine==KeywordCombine::kcWhere){
                this->setValue(map);
                this->c().condition(condition);
                return;
            }

            if(alias.isValid())
                map.insert(QStringLiteral("alias"), alias);

            map.insert(QStringLiteral("on"),condition);
            this->setValue(map);
        }

        auto &alias(const QString &alias)
        {
            auto map=this->toHash();
            map.insert(QStringLiteral("alias"), alias);
            this->setValue(map);
            return *this;
        }

        auto &makeOperator(const KeywordOperator&keywordOperator=KeywordOperator::koEqual)
        {
            auto map=this->toHash();
            map.insert(QStringLiteral("keywordOperator"),keywordOperator);
            this->setValue(map);
            return *this;
        }

        auto &makeLogical(const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
        {
            auto map=this->toHash();
            map.insert(QStringLiteral("keywordLogical"),keywordLogical);
            this->setValue(map);
            return *this;
        }

        auto &equal(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
        {
            this->c().equal(valueA, valueB, keywordLogical);
            return *this;
        }
        auto &equalBigger(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
        {
            this->c().equalBigger(valueA, valueB, keywordLogical);
            return *this;
        }
        auto &equalMinor(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
        {
            this->c().equalMinor(valueA, valueB, keywordLogical);
            return *this;
        }
        auto &bigger(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
        {
            this->c().bigger(valueA, valueB, keywordLogical);
            return *this;
        }
        auto &minor(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
        {
            this->c().minor(valueA, valueB, keywordLogical);
            return *this;
        }
        auto &like(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical)
        {
            this->c().like(valueA, valueB, keywordLogical);
            return *this;
        }
        auto &in(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
        {
            this->c().in(valueA, valueB, keywordLogical);
            return *this;
        }
        auto &in(const QVariant &valueA, const KeywordLogical &keywordLogical=KeywordLogical::klAnd){
            this->c().in(valueA, keywordLogical);
            return *this;
        }
        auto &inOut(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
        {
            this->c().inOut(valueA, valueB, keywordLogical);
            return *this;
        }
        auto &inOut(const QVariant &valueA,const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
        {
            this->c().inOut(valueA, keywordLogical);
            return *this;
        }
        auto &isNull(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
        {
            this->c().isNull(valueA, valueB, keywordLogical);
            return *this;
        }
        auto &isNull(const QVariant &valueA, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
        {
            this->c().isNull(valueA, keywordLogical);
            return *this;
        }
        auto &notNull(const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
        {
            this->c().isNotNull(valueA, valueB, keywordLogical);
            return *this;
        }
        auto &notNull(const QVariant &valueA, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
        {
            this->c().isNotNull(valueA, keywordLogical);
            return *this;
        }
        auto &between(const QVariant &field, const QVariant &valueA, const QVariant &valueB, const KeywordLogical &keywordLogical=KeywordLogical::klAnd)
        {
            this->c().between(field, valueA, valueB, keywordLogical);
            return *this;
        }

        SqlParserConditions<SqlParserCombinations<T>>&c()
        {
            return this->condition();
        }

        SqlParserConditions<SqlParserCombinations<T>>&condition()
        {
            auto cond=(this->oPointer<SqlParserConditions<SqlParserCombinations<T>>*>(__func__));
            if(cond==nullptr){
                cond=new SqlParserConditions<SqlParserCombinations<T>>{this->vMap(__func__)};
                cond->parent=this;
                this->setPointer(__func__, cond);
            }
            return *cond;
        }

        void make()
        {
        }

        virtual QStringList toScript(SqlSuitableKeyWord &parser)
        {

            QVariantMap map;
            {//convercao para mapa
                auto typeId=this->typeId();
                switch (typeId) {
                case QMetaType::QVariantList:
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

            auto kCombine=KeywordCombine(map.value(QStringLiteral("combine")).toInt());
            auto kCombineAlias=map.value(QStringLiteral("alias")).toString();
            auto kCombineConditions=getVariantStartsWith(QStringLiteral("condition"), map).toMap();

            auto kCombineOn=map.value(QStringLiteral("on"));
            const auto &modelInfo=QOrm::ModelInfo::from(kCombineOn);
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
                    return QStringList{QStringLiteral("%1 %2").arg(connection, tableName)};
                return QStringList{QStringLiteral("%1 %2 as %3").arg(connection, tableName, kCombineAlias)};
            default:
                if (kCombineCheckList.contains(kCombine)){
                    if(kCombine!=KeywordCombine::kcWhere){
                        if(kCombineAlias.isEmpty())
                            connection=QStringLiteral("%1 %2").arg(connection, tableName);
                        else
                            connection=QStringLiteral("%1 %2 as %3").arg(connection, tableName, kCombineAlias);
                    }

                    QStringList lstCond;

                    QMapIterator<QString, QVariant> i(kCombineConditions);
                    bool first=true;
                    while (i.hasNext()) {
                        i.next();
                        const auto &map=i.value();
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
                        lstCond.append(line);
                    }

                    if(kCombine==KeywordCombine::kcJoinCross){
                        return QStringList{connection};
                    }

                    if(lstCond.isEmpty())
                        return {};

                    QStringList output;
                    if(kCombine==KeywordCombine::kcWhere)
                        output.append(connection);
                    else
                        output.append(connection+QStringLiteral(" on "));
                    for(auto &v:lstCond)
                        output.append(v);
                    return output;
                }
            }
            return {};
        }
    };


    explicit SqlParserCombinations(const QVariant &v):SqlParserCommand{v}
    {
    }

    auto &limit(int limit)
    {
        if(limit>0){
            auto map=this->toMap();
            map.insert(__func__, limit);
            this->setValue(map);
        }
        return *this;
    }

    auto &from()
    {
        return this->from({});
    }

    auto &from(const QVariant &fromObject)
    {
        return this->from(fromObject, QString{});
    }

    auto &from(const QVariant &fromObject, const QString &alias)
    {
        SqlParserCombination *c=nullptr;
        QMapIterator<QString, SqlParserCommand*> i(this->mPointer());
        while (i.hasNext()) {
            i.next();
            if(i.key().startsWith(__func__)){
                c=static_cast<SqlParserCombination*>(i.value());
                break;
            }
        }
        if(c==nullptr){
            c = new SqlParserCombination{__func__, this, KeywordCombine::kcFrom, fromObject, alias};
            this->setPointer(c->sName(), c);
        }
        return *this;
    }

    auto &where()
    {
        return this->where({});
    }
    auto &where(const QVariant &whereObject)
    {
        SqlParserCombination *c=nullptr;
        QMapIterator<QString, SqlParserCommand*> i(this->mPointer());
        while (i.hasNext()) {
            i.next();
            if(i.key().startsWith(__func__)){
                c=static_cast<SqlParserCombination*>(i.value());
                break;
            }
        }
        if(c==nullptr){
            c=new SqlParserCombination{__func__, this, KeywordCombine::kcWhere, whereObject};
            this->setPointer(c->sName(), c);
        }
        else if(!whereObject.isValid()){
            c->condition().condition(whereObject);
        }
        return c->condition();
    }

    SqlParserCombination &joinInner(const QVariant &on)
    {
        auto c=new SqlParserCombination{QStringLiteral("join"), this, KeywordCombine::kcJoinInner, on};
        this->setPointer(c->sName(), c);
        return *c;
    }
    SqlParserCombination &joinLeft(const QVariant &on)
    {
        auto c=new SqlParserCombination{QStringLiteral("join"), this, KeywordCombine::kcJoinLeft, on};
        this->setPointer(c->sName(), c);
        return *c;
    }
    SqlParserCombination &joinOuter(const QVariant &on)
    {
        auto c=new SqlParserCombination{QStringLiteral("join"), this, KeywordCombine::kcJoinOuter, on};
        this->setPointer(c->sName(), c);
        return *c;
    }
    SqlParserCombination &joinFull(const QVariant &on)
    {
        auto c=new SqlParserCombination{QStringLiteral("join"), this, KeywordCombine::kcJoinFull, on};
        this->setPointer(c->sName(), c);
        return *c;
    }
    SqlParserCombination &joinCross(const QVariant &on)
    {
        auto c=new SqlParserCombination{QStringLiteral("join"), this, KeywordCombine::kcJoinCross, on};
        this->setPointer(c->sName(), c);
        return *c;
    }
    SqlParserCombination &workTable(const QVariant &workTable)
    {
        auto c=new SqlParserCombination{__func__, this, KeywordCombine::kcWorkTable, workTable};
        this->setPointer(c->sName(), c);
        return *c;
    }
    SqlParserCombination &cte(const QVariant &cte)
    {
        auto c=new SqlParserCombination{__func__, this, KeywordCombine::kcCte, cte};
        this->setPointer(c->sName(), c);
        return *c;
    }

    SqlParserCombination &combination()
    {
        auto v=(this->oPointer<SqlParserCombination*>(__func__));
        if(v==nullptr){
            v=new SqlParserCombination{};
            this->setPointer(__func__, v);
        }
        return *v;
    }

    void m()
    {
        this->make();
    }

    void make()
    {
        auto map=this->toHash();
        map.insert(QStringLiteral("combination"), this->combination());
        this->setValue(map);
    }

    QStringList toScript(SqlSuitableKeyWord &parser)
    {
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
            auto kCombine=KeywordCombine(map.value(QStringLiteral("combine")).toInt());
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
        for(auto &v:lstFrom)
            lst.append(v);
        for(auto &v:lstJoin)
            lst.append(v);
        for(auto &v:lstWhere)
            lst.append(v);

        QStringList output;
        for(auto &vCombine:lst){
            auto combine=SqlParserCombinations::SqlParserCombination{vCombine};
            output.append(combine.toScript(parser));
        }

        return output;
    }
};

template <class T>
class SqlParserFrom:public SqlParserCombinations<T>{
public:

    explicit SqlParserFrom(const QVariant &v={}):SqlParserCombinations<T>{v}
    {
    }

    auto &distinct()
    {
        auto map=this->toMap();
        map.insert(QStringLiteral("distinct"), true);
        this->setValue(map);
        return *dynamic_cast<T*>(this);
    }

    auto &distinct(bool value)
    {
        auto map=this->toMap();
        map.insert(QStringLiteral("distinct"), value);
        this->setValue(map);
        return *dynamic_cast<T*>(this);
    }
};


class SqlParserCallObject:public SqlParserCommand{
public:

    auto &database(const QVariant &v)
    {
        auto object=this->oPointer<SqlParserCommand*>(__func__);
        if(object==nullptr){
            object=new SqlParserCommand{v};
            this->setPointer(__func__,object);
        }
        return *this;
    }

    auto &schema(const QVariant &v)
    {
        auto object=this->oPointer<SqlParserCommand*>(__func__);
        if(object==nullptr){
            object=new SqlParserCommand{v};
            this->setPointer(__func__,object);
        }
        return *this;
    }

    auto &object(const QVariant &v)
    {
        auto object=this->oPointer<SqlParserCommand*>(__func__);
        if(object==nullptr){
            object=new SqlParserCommand{v};
            this->setPointer(__func__,object);
        }
        return *this;
    }

    SqlParserValues &values(const QVariant &values)
    {
        auto object=this->oPointer<SqlParserValues*>(__func__);
        if(object==nullptr){
            object=new SqlParserValues{};
            this->setPointer(__func__,object);
        }
        return object->v(values);
    }

    SqlParserValues &values()
    {
        return this->values({});
    }

    SqlParserValues &v()
    {
        return this->values();
    }

    explicit SqlParserCallObject(const QVariant &v={}):SqlParserCommand{v}
    {
    }
};

}

