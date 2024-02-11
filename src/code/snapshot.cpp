#include "snapshot.h"
#include <QProcess>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <KConfig>
#include <KConfigGroup>
#include <QVariantMap>

Snapshot::Snapshot(QObject *parent)
    : QObject(parent)
{

}

QVariantList Snapshot::points() const
{
    return m_points;
}

void Snapshot::setPoints(const QVariantList &points)
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
    // modes: create, restore, remove, console, daily, weekly, monthly

    m_mode = mode;
}

QString Snapshot::getMode()
{
    return m_mode;
}

void Snapshot::setDailyConfig(bool daily, int keepDaily)
{
    m_daily = daily;
    m_keepDaily = keepDaily;

    KConfig novarewindrc("/novarewind/rc/novarewindrc");
    KConfigGroup dailyConfig = novarewindrc.group("DailyConfig");
    dailyConfig.writeEntry("daily", m_daily);
    dailyConfig.writeEntry("keepDaily", m_keepDaily);

    if (m_password != "") {
        removeAutoSnapshotsIfNeeded();
        saveSnapshotsList();
    }
}

void Snapshot::setWeeklyConfig(bool weekly, int keepWeekly)
{
    m_weekly = weekly;
    m_keepWeekly = keepWeekly;

    KConfig novarewindrc("/novarewind/rc/novarewindrc");
    KConfigGroup weeklyConfig = novarewindrc.group("WeeklyConfig");
    weeklyConfig.writeEntry("weekly", m_weekly);
    weeklyConfig.writeEntry("keepWeekly", m_keepWeekly);

    if (m_password != "") {
        removeAutoSnapshotsIfNeeded();
        saveSnapshotsList();
    }
}

void Snapshot::setMonthlyConfig(bool monthly, int keepMonthly)
{
    m_monthly = monthly;
    m_keepMonthly = keepMonthly;

    KConfig novarewindrc("/novarewind/rc/novarewindrc");
    KConfigGroup monthlyConfig = novarewindrc.group("MonthlyConfig");
    monthlyConfig.writeEntry("monthly", m_monthly);
    monthlyConfig.writeEntry("keepMonthly", m_keepMonthly);

    if (m_password != "") {
        removeAutoSnapshotsIfNeeded();
        saveSnapshotsList();
    }
}

int Snapshot::createSnapshot()
{
    if ((getMode() == "daily" && m_daily == false) || (getMode() == "weekly" && m_weekly == false) || (getMode() == "monthly" && m_monthly == false)) {
        return 1;
    }

    m_points.clear();
    readSnapshotsList();

    // Get current date and time
    QDateTime dateTime = dateTime.currentDateTime();
    QString currentDateTime = dateTime.toString("yyyyMMdd-hhmmss");

    // Password
    QByteArray sudoPwd(m_password.toUtf8());

    // Create snapshot
    int error = 0;


    QDir dir;
    if (dir.exists("/novarewind/snapshots/" + currentDateTime)) {
        return 1;
    }

    if (getMode() == "console" || getMode() == "daily" || getMode() == "weekly" || getMode() == "monthly") {
        error = system("sudo btrfs subvolume snapshot -r /novarewind/@ /novarewind/snapshots/" + currentDateTime.toUtf8());
    }
    else {
        error = system("echo " + sudoPwd + " | sudo -S btrfs subvolume snapshot -r /novarewind/@ /novarewind/snapshots/" + currentDateTime.toUtf8());
    }

    if (error == 0) {
        addSnapshot(currentDateTime);
        error = saveSnapshotsList();
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

QVariantList Snapshot::listSnapshots()
{
    int error = 0;

    error = readSnapshotsList();
    error = saveSnapshotsList();

    if (error != 0) {
        m_password = "";
        failed();
    }

    return m_points;
}

int Snapshot::removeSnapshot(const QString& dateTime)
{
    int error = 0;

    // Mount partition
    error = mount();

    // Password
    QByteArray sudoPwd(m_password.toUtf8());

    if (getMode() == "console" || getMode() == "daily" || getMode() == "weekly" || getMode() == "monthly") {
        error = system("sudo btrfs subvolume delete /novarewind/snapshots/" + dateTime.toUtf8());
        if (error == 0) {
            for (int i = 0 ; i < m_points.count(); i++) {
                QVariantMap point = m_points[i].toMap();
                if (dateTime == point["dateTime"].toString()) {
                    m_points.removeAt(i);
                }
            }
        }
        error = system("sudo rm /novarewind/rc/novarewindrc");
    }
    else {
        error = system("echo " + sudoPwd + " | sudo -S btrfs subvolume delete /novarewind/snapshots/" + dateTime.toUtf8());
        if (error == 0) {
            for (int i=0 ; i<m_points.count() ; i++) {
                QVariantMap point = m_points[i].toMap();
                if (dateTime == point["dateTime"].toString()) {
                    m_points.removeAt(i);
                }
            }
        }
        error = system("echo " + sudoPwd + " | sudo -S rm /novarewind/rc/novarewindrc");
    }

    error = saveSnapshotsList();

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
    int error = 0;

    // Mount partition
    error = mount();

    // Password
    QByteArray sudoPwd(m_password.toUtf8());

    QDir dirOrig;

    if (getMode() == "console") {
        if (dirOrig.exists("/novarewind/@orig")) {
            error = system("sudo btrfs subvolume delete /novarewind/@orig");
        }
        error = system("sudo mv /novarewind/@ /novarewind/@orig");
        error = system("sudo btrfs subvolume snapshot /novarewind/snapshots/" + dateTime.toUtf8() + " /novarewind/@");
    }
    else {
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

int Snapshot::saveSnapshotsList()
{
    int error = 0;

    QString userName = qgetenv("USER");
    QByteArray sudoPwd(m_password.toUtf8());

    QDir dir;
    if (!dir.exists("/novarewind/rc")) {
        if (getMode() == "console" || getMode() == "daily" || getMode() == "weekly" || getMode() == "monthly") {
            error = system("sudo mkdir -p /novarewind/rc");
            error = system("sudo chown " + userName.toUtf8() + ":" + userName.toUtf8() + " /novarewind/rc");
        }
        else {
            error = system("echo " + sudoPwd + " | sudo -S mkdir -p /novarewind/rc");
            error = system("echo " + sudoPwd + " | sudo -S chown " + userName.toUtf8() + ":" + userName.toUtf8() + " /novarewind/rc");
        }
    }

    if (getMode() == "console" || getMode() == "daily" || getMode() == "weekly" || getMode() == "monthly") {
        if (QFile::exists("/novarewind/rc/novarewindrc")) {
            error = system("sudo rm /novarewind/rc/novarewindrc");
        }
    }
    else {
        if (QFile::exists("/novarewind/rc/novarewindrc")) {
            error = system("echo " + sudoPwd + " | sudo -S rm /novarewind/rc/novarewindrc");
        }
    }

    KConfig novarewindrc("/novarewind/rc/novarewindrc");

    QStringList points;

    for (int i = 0; i < m_points.count(); i++) {
        QVariantMap point = m_points[i].toMap();
        points << point["dateTime"].toString();

        KConfigGroup snapshot = novarewindrc.group(point["dateTime"].toString());
        snapshot.writeEntry("type",point["type"].toString());
        snapshot.writeEntry("description",point["description"].toString());
    }

    KConfigGroup snapshots = novarewindrc.group("Snapshots");
    snapshots.writeEntry("points", points);

    KConfigGroup dailyConfig = novarewindrc.group("DailyConfig");
    dailyConfig.writeEntry("daily", m_daily);
    dailyConfig.writeEntry("keepDaily", m_keepDaily);

    KConfigGroup weeklyConfig = novarewindrc.group("WeeklyConfig");
    weeklyConfig.writeEntry("weekly", m_weekly);
    weeklyConfig.writeEntry("keepWeekly", m_keepWeekly);

    KConfigGroup monthlyConfig = novarewindrc.group("MonthlyConfig");
    monthlyConfig.writeEntry("monthly", m_monthly);
    monthlyConfig.writeEntry("keepMonthly", m_keepMonthly);

    return error;
}

int Snapshot::readSnapshotsList()
{
    int error = 0;

    // Mount file system
    error = mount();

    // Password
    QByteArray sudoPwd(m_password.toUtf8());

    m_points.clear();
    QString userName = qgetenv("USER");

    if (QFile::exists("/novarewind/rc/novarewindrc")) {
        KConfig novarewindrc("/novarewind/rc/novarewindrc");
        KConfigGroup snapshots = novarewindrc.group("Snapshots");
        QStringList points = snapshots.readEntry("points", QStringList());

        for (int i = 0; i < points.count(); i++) {
            KConfigGroup snapshot = novarewindrc.group(points[i]);

            QVariantMap point;
            point["dateTime"] = points[i];
            point["type"] = snapshot.readEntry("type",QString());
            point["description"] = snapshot.readEntry("description",QString());
            m_points.append(point);
        }

        KConfigGroup dailyConfig = novarewindrc.group("DailyConfig");
        m_daily = dailyConfig.readEntry("daily", bool());
        m_keepDaily = dailyConfig.readEntry("keepDaily", int());

        KConfigGroup weeklyConfig = novarewindrc.group("WeeklyConfig");
        m_weekly = weeklyConfig.readEntry("weekly", bool());
        m_keepWeekly = weeklyConfig.readEntry("keepWeekly", int());

        KConfigGroup monthlyConfig = novarewindrc.group("MonthlyConfig");
        m_monthly = monthlyConfig.readEntry("monthly", bool());
        m_keepMonthly = monthlyConfig.readEntry("keepMonthly", int());

        removeAutoSnapshotsIfNeeded();
    }
    else {
        QString path = "/novarewind/snapshots";
        QDir directory = QDir(path);
        QStringList points = directory.entryList(QStringList() << "*", QDir::Dirs | QDir::NoDotAndDotDot);

        for (int i = 0; i < points.count(); i++) {
            QVariantMap snapshot;
            snapshot["dateTime"] = points[i];
            snapshot["type"] = "manual";
            snapshot["description"] = "";
            m_points.append(snapshot);
        }
    }

    return error;
}

int Snapshot::mount()
{
    int error = 0;

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

    // Password
    QByteArray sudoPwd(m_password.toUtf8());

    if (getMode() == "console" || getMode() == "daily" || getMode() == "weekly" || getMode() == "monthly") {
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

    // Al ejecutar novarewind como root en "recovery mode" cambia el propietario. Recuperar usuario como propietario.

    QString userName = qgetenv("USER");

    if (QFile::exists("/novarewind/rc/novarewindrc")) {
        if (getMode() == "console" || getMode() == "daily" || getMode() == "weekly" || getMode() == "monthly") {
            error = system("sudo chown " + userName.toUtf8() + ":" + userName.toUtf8() + " /novarewind/rc");
            error = system("sudo chown " + userName.toUtf8() + ":" + userName.toUtf8() + " /novarewind/rc/novarewindrc");
        }
        else {
            error = system("echo " + sudoPwd + " | sudo -S chown " + userName.toUtf8() + ":" + userName.toUtf8() + " /novarewind/rc");
            error = system("echo " + sudoPwd + " | sudo -S chown " + userName.toUtf8() + ":" + userName.toUtf8() + " /novarewind/rc/novarewindrc");
        }
    }

    return error;
}

void Snapshot::addSnapshot(const QString& dateTime)
{
    QVariantMap snapshot;
    snapshot["dateTime"] = dateTime;
    if (getMode() == "console" || getMode() == "create") {
        snapshot["type"] = "manual";
    }
    else {
        snapshot["type"] = getMode();
    }
    snapshot["description"] = "";
    m_points.append(snapshot);

    removeAutoSnapshotsIfNeeded();
}

void Snapshot::removeAutoSnapshotsIfNeeded()
{
    int error = 0;

    // Password
    QByteArray sudoPwd(m_password.toUtf8());

    // Check password
    if (getMode() == "console" || getMode() == "daily" || getMode() == "weekly" || getMode() == "monthly") {
        error = system("sudo echo");
    }
    else {
        error = system("echo " + sudoPwd + " | sudo -S echo");
    }

    if (error == 0) {
        removeDailySnapshotsIfNeeded();
        removeWeeklySnapshotsIfNeeded();
        removeMonthlySnapshotsIfNeeded();
    }
}

void Snapshot::removeDailySnapshotsIfNeeded()
{
    int count = 0;

    for (int i = 0; i < m_points.count(); i++) {
        QVariantMap point = m_points[i].toMap();
        if (point["type"].toString() == "daily") {
            count++;
        }
    }

    int itemsToRemove = 0;

    if (count > m_keepDaily) {
        itemsToRemove = count - m_keepDaily;
    }

    int itemsRemoved = 0;

    if (itemsToRemove > 0) {
        for (int i = 0; i < m_points.count(); i++) {
            QVariantMap point = m_points[i].toMap();
            if (point["type"].toString() == "daily") {
                //m_points.removeAt(i);
                removeSnapshot(point["dateTime"].toString());
                itemsRemoved++;
            }
            if (itemsRemoved == itemsToRemove) {
                break;
            }
        }
    }
}

void Snapshot::removeWeeklySnapshotsIfNeeded()
{
    int count = 0;

    for (int i = 0; i < m_points.count(); i++) {
        QVariantMap point = m_points[i].toMap();
        if (point["type"].toString() == "weekly") {
            count++;
        }
    }

    int itemsToRemove = 0;

    if (count > m_keepWeekly) {
        itemsToRemove = count - m_keepWeekly;
    }

    int itemsRemoved = 0;

    if (itemsToRemove > 0) {
        for (int i = 0; i < m_points.count(); i++) {
            QVariantMap point = m_points[i].toMap();
            if (point["type"].toString() == "weekly") {
                //m_points.removeAt(i);
                removeSnapshot(point["dateTime"].toString());
                itemsRemoved++;
            }
            if (itemsRemoved == itemsToRemove) {
                break;
            }
        }
    }
}

void Snapshot::removeMonthlySnapshotsIfNeeded()
{
    int count = 0;

    for (int i = 0; i < m_points.count(); i++) {
        QVariantMap point = m_points[i].toMap();
        if (point["type"].toString() == "monthly") {
            count++;
        }
    }

    int itemsToRemove = 0;

    if (count > m_keepMonthly) {
        itemsToRemove = count - m_keepMonthly;
    }

    int itemsRemoved = 0;

    if (itemsToRemove > 0) {
        for (int i = 0; i < m_points.count(); i++) {
            QVariantMap point = m_points[i].toMap();
            if (point["type"].toString() == "monthly") {
                //m_points.removeAt(i);
                removeSnapshot(point["dateTime"].toString());
                itemsRemoved++;
            }
            if (itemsRemoved == itemsToRemove) {
                break;
            }
        }
    }
}
