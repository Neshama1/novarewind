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
    // modes: create, restore, remove, console

    m_mode = mode;
}

QString Snapshot::getMode()
{
    return m_mode;
}

int Snapshot::createSnapshot()
{
    // Gets partition name for system mounted on /
    QProcess process;
    QString findmntcommand = "findmnt / --output=SOURCE | grep [/@]";
    process.start("bash", QStringList() << "-c" << findmntcommand);
    process.waitForFinished(-1);

    QByteArray result = process.readAll();
    QString mntPoint = QString(result);
    m_mntPoint = mntPoint;
    m_mntPoint = m_mntPoint.simplified();
    m_mntPoint = m_mntPoint.remove("[/@]");

    // Get current date and time
    QDateTime dateTime = dateTime.currentDateTime();
    QString currentDateTime = dateTime.toString("yyyyMMdd-hhmmss");

    QByteArray sudoPwd(m_password.toUtf8());

    // Create snapshot
    int error = 0;
    if (getMode() == "console") {
        QDir dir;
        if (!dir.exists("/novarewind/@")) {
            error = system("sudo mkdir -p /novarewind");
            error = system("sudo mount " + m_mntPoint.toUtf8() + " /novarewind");
            error = system("sudo mkdir -p /novarewind/snapshots");
        }
        error = system("sudo btrfs subvolume snapshot -r /novarewind/@ /novarewind/snapshots/" + currentDateTime.toUtf8());
    }
    else {
        QDir dir;
        if (!dir.exists("/novarewind/@")) {
            error = system("echo " + sudoPwd + " | sudo -S mkdir -p /novarewind");
            error = system("echo " + sudoPwd + " | sudo -S mount " + m_mntPoint.toUtf8() + " /novarewind");
            error = system("echo " + sudoPwd + " | sudo -S mkdir -p /novarewind/snapshots");
        }
        error = system("echo " + sudoPwd + " | sudo -S btrfs subvolume snapshot -r /novarewind/@ /novarewind/snapshots/" + currentDateTime.toUtf8());
    }

    if (error != 0) {
        m_password = "";
        failed();
    }
    else {
        finished(currentDateTime);
    }

    return error;
}

QStringList Snapshot::listSnapshots()
{
    // Mount partition
    QProcess process;
    QString findmntcommand = "findmnt / --output=SOURCE | grep [/@]";
    process.start("bash", QStringList() << "-c" << findmntcommand);
    process.waitForFinished(-1);

    QByteArray result = process.readAll();
    QString mntPoint = QString(result);
    m_mntPoint = mntPoint;
    m_mntPoint = m_mntPoint.simplified();
    m_mntPoint = m_mntPoint.remove("[/@]");

    QDateTime dateTime = dateTime.currentDateTime();
    QString currentDateTime = dateTime.toString("yyyyMMdd-hhmmss");

    QByteArray sudoPwd(m_password.toUtf8());

    int error = 0;

    if (getMode() == "console") {
        QDir dir;
        if (!dir.exists("/novarewind/@")) {
            error = system("sudo mkdir -p /novarewind");
            error = system("sudo mount " + m_mntPoint.toUtf8() + " /novarewind");
            error = system("sudo mkdir -p /novarewind/snapshots");
        }
    }
    else {
        QDir dir;
        if (!dir.exists("/novarewind/@")) {
            error = system("echo " + sudoPwd + " | sudo -S mkdir -p /novarewind");
            error = system("echo " + sudoPwd + " | sudo -S mount " + m_mntPoint.toUtf8() + " /novarewind");
            error = system("echo " + sudoPwd + " | sudo -S mkdir -p /novarewind/snapshots");
        }
    }

    QString path = "/novarewind/snapshots";
    QDir directory = QDir(path);
    m_points = directory.entryList(QStringList() << "*", QDir::Dirs | QDir::NoDotAndDotDot);

    if (error != 0) {
        m_password = "";
        failed();
    }

    return m_points;
}

int Snapshot::removeSnapshot(const QString& dateTime)
{
    // Mount partition
    QProcess process;
    QString findmntcommand = "findmnt / --output=SOURCE | grep [/@]";
    process.start("bash", QStringList() << "-c" << findmntcommand);
    process.waitForFinished(-1);

    QByteArray result = process.readAll();
    QString mntPoint = QString(result);
    m_mntPoint = mntPoint;
    m_mntPoint = m_mntPoint.simplified();
    m_mntPoint = m_mntPoint.remove("[/@]");

    QByteArray sudoPwd(m_password.toUtf8());

    int error = 0;
    if (getMode() == "console") {
        QDir dir;
        if (!dir.exists("/novarewind/@")) {
            error = system("sudo mkdir -p /novarewind");
            error = system("sudo mount " + m_mntPoint.toUtf8() + " /novarewind");
            error = system("sudo mkdir -p /novarewind/snapshots");
        }
        error = system("sudo btrfs subvolume delete /novarewind/snapshots/" + dateTime.toUtf8());
    }
    else {
        QDir dir;
        if (!dir.exists("/novarewind/@")) {
            error = system("echo " + sudoPwd + " | sudo -S mkdir -p /novarewind");
            error = system("echo " + sudoPwd + " | sudo -S mount " + m_mntPoint.toUtf8() + " /novarewind");
            error = system("echo " + sudoPwd + " | sudo -S mkdir -p /novarewind/snapshots");
        }
        error = system("echo " + sudoPwd + " | sudo -S btrfs subvolume delete /novarewind/snapshots/" + dateTime.toUtf8());
    }

    if (error != 0) {
        failed();
    }
    else {
        finished("");
    }

    return error;
}

int Snapshot::restoreSnapshot(const QString& dateTime)
{
    // Mount partition
    QProcess process;
    QString findmntcommand = "findmnt / --output=SOURCE | grep [/@]";
    process.start("bash", QStringList() << "-c" << findmntcommand);
    process.waitForFinished(-1);

    QByteArray result = process.readAll();
    QString mntPoint = QString(result);
    m_mntPoint = mntPoint;
    m_mntPoint = m_mntPoint.simplified();
    m_mntPoint = m_mntPoint.remove("[/@]");

    QByteArray sudoPwd(m_password.toUtf8());

    int error = 0;
    if (getMode() == "console") {
        QDir dir;
        if (!dir.exists("/novarewind/@")) {
            error = system("sudo mkdir -p /novarewind");
            error = system("sudo mount " + m_mntPoint.toUtf8() + " /novarewind");
            error = system("sudo mkdir -p /novarewind/snapshots");
        }

        QDir dirOrig;
        if (dirOrig.exists("/novarewind/@orig")) {
            error = system("sudo btrfs subvolume delete /novarewind/@orig");
        }

        error = system("sudo mv /novarewind/@ /novarewind/@orig");
        error = system("sudo btrfs subvolume snapshot /novarewind/snapshots/" + dateTime.toUtf8() + " /novarewind/@");
    }
    else {
        QDir dir;
        if (!dir.exists("/novarewind/@")) {
            error = system("echo " + sudoPwd + " | sudo -S mkdir -p /novarewind");
            error = system("echo " + sudoPwd + " | sudo -S mount " + m_mntPoint.toUtf8() + " /novarewind");
            error = system("echo " + sudoPwd + " | sudo -S mkdir -p /novarewind/snapshots");
        }

        QDir dirOrig;
        if (dirOrig.exists("/novarewind/@orig")) {
            error = system("echo " + sudoPwd + " | sudo -S btrfs subvolume delete /novarewind/@orig");
        }

        error = system("echo " + sudoPwd + " | sudo -S mv /novarewind/@ /novarewind/@orig");
        error = system("echo " + sudoPwd + " | sudo -S btrfs subvolume snapshot /novarewind/snapshots/" + dateTime.toUtf8() + " /novarewind/@");
    }

    if (error != 0) {
        failed();
    }
    else {
        finished("");
    }

    return error;
}
