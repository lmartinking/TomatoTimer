#include "mainwindow.h"
#include <QApplication>
#include <QAbstractNativeEventFilter>
#include <QDebug>
#include <QWindow>

#include "notifications.h"

class TotatoTimeApplication : public QApplication
{
public:
	TotatoTimeApplication(int argc, char* argv[]) : QApplication(argc, argv) {};

protected:
	void customEvent(QEvent* e)
	{
		qDebug() << "app custom event: " << e->type();

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

	MainWindow w;

	return a.exec();
}
