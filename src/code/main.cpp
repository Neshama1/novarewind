#include <QApplication>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>
#include <QDate>
#include <QIcon>
#include <QQmlContext>

#include <MauiKit3/Core/mauiapp.h>

#include <KAboutData>
#include <KI18n/KLocalizedString>

#include "../project_version.h"

#include "snapshot.h"

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QtGlobal>

//Useful for setting quickly an app template
#define ORG_NAME "KDE"
#define PROJECT_NAME "Nova Rewind"
#define COMPONENT_NAME "novarewind"
#define PROJECT_DESCRIPTION "Create restore points on btrfs file sytems"
#define PROJECT_YEAR "2024"
#define PRODUCT_NAME "novarewind"
#define PROJECT_PAGE "https://www.novaflowos.com"
#define REPORT_PAGE "https://bugs.kde.org/enter_bug.cgi?product=novarewind"

int main(int argc, char *argv[])
{
    if (argc > 1) {

        qputenv("QT_QPA_PLATFORM",QByteArray("offscreen"));

        QApplication appCLI(argc, argv, false);

        QCommandLineParser parserCLI;
        parserCLI.setApplicationDescription("\nCreate restore points on btrfs file sytems\n\nUsage:\n  novarewind list\n  novarewind create\n  novarewind restore 20240207-191526\n  novarewind remove 20240208-110002");
        parserCLI.addHelpOption();
        parserCLI.addPositionalArgument(QStringLiteral("list"), "list restore points", "+[list]");
        parserCLI.addPositionalArgument(QStringLiteral("create"), "create a restore point", "+[create]");
        parserCLI.addPositionalArgument(QStringLiteral("restore"), "restore a snapshot", "+[restore]");
        parserCLI.addPositionalArgument(QStringLiteral("remove"), "remove a snapshot", "+[remove]");
        parserCLI.process(appCLI);

        QStringList args = parserCLI.positionalArguments();

        QString arg0;
        QString arg1;

        arg0 = args[0];

        if (args.size() == 2) {
            arg1 = args[1];
        }

        Snapshot snapshot;
        snapshot.setMode("console");

        if (arg0 == "list") {
            QStringList points = snapshot.listSnapshots();
            qInfo() << "\nRestore points:\n";
            for (int i=0 ; i<points.count() ; i++) {
                qInfo() << i+1 << points[i];
            }

            qInfo() << "\n";
        }
        else if (arg0 == "create") {
            int error = snapshot.createSnapshot();
            if (error == 0) {
                qInfo() << "\nA new restore point has been created successfully\n";
            }
        }
        else if (arg0 == "restore") {
            int error = snapshot.restoreSnapshot(arg1);
            if (error == 0) {
                qInfo() << "\nA new point has been restored successfully\n";
            }
        }
        else if (arg0 == "remove") {
            int error = snapshot.removeSnapshot(arg1);
            if (error == 0) {
                qInfo() << "\nRestore point has been removed successfully\n";
            }
        }

        return 0;
    }
    else if (argc == 1) {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

        QApplication app(argc, argv, true);
        app.setOrganizationName(QStringLiteral(ORG_NAME));
        app.setWindowIcon(QIcon(":/logo.png"));

        KLocalizedString::setApplicationDomain(COMPONENT_NAME);

        KAboutData about(QStringLiteral(COMPONENT_NAME), i18n(PROJECT_NAME), PROJECT_VERSION_STRING, i18n(PROJECT_DESCRIPTION),
                        KAboutLicense::LGPL_V3, QString("© %1-%2 %3 Development Team").arg(PROJECT_YEAR, QString::number(QDate::currentDate().year()), ORG_NAME), QString(GIT_BRANCH) + "/" + QString(GIT_COMMIT_HASH));

        about.addAuthor(i18n("Miguel Beltrán"), i18n("Developer"), QStringLiteral("novaflowos@gmail.com"));

        about.setHomepage(PROJECT_PAGE);
        about.setProductName(PRODUCT_NAME);
        about.setBugAddress(REPORT_PAGE);
        about.setOrganizationDomain(PROJECT_URI);
        about.setProgramLogo(app.windowIcon());
        //about.addComponent("Akonadi");
        about.addCredit(i18n("MauiKit Developers"));

        KAboutData::setApplicationData(about);
        MauiApp::instance()->setIconName("qrc:/logo.svg");

        QCommandLineParser parser;
        parser.setApplicationDescription(about.shortDescription());
        parser.process(app);
        about.processCommandLine(&parser);

        QQmlApplicationEngine engine;

        Snapshot snapshot;
        qmlRegisterSingletonInstance<Snapshot>("org.kde.novarewind", 1, 0, "Snapshot", &snapshot);

        //qmlRegisterType<Process>("Process", 1, 0, "Process");

        engine.rootContext()->setContextObject(new KLocalizedContext(&engine));

        const QUrl url(QStringLiteral("qrc:/main.qml"));
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
        engine.load(url);

        return app.exec();
    }
}
