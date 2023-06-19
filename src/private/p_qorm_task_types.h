#pragma once

#include <QVariant>
#include <QVariantHash>
#include <QVariantList>
#include <QSqlDatabase>

typedef std::function<QVariant(QSqlDatabase &db, const QVariant &task)> TaskRunnerMethod;
