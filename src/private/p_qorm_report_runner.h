#pragma once

#include "../qorm_report_runner.h"
#include "../qorm_model_dto.h"
#include <QJsonDocument>
#include <QJsonObject>

namespace QOrm {


class RunnerItem:public QStm::Object{
public:
    const QString v_object_name=qsl("object.name");
    const QString v_object_query=qsl("object.query");
    const QString v_object_header=qsl("object.header");
    const QString v_object_filter=qsl("object.filter");
    const QString v_object_filter_value=qsl("object.filter.value");
    const QString v_object_data=qsl("object.data");
    const QString v_object_chart=qsl("object.chart");
    const QString v_object_strategy=qsl("object.strategy");
    const QString v_object_iterator=qsl("object.iterator");
    const QString v_object_source=qsl("object.source");

    QVariantHash vBodyMap;
    explicit RunnerItem(QObject*parent=nullptr):QStm::Object(parent){
    }
    explicit RunnerItem(const QVariant&v, QObject*parent=nullptr):QStm::Object(parent){
        this->vBodyMap=v.toHash();
    }

    virtual ~RunnerItem(){
    }

    virtual bool isValid(){
        return true;
    }

    virtual ResultValue&read(RunnerItem&item, const QVariant&v){
        Q_UNUSED(item)
        this->vBodyMap=v.toHash();
        return this->lr();
    }

    virtual QVariantMap toMap()const{
        return QVariantMap();
    }

    virtual QVariantHash var()const{
        QHashIterator<QString, QVariant> i(this->toHash());
        QVariantHash vMap;
        while (i.hasNext()) {
            i.next();
            const auto k=qsl("${%1}").arg(i.key());
            const auto&v=i.value();
            vMap.insert(k,v);
        }
        return vMap;
    }

    virtual QVariant parserVar(const QVariant&v)const{
        QVariant vOut;
        const auto var=this->var();
        QString vStr;
        if(v.type()==v.List || v.type()==v.StringList || v.type()==v.Map || v.type()==v.Hash)
            vStr=QJsonDocument::fromVariant(v).toJson();
        else
            vStr=v.toString();
        QHashIterator<QString, QVariant> i(var);
        while (i.hasNext()) {
            i.next();
            QString sVal;
            auto v=i.value();
            if(v.type()==v.List || v.type()==v.StringList || v.type()==v.Map || v.type()==v.Hash)
                sVal=QJsonDocument::fromVariant(v).toJson();
            else
                sVal=v.toString();
            vStr=vStr.replace(i.key(),sVal);
        }

        vOut=QJsonDocument::fromJson(vStr.toUtf8()).toVariant();

        return (vOut.isValid())?vOut:vStr;;
    }
};

class RunnerEngine:public QStm::Object{
public:
    class Strategy;

    class Iterator:public RunnerItem{
    public:
        QVariant source;
        QVariant strategy;
        QVariant into;
        QVariant property;

        explicit Iterator(const QVariant&v=QVariant()):RunnerItem(v){
        }
        virtual ~Iterator(){
        }

        ResultValue&read(RunnerItem&item, const QVariant&v){
            this->vBodyMap=v.toHash();
            this->source    =item.parserVar(vBodyMap.value(qsl("source")));
            this->strategy  =item.parserVar(vBodyMap.value(qsl("strategy")));
            this->into      =item.parserVar(vBodyMap.value(qsl("into")));
            this->property  =item.parserVar(vBodyMap.value(qsl("property")));
            return this->lr();
        }

    };

    class Source:public RunnerItem{
    public:
        QVariant source;
        QVariant strategy;
        QVariant into;
        QVariant property;

        explicit Source(const QVariant&v=QVariant()):RunnerItem(v){
        }
        virtual ~Source(){
        }

        ResultValue&read(RunnerItem&item, const QVariant&v){
            this->vBodyMap=v.toHash();
            this->source=item.parserVar(vBodyMap.value(qsl("source")));
            this->strategy=item.parserVar(vBodyMap.value(qsl("strategy")));
            this->into=item.parserVar(vBodyMap.value(qsl("into")));
            this->property=item.parserVar(vBodyMap.value(qsl("property")));
            return this->lr();
        }

        auto&retreive(Strategy&item){
            Q_UNUSED(item)
            return this->lr();
        }

    };


    class Strategy:public RunnerItem{
    public:

        QString name;
        QVariantHash query;
        QVariantHash header;
        QVariantHash filter;
        QVariantHash filter_value;
        QVariantList data;
        QVariantHash chart;
        QVariantHash strategy;
        Iterator iterator;
        Source source;

        explicit Strategy(const QVariant&v=QVariant()):RunnerItem(v){
            auto&item=*dynamic_cast<RunnerItem*>(this);
            this->name=vBodyMap.value(qsl("name")).toString();
            this->query=vBodyMap.value(qsl("query")).toHash();
            this->header=vBodyMap.value(qsl("header")).toHash();
            this->filter=vBodyMap.value(qsl("filter")).toHash();
            this->filter_value=vBodyMap.value(qsl("filter_value")).toHash();
            this->data=vBodyMap.value(qsl("data")).toList();
            this->chart=vBodyMap.value(qsl("chart")).toHash();
            this->strategy=vBodyMap.value(qsl("strategy")).toHash();
            this->iterator.read(item, vBodyMap.value(qsl("iterator")));
            this->source.read(item, vBodyMap.value(qsl("source")));
        }

        virtual ~Strategy(){
        }

        QVariantMap toMap()const{
            QVariantMap v={
                 {v_object_name          ,this->name                }
                ,{v_object_query        ,this->query                }
                ,{v_object_header       ,this->header               }
                ,{v_object_filter       ,this->filter               }
                ,{v_object_filter_value ,this->filter_value         }
                ,{v_object_data         ,this->data                 }
                ,{v_object_chart        ,this->chart                }
                ,{v_object_strategy     ,this->strategy             }
                ,{v_object_iterator     ,this->iterator.vBodyMap    }
                ,{v_object_source       ,this->source.vBodyMap      }
            };
            return v;
        }

        virtual ResultValue&doExecute(){
            this->doDataRetreive();
            this->doIterator();
            return this->lr();
        };
    private:
        bool doDataRetreive(){
            this->data=source.retreive(*this).resultList();
            return true;
        };

        bool doIterator(){
            return true;
        };


    };

    QVariantHash strategyMap;
    QVariant data;
    explicit RunnerEngine(QObject*parent):QStm::Object(parent){
    }

    virtual ~RunnerEngine(){
    }

    ResultValue &execute(ReportItem&item){
        QVariant __return;
        this->strategyMap.clear();
        const auto strategyRunnerList=item.strategyRunner();
        if(!strategyRunnerList.isEmpty()){
            const auto strategyList=item.strategyList();
            //prepare parser
            for(auto&strategyName:strategyRunnerList){
                if(this->strategyMap.contains(strategyName))
                    continue;
                else if(!strategyList.contains(strategyName))
                    continue;
                else{
                    auto vStrategy=strategyList.value(strategyName);
                    RunnerEngine::Strategy strategy(vStrategy);
                    if(strategy.isValid())
                        this->strategyMap.insert(strategy.name, strategy.toHash());
                }
            }

            if(!strategyMap.isEmpty()){
                QVariantList vList;
                QHashIterator<QString, QVariant> i(strategyMap);
                while (i.hasNext()) {
                    i.next();
                    RunnerEngine::Strategy strategy(i.value());
                    vList<<strategy.doExecute().resultVariant();
                }
                __return=vList.size()==1?vList.first():vList;
            }
        }
        return this->lr(__return);
    }
};
}
