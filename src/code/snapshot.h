#pragma once

#include <QObject>
#include <QDebug>
#include <QVariantList>
#include <QDate>

class Snapshot : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList points READ points WRITE setPoints NOTIFY pointsChanged)

public:
    explicit Snapshot(QObject *parent = nullptr);

public:
    QVariantList points() const;
    void setPoints(const QVariantList &points);
    Q_SIGNAL void pointsChanged();

private:
    QVariantList m_points;

private Q_SLOTS:
    void on_PointsChanged();

public:
    Q_INVOKABLE void setPassword(const QString& password);
    Q_INVOKABLE int createSnapshot();
    Q_INVOKABLE QVariantList listSnapshots();
    Q_INVOKABLE int removeSnapshot(const QString& dateTime);
    Q_INVOKABLE int restoreSnapshot(const QString& dateTime);
    Q_INVOKABLE void setMode(const QString& mode);
    Q_INVOKABLE QString getMode();
    Q_INVOKABLE int saveSnapshotsList();
    Q_INVOKABLE int readSnapshotsList();
    Q_INVOKABLE int mount();
    Q_INVOKABLE void addSnapshot(const QString& dateTime);
    Q_INVOKABLE void setDailyConfig(bool daily, int keepDaily);
    Q_INVOKABLE void setWeeklyConfig(bool weekly, int keepWeekly);
    Q_INVOKABLE void setMonthlyConfig(bool monthly, int keepMonthly);
    Q_INVOKABLE void removeAutoSnapshotsIfNeeded();
    Q_INVOKABLE void removeDailySnapshotsIfNeeded();
    Q_INVOKABLE void removeWeeklySnapshotsIfNeeded();
    Q_INVOKABLE void removeMonthlySnapshotsIfNeeded();
    Q_INVOKABLE QDate getLastSnapshot(const QString& mode);
    Q_INVOKABLE int createManualSnapshot();
    Q_INVOKABLE int createAutoSnapshot(const QString& mode, const QString& currentDateTime);
    Q_INVOKABLE QDate getSnapshotDate(const QString& dateTime);

private:
    QString m_password;
    QString m_mntPoint;
    QString m_mode;
    bool m_daily;
    bool m_weekly;
    bool m_monthly;
    int m_keepDaily;
    int m_keepWeekly;
    int m_keepMonthly;

    QString m_command;

Q_SIGNALS:
    void finished(QString data);
    void failed();
};
