#pragma once

#include <QStringList>
#include <QVariant>

#define Q_ORM_DECLARE_PROPERTY_IGNORE_LIST Q_GLOBAL_STATIC_WITH_ARGS(QStringList, staticPropertyIgnoredList, ({"","objectname","objectName","tableprefix","tablePrefix","tablename","tableName","tablepk","tablePk","tableprimarykey","tablePrimaryKey","tablesequence","tableSequence","tableschema","tableSchema","tableprefixseparator","tablePrefixSeparator","tablePkAutoGenerate"}))
