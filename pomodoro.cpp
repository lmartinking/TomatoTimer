#include "pomodoro.h"


Pomodoro::Pomodoro(QObject *parent) : QObject(parent)
{
	timer = new QTimer(this);

	connect(timer, &QTimer::timeout, this, &Pomodoro::timeout);

	resetToDefaults();
}

PomodoroState Pomodoro::currentState()
{
	return state;
}

void Pomodoro::start()
{
	if (state == STOPPED)
	{
		timeout();
	}
}

void Pomodoro::stop()
{
	timer->stop();

	PomodoroState prevState = state;
	state = STOPPED;

	emit stateTransition(prevState, state);
}

void Pomodoro::resetToDefaults()
{
	state = STOPPED;
	pomodoro_count = 0;
	pomodoro_duration = 25;
	short_break_duration = 5;
	long_break_duration = 15;
	pomodoro_per_long_break = 4;
}

void Pomodoro::setPomodoroDuration(int minutes)
{
	pomodoro_duration = minutes;
}

void Pomodoro::setShortBreakDuration(int minutes)
{
	short_break_duration = minutes;
}

void Pomodoro::setLongBreakDuration(int minutes)
{
	long_break_duration = minutes;
}

void Pomodoro::setPomodoroPerLongBreak(int count)
{
	pomodoro_per_long_break = count;
}

void Pomodoro::timeout()
{
	PomodoroState currentState = state;
	PomodoroState nextState = state;

	switch (currentState)
	{
		case STOPPED:
		{
			nextState = POMODORO;
		} break;

		case POMODORO:
		{
			pomodoro_count++;

			if (pomodoro_count == pomodoro_per_long_break)
			{
				nextState = LONG_BREAK;
			}
			else
			{
				nextState = SHORT_BREAK;
			}
		} break;

		case SHORT_BREAK:
		{
			nextState = POMODORO;
		} break;

		case LONG_BREAK:
		{
			nextState = POMODORO;
		} break;
	}

	int nextInterval = 0;

	switch (nextState)
	{
		case POMODORO:
		{
			if (currentState == STOPPED)
			{
				pomodoro_count = 0;
			}

			nextInterval = pomodoro_duration;
		} break;

		case SHORT_BREAK:
		{
			nextInterval = short_break_duration;
		} break;

		case LONG_BREAK:
		{
			pomodoro_count = 0;
			nextInterval = long_break_duration;
		} break;
	}

	if (nextInterval)
	{
		timer->setSingleShot(true);
		timer->start(nextInterval * 60 * 1000);
	}

	state = nextState;
	emit stateTransition(currentState, nextState);
}
