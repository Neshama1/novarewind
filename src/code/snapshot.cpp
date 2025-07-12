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
    // modes: create, restore, remove, console
    // system-d timers: daily, weekly, monthly

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
    // Read snapshots

    m_points.clear();
    readSnapshotsList();

    // Get current date and time

    QDate today = today.currentDate();
    QDateTime dateTime = dateTime.currentDateTime();
    QString currentDateTime = dateTime.toString("yyyyMMdd-hhmmss");

    //QString today = dateTime.toString("yyyyMMdd");

    int error = 0;

    // Create snapshot

    // Create automatic snapshot on boot

    if (m_daily && (getMode() == "daily"))
    {
        QDate lastSnapshot = getLastSnapshot("daily");
        int days = lastSnapshot.daysTo(today);

        if ((days > 0) || lastSnapshot.isNull()) {
            error = createAutoSnapshot("daily", currentDateTime);
        }
    }

    if (m_weekly && (getMode() == "weekly"))
    {
        QDate lastSnapshot = getLastSnapshot("weekly");
        int days = lastSnapshot.daysTo(today);

        // Si han pasado 7 días o más o último snapshot semanal fue en día distinto de lunes y hoy es lunes

        if ((days >= 7) || ((days < 7) && ((lastSnapshot.day() != 1 && (today.day() == 1)))) || lastSnapshot.isNull()) {
            error = createAutoSnapshot("weekly", currentDateTime);
        }
    }

    if (m_monthly && (getMode() == "monthly"))
    {
        QDate lastSnapshot = getLastSnapshot("monthly");

        if ((lastSnapshot.month() != today.month()) || (lastSnapshot.year() != today.year()) || lastSnapshot.isNull()) {
            error = createAutoSnapshot("monthly", currentDateTime);
        }
    }

    if ((getMode() == "create") || (getMode() == "console"))
    {
        // Create manual snapshot

        error = createManualSnapshot();
    }

    return error;
}

int Snapshot::createManualSnapshot()
{
    int error = 0;

    QDateTime dateTime = dateTime.currentDateTime();
    QString currentDateTime = dateTime.toString("yyyyMMdd-hhmmss");

    // Password

    QByteArray sudoPwd(m_password.toUtf8());

    // Create snapshot

    QDir dir;
    if (dir.exists("/novarewind/snapshots/" + currentDateTime)) {
        return 1;
    }

    if (getMode() == "console") {
        m_command = "sudo btrfs subvolume snapshot -r /novarewind/@ /novarewind/snapshots/" + currentDateTime;
        error = system(m_command.toUtf8());
    }
    else {
        m_command = "echo " + sudoPwd + " | sudo -S btrfs subvolume snapshot -r /novarewind/@ /novarewind/snapshots/" + currentDateTime;
        error = system(m_command.toUtf8());
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

int Snapshot::createAutoSnapshot(const QString& mode, const QString& currentDateTime)
{
    setMode(mode);

    int error = 0;

    QDir dir;
    if (dir.exists("/novarewind/snapshots/" + currentDateTime)) {
        return 1;
    }

    m_command = "sudo btrfs subvolume snapshot -r /novarewind/@ /novarewind/snapshots/" + currentDateTime;
    error = system(m_command.toUtf8());


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

QDate Snapshot::getLastSnapshot(const QString& mode)
{
    m_points.clear();
    readSnapshotsList();

    QString lastDay;

    for (int i = 0; i < m_points.count(); i++) {
        QVariantMap point = m_points[i].toMap();
        if (point["type"].toString() == mode)
        {
            lastDay = point["dateTime"].toString();
        }
    }

    lastDay.remove(8, 7);
    QDate lastSnapshot = getSnapshotDate(lastDay);

    return lastSnapshot;
}

QDate Snapshot::getSnapshotDate(const QString& date)
{
    int year = date.mid(0,4).toInt();
    int month = date.mid(4,2).toInt();
    int day = date.mid(6,2).toInt();

    QDate snapshotDate(year, month, day);
    return snapshotDate;
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
        m_command = "sudo btrfs subvolume delete /novarewind/snapshots/" + dateTime;
        error = system(m_command.toUtf8());
        if (error == 0) {
            for (int i = 0 ; i < m_points.count(); i++) {
                QVariantMap point = m_points[i].toMap();
                if (dateTime == point["dateTime"].toString()) {
                    m_points.removeAt(i);
                }
            }
        }
        m_command = "sudo rm /novarewind/rc/novarewindrc";
        error = system(m_command.toUtf8());
    }
    else {
        m_command = "echo " + sudoPwd + " | sudo -S btrfs subvolume delete /novarewind/snapshots/" + dateTime;
        error = system(m_command.toUtf8());
        if (error == 0) {
            for (int i=0 ; i<m_points.count() ; i++) {
                QVariantMap point = m_points[i].toMap();
                if (dateTime == point["dateTime"].toString()) {
                    m_points.removeAt(i);
                }
            }
        }
        m_command = "echo " + sudoPwd + " | sudo -S rm /novarewind/rc/novarewindrc";
        error = system(m_command.toUtf8());
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

            m_command = "sudo btrfs subvolume delete /novarewind/@orig";
            error = system(m_command.toUtf8());
        }

        m_command = "sudo mv /novarewind/@ /novarewind/@orig";
        error = system(m_command.toUtf8());

        m_command = "sudo btrfs subvolume snapshot /novarewind/snapshots/" + dateTime + " /novarewind/@";
        error = system(m_command.toUtf8());
    }
    else {
        if (dirOrig.exists("/novarewind/@orig")) {
            m_command = "echo " + sudoPwd + " | sudo -S btrfs subvolume delete /novarewind/@orig";
            error = system(m_command.toUtf8());
        }

        m_command = "echo " + sudoPwd + " | sudo -S mv /novarewind/@ /novarewind/@orig";
        error = system(m_command.toUtf8());

        m_command = "echo " + sudoPwd + " | sudo -S btrfs subvolume snapshot /novarewind/snapshots/" + dateTime + " /novarewind/@";
        error = system(m_command.toUtf8());
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

            m_command = "sudo mkdir -p /novarewind/rc";
            error = system(m_command.toUtf8());

            m_command = "sudo chown " + userName + ":" + userName + " /novarewind/rc";
            error = system(m_command.toUtf8());
        }
        else {
            m_command = "echo " + sudoPwd + " | sudo -S mkdir -p /novarewind/rc";
            error = system(m_command.toUtf8());

            m_command = "echo " + sudoPwd + " | sudo -S chown " + userName.toUtf8() + ":" + userName.toUtf8() + " /novarewind/rc";
            error = system(m_command.toUtf8());
        }
    }

    if (getMode() == "console" || getMode() == "daily" || getMode() == "weekly" || getMode() == "monthly") {
        if (QFile::exists("/novarewind/rc/novarewindrc")) {
            m_command = "sudo rm /novarewind/rc/novarewindrc";
            error = system(m_command.toUtf8());
        }
    }
    else {
        if (QFile::exists("/novarewind/rc/novarewindrc")) {
            m_command = "echo " + sudoPwd + " | sudo -S rm /novarewind/rc/novarewindrc";
            error = system(m_command.toUtf8());
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
            m_command = "sudo mkdir -p /novarewind";
            error = system(m_command.toUtf8());

            m_command = "sudo mount " + m_mntPoint + " /novarewind";
            error = system(m_command.toUtf8());

            m_command = "sudo mkdir -p /novarewind/snapshots";
            error = system(m_command.toUtf8());
        }
    }
    else {
        QDir dir;
        if (!dir.exists("/novarewind/@")) {
            m_command = "echo " + sudoPwd + " | sudo -S mkdir -p /novarewind";
            error = system(m_command.toUtf8());

            m_command = "echo " + sudoPwd + " | sudo -S mount " + m_mntPoint + " /novarewind";
            error = system(m_command.toUtf8());

            m_command = "echo " + sudoPwd + " | sudo -S mkdir -p /novarewind/snapshots";
            error = system(m_command.toUtf8());
        }
    }

    // Al ejecutar novarewind como root en "recovery mode" cambia el propietario. Recuperar usuario como propietario.

    QString userName = qgetenv("USER");

    if (QFile::exists("/novarewind/rc/novarewindrc")) {
        if (getMode() == "console" || getMode() == "daily" || getMode() == "weekly" || getMode() == "monthly") {

            m_command = "sudo chown " + userName + ":" + userName + " /novarewind/rc";
            error = system(m_command.toUtf8());

            m_command = "sudo chown " + userName + ":" + userName + " /novarewind/rc/novarewindrc";
            error = system(m_command.toUtf8());
        }
        else {

            m_command = "echo " + sudoPwd + " | sudo -S chown " + userName + ":" + userName + " /novarewind/rc";
            error = system(m_command.toUtf8());

            m_command = "echo " + sudoPwd + " | sudo -S chown " + userName + ":" + userName + " /novarewind/rc/novarewindrc";
            error = system(m_command.toUtf8());
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
        m_command = "sudo echo";
        error = system(m_command.toUtf8());
    }
    else {
        m_command = "echo " + sudoPwd + " | sudo -S echo";
        error = system(m_command.toUtf8());
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
