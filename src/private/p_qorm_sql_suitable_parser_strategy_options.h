#pragma once

#include "./p_qorm_sql_suitable_parameter.h"
#include "./p_qorm_sql_suitable_parser_command.h"
#include "./p_qorm_sql_suitable_parser_item.h"
#include "./p_qorm_sql_suitable_parser_object.h"
#include "./p_qorm_sql_suitable_parser_field.h"
#include "./p_qorm_sql_suitable_parser_value.h"

namespace QOrm {

template<class TemplateParent>
class SqlParserConditions:public SqlParserCommand{
public:
    class SqlParserCondition: public SqlParserCommand{
    public:
        QString funcName;
        QString sName()const
        {
            return this->funcName+this->suuid(qsl("."));
        }
        explicit SqlParserCondition(const QVariant &v = QVariant()):SqlParserCommand(v)
        {
        }
        explicit SqlParserCondition(QString funcName, const QVariant&field, const QVariant &valueA, const QVariant &valueB, const KeywordOperator&keywordOperator, const KeywordLogical&keywordLogical):SqlParserCommand()
        {
            auto map=this->toMap();
            this->funcName=funcName;
            map.insert(qsl("funcName"),funcName);
            map.insert(qsl("field"),field);
            map.insert(qsl("valueA"),valueA);
            map.insert(qsl("valueB"),valueB);
            map.insert(qsl("keywordOperator"),keywordOperator);
            map.insert(qsl("keywordLogical"),keywordLogical);
            this->setValue(map);
        }

        explicit SqlParserCondition(QString funcName, const QVariant &valueA, const QVariant &valueB, const KeywordOperator&keywordOperator, const KeywordLogical&keywordLogical):SqlParserCommand()
        {
            auto map=this->toMap();
            this->funcName=funcName;
            map.insert(qsl("funcName"),funcName);
            map.insert(qsl("valueA"),valueA);
            map.insert(qsl("valueB"),valueB);
            map.insert(qsl("keywordOperator"),keywordOperator);
            map.insert(qsl("keywordLogical"),keywordLogical);
            this->setValue(map);
        }

        QStringList toScript(SqlSuitableKeyWord &parser)
        {
            auto map=this->toMap();
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

            QVariant valueBval;
            switch (valueBTypeId) {
            case QMetaType_QVariantHash:
            case QMetaType_QVariantMap:
            {
                auto v=valueB.toHash();
                if(v.contains(qsl("value"))){
                    valueBval=valueB.toHash().value(qsl("value"));
                    break;
                }
                valueBval=v.values();
                break;
            }
            default:
                valueBval=valueB;
                break;
            }

            auto valueBvalTypeId=qTypeId(valueBval);

            switch (valueBvalTypeId) {
            case QMetaType_QVariantHash:
            case QMetaType_QVariantMap:
                valueBval=valueBval.toHash().values();
                valueBvalTypeId=qTypeId(valueBval);
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
                        case QMetaType_QVariantList:
                        case QMetaType_QStringList:
                            return KeywordOperator::koIn;
                        default:
                            break;
                        }

                        switch (valueBvalTypeId) {
                        case QMetaType_QVariantList:
                        case QMetaType_QStringList:
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
                    case QMetaType_QVariantList:
                    case QMetaType_QStringList:
                    {

                        switch (valueBvalTypeId) {
                        case QMetaType_QVariantList:
                        case QMetaType_QStringList:
                            break;
                        default:
                            QORM_VARIABLE_INVERTER(valueA, valueB)
                            QORM_VARIABLE_INVERTER(valueATypeId, valueBTypeId)
                            QORM_VARIABLE_INVERTER(valueAvalTypeId, valueBvalTypeId)
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
            auto keywordLogical = KeywordLogical ( map.contains(qsl("keywordLogical"))?map.value(qsl("keywordLogical") ).toInt(): -1 );

            auto kOperator = parser.parserCommand(keywordOperator );
            auto kLogical = parser.parserCommand(keywordLogical );

            static const auto vLikeNulls=QVector<QString>{qsl_null, qsl("''"), qsl("'%'"), qsl("'%%'")};

            switch (keywordOperator) {
            case KeywordOperator::koLike:
            {
                if(vLikeNulls.contains(vValueB) || vLikeNulls.contains(vValueA))
                    return {};
                break;
            }
            case koIsNull:
            case koIsNotNull:
                return qvsl{qsl(" %1 %2").arg(kLogical, kOperator.arg(vValueA)).trimmed()};
            case koBetween:
            case koNotBetween:
                return qvsl{qsl(" %1 %2").arg(kLogical, kOperator.isEmpty()?kOperator:(kOperator.arg(vField, vValueA, vValueB))).trimmed()};
            default:
                break;
            }
            return qvsl{qsl(" %1 %2").arg(kLogical, kOperator.isEmpty()?kOperator:(kOperator.arg(vValueA, vValueB))).trimmed()};
            //if((keywordOperator==koLike && vLikeNulls.contains(vValueB)) || (keywordOperator==koLike && vLikeNulls.contains(vValueA)))
            //    return {};
            //
            //if(keywordOperator==koIsNull || keywordOperator==koIsNotNull)
            //    return QStringList{qsl(" %1 %2").arg(kLogical, kOperator.arg(vValueA)).trimmed()};
            //
            //if(keywordOperator==koBetween || keywordOperator==koNotBetween)
            //    return QStringList{qsl(" %1 %2").arg(kLogical, kOperator.isEmpty()?kOperator:(kOperator.arg(vField, vValueA, vValueB))).trimmed()};
            //
            //return QStringList{qsl(" %1 %2").arg(kLogical, kOperator.isEmpty()?kOperator:(kOperator.arg(vValueA, vValueB))).trimmed()};
        }

        virtual void defineFirst()
        {
            auto map=this->toHash();
            map.insert(qsl("first"), true);
            this->setValue(map);
        }
    };

public:
    void*parent=nullptr;
    explicit SqlParserConditions(const QVariant &v):SqlParserCommand(v)
    {
    }
    explicit SqlParserConditions(void*parent, const QString &name, const QVariant &v=QVariant()):SqlParserCommand(v)
    {
        this->parent=parent;
        Q_UNUSED(name)
    }

    auto &condition(const QVVM&v)
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
        return*this;
    }
    auto &equal(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        KeywordOperator keywordOperator;
        switch (qTypeId(valueA)) {
        case QMetaType_QVariantList:
        case QMetaType_QStringList:
            keywordOperator=KeywordOperator::koIn;
            break;
        default:
            switch (qTypeId(valueB)) {
            case QMetaType_QVariantList:
            case QMetaType_QStringList:
                keywordOperator=KeywordOperator::koIn;
                break;
            default:
                keywordOperator=KeywordOperator::koEqual;
            }
        }
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, keywordOperator, keywordLogical));
    }
    auto &notEqual(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        KeywordOperator keywordOperator;
        switch (qTypeId(valueA)) {
        case QMetaType_QVariantList:
        case QMetaType_QStringList:
            keywordOperator=KeywordOperator::koInOut;
            break;
        default:
            switch (qTypeId(valueB)) {
            case QMetaType_QVariantList:
            case QMetaType_QStringList:
                keywordOperator=KeywordOperator::koInOut;
                break;
            default:
                keywordOperator=KeywordOperator::koNotEqual;
            }
        }
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, keywordOperator, keywordLogical));
    }
    auto &equalBigger(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koEqualBigger, keywordLogical));
    }
    auto &equalMinor(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koEqualMinor, keywordLogical));
    }
    auto &bigger(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koBigger, keywordLogical));
    }
    auto &minor(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koMinor, keywordLogical));
    }
    auto &like(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koLike, keywordLogical));
    }
    auto &soundex(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koSoundexEqual, keywordLogical));
    }
    auto &in(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koIn, keywordLogical));
    }
    auto &in(const QVariant &valueA, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        auto valueB=SqlParserObject(qsl(":%1").arg(valueA.toString()));
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koIn, keywordLogical));
    }
    auto &inOut(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koInOut, keywordLogical));
    }
    auto &inOut(const QVariant &valueA,const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        auto valueB=SqlParserObject(qsl(":%1").arg(valueA.toString()));
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koInOut, keywordLogical));
    }
    auto &isNull(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koIsNull, keywordLogical));
    }
    auto &isNull(const QVariant &valueA, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        auto valueB=SqlParserObject(qsl(":%1").arg(valueA.toString()));
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koIsNull, keywordLogical));
    }
    auto &isNotNull(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koIsNotNull, keywordLogical));
    }
    auto &isNotNull(const QVariant &valueA, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        auto valueB=SqlParserObject(qsl(":%1").arg(valueA.toString()));
        return this->addCondition(new SqlParserCondition(__func__, valueA, valueB, KeywordOperator::koIsNotNull, keywordLogical));
    }
    SqlParserConditions<TemplateParent>&between(const QVariant&field, const QVariant &valueA, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        switch (qTypeId(valueA)) {
        case QMetaType_QDateTime:
        case QMetaType_QDate:
        case QMetaType_QTime:
        {
            auto valueB=QDateTime(valueA.toDateTime().date(), QTime(23,59,59,998));
            return this->addCondition(new SqlParserCondition(__func__, field, valueA, valueB, KeywordOperator::koBetween, keywordLogical));
        }
        default:
            return this->equal(field, valueA, keywordLogical);
        }
    }
    auto &between(const QVariant&field, const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
    {
        return this->addCondition(new SqlParserCondition(__func__, field, valueA, valueB, KeywordOperator::koBetween, keywordLogical));
    }

    QStringList toScript(SqlSuitableKeyWord &parser)
    {
        auto list = this->toList();
        if(list.isEmpty())
            return {};
        QStringList output;
        output<<parser.parserCommand(KeywordCombine::kcWhere);
        auto first=true;
        for(auto &v:list){
            auto map=v.toHash();
            if(first){
                map.remove(qsl("keywordLogical"));
                first=false;
            }
            SqlParserCondition condition(map);
            output<<condition.toScript(parser);
        }
        return output;
    }
private:
    auto &addCondition(SqlParserCondition*v)
    {
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
        QString sName()
        {
            return this->funcName+this->suuid(qsl("."));
        }

        SqlParserCombinations<T> * parent=nullptr;
        explicit SqlParserCombination(const QVariant &v=QVariant()):SqlParserCommand(v)
        {
            this->parent=nullptr;
            this->makeUuid();
        }

        explicit SqlParserCombination(const QString &funcName, SqlParserCombinations<T> * parent, const KeywordCombine&combine, const QVariant&condition, const QVariant&alias=QVariant()):SqlParserCommand()
        {
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

            if(alias.isValid())
                map.insert(qsl("alias"), alias);

            map.insert(qsl("on"),condition);
            this->setValue(map);
        }

        auto &alias(const QString &alias)
        {
            auto map=this->toHash();
            map.insert(qsl("alias"), alias);
            this->setValue(map);
            return*this;
        }

        auto &makeOperator(const KeywordOperator&keywordOperator=KeywordOperator::koEqual)
        {
            auto map=this->toHash();
            map.insert(qsl("keywordOperator"),keywordOperator);
            this->setValue(map);
            return*this;
        }

        auto &makeLogical(const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
        {
            auto map=this->toHash();
            map.insert(qsl("keywordLogical"),keywordLogical);
            this->setValue(map);
            return*this;
        }

        auto &equal(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
        {
            this->c().equal(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &equalBigger(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
        {
            this->c().equalBigger(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &equalMinor(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
        {
            this->c().equalMinor(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &bigger(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
        {
            this->c().bigger(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &minor(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
        {
            this->c().minor(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &like(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical)
        {
            this->c().like(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &in(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
        {
            this->c().in(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &in(const QVariant &valueA, const KeywordLogical&keywordLogical=KeywordLogical::klAnd){
            this->c().in(valueA, keywordLogical);
            return*this;
        }
        auto &inOut(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
        {
            this->c().inOut(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &inOut(const QVariant &valueA,const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
        {
            this->c().inOut(valueA, keywordLogical);
            return*this;
        }
        auto &isNull(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
        {
            this->c().isNull(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &isNull(const QVariant &valueA, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
        {
            this->c().isNull(valueA, keywordLogical);
            return*this;
        }
        auto &notNull(const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
        {
            this->c().isNotNull(valueA, valueB, keywordLogical);
            return*this;
        }
        auto &notNull(const QVariant &valueA, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
        {
            this->c().isNotNull(valueA, keywordLogical);
            return*this;
        }
        auto &between(const QVariant&field, const QVariant &valueA, const QVariant &valueB, const KeywordLogical&keywordLogical=KeywordLogical::klAnd)
        {
            this->c().between(field, valueA, valueB, keywordLogical);
            return*this;
        }

        SqlParserConditions<SqlParserCombinations<T>>&c()
        {
            return this->condition();
        }

        SqlParserConditions<SqlParserCombinations<T>>&condition()
        {
            auto cond=(this->oPointer<SqlParserConditions<SqlParserCombinations<T>>*>(__func__));
            if(cond==nullptr){
                cond=new SqlParserConditions<SqlParserCombinations<T>>(this->vMap(__func__));
                cond->parent=this;
                this->setPointer(__func__, cond);
            }
            return*cond;
        }

        void make()
        {
        }

        virtual QStringList toScript(SqlSuitableKeyWord&parser)
        {

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
            const auto &modelInfo=QOrm::ModelInfo::modelInfo(kCombineOn);
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
                    for(auto &v:lstCond)
                        output<<v;
                    return output;
                }
            }
            return {};
        }
    };


    explicit SqlParserCombinations(const QVariant &v):SqlParserCommand(v)
    {
    }

    auto &limit(int limit)
    {
        if(limit>0){
            auto map=this->toMap();
            map.insert(__func__, limit);
            this->setValue(map);
        }
        return*this;
    }

    auto &from()
    {
        return this->from(QVariant());
    }

    auto &from(const QVariant&fromObject)
    {
        return this->from(fromObject, QString());
    }

    auto &from(const QVariant&fromObject, const QString &alias)
    {
        SqlParserCombination*c=nullptr;
        QMapIterator<QString, SqlParserCommand*> i(this->mPointer());
        while (i.hasNext()) {
            i.next();
            if(i.key().startsWith(__func__)){
                c=static_cast<SqlParserCombination*>(i.value());
            }
        }
        if(c==nullptr){
            c = new SqlParserCombination(__func__, this, KeywordCombine::kcFrom, fromObject, alias);
            this->setPointer(c->sName(), c);
        }
        return*this;
    }

    auto &where()
    {
        return this->where(QVariant());
    }
    auto &where(const QVariant&whereObject)
    {
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

    SqlParserCombination&joinInner(const QVariant&on)
    {
        auto c=new SqlParserCombination(qsl("join"), this, KeywordCombine::kcJoinInner, on);
        this->setPointer(c->sName(), c);
        return*c;
    }
    SqlParserCombination&joinLeft(const QVariant&on)
    {
        auto c=new SqlParserCombination(qsl("join"), this, KeywordCombine::kcJoinLeft, on);
        this->setPointer(c->sName(), c);
        return*c;
    }
    SqlParserCombination&joinOuter(const QVariant&on)
    {
        auto c=new SqlParserCombination(qsl("join"), this, KeywordCombine::kcJoinOuter, on);
        this->setPointer(c->sName(), c);
        return*c;
    }
    SqlParserCombination&joinFull(const QVariant&on)
    {
        auto c=new SqlParserCombination(qsl("join"), this, KeywordCombine::kcJoinFull, on);
        this->setPointer(c->sName(), c);
        return*c;
    }
    SqlParserCombination&joinCross(const QVariant&on)
    {
        auto c=new SqlParserCombination(qsl("join"), this, KeywordCombine::kcJoinCross, on);
        this->setPointer(c->sName(), c);
        return*c;
    }
    SqlParserCombination&workTable(const QVariant&workTable)
    {
        auto c=new SqlParserCombination(__func__, this, KeywordCombine::kcWorkTable, workTable);
        this->setPointer(c->sName(), c);
        return*c;
    }
    SqlParserCombination&cte(const QVariant&cte)
    {
        auto c=new SqlParserCombination(__func__, this, KeywordCombine::kcCte, cte);
        this->setPointer(c->sName(), c);
        return*c;
    }

    SqlParserCombination&combination()
    {
        auto v=(this->oPointer<SqlParserCombination*>(__func__));
        if(v==nullptr){
            v=new SqlParserCombination();
            this->setPointer(__func__, v);
        }
        return*v;
    }

    void m()
    {
        this->make();
    }

    void make()
    {
        auto map=this->toHash();
        map.insert(qsl("combination"), this->combination());
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
        for(auto &v:lstFrom)
            lst<<v;
        for(auto &v:lstJoin)
            lst<<v;
        for(auto &v:lstWhere)
            lst<<v;

        QStringList output;
        for(auto &vCombine:lst){
            auto combine=SqlParserCombinations::SqlParserCombination(vCombine);
            output<<combine.toScript(parser);
        }

        return output;
    }
};

template <class T>
class SqlParserFrom:public SqlParserCombinations<T>{
public:

    explicit SqlParserFrom(const QVariant &v=QVariant()):SqlParserCombinations<T>(v)
    {
    }
    /**
     * @brief distinct
     * @return
     */
    auto &distinct()
    {
        auto map=this->toMap();
        map.insert(qsl("distinct"), true);
        this->setValue(map);
        return*dynamic_cast<T*>(this);
    }
    /**
     * @brief distinct
     * @return
     */
    auto &distinct(bool value)
    {
        auto map=this->toMap();
        map.insert(qsl("distinct"), value);
        this->setValue(map);
        return*dynamic_cast<T*>(this);
    }
};


class SqlParserCallObject:public SqlParserCommand{
public:

    auto &database(const QVariant &v)
    {
        auto object=this->oPointer<SqlParserCommand*>(__func__);
        if(object==nullptr){
            object=new SqlParserCommand(v);
            this->setPointer(__func__,object);
        }
        return*this;
    }

    auto &schema(const QVariant &v)
    {
        auto object=this->oPointer<SqlParserCommand*>(__func__);
        if(object==nullptr){
            object=new SqlParserCommand(v);
            this->setPointer(__func__,object);
        }
        return*this;
    }

    auto &object(const QVariant &v)
    {
        auto object=this->oPointer<SqlParserCommand*>(__func__);
        if(object==nullptr){
            object=new SqlParserCommand(v);
            this->setPointer(__func__,object);
        }
        return*this;
    }

    SqlParserValues&values(const QVariant &values)
    {
        auto object=this->oPointer<SqlParserValues*>(__func__);
        if(object==nullptr){
            object=new SqlParserValues(QVariant());
            this->setPointer(__func__,object);
        }
        return object->v(values);
    }

    SqlParserValues&values()
    {
        return this->values(QVariant());
    }

    SqlParserValues&v()
    {
        return this->values();
    }

    explicit SqlParserCallObject():SqlParserCommand()
    {
    }

    explicit SqlParserCallObject(const QVariant &v=QVariant()):SqlParserCommand(v)
    {
    }
};

}

