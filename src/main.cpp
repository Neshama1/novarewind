// INCLUDE (BASIC SET)

#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QIcon>

#include <KAboutData>
#include <KLocalizedString>

// INCLUDE

#include <MauiKit4/Core/mauiapp.h>
#include <MauiKit4/FileBrowsing/fmstatic.h>
#include <MauiKit4/FileBrowsing/moduleinfo.h>
#include <MauiMan4/thememanager.h>
#include "code/snapshot.h"
#include "../novarewind_version.h"

#define NOVAREWIND_URI "org.kde.novarewind"

// MAIN FUNCTION

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    // APP

    if (argc > 1) {

        qputenv("QT_QPA_PLATFORM",QByteArray("offscreen"));

        QApplication appCLI(argc, argv, false);

        QCommandLineParser parserCLI;
        parserCLI.setApplicationDescription("\nCreate restore points on btrfs file sytems\n\nUsage:\n  novarewind list\n  novarewind create\n  novarewind restore 20240207-191526\n  novarewind remove 20240208-110002");
        parserCLI.addHelpOption();
        parserCLI.addPositionalArgument(QStringLiteral("list"), "list restore points", "[list]");
        parserCLI.addPositionalArgument(QStringLiteral("create"), "create a restore point", "[create]");
        parserCLI.addPositionalArgument(QStringLiteral("restore"), "restore a snapshot", "[restore]");
        parserCLI.addPositionalArgument(QStringLiteral("remove"), "remove a snapshot", "[remove]");
        parserCLI.process(appCLI);

        QStringList args = parserCLI.positionalArguments();

        QString arg0;
        QString arg1;

        arg0 = args[0];

        if (args.size() == 2) {
            arg1 = args[1];
        }

        Snapshot snapshot;

        if (arg0 == "list") {
            snapshot.setMode("console");
            QVariantList points = snapshot.listSnapshots();
            qInfo() << "\nRestore points:\n";
            for (int i=0 ; i<points.count() ; i++) {
                QVariantMap point = points[i].toMap();
                qInfo() << i+1 << point["dateTime"].toString() << "type:" << point["type"].toString();
            }

            qInfo() << "\n";
        }
        else if (arg0 == "create") {
            if (arg1 == "") {
                snapshot.setMode("console");
            }
            else {
                snapshot.setMode(arg1);
            }
            int error = 0;

            error = snapshot.readSnapshotsList();
            error = snapshot.createSnapshot();

            if (error == 0) {
                qInfo() << "\nA new restore point has been created successfully\n";
            }
        }
        else if (arg0 == "restore") {
            snapshot.setMode("console");
            int error = snapshot.restoreSnapshot(arg1);
            if (error == 0) {
                qInfo() << "\nA new point has been restored successfully\n";
            }
        }
        else if (arg0 == "remove") {
            snapshot.setMode("console");
            int error = 0;
            error = snapshot.readSnapshotsList();
            error = snapshot.removeSnapshot(arg1);
            if (error == 0) {
                qInfo() << "\nRestore point has been removed successfully\n";
            }
        }

        return 0;
    }
	else if (argc == 1)
	{
		#ifdef Q_OS_ANDROID
    		QGuiApplication app(argc, argv);
    		if (!MAUIAndroid::checkRunTimePermissions({"android.permission.WRITE_EXTERNAL_STORAGE"}))
        		return -1;
		#else
    		QGuiApplication app(argc, argv);
		#endif

    	app.setOrganizationName("KDE");
        app.setWindowIcon(QIcon(":/assets/logo.svg"));
    	QGuiApplication::setDesktopFileName(QStringLiteral("project"));
    	KLocalizedString::setApplicationDomain("novarewind");

    	// SETS THE VALUE OF THE ENVIRONMENT VARIABLES

    	// qputenv("ENVIRONMENT_VARIABLE", "1");

    	// ABOUT DIALOG

    	KAboutData about(QStringLiteral("novarewind"),
        	             QStringLiteral("novarewind"),
            	         NOVAREWIND_VERSION_STRING,
                         i18n(""),
                         KAboutLicense::LGPL_V3,
                     	 APP_COPYRIGHT_NOTICE,
                     	 QString(GIT_BRANCH) + "/" + QString(GIT_COMMIT_HASH));

    	about.addAuthor(QStringLiteral("Miguel Beltrán"), i18n("Developer"), QStringLiteral("novaflowos@gmail.com"));
        about.setHomepage("https://www.novaflowos.com");
        about.setProductName("novarewind");
        about.setBugAddress("https://github.com/Neshama1/novarewind/issues");
        about.setOrganizationDomain(NOVAREWIND_URI);
        about.setProgramLogo(app.windowIcon());

        const auto FBData = MauiKitFileBrowsing::aboutData();
        about.addComponent(FBData.name(), MauiKitFileBrowsing::buildVersion(), FBData.version(), FBData.webAddress());

        KAboutData::setApplicationData(about);
        MauiApp::instance()->setIconName("qrc:/assets/logo.svg");

        // COMMAND LINE

        QCommandLineParser parser;

        about.setupCommandLine(&parser);
        parser.process(app);
        about.processCommandLine(&parser);

        const QStringList args = parser.positionalArguments();
        QPair<QString, QList<QUrl>> arguments;

        // QQMLAPPLICATIONENGINE

        QQmlApplicationEngine engine;
        const QUrl url(QStringLiteral("qrc:/main.qml"));
        QObject::connect(
            &engine,
            &QQmlApplicationEngine::objectCreated,
            &app,
            [url, &arguments](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            },
            Qt::QueuedConnection);

        // C++ BACKENDS

		Snapshot snapshot;
        qmlRegisterSingletonInstance<Snapshot>("org.kde.novarewind", 1, 0, "Snapshot", &snapshot);

        // TIPOS

        qmlRegisterType<MauiMan::ThemeManager>("org.kde.novarewind", 1, 0, "ThemeManager");

        // LOAD MAIN.QML

        engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
        engine.load(url);

        // EXEC APP

        return app.exec();
	}
}
