#include "snapshot.h"
#include <QProcess>
#include <QDateTime>
#include <QDir>

QStringList Snapshot::points() const
{
    return m_points;
}

void Snapshot::setPoints(const QStringList &points)
{
    m_points = points;
    Q_EMIT pointsChanged();
}

void Snapshot::on_PointsChanged()
{
}

void Snapshot::setPassword(const QString& password)
{
    m_password = password;
}

void Snapshot::setMode(const QString& mode)
{
    // modes: create, restore, remove

    m_mode = mode;
}

QString Snapshot::getMode()
{
    return m_mode;
}

void Snapshot::createSnapshot(const QString& findmntcommand)
{
    // Gets partition name for system mounted on /
    QProcess process;
    process.start("bash", QStringList() << "-c" << findmntcommand);
    process.waitForFinished(-1);

    QByteArray result = process.readAll();
    QString mntPoint = QString(result);
    m_mntPoint = mntPoint;
    m_mntPoint = m_mntPoint.simplified();

    // Get current date and time
    QDateTime dateTime = dateTime.currentDateTime();
    QString currentDateTime = dateTime.toString("yyyyMMdd-hhmmss");

    QByteArray sudoPwd(m_password.toUtf8());

    // Create snapshot
    int error;
    error = system("echo " + sudoPwd + " | sudo -S mkdir -p /novarewind");
    error = system("echo " + sudoPwd + " | sudo -S mount " + m_mntPoint.toUtf8() + " /novarewind");
    error = system("echo " + sudoPwd + " | sudo -S mkdir -p /novarewind/snapshots");
    error = system("echo " + sudoPwd + " | sudo -S btrfs subvolume snapshot -r /novarewind/@ /novarewind/snapshots/" + currentDateTime.toUtf8());

    if (error != 0) {
        m_password = "";
        failed();
    }
    else {
        finished(currentDateTime);
    }
}

void Snapshot::listSnapshots(const QString& findmntcommand)
{
    // Mount partition
    QProcess process;
    process.start("bash", QStringList() << "-c" << findmntcommand);
    process.waitForFinished(-1);

    QByteArray result = process.readAll();
    QString mntPoint = QString(result);
    m_mntPoint = mntPoint;
    m_mntPoint = m_mntPoint.simplified();

    QDateTime dateTime = dateTime.currentDateTime();
    QString currentDateTime = dateTime.toString("yyyyMMdd-hhmmss");

    QByteArray sudoPwd(m_password.toUtf8());

    int error;
    error = system("echo " + sudoPwd + " | sudo -S mkdir -p /novarewind");
    error = system("echo " + sudoPwd + " | sudo -S mount " + m_mntPoint.toUtf8() + " /novarewind");
    error = system("echo " + sudoPwd + " | sudo -S mkdir -p /novarewind/snapshots");

    QString path = "/novarewind/snapshots";
    QDir directory = QDir(path);
    m_points = directory.entryList(QStringList() << "*", QDir::Dirs | QDir::NoDotAndDotDot);

    if (error != 0) {
        m_password = "";
        failed();
    }
}

void Snapshot::removeSnapshot(const QString& dateTime)
{
    QByteArray sudoPwd(m_password.toUtf8());

    int error;
    error = system("echo " + sudoPwd + " | sudo -S btrfs subvolume delete /novarewind/snapshots/" + dateTime.toUtf8());

    if (error != 0) {
        failed();
    }
    else {
        finished("");
    }
}

void Snapshot::restoreSnapshot(const QString& dateTime)
{
    QByteArray sudoPwd(m_password.toUtf8());

    int error;
    system("echo " + sudoPwd + " | sudo -S btrfs subvolume delete /novarewind/@orig");
    error = system("echo " + sudoPwd + " | sudo -S mv /novarewind/@ /novarewind/@orig");
    error = system("echo " + sudoPwd + " | sudo -S btrfs subvolume snapshot /novarewind/snapshots/" + dateTime.toUtf8() + " /novarewind/@");

    if (error != 0) {
        failed();
    }
    else {
        finished("");
    }
}
