
#include <iostream>

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "mainWindow.h"

using namespace std;

bool debugMode = true;

void messageOutput(QtMsgType type, const char *msg)
{
	switch (type) {
		case QtDebugMsg:
			if (debugMode) 
				cerr << "Debug: " << msg << endl;
			break;
		case QtWarningMsg:
			cerr << "Warning: " << msg << endl;
			break;
		case QtCriticalMsg:
			cerr << "Critical: " << msg << endl;
			break;
		case QtFatalMsg:
			cerr << "Fatal: " << msg << endl;
			abort();
	}
	
	cerr.flush();
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
