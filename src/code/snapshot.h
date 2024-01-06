#pragma once

#include <QObject>
#include <QDebug>

class Snapshot : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList points READ points WRITE setPoints NOTIFY pointsChanged)

public:
    QStringList points() const;
    void setPoints(const QStringList &points);
    Q_SIGNAL void pointsChanged();

private:
    QStringList m_points;

private slots:
    void on_PointsChanged();

public:
    Q_INVOKABLE void setPassword(const QString& password);
    Q_INVOKABLE void createSnapshot(const QString& findmntcommand);
    Q_INVOKABLE void listSnapshots(const QString& findmntcommand);
    Q_INVOKABLE void removeSnapshot(const QString& dateTime);
    Q_INVOKABLE void restoreSnapshot(const QString& dateTime);
    Q_INVOKABLE void setMode(const QString& mode);
    Q_INVOKABLE QString getMode();

private:
    QString m_password;
    QString m_mntPoint;
    QString m_mode;

signals:
    void finished(QString data);
    void failed();
};
