#include <QDebug>
#include <QSettings>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "notifications/notifications.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	setAttribute(Qt::WA_QuitOnClose, false); // Keep app around

	pomodoro_count = 0;
	pomodoro_per_long_break = 4;

	settings = new QSettings("com.irrationalidiom", "tomatotimer", this);

	timer = new QTimer(this);
	timer_type = TimerType::POMODORO;
	connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);

	loadSettings();

	ui->setupUi(this);

	buildMenu();
	buildTrayIcon();

	tray_icon->show();

	qDebug() << "timer: " << timer;
	qDebug() << "timer type: " << timer_type;

	ShowNotification("Tomato Timer", nullptr, "Tomato Timer is now running");
}

MainWindow::~MainWindow()
{
	saveSettings();

	delete ui;
}

void MainWindow::buildMenu()
{
	QMenu* m = new QMenu(this);

	auto startTimerAction = new QAction("Start Pomodoro", nullptr);
	startTimerAction->setIcon(QIcon(":/resources/tomato.png"));
	connect(startTimerAction, &QAction::triggered, this, &MainWindow::doTimerStart);

	auto stopTimerAction = new QAction("Stop", nullptr);
	stopTimerAction->setIcon(QIcon(":/resources/tomato_desat.png"));
	connect(stopTimerAction, &QAction::triggered, this, &MainWindow::doTimerStop);

	auto prefsAction = new QAction("Preferences...", nullptr);
	connect(prefsAction, &QAction::triggered, this, &MainWindow::doShowPrefs);

	auto quitAction = new QAction("Quit", nullptr);
	connect(quitAction, &QAction::triggered, this, &MainWindow::doQuit);

	m->addAction(startTimerAction);
	m->addAction(stopTimerAction);
	m->addSeparator();
	m->addAction(prefsAction);
	m->addSeparator();
	m->addAction(quitAction);

	this->tray_menu = m;
}

void MainWindow::buildTrayIcon()
{
	Q_ASSERT(this->tray_menu);

	QSystemTrayIcon *ico = new QSystemTrayIcon(this);
	ico->setIcon(QIcon(":/resources/tomato_desat.png"));
	ico->setContextMenu(this->tray_menu);

	this->tray_icon = ico;
}

void MainWindow::loadSettings()
{
	qDebug() << "load settings";

	pomodoro_duration = settings->value("pomodoro_duration", 25).toInt();
	short_break_duration = settings->value("short_break_duration", 5).toInt();
	long_break_duration = settings->value("long_break_duration", 15).toInt();

	// TODO current timer state
}

void MainWindow::saveSettings()
{
	qDebug() << "save settings";

	settings->setValue("pomodoro_duration", pomodoro_duration);
	settings->setValue("short_break_duration", short_break_duration);
	settings->setValue("long_break_duration", long_break_duration);

	// TODO current timer state

	settings->sync();
}

void MainWindow::doTimerStart()
{
	qDebug() << "Timer start";

	int interval = 0;
	QString icon = ":/resources/tomato_desat.png";
	QString notification_icon = "";
	QString message = "";

	if (timer_type == POMODORO)
	{
		interval = pomodoro_duration;
		icon = ":/resources/tomato.png";
		message = "Pomodoro started";
	}
	else if (timer_type == SHORT_BREAK)
	{
		interval = short_break_duration;
		icon = ":/resources/tomato_green.png";
		message = "Time for a short break";
	}
	else if (timer_type == LONG_BREAK)
	{
		interval = long_break_duration;
		icon = ":/resources/tomato_blue.png";
		message = "Time for a longer break";
	}

	interval = 1; // FUDGE

	tray_icon->setIcon(QIcon(icon));

	if (interval)
	{
		timer->setSingleShot(true);
		timer->start(interval * 60 * 1000);
	}

	if (message.length())
	{
		ShowNotification("Tomato Timer", nullptr, message.toLocal8Bit().data());
		//tray_icon->showMessage("Tomato Timer", message, QSystemTrayIcon::Information, 5000);
	}
}

void MainWindow::doTimerStop()
{
	qDebug() << "Timer stop";

	tray_icon->setIcon(QIcon(":/resources/tomato_desat.png"));
	timer->stop();
}

void MainWindow::doTimerPause()
{
	qDebug() << "Timer pause";
}

void MainWindow::onTimerTimeout()
{
	qDebug() << "Timer timeout";

	if (timer_type == POMODORO)
	{
		pomodoro_count++;

		qDebug() << "pomodoro count: " << pomodoro_count;

		if (pomodoro_count == pomodoro_per_long_break)
		{
			pomodoro_count = 0;
			timer_type = LONG_BREAK;
		}
		else
		{
			timer_type = SHORT_BREAK;
		}
	}
	else if (timer_type == SHORT_BREAK)
	{
		timer_type = POMODORO;
	}
	else if (timer_type == LONG_BREAK)
	{
		timer_type = POMODORO;
	}

	qDebug() << "timer type now: " << timer_type;

	doTimerStart();
}

void MainWindow::doShowPrefs()
{
	if (this->isVisible())
	{
		this->raise();
	}
	else
	{
		this->show();
	}
}

void MainWindow::doQuit()
{
	qDebug() << "do quit";

	qApp->quit();
}

bool MainWindow::event(QEvent* event)
{
	qDebug() << "main window event: " << event->type();
	return QMainWindow::event(event);
}

void MainWindow::customEvent(QEvent* event)
{
	qDebug() << "custom event: " << event->type();

	QMainWindow::customEvent(event);

	if (event->type() == NOTIFICATION_EVENT)
	{
		NotificationEvent* e = (NotificationEvent*)event;

		qDebug() << "NotificationEvent" << e->context.title << e->context.subtitle << e->context.message;
		qDebug() << "NofificationEvent buttons" << e->context.actionButton << e->context.otherButton;
	}
}
