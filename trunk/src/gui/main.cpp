
#include <iostream>

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "mainWindow.h"

#include "debug.h"

using namespace std;

bool debugMode = true;

void messageOutput(QtMsgType type, const char *msg)
{
	switch (type) {
		case QtDebugMsg:
			if (debugMode)
				DEBUG(DBG_GUI, msg); 
			break;
		case QtWarningMsg:
			DEBUG(DBG_GUI, "WARNING: %s", msg);
			break;
		case QtCriticalMsg:
			DEBUG(DBG_GUI, "CRITICAL: %s", msg);
			break;
		case QtFatalMsg:
			DEBUG(DBG_GUI, "FATAL: %s", msg);
			abort();
	}
}

int main(int argc, char* argv[]) {
	Q_INIT_RESOURCE(dvt);
	
	if (argc > 1) {
		QString s = argv[1];
		if (s == "--debug") debugMode = true;
	}
	
	qInstallMsgHandler(messageOutput);
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/dvt/logo/dvt48.png"));
	if (app.windowIcon().isNull())
		qDebug("Window icon is null!");
	
	QString sysLocale = QLocale::system().name();
	QTranslator translatorDvt, translatorQt;
	translatorDvt.load(QString("dvt_%1").arg(sysLocale), "trans");
	translatorQt.load(QString("qt_%1").arg(sysLocale), "trans");
	app.installTranslator(&translatorDvt);
	app.installTranslator(&translatorQt);
	
	MainWindow mainWindow;
	mainWindow.show();
	
	return app.exec();
}
