#pragma once

#include "./qorm_global.h"
#include <QVariant>
#include <QVariantHash>
#include <QVariantList>

namespace QOrm {

enum CRUDStrategy {
    Undefined = 1,
    Search = 2,
    Insert = 4,
    Update = 8,
    Upsert = 16,
    Remove = 32,
    Deactivate = 64
};

namespace Private {
Q_GLOBAL_STATIC_WITH_ARGS(QVariantList,
                          __staticListToStrategy,
                          ({QOrm::Search, QOrm::Update, QOrm::Upsert, QOrm::Deactivate}));
Q_GLOBAL_STATIC_WITH_ARGS(QVariantHash,
                          __staticStringToStrategy,
                          ({{QString::number(QOrm::Search), QOrm::Search},
                            {("search"), QOrm::Search},
                            {("get"), QOrm::Search},
                            {("find"), QOrm::Search},
                            {QString::number(QOrm::Insert), QOrm::Insert},
                            {("insert"), QOrm::Insert},
                            {QString::number(QOrm::Upsert), QOrm::Upsert},
                            {("upsert"), QOrm::Upsert},
                            {("post"), QOrm::Upsert},
                            {("save"), QOrm::Upsert},
                            {QString::number(QOrm::Update), QOrm::Update},
                            {("update"), QOrm::Update},
                            {("put"), QOrm::Update},
                            {QString::number(QOrm::Remove), QOrm::Remove},
                            {("remove"), QOrm::Remove},
                            {QString::number(QOrm::Deactivate), QOrm::Deactivate},
                            {("deactivate"), QOrm::Deactivate},
                            {("delete"), QOrm::Deactivate}}));
} // namespace Private
static const auto &__listToStrategy = *Private::__staticListToStrategy;
static const auto &__stringToStrategy = *Private::__staticStringToStrategy;

} // namespace QOrm
