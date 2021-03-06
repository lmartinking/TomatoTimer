#ifndef POMODORO_H
#define POMODORO_H

#include <QObject>
#include <QTimer>
#include <QMap>


class Pomodoro : public QObject
{
	Q_OBJECT

public:
	enum PomodoroState {
		STOPPED,
		POMODORO,
		SHORT_BREAK,
		LONG_BREAK
	};

	Q_ENUM(PomodoroState)

public:
	explicit Pomodoro(QObject *parent = nullptr);

	PomodoroState currentState();

	int		nextTimeout();

	int		pomodoroDuration() { return pomodoro_duration; }
	int		shortBreakDuration() { return short_break_duration; }
	int		longBreakDuration() { return long_break_duration; }
	int		pomodoroPerLongBreak() { return pomodoro_per_long_break; }

signals:
	void	stateTransition(PomodoroState oldState, PomodoroState newState);

public slots:
	void	start();
	void	stop();

	void	resetToDefaults();

	void	setPomodoroDuration(int minutes);
	void	setShortBreakDuration(int minutes);
	void	setLongBreakDuration(int minutes);
	void	setPomodoroPerLongBreak(int count);

private slots:
	void	timeout();

private:
	QTimer*	timer;

	PomodoroState	state;

	int		pomodoro_count;

	int		pomodoro_duration;
	int		short_break_duration;
	int		long_break_duration;
	int		pomodoro_per_long_break;
};

#endif // POMODORO_H
