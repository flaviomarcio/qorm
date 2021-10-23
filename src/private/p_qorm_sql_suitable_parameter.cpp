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

//    const static QString toBytes(const QVariant &v)
//    {
//        if(v.type()==QVariant::List || v.type()==QVariant::StringList || v.type()==QVariant::Map || v.type()==QVariant::Hash)
//            return QJsonDocument::fromVariant(v).toJson(QJsonDocument::Compact);
//        else if(v.type()==QVariant::Uuid)
//            return v.toUuid().toString();
//        else if(v.type()==QVariant::Url)
//            return v.toUrl().toString();
//        else
//            return v.toByteArray();
//    }

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

SearchParam::SearchParam(const QVariant &valueA, const QVariant &valueB, const QVariant &valueC, const QVariant &keywordOperator, const QVariant &keywordLogical):qvh({{qsl("valueA"),valueA}, {qsl("valueB"),valueB}, {qsl("valueC"),valueC}, {qsl("keywordOperator"), keywordOperator},{qsl("keywordLogical"), keywordLogical}})
{
}

SearchParam::SearchParam(const QVariant &valueA, const QVariant &valueB, const QVariant &keywordOperator, const QVariant &keywordLogical):qvh({{qsl("valueA"),valueA}, {qsl("valueB"),valueB}, {qsl("keywordOperator"), keywordOperator},{qsl("keywordLogical"), keywordLogical}})
{
}

SearchParam::~SearchParam()
{
}

bool SearchParam::isValid()
{
    auto va=this->valueA();
    if((va.type()==va.Map || va.type()==va.Hash) && (!va.toHash().isEmpty()))
        return true;
    else if((va.type()==va.List || va.type()==va.StringList) && (!va.toList().isEmpty()))
        return true;
    else{
        auto vb=this->valueB();
        if((vb.type()==vb.Map || vb.type()==vb.Hash) && (!vb.toHash().isEmpty()))
            return true;
        else if((vb.type()==vb.List || vb.type()==vb.StringList) && (!vb.toList().isEmpty()))
            return true;
        else
            return false;
    }
}

QString SearchParam::key() const
{
    auto va=this->valueA().toString().trimmed();
    auto vb=this->valueB().toString().trimmed();
    if(!va.isEmpty())
        return va;
    else if(!vb.isEmpty())
        return vb;
    else
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
    else
        return QOrm::koEqual;
}

QOrm::KeywordLogical SearchParam::keywordLogical() const
{
    auto v = this->value(qsl("keywordLogical"));
    auto i = QOrm::KeywordLogical(v.toInt());
    if(i==QOrm::klAnd)
        return i;
    else if(i==QOrm::klOr)
        return i;
    else
        return QOrm::klAnd;
}

SearchParam SearchParam::from(const QVariant &value)
{
    if(value.type()==value.Hash || value.type()==value.Map){
        auto vHash=value.toHash();
        if(vHash.contains(qsl("valueA")) || vHash.contains(qsl("valueB"))){
            SearchParam __return(vHash);
            return __return;
        }
        else if(vHash.size()==1){
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
    if(other.type()==other.List){
        for(auto&v:other.toList()){
            auto s=SearchParam::from(v);
            if(v.isValid())
                p.insert(s.valueA(), s.valueB(), s.valueC(), s.keywordOperator(), s.keywordLogical());
        }
    }
    else if(other.type()==other.Map || other.type()==other.Hash){
        QHashIterator<QString, QVariant> i(other.toHash());
        while (i.hasNext()) {
            i.next();
            auto s=SearchParam::from(i.value());
            if(s.isValid())
                p.insert(s.valueA(), s.valueB(), s.valueC(), s.keywordOperator(), s.keywordLogical());
            else {
                auto k=i.key();
                auto v=i.value();
                auto vA=QOrm::SqlParserItem::from(k);
                auto vB=QOrm::SqlParserItem::from(v);
                if(vA.isValid() && vB.isValid()){
                    p.insert(vA, vB, QVariant(), QVariant(), QVariant());
                }
            }
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
    if(v.type()==v.List){
        for(auto&v:v.toList()){
            auto s=SearchParam::from(v);
            if(v.isValid())
                return true;
        }
    }
    else if(v.type()==v.Map || v.type()==v.Hash){
        QHashIterator<QString, QVariant> i(v.toHash());
        while (i.hasNext()) {
            i.next();
            auto v=i.value().toHash();
            auto s=SearchParam::from(v);
            if(s.isValid())
                return true;
            else{
                auto va=QOrm::SqlParserItem::from(s.valueA());
                auto vb=QOrm::SqlParserItem::from(s.valueB());
                auto vc=QOrm::SqlParserItem::from(s.valueC());
                if(va.isValid() || vb.isValid() || vc.isValid()){
                    return true;
                }
            }
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
    VariantUtil vu;
    QVariant::clear();
    p.values.clear();
    if(v.type()==v.List || v.type()==v.StringList){
        auto vList=vu.toList(v);
        for(auto&v:vList) {
            SearchParam param(v.toHash());
            if(param.isValid())
                p.values<<v;
        }
    }
    else if(v.type()==v.Hash || v.type()==v.Map){
        auto vMap=vu.toHash(v);
        QHashIterator<QString, QVariant> i(vMap);
        while (i.hasNext()) {
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
    VariantUtil vu;
    if(v.type()==v.List || v.type()==v.StringList){
        auto vList=vu.toList(v);
        for(auto&v:vList) {
            SearchParam param(v.toHash());
            if(param.isValid())
                p.values<<v;
        }
    }
    else if(v.type()==v.Hash || v.type()==v.Map){
        auto vMap=vu.toHash(v);
        QHashIterator<QString, QVariant> i(vMap);
        while (i.hasNext()) {
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
    SearchParameters vvm;
    vvm+=v;
    return vvm;
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

