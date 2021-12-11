#include "./p_qorm_sql_suitable_parameter.h"
#include "./p_qorm_sql_suitable_parser_strategy.h"
#include "./qstm_util_variant.h"
#include <QJsonDocument>

#define dPvt()\
    auto&p = *reinterpret_cast<SearchParametersPvt*>(this->p)

#define dPvt2()\
    auto&p = *reinterpret_cast<SearchParamPvt*>(this->p)

class SearchParamPvt{
public:
    explicit SearchParamPvt(){
    }
    virtual ~SearchParamPvt(){
    }
};

class SearchParametersPvt{
public:
    QVariantList values;
    QList<SearchParam> vList;
    explicit SearchParametersPvt(){
    }
    virtual ~SearchParametersPvt(){
    }

    void insert(const QVariant &valueA, const QVariant &valueB, const QVariant &valueC, const QVariant &keywordOperator, const QVariant &keywordLogical){
        SearchParam vHash(valueA, valueB, valueC, keywordOperator, keywordLogical);
        if(vHash.isValid())
            this->values<<vHash;
    }

    void remove(const QVariant &searchField){
        Q_UNUSED(searchField)
        this->values.clear();
    }
};


SearchParam::SearchParam(const QVariant &v):QVariantHash(v.toHash())
{
}

SearchParam::SearchParam(const QVariant &valueA, const QVariant &valueB, const QVariant &valueC, const QVariant &keywordOperator, const QVariant &keywordLogical):qvh{{qsl("valueA"),valueA}, {qsl("valueB"),valueB}, {qsl("valueC"),valueC}, {qsl("keywordOperator"), keywordOperator},{qsl("keywordLogical"), keywordLogical}}
{
}

SearchParam::SearchParam(const QVariant &valueA, const QVariant &valueB, const QVariant &keywordOperator, const QVariant &keywordLogical):qvh{{qsl("valueA"),valueA}, {qsl("valueB"),valueB}, {qsl("keywordOperator"), keywordOperator},{qsl("keywordLogical"), keywordLogical}}
{
}

SearchParam::~SearchParam()
{
}

bool SearchParam::isValid()
{
    auto va=this->valueA();
    if((qTypeId(va)==QMetaType_QVariantHash || qTypeId(va)==QMetaType_QVariantMap) && (!va.toHash().isEmpty()))
        return true;

    if((qTypeId(va)==QMetaType_QVariantList || qTypeId(va)==QMetaType_QStringList) && (!va.toList().isEmpty()))
        return true;

    auto vb=this->valueB();
    if((qTypeId(vb)==QMetaType_QVariantHash || qTypeId(vb)==QMetaType_QVariantMap) && (!vb.toHash().isEmpty()))
        return true;

    if((qTypeId(vb)==QMetaType_QVariantList || qTypeId(vb)==QMetaType_QStringList) && (!vb.toList().isEmpty()))
        return true;

    return false;
}

QString SearchParam::key() const
{
    auto va=this->valueA().toString().trimmed();
    auto vb=this->valueB().toString().trimmed();
    if(!va.isEmpty())
        return va;

    if(!vb.isEmpty())
        return vb;

    return qsl_null;
}

QVariant SearchParam::valueA() const
{
    return this->value(qsl("valueA"));
}

QVariant SearchParam::valueB() const
{
    return this->value(qsl("valueB"));
}

QVariant SearchParam::valueC() const
{
    return this->value(qsl("valueC"));
}

QOrm::KeywordOperator SearchParam::keywordOperator() const
{
    auto v = this->value(qsl("keywordOperator"));
    auto i = v.toInt();//QOrm::KeywordOperator();
    if(QOrm::KeywordOperators.contains(i))
        return QOrm::KeywordOperator(i);

    return QOrm::koEqual;
}

QOrm::KeywordLogical SearchParam::keywordLogical() const
{
    auto v = this->value(qsl("keywordLogical"));
    auto i = QOrm::KeywordLogical(v.toInt());
    if(i==QOrm::klAnd)
        return i;

    if(i==QOrm::klOr)
        return i;

    return QOrm::klAnd;
}

SearchParam SearchParam::from(const QVariant &value)
{
    if(qTypeId(value)==QMetaType_QVariantHash || qTypeId(value)==QMetaType_QVariantMap){
        auto vHash=value.toHash();
        if(vHash.contains(qsl("valueA")) || vHash.contains(qsl("valueB"))){
            SearchParam __return(vHash);
            return __return;
        }

        if(vHash.size()==1){
            Q_V_HASH_ITERATOR(vHash){
                i.next();
                SearchParam __return(i.key(), i.value(), QVariant(), QVariant());
                return __return;
            }
        }
    }
    SearchParam __return;
    return __return;
}

SearchParameters::SearchParameters(const QVariant &other):QVariant()
{
    this->p = new SearchParametersPvt();
    dPvt();
    if(qTypeId(other)==QMetaType_QVariantList){
        for(auto&v:other.toList()){
            auto s=SearchParam::from(v);
            if(v.isValid())
                p.insert(s.valueA(), s.valueB(), s.valueC(), s.keywordOperator(), s.keywordLogical());
        }
    }
    else if(qTypeId(other)==QMetaType_QVariantMap || qTypeId(other)==QMetaType_QVariantHash){
        Q_V_HASH_ITERATOR (other.toHash()){
            i.next();
            auto s=SearchParam::from(i.value());
            if(!s.isValid()){
                const auto&k=i.key();
                const auto&v=i.value();

                auto keywordOperator=QOrm::koEqual;
                auto keywordLogical=QOrm::klAnd;

                auto vA=QOrm::SqlParserItem::from(k);
                auto vB=QOrm::SqlParserItem::from(v);

                if(!vA.value().isValid() && !vB.value().isValid())
                    continue;

                if(vA.isValue() && QStmTypesListString.contains(qTypeId(vA.value()))){
                    auto s=vA.value().toString();
                    if(!s.contains(qsl("%")))
                        s+=qsl("%");
                    keywordOperator=QOrm::koLike;
                    vB.setValue(s);
                }

                if(vB.isValue() && QStmTypesListString.contains(qTypeId(vB.value()))){
                    auto s=vB.value().toString();
                    if(!s.contains(qsl("%")))
                        s+=qsl("%");
                    keywordOperator=QOrm::koLike;
                    vB.setValue(s);
                }

                s=SearchParam(vA, vB, keywordOperator, keywordLogical);
            }
            p.insert(s.valueA(), s.valueB(), s.valueC(), s.keywordOperator(), s.keywordLogical());
        }
    }
}

SearchParameters::~SearchParameters()
{
    dPvt();
    delete&p;
}

bool SearchParameters::canRead(const QVariant &v)
{
    this->p = new SearchParametersPvt();
    if(qTypeId(v)==QMetaType_QVariantList){
        for(auto&v:v.toList()){
            auto s=SearchParam::from(v);
            if(v.isValid())
                return true;
        }
    }
    else if(qTypeId(v)==QMetaType_QVariantMap || qTypeId(v)==QMetaType_QVariantHash){
        Q_V_HASH_ITERATOR (v.toHash()){
            i.next();
            auto v=i.value().toHash();
            auto s=SearchParam::from(v);
            if(s.isValid())
                return true;

            auto va=QOrm::SqlParserItem::from(s.valueA());
            auto vb=QOrm::SqlParserItem::from(s.valueB());
            auto vc=QOrm::SqlParserItem::from(s.valueC());
            if(va.isValid() || vb.isValid() || vc.isValid())
                return true;
        }
    }
    return false;
}

bool SearchParameters::isEmpty()const
{
    dPvt();
    return p.values.isEmpty();
}

SearchParameters &SearchParameters::operator=(const QVariant &v)
{
    dPvt();
    Q_DECLARE_VU;
    QVariant::clear();
    p.values.clear();
    if(qTypeId(v)==QMetaType_QVariantList || qTypeId(v)==QMetaType_QStringList){
        auto vList=vu.toList(v);
        for(auto&v:vList) {
            SearchParam param(v.toHash());
            if(param.isValid())
                p.values<<v;
        }
    }
    else if(qTypeId(v)==QMetaType_QVariantHash || qTypeId(v)==QMetaType_QVariantMap){
        auto vMap=vu.toHash(v);
        Q_V_HASH_ITERATOR (vMap){
            i.next();
            auto v=i.value().toHash();
            SearchParam param(v);
            if(param.isValid()){
                p.values<<i.value();
            }
        }
    }
    return*this;
}

SearchParameters &SearchParameters::operator+=(const QVariant &v)
{
    dPvt();
    Q_DECLARE_VU;
    if(qTypeId(v)==QMetaType_QVariantList || qTypeId(v)==QMetaType_QStringList){
        auto vList=vu.toList(v);
        for(auto&v:vList) {
            SearchParam param(v.toHash());
            if(param.isValid())
                p.values<<v;
        }
    }
    else if(qTypeId(v)==QMetaType_QVariantHash || qTypeId(v)==QMetaType_QVariantMap){
        auto vMap=vu.toHash(v);
        Q_V_HASH_ITERATOR (vMap){
            i.next();
            auto v=i.value().toHash();
            SearchParam param(v);
            if(param.isValid()){
                p.values<<i.value();
            }
        }
    }
    return*this;
}

SearchParameters SearchParameters::from(const QVariantHash &v)
{
    SearchParameters vvm;
    vvm+=v;
    return vvm;
}

SearchParameters SearchParameters::from(const QVariantMap &v)
{
    SearchParameters param;
    param+=v;
    return param;
}

SearchParameters &SearchParameters::insert(const QVariant &valueA, const QVariant &valueB)
{
    dPvt();
    p.insert(valueA, valueB, QVariant(), QVariant(), QVariant());
    return*this;
}

SearchParameters &SearchParameters::insert(const QVariant &valueA, const QVariant &valueB, const QVariant &keywordOperator)
{
    dPvt();
    p.insert(valueA, valueB, QVariant(), keywordOperator, QVariant());
    return*this;
}

SearchParameters &SearchParameters::insert(const QVariant &valueA, const QVariant &valueB, const QVariant&keywordOperator, const QVariant&keywordLogical)
{
    dPvt();
    p.insert(valueA, valueB, QVariant(), keywordOperator, keywordLogical);
    return*this;
}

SearchParameters &SearchParameters::insert(const QVariant &valueA, const QVariant &valueB, const QVariant &valueC, const QVariant &keywordOperator, const QVariant &keywordLogical)
{
    dPvt();
    p.insert(valueA, valueB, valueC, keywordOperator, keywordLogical);
    return*this;
}

QList<SearchParam> SearchParameters::build() const
{
    dPvt();
    p.vList.clear();
    for(auto&i:p.values){
        p.vList << SearchParam::from(i);
    }
    return p.vList;
}

QVariant SearchParameters::buildVariant() const
{
    dPvt();
    p.vList.clear();
    QVariantList vList;
    for(auto&i:p.values){
        auto v=i.toHash();
        p.vList << SearchParam::from(v);
        vList<<v;
    }
    return vList.isEmpty()?QVariant():vList;
}

