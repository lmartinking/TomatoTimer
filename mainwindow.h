#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QSystemTrayIcon>
#include <QSettings>
#include <QTimer>
#include <QMenu>
#include <QUuid>

namespace Ui {
	class MainWindow;
}


enum TimerType {
	POMODORO,
	SHORT_BREAK,
	LONG_BREAK
};

class MainWindow : public QMainWindow
{
	Q_OBJECT
	Q_ENUM(TimerType)

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

	void	onTimerTimeout();

	void	doShowPrefs();
	void	doQuit();

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

protected:
	bool	event(QEvent* event);
	void	customEvent(QEvent* event);

private:
	Ui::MainWindow *ui;

	QSystemTrayIcon*	tray_icon;
	QMenu*				tray_menu;

	int				pomodoro_duration;
	int				short_break_duration;
	int				long_break_duration;
	int				pomodoro_per_long_break;

	int				pomodoro_count;

	bool			sound_enabled;
	QString			sound_name;

	bool			notifications_enabled;

	QTimer*			timer;
	TimerType		timer_type;

	QSettings*		settings;
};

#endif // MAINWINDOW_H
