#pragma once

#include "./qorm_global.h"
#include "./qorm_object_db.h"
#include <QFileInfo>
#include <QFileInfoList>

namespace QOrm {
class ScriptExecPvt;
//!
//! \brief The ScriptExec class
//!class for execute script on database
class Q_ORM_EXPORT ScriptExec : public QOrm::ObjectDb
{
    Q_OBJECT
public:
    //!
    //! \brief ScriptExec
    //! \param parent
    //!
    Q_INVOKABLE explicit ScriptExec(QObject *parent = nullptr);

    ScriptExec &operator=(const QVariant &v);
    ScriptExec &operator=(const QFileInfoList &v);
    ScriptExec &operator<<(const QVariant &v);
    ScriptExec &operator<<(const QFileInfoList &v);

    //!
    //! \brief append
    //! \param entryInfoList
    //! \return
    //!
    ScriptExec &append(const QFileInfoList &v);

    //!
    //! \brief append
    //! \param v
    //! \return
    //!
    ScriptExec &append(const QVariant &v);

    //!
    //! \brief scriptValues
    //! \return
    //!list of added values
    QVariantList scriptValues() const;

    //!
    //! \brief scriptedValues
    //! \return
    //!list of analyzed values
    const QStringList &scriptedValues();

    //!
    //! \brief exec
    //! \return
    //!execute values on database
    ResultValue &exec();

private:
    ScriptExecPvt *p = nullptr;
};

} // namespace QOrm
