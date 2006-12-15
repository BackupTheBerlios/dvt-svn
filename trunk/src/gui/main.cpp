
#include <iostream>

#include <QApplication>

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
//	Q_INIT_RESOURCE(main);
	
	qInstallMsgHandler(messageOutput);
	QApplication app(argc, argv);
	
	if (argc > 1) {
		QString s = argv[1];
		if (s == "--debug") debugMode = true;
	}
	
	MainWindow mainWindow;
	mainWindow.show();
	
	return app.exec();
}
