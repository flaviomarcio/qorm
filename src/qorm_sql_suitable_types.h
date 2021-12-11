#pragma once

#include "./qorm_global.h"
#include "./qorm_types.h"

namespace QOrm {

enum KeywordGenericCommand {
     kgcAll
    ,kgcObject
    ,kgcValue
    ,kgcAlter
    ,kgcAnd
    ,kgcAny
    ,kgcAs
    ,kgcAsc
    ,kgcAvg
    ,kgcAvgAs
    ,kgcBetween
    ,kgcNotBetween
    ,kgcBigger
    ,kgcCascade
    ,kgcCase
    ,kgcCheck
    ,kgcClustered
    ,kgcConstraint
    ,kgcCount
    ,kgcCountAs
    ,kgcCreate
    ,kgcCreateOrReplace
    ,kgcDataBase
    ,kgcDeclare
    ,kgcDefault
    ,kgcDelete
    ,kgcDeleteFrom
    ,kgcDeleteFromUsing
    ,kgcDesc
    ,kgcDistinct
    ,kgcDrop
    ,kgcEnd
    ,kgcEqual
    ,kgcNotEqual
    ,kgcEqualBigger
    ,kgcEqualMinor
    ,kgcExists
    ,kgcForeignKey
    ,kgcFrom
    ,kgcFromValues
    ,kgcFull
    ,kgcFunction
    ,kgcGroupBy
    ,kgcHaving
    ,kgcIfExists
    ,kgcILike
    ,kgcIn
    ,kgcIndex
    ,kgcInOut
    ,kgcInsert
    ,kgcInsertInto
    ,kgcIsNull
    ,kgcIsNullCheckValue
    ,kgcValueIsNull
    ,kgcValueIsNotNull
    ,kgcJoin
    ,kgcJoinCross
    ,kgcJoinFull
    ,kgcJoinInner
    ,kgcJoinLeft
    ,kgcJoinOuter
    ,kgcJoinRigth
    ,kgcKey
    ,kgcLeft
    ,kgcLike
    ,kgcSoundexEqual
    ,kgcLimit
    ,kgcOffset
    ,kgcForUpdateInObject
    ,kgcForSkipLockedInObject
    ,kgcForUpdateFinishScript
    ,kgcForSkipLockedFinishScript
    ,kgcMax
    ,kgcMaxAs
    ,kgcMin
    ,kgcMinAs
    ,kgcMinor
    ,kgcNot
    ,kgcNotExists
    ,kgcNotNull
    ,kgcNull
    ,kgcOn
    ,kgcOr
    ,kgcOrderBy
    ,kgcPivot
    ,kgcPrimary
    ,kgcPrimaryKey
    ,kgcProcedure
    ,kgcReplace
    ,kgcRigth
    ,kgcRowNumber
    ,kgcSchema
    ,kgcSelect
    ,kgcSelectTop
    ,kgcSelectDistinct
    ,kgcSelectDistinctTop
    ,kgcSelectForExists
    ,kgcSelectValues
    ,kgcSet
    ,kgcSum
    ,kgcSumAs
    ,kgcTable
    ,kgcTableTemp
    ,kgcTemp
    ,kgcThen
    ,kgcTruncate
    ,kgcTruncateTable
    ,kgcTruncateTableCacade
    ,kgcUnion
    ,kgcUnionAll
    ,kgcIntersect
    ,kgcIntersectAll
    ,kgcExcept
    ,kgcExceptAll
    ,kgcUnique
    ,kgcUpdate
    ,kgcUpdateSet
    ,kgcUpdateSetValues
    ,kgcUpsertSet
    ,kgcUpsertSetValues
    ,kgcUser
    ,kgcUsing
    ,kgcValues
    ,kgcValuesParameter
    ,kgcView
    ,kgcWhen
    ,kgcWhere
    ,kgcCte
    ,kgcWorkTable
    ,kgcSetApplicationName
    ,kgcSetSearchPath
    ,kgcSetTransactionReadOnlyOn
    ,kgcSetTransactionReadOnlyOff
    ,kgcSequence
    ,kgcNextVal
    ,kgcNextValSelect
};

enum KeywordCombine{
     kcFrom         = kgcFrom
    ,kcFromValues   = kgcFromValues
    ,kcWhere        = kgcWhere
    ,kcJoin         = kgcJoin
    ,kcJoinLeft     = kgcJoinLeft
    ,kcJoinInner    = kgcJoinInner
    ,kcJoinOuter    = kgcJoinOuter
    ,kcJoinCross    = kgcJoinCross
    ,kcJoinFull     = kgcJoinFull
    ,kcWorkTable    = kgcWorkTable
    ,kcCte          = kgcCte
    ,kcOrderBy      = kgcOrderBy
};

enum KeywordLogical{
     klNone = -1
    ,klAnd  = kgcAnd
    ,klOr   = kgcOr
};

enum KeywordObjectInfo{
     koiObject  = kgcObject
    ,koiValue   = kgcValue
};

enum KeywordOrintation{
      koAsc = kgcAsc
    , koDesc= kgcDesc
};

enum KeywordOperator{
     koEqual        = kgcEqual
    ,koNotEqual     = kgcNotEqual
    ,koEqualMinor   = kgcEqualMinor
    ,koEqualBigger  = kgcEqualBigger
    ,koBigger       = kgcBigger
    ,koMinor        = kgcMinor
    ,koIn           = kgcIn
    ,koInOut        = kgcInOut
    ,koLike         = kgcLike
    ,koSoundexEqual = kgcSoundexEqual
    ,koNotNull      = kgcNotNull
    ,koBetween      = kgcBetween
    ,koNotBetween   = kgcNotBetween
    ,koIsNull       = kgcValueIsNull
    ,koIsNotNull    = kgcValueIsNotNull
    ,koAny          = kgcAny
};

static const auto KeywordOperators=QVector<int>()<<koEqual<<koNotEqual<<koEqualMinor<<koEqualBigger<<koBigger<<koMinor<<koIn<<koInOut<<koLike<<koNotNull<<koBetween<<koNotBetween<<koIsNull<<koIsNotNull<<koAny;

enum KeywordGrouping{
     kgNone     = -1
    ,kgMin      = kgcMin
    ,kgMinAs    = kgcMinAs
    ,kgMax      = kgcMax
    ,kgMaxAs    = kgcMaxAs
    ,kgAvg      = kgcAvg
    ,kgAvgAs    = kgcAvgAs
    ,kgSum      = kgcSum
    ,kgSumAs    = kgcSumAs
    ,kgCount    = kgcCount
    ,kgCountAs  = kgcCountAs
};


//!
//! \brief The TypeUtil class
//!
class Q_ORM_EXPORT TypeUtil{
public:
    //!
    //! \brief TypeUtil
    //!
    explicit TypeUtil();

    //!
    //! \brief ~TypeUtil
    //!
    virtual ~TypeUtil();

    //!
    //! \brief strToDataType
    //! \param driverName
    //! \return
    //!
    static QSqlDriver::DbmsType strToDataType(const QString &driverName);

    //!
    //! \brief keywordGroupingTypes
    //! \return
    //!
    static const QList<int> keywordGroupingTypes();

};

}
