#include "./qorm_sql_suitable_types.h"

namespace QOrm {

TypeUtil::TypeUtil(){

}

TypeUtil::~TypeUtil(){

}

QSqlDriver::DbmsType TypeUtil::strToDataType(const QString&driverName)
    {
        if(driverName == driver_QODBC)
            return QSqlDriver::MSSqlServer;
        else if(driverName == driver_QSQLITE)
            return QSqlDriver::SQLite;
        else if(driverName == driver_QPSQL)
            return QSqlDriver::PostgreSQL;
        else if(driverName == driver_QOIC)
            return QSqlDriver::Oracle;
        else if(driverName == driver_QMYSQL)
            return QSqlDriver::MySqlServer;
        else
            return QSqlDriver::UnknownDbms;
}

const QList<int> TypeUtil::keywordGroupingTypes()
{
   QList<int> __keywordGroupingTypes;
   if(__keywordGroupingTypes.isEmpty()){
       __keywordGroupingTypes<<kgMin<<kgMax<<kgAvg<<kgSum<<kgCount;
       __keywordGroupingTypes<<kgMinAs<<kgMaxAs<<kgAvgAs<<kgSumAs<<kgCountAs;
   }
   return __keywordGroupingTypes;

}

}
