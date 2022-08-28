#include "./p_qorm_sql_suitable_parameter.h"
#include "./p_qorm_sql_suitable_parser_item.h"
#include "../../../qstm/src/qstm_util_variant.h"
#include "../../../qstm/src/qstm_macro.h"
#include <QJsonDocument>

static const auto __valueA="valueA";
static const auto __valueB="valueB";
static const auto __valueC="valueC";
static const auto __keywordOperator="keywordOperator";
static const auto __keywordLogical="keywordLogical";
static const auto __percentage="%";


class SearchParametersPvt{
public:
    QVariantList values;
    QList<SearchParam> vList;
    explicit SearchParametersPvt()
    {
    }
    virtual ~SearchParametersPvt()
    {
    }

    void insert(const QVariant &valueA, const QVariant &valueB, const QVariant &valueC, const QVariant &keywordOperator, const QVariant &keywordLogical)
    {
        SearchParam vHash(valueA, valueB, valueC, keywordOperator, keywordLogical);
        if(vHash.isValid())
            this->values.append(vHash);
    }

    void remove(const QVariant &searchField)
    {
        Q_UNUSED(searchField)
        this->values.clear();
    }

    static bool canRead(const QVariant &v)
    {
        switch (v.typeId()) {
        case QMetaType::QVariantList:
        {
            for(auto &v:v.toList()){
                auto s=SearchParam::from(v);
                if(v.isValid())
                    return true;
            }
            break;
        }
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
        {
            Q_V_HASH_ITERATOR (v.toHash()){
                i.next();
                auto v=i.value().toHash();
                if(v.contains(__valueA) || v.contains(__valueB) || v.contains(__valueC))
                    return true;

//                auto s=SearchParam::from(v);
//                if(s.isValid())
//                    return true;

//                auto va=QOrm::SqlParserItem::from(s.valueA());
//                auto vb=QOrm::SqlParserItem::from(s.valueB());
//                auto vc=QOrm::SqlParserItem::from(s.valueC());
//                if(va.isValid() || vb.isValid() || vc.isValid())
//                    return true;
            }
            break;
        }
        default:
            break;
        }
        return false;
    }

    void add(const QVariant &v)
    {
        Q_DECLARE_VU;
        switch (v.typeId()) {
        case QMetaType::QVariantList:
        {
            auto vList=v.toList();
            for(auto &v:vList) {
                SearchParam param{v.toHash()};
                if(param.isValid())
                    this->values.append(v);
            }
            break;
        }
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
        {
            auto vHash=v.toHash();
            if(this->canRead(vHash))
                this->values.append(v);
            else{//qvvm
                Q_V_HASH_ITERATOR (vHash){
                    i.next();
                    auto v1=vu.toVariant(i.key());
                    auto v2=vu.toVariant(i.value());

                    auto opr=QOrm::koEqual;
                    switch (v2.typeId()) {
                    case QMetaType::QVariantList:
                    case QMetaType::QStringList:
                        opr=QOrm::koIn;
                        break;
                    default:
                        break;
                    }

                    SearchParam param{v1, v2, opr, QOrm::klAnd};
                    if(param.isValid())
                        this->values.append(param);
                }
            }

            break;
        }
        default:
            break;
        }
    }
};


SearchParam::SearchParam(const QVariant &v):QVariantHash(v.toHash())
{
}

SearchParam::SearchParam(const QVariant &valueA, const QVariant &valueB, const QVariant &valueC, const QVariant &keywordOperator, const QVariant &keywordLogical):
    QVariantHash{
                    {__valueA, valueA},
                    {__valueB, valueB},
                    {__valueC, valueC},
                    {__keywordOperator, keywordOperator},
                    {__keywordLogical, keywordLogical}
                }
{
}

SearchParam::SearchParam(const QVariant &valueA, const QVariant &valueB, const QVariant &keywordOperator, const QVariant &keywordLogical):
    QVariantHash{
            {__valueA, valueA},
            {__valueB, valueB},
            {__keywordOperator, keywordOperator},
            {__keywordLogical, keywordLogical}
        }
{
}

SearchParam::~SearchParam()
{
}

bool SearchParam::isValid()
{
    auto va=this->valueA();
    if((va.typeId()==QMetaType::QVariantHash || va.typeId()==QMetaType::QVariantMap) && (!va.toHash().isEmpty()))
        return true;

    if((va.typeId()==QMetaType::QVariantList || va.typeId()==QMetaType::QStringList) && (!va.toList().isEmpty()))
        return true;

    auto vb=this->valueB();
    if((vb.typeId()==QMetaType::QVariantHash || vb.typeId()==QMetaType::QVariantMap) && (!vb.toHash().isEmpty()))
        return true;

    if((vb.typeId()==QMetaType::QVariantList || vb.typeId()==QMetaType::QStringList) && (!vb.toList().isEmpty()))
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

    return {};
}

QVariant SearchParam::valueA() const
{
    return this->value(__valueA);
}

QVariant SearchParam::valueB() const
{
    return this->value(__valueB);
}

QVariant SearchParam::valueC() const
{
    return this->value(__valueC);
}

QOrm::KeywordOperator SearchParam::keywordOperator() const
{
    auto v = this->value(__keywordOperator);
    auto i = v.toInt();//QOrm::KeywordOperator();
    if(QOrm::KeywordOperators.contains(i))
        return QOrm::KeywordOperator(i);

    return QOrm::koEqual;
}

QOrm::KeywordLogical SearchParam::keywordLogical() const
{
    auto v = this->value(__keywordLogical);
    auto i = QOrm::KeywordLogical(v.toInt());
    if(i==QOrm::klAnd)
        return i;

    if(i==QOrm::klOr)
        return i;

    return QOrm::klAnd;
}

SearchParam SearchParam::from(const QVariant &value)
{
    QVariantHash vHash;
    switch (value.typeId()) {
    case QMetaType::QString:
    case QMetaType::QByteArray:
        vHash=QJsonDocument::fromJson(value.toByteArray()).toVariant().toHash();
        break;
    default:
        vHash=value.toHash();
    }

    if(vHash.isEmpty())
        return SearchParam{};

    if(vHash.contains(__valueA) || vHash.contains(__valueB))
        return SearchParam{vHash};

    if(vHash.size()==1)
        return SearchParam{vHash.begin().key(), vHash.begin().value(), {}, {}};

    return SearchParam{};
}

SearchParameters::SearchParameters(const QVariant &other):QVariant{}
{
    this->p = new SearchParametersPvt{};
    p->add(other);
//    QVariant vOther;
//    switch (other.typeId()) {
//    case QMetaType::QString:
//    case QMetaType::QByteArray:
//        vOther=QJsonDocument::fromJson(other.toByteArray()).toVariant();
//        break;
//    default:
//        vOther=other;
//    }


//    switch (vOther.typeId()) {
//    case QMetaType::QVariantList:
//    {
//        for(auto &v:vOther.toList()){
//            auto s=SearchParam::from(v);
//            if(v.isValid())
//                p->insert(s.valueA(), s.valueB(), s.valueC(), s.keywordOperator(), s.keywordLogical());
//        }
//        break;
//    }
//    case QMetaType::QVariantHash:
//    case QMetaType::QVariantMap:
//    {
//        Q_V_HASH_ITERATOR (vOther.toHash()){
//            i.next();
//            auto s=SearchParam::from(i.value());
//            if(!s.isValid()){
//                const auto &k=i.key();
//                const auto &v=i.value();

//                auto keywordOperator=QOrm::koEqual;
//                auto keywordLogical=QOrm::klAnd;

//                auto vA=QOrm::SqlParserItem::from(k);
//                auto vB=QOrm::SqlParserItem::from(v);

//                if(!vA.value().isValid() && !vB.value().isValid())
//                    continue;

//                if(vA.isValue() && QMetaTypeUtilString.contains(vA.typeId())){
//                    auto s=vA.value().toString();
//                    if(!s.contains(__percentage))
//                        s+=__percentage;
//                    keywordOperator=QOrm::koLike;
//                    vB.setValue(s);
//                }

//                switch (vA.typeId()){
//                case QMetaType::QString:
//                case QMetaType::QByteArray:
//                case QMetaType::QChar:
//                case QMetaType::QBitArray:
//                {
//                    if(vB.isValue() && QMetaTypeUtilString.contains(vB.typeId())){
//                        auto s=vB.value().toString();
//                        if(!s.contains(__percentage))
//                            s+=__percentage;
//                        keywordOperator=QOrm::koLike;
//                        vB.setValue(s);
//                    }
//                    break;
//                }
//                default:
//                    break;
//                }


//                s=SearchParam{vA, vB, keywordOperator, keywordLogical};
//            }
//            p->insert(s.valueA(), s.valueB(), s.valueC(), s.keywordOperator(), s.keywordLogical());
//        }
//        break;
//    }
//    default:
//        break;
//    }
}

SearchParameters::~SearchParameters()
{
    delete p;
}

bool SearchParameters::canRead(const QVariant &v)
{
    return SearchParametersPvt::canRead(v);
}

bool SearchParameters::isEmpty()const
{
    return p->values.isEmpty();
}

SearchParameters &SearchParameters::operator=(const QVariant &v)
{
    QVariant::clear();
    p->values.clear();
    p->add(v);
    return *this;
}

SearchParameters &SearchParameters::operator+=(const QVariant &v)
{
    p->add(v);
    return *this;
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
    p->insert(valueA, valueB, {}, {}, {});
    return *this;
}

SearchParameters &SearchParameters::insert(const QVariant &valueA, const QVariant &valueB, const QVariant &keywordOperator)
{
    p->insert(valueA, valueB, {}, keywordOperator, {});
    return *this;
}

SearchParameters &SearchParameters::insert(const QVariant &valueA, const QVariant &valueB, const QVariant &keywordOperator, const QVariant &keywordLogical)
{
    p->insert(valueA, valueB, QVariant{}, keywordOperator, keywordLogical);
    return *this;
}

SearchParameters &SearchParameters::insert(const QVariant &valueA, const QVariant &valueB, const QVariant &valueC, const QVariant &keywordOperator, const QVariant &keywordLogical)
{
    p->insert(valueA, valueB, valueC, keywordOperator, keywordLogical);
    return *this;
}

QList<SearchParam> SearchParameters::build() const
{
    p->vList.clear();
    for(auto &i:p->values){
        p->vList.append(SearchParam::from(i));
    }
    return p->vList;
}

QVariant SearchParameters::buildVariant() const
{
    p->vList.clear();
    QVariantList vList;
    for(auto &i:p->values){
        auto v=i.toHash();
        p->vList.append(SearchParam::from(v));
        vList.append(v);
    }
    return vList.isEmpty()?QVariant{}:vList;
}

