#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QSystemTrayIcon>
#include <QSettings>
#include <QTimer>
#include <QMenu>

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

	void	loadSettings();
	void	saveSettings();

private slots:
	void	doTimerStart();
	void	doTimerStop();
	void	doTimerPause();

	void	onTimerTimeout();

	void	doShowPrefs();
	void	doQuit();

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

	QTimer*			timer;
	TimerType		timer_type;

	QSettings*		settings;
};

#endif // MAINWINDOW_H
