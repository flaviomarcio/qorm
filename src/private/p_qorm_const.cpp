#include "./p_qorm_const.h"
#include "../qorm_startup.h"
#include <QDir>
#include <QFile>
#include <QStandardPaths>

namespace QOrm {

Q_GLOBAL_STATIC(QString, staticLogDir);
static bool static_Q_LOG_ENABLED=false;
static bool staticLogRegister=false;

static void staticLogDirClear(const QString &ormLogDir)
{
    QStringList dir_found;
    QStringList dir_rm_file;
    dir_found.append(ormLogDir);
    while(!dir_found.isEmpty()){
        auto scanDir = dir_found.takeFirst();
        dir_rm_file.append(scanDir);
        QDir dir(scanDir);
        if(!dir.exists(scanDir))
            continue;

        dir.setFilter(QDir::AllDirs);
        for(auto &scanInDir:dir.entryList()){
            if(scanInDir==QStringLiteral(".") || scanInDir==QStringLiteral(".."))
                continue;

            auto dir=QStringLiteral("%1/%2").arg(scanDir, scanInDir);
            dir_rm_file.append(dir);
            dir_found.append(dir);
        }
    }

    auto ext=QStringList{QByteArrayLiteral("*.*")};
    for(auto &sdir:dir_rm_file){
        QDir scanDir(sdir);
        if(!scanDir.exists())
            continue;
        scanDir.setFilter(QDir::Drives | QDir::Files);
        scanDir.setNameFilters(ext);
        for(auto &dirFile : scanDir.entryList()){
            auto fileName=sdir+QByteArrayLiteral("/")+dirFile;
            QFile::remove(fileName);
        }
    }
}

static void staticLogInitDir()
{
#ifdef QT_DEBUG
    static_Q_LOG_ENABLED = true;
#else
    auto static_Q_LOG_ENABLED = QVariant{QString{getenv(QByteArrayLiteral("Q_LOG_ENABLED"))}.trimmed()}.toBool();
    static_Q_LOG_ENABLED = env.isEmpty()?false:QVariant(env).toBool();
#endif

    staticLogRegister=static_Q_LOG_ENABLED;

    const auto log_local_name=QString{__PRETTY_FUNCTION__}.split(QStringLiteral("::")).first().replace(QStringLiteral("void "), "").split(QStringLiteral(" ")).last();
    *staticLogDir=(new QString("%1/%2/%3"))->arg(QDir::homePath(), log_local_name, qApp->applicationName());

    QDir dir(*staticLogDir);
    if(!dir.exists(*staticLogDir))
        dir.mkpath(*staticLogDir);

    if(dir.exists(*staticLogDir))
        staticLogDirClear(*staticLogDir);
}

Q_COREAPP_STARTUP_FUNCTION(staticLogInitDir);

QString &logDir()
{
    return *staticLogDir;
}

bool logRegister()
{
    return staticLogRegister;
}

}
