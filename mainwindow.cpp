#include <QDebug>
#include <QSettings>
#include <QMetaEnum>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "notifications/notifications.h"
#include "systemsounds/systemsounds.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	setAttribute(Qt::WA_QuitOnClose, false); // Keep app around

	// Engine
	pomodoro = new Pomodoro(this);
	connect(pomodoro, &Pomodoro::stateTransition, this, &MainWindow::onStateTransition);

	// Settings
	settings = new QSettings("com.irrationalidiom", "tomatotimer", this);

	// Task bar blink
	blink_timer = new QTimer(this);
	connect(blink_timer, &QTimer::timeout, this, &MainWindow::onBlinkTimerTimeout);

	// Bring up UI from designer...
	ui->setupUi(this);

	loadSettings();

	buildMenu();
	buildTrayIcon();
	buildSounds();

	connect(tray_menu, &QMenu::aboutToShow, this, &MainWindow::onMenuShow);
	connect(tray_menu, &QMenu::aboutToHide, this, &MainWindow::onMenuHide);

	tray_menu_timer = new QTimer(this);
	connect(tray_menu_timer, &QTimer::timeout, this, &MainWindow::onMenuTimerTick);

	tray_icon->show();

	if (notifications_enabled)
		ShowNotification("Tomato Timer", "", "Tomato Timer is now running");
}

MainWindow::~MainWindow()
{
	saveSettings();

	delete ui;
}

void MainWindow::buildMenu()
{
	QMenu* m = new QMenu(this);

	auto stateAction = new QAction("Pomodoro Stopped", nullptr);
	stateAction->setEnabled(false);
	Q_ASSERT(! state_action);
	state_action = stateAction;

	auto startTimerAction = new QAction("Start Pomodoro", nullptr);
	startTimerAction->setIcon(QIcon(":/resources/tomato.png"));
	connect(startTimerAction, &QAction::triggered, this, &MainWindow::doTimerStart);
	Q_ASSERT(! start_action);
	start_action = startTimerAction;

	auto stopTimerAction = new QAction("Stop", nullptr);
	stopTimerAction->setIcon(QIcon(":/resources/tomato_desat.png"));
	connect(stopTimerAction, &QAction::triggered, this, &MainWindow::doTimerStop);
	Q_ASSERT(! stop_action);
	stop_action = stopTimerAction;

	auto prefsAction = new QAction("Preferences...", nullptr);
	connect(prefsAction, &QAction::triggered, this, &MainWindow::doShowPrefs);

	auto quitAction = new QAction("Quit", nullptr);
	connect(quitAction, &QAction::triggered, this, &MainWindow::doQuit);

	m->addAction(stateAction);
	m->addSeparator();
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

void MainWindow::buildSounds()
{
	auto sounds = GetSystemSoundNames();

	ui->playSoundCombo->blockSignals(true);
	ui->playSoundCombo->addItems(sounds);

	if (sound_name.isEmpty())
	{
		ui->playSoundCombo->setCurrentIndex(0);
	}
	else
	{
		auto index = sounds.indexOf(sound_name);
		ui->playSoundCombo->setCurrentIndex(index);
		qDebug() << "sound index" << index;
	}

	ui->playSoundCombo->blockSignals(false);

	ui->playSoundCheck->setChecked(sound_enabled);
	ui->playSoundCombo->setEnabled(sound_enabled);

}

void MainWindow::loadSettings()
{
	qDebug() << "load settings";

	pomodoro->setPomodoroDuration(settings->value("pomodoro_duration", 25).toInt());
	pomodoro->setShortBreakDuration(settings->value("short_break_duration", 5).toInt());
	pomodoro->setLongBreakDuration(settings->value("long_break_duration", 15).toInt());
	pomodoro->setPomodoroPerLongBreak(settings->value("pomodoro_per_long_break", 4).toInt());

	sound_enabled = settings->value("sound_enabled", true).toBool();
	sound_name = settings->value("sound_name", "").toString();

	notifications_enabled = settings->value("notifications_enabled", true).toBool();

	blink_enabled = settings->value("blink_enabled", true).toBool();
}

void MainWindow::saveSettings()
{
	qDebug() << "save settings";

	settings->setValue("pomodoro_duration", pomodoro->pomodoroDuration());
	settings->setValue("short_break_duration", pomodoro->shortBreakDuration());
	settings->setValue("long_break_duration", pomodoro->longBreakDuration());
	settings->setValue("pomodoro_per_long_break", pomodoro->pomodoroPerLongBreak());

	settings->setValue("sound_enabled", sound_enabled);
	settings->setValue("sound_name", sound_name);

	settings->setValue("notifications_enabled", notifications_enabled);

	settings->setValue("blink_enabled", blink_enabled);

	settings->sync();
}

void MainWindow::doTimerStart()
{
	qDebug() << "Timer start";
	pomodoro->start();
}

void MainWindow::doTimerStop()
{
	qDebug() << "Timer stop";
	pomodoro->stop();
}

void MainWindow::doTimerPause()
{
	qDebug() << "Timer pause";
}

void MainWindow::onBlinkTimerTimeout()
{
	blink_count++;

	if (blink_count < 10)
	{
		bool blink = blink_count % 2 == 0;
		tray_icon->setIcon(blink_icon[blink ? 1 : 0]);
	}
	else
	{
		blink_timer->stop();
		tray_icon->setIcon(blink_icon[0]);
	}
}

void MainWindow::doShowPrefs()
{
	this->ui->shortBreakLengthSlider->setValue(pomodoro->shortBreakDuration());
	this->ui->longBreakLengthSlider->setValue(pomodoro->longBreakDuration());
	this->ui->pomodoroLengthSlider->setValue(pomodoro->pomodoroDuration());

	this->ui->pomodoroToLongBreakSpinner->setValue(pomodoro->pomodoroPerLongBreak());

	on_longBreakLengthSlider_sliderReleased();
	on_pomodoroLengthSlider_sliderReleased();
	on_shortBreakLengthSlider_sliderReleased();

	this->ui->playSoundCheck->setChecked(sound_enabled);

	this->ui->displayNotificationsCheck->setChecked(notifications_enabled);

	this->ui->blinkIconCheck->setChecked(blink_enabled);

	this->show();
	this->raise();
}

void MainWindow::doQuit()
{
	qDebug() << "do quit";

	qApp->quit();
}

void MainWindow::onStateTransition(Pomodoro::PomodoroState oldState, Pomodoro::PomodoroState newState)
{
	qDebug() << "old state:" << oldState;
	qDebug() << "new state:" << newState;

	QString icon = ":/resources/tomato_desat.png";
	QString message = "";
	QString menuMessage = "";

	switch (newState)
	{
		case Pomodoro::POMODORO:
		{
			icon = ":/resources/tomato.png";
			message = "Pomodoro started";
			menuMessage = "Pomodoro %1";
		} break;

		case Pomodoro::STOPPED:
		{
			message = "Pomodoro stopped";
			menuMessage = "Pomodoro stopped";
		} break;

		case Pomodoro::SHORT_BREAK:
		{
			icon = ":/resources/tomato_green.png";
			message = "Time for a short break";
			menuMessage = "Short break %1";
		} break;

		case Pomodoro::LONG_BREAK:
		{
			icon = ":/resources/tomato_blue.png";
			message = "Time for a longer break";
			menuMessage = "Long break %1";
		} break;
	}

	blink_timer->stop();

	tray_icon->setIcon(QIcon(icon));

	blink_icon[0] = QIcon(icon);
	blink_icon[1] = QIcon(":/resources/tomato_desat_trans.png");

	if (menuMessage.length())
	{
		state_action_text = menuMessage;
		state_action->setText(menuMessage);
	}

	if (message.length())
	{
		if (notifications_enabled)
			ShowNotification("Tomato Timer", nullptr, message.toLocal8Bit().data());

		if (sound_enabled)
			PlaySystemSound(sound_name);

		if (blink_enabled && newState != Pomodoro::STOPPED)
		{
			blink_count = 0;
			blink_timer->setInterval(1000);
			blink_timer->setSingleShot(false);
			blink_timer->start();
		}
	}
}

void MainWindow::onMenuShow()
{
	qDebug() << "Menu show";

	start_action->setEnabled(pomodoro->currentState() == Pomodoro::STOPPED);
	stop_action->setEnabled(pomodoro->currentState() != Pomodoro::STOPPED);

	tray_menu_timer->setSingleShot(false);
	tray_menu_timer->setInterval(1000);
	tray_menu_timer->start();
	onMenuTimerTick();
}

void MainWindow::onMenuHide()
{
	qDebug() << "Menu hide";
	tray_menu_timer->stop();
}

void MainWindow::onMenuTimerTick()
{
	if (! state_action_text.contains("%1"))
		return;

	int seconds = pomodoro->nextTimeout();
	int minutes = seconds / 60;
	int remSeconds = seconds % 60;

	auto timeStr = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(remSeconds, 2, 10, QChar('0'));

	QString text = state_action_text.arg(timeStr);
	state_action->setText(text);
}

bool MainWindow::event(QEvent* event)
{
	//qDebug() << "main window event: " << event->type();
	return QMainWindow::event(event);
}

void MainWindow::customEvent(QEvent* event)
{
	qDebug() << "custom event: " << event->type();

	QMainWindow::customEvent(event);

	if (event->type() == NOTIFICATION_EVENT)
	{
		NotificationEvent* e = static_cast<NotificationEvent*>(event);

		qDebug() << "NotificationEvent" << e->context.title << e->context.subtitle << e->context.message;
		qDebug() << "NofificationEvent buttons" << e->context.actionButton << e->context.otherButton;
		qDebug() << "NotificationEvent type" << e->type;
	}
}

/*
 * Preferences - Timer Tab
 */

// ---

void MainWindow::on_pomodoroLengthSlider_valueChanged(int value)
{
	this->ui->pomodoroLengthValueLbl->setText(tr("%n minute(s)", "", value));
	pomodoro->setPomodoroDuration(value);
}

void MainWindow::on_pomodoroLengthSlider_sliderPressed()
{
	this->ui->pomodoroLengthValueLbl->show();
}

void MainWindow::on_pomodoroLengthSlider_sliderReleased()
{
	this->ui->pomodoroLengthValueLbl->hide();
}

// ---

void MainWindow::on_shortBreakLengthSlider_valueChanged(int value)
{
	this->ui->shortBreakLengthValueLbl->setText(tr("%n minute(s)", "", value));
	pomodoro->setShortBreakDuration(value);
}

void MainWindow::on_shortBreakLengthSlider_sliderPressed()
{
	this->ui->shortBreakLengthValueLbl->show();
}

void MainWindow::on_shortBreakLengthSlider_sliderReleased()
{
	this->ui->shortBreakLengthValueLbl->hide();
}

// ---

void MainWindow::on_longBreakLengthSlider_valueChanged(int value)
{
	this->ui->longBreakLengthValueLbl->setText(tr("%n minute(s)", "", value));
	pomodoro->setLongBreakDuration(value);
}

void MainWindow::on_longBreakLengthSlider_sliderPressed()
{
	this->ui->longBreakLengthValueLbl->show();
}

void MainWindow::on_longBreakLengthSlider_sliderReleased()
{
	this->ui->longBreakLengthValueLbl->hide();
}

void MainWindow::on_pomodoroToLongBreakSpinner_valueChanged(int value)
{
	pomodoro->setPomodoroPerLongBreak(value);
}

// ---

/*
 * Preferences - Notifications Tab
 */

void MainWindow::on_playSoundCombo_currentIndexChanged(const QString &item)
{
	sound_name = item;

	qDebug() << "sound name changed" << sound_name;

	if (sound_enabled)
	{
		PlaySystemSound(sound_name);
	}
}

void MainWindow::on_playSoundCheck_toggled(bool checked)
{
	ui->playSoundCombo->setEnabled(checked);
	sound_enabled = checked;
}

void MainWindow::on_displayNotificationsCheck_toggled(bool checked)
{
	notifications_enabled = checked;
}

void MainWindow::on_blinkIconCheck_toggled(bool checked)
{
	blink_enabled = checked;
}
