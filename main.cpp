#include <QApplication>
#include <QAbstractNativeEventFilter>
#include <QDebug>
#include <QWindow>

#include <QTranslator>
#include <QLibraryInfo>

#include "mainwindow.h"


class TotatoTimeApplication : public QApplication
{
public:
	TotatoTimeApplication(int& argc, char* argv[]) : QApplication(argc, argv) {}

protected:
	void customEvent(QEvent* e)
	{
		qDebug() << "app custom event: " << e->type();

		// This is here soley to propagate custom events
		for (QWindow* w : this->topLevelWindows())
		{
			sendEvent(w, e);
		}
	}
};

int main(int argc, char *argv[])
{
	TotatoTimeApplication a(argc, argv);
	a.setWindowIcon(QIcon(":/resources/tomato.png"));

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(),
	QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&qtTranslator);

	QTranslator appTranslator;
	appTranslator.load(":/languages/en.qm");
	a.installTranslator(&appTranslator);

	MainWindow w;

	return a.exec();
}
