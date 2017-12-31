#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QSystemTrayIcon>
#include <QSettings>
#include <QTimer>
#include <QMenu>
#include <QUuid>

#include "pomodoro.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	void	buildMenu();
	void	buildTrayIcon();
	void	buildSounds();

	void	loadSettings();
	void	saveSettings();

private slots:
	void	doTimerStart();
	void	doTimerStop();
	void	doTimerPause();

	void	onBlinkTimerTimeout();

	void	doShowPrefs();
	void	doQuit();

	void	onStateTransition(Pomodoro::PomodoroState oldState, Pomodoro::PomodoroState newState);

	void	on_pomodoroToLongBreakSpinner_valueChanged(int arg1);
	void	on_longBreakLengthSlider_valueChanged(int value);
	void	on_shortBreakLengthSlider_valueChanged(int value);
	void	on_pomodoroLengthSlider_valueChanged(int value);

	void	on_pomodoroLengthSlider_sliderPressed();
	void	on_pomodoroLengthSlider_sliderReleased();

	void	on_shortBreakLengthSlider_sliderPressed();
	void	on_shortBreakLengthSlider_sliderReleased();

	void	on_longBreakLengthSlider_sliderPressed();
	void	on_longBreakLengthSlider_sliderReleased();

	void	on_playSoundCombo_currentIndexChanged(const QString &item);
	void	on_playSoundCheck_toggled(bool checked);
	void	on_displayNotificationsCheck_toggled(bool checked);
	void	on_blinkIconCheck_toggled(bool checked);

protected:
	bool	event(QEvent* event);
	void	customEvent(QEvent* event);

private:
	Ui::MainWindow *ui;

	QSystemTrayIcon*	tray_icon;
	QMenu*				tray_menu;

	QAction*			state_action;

	Pomodoro*			pomodoro;

	bool			sound_enabled;
	QString			sound_name;

	bool			notifications_enabled;

	bool			blink_enabled;
	QTimer*			blink_timer;
	bool			blink;
	int				blink_count;
	QIcon			blink_icon[2];

	QSettings*		settings;
};

#endif // MAINWINDOW_H
