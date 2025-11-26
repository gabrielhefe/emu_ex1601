#ifndef TICK_TIMER_H
#define TICK_TIMER_H

class TickTimer
{
public:
	TickTimer();
	virtual ~TickTimer();

	void Reset();
	void Start();
	void Start(uint32 delay);

	bool Elapsed();
	bool Elapsed(uint32 delay);
	bool ExElapsed(uint32 delay);

	uint32 GetElapsed() const;
	uint32 GetRemain() const;
	bool Finished() const;

	TCType m_Timer;
	uint32 m_Delay;
};

struct Timer
{
	TCType m_Tick;
	int32 m_RemainMsec;

	Timer() { this->Reset(); }
	virtual ~Timer() {}

	void Reset()
	{
		this->m_Tick = 0;
		this->m_RemainMsec = 0;
	}

	void Start(int32 remain_msec)
	{
		this->m_Tick = MyGetTickCount();
		this->m_RemainMsec = remain_msec;
	}

	bool Elapsed(int32 msec)
	{
		int32 iTICK_MSEC = int32(MyGetTickCount() - this->m_Tick);

		if ( iTICK_MSEC >= msec )
		{
			this->m_RemainMsec -= iTICK_MSEC;
			this->m_Tick = MyGetTickCount();
			return true;
		}

		return false;
	}

	bool Finised() const { return this->m_RemainMsec <= 0; }
};

#endif
