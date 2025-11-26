TickTimer::TickTimer()
{
	this->Reset();
}

TickTimer::~TickTimer()
{

}

void TickTimer::Reset()
{
	this->m_Timer = MyGetTickCount();
	this->m_Delay = 0;
}

void TickTimer::Start()
{
	this->m_Timer = MyGetTickCount();
}

void TickTimer::Start(uint32 delay)
{
	this->m_Timer = MyGetTickCount();
	this->m_Delay = delay;
}

bool TickTimer::Elapsed()
{
	return this->Elapsed(this->m_Delay);
}

bool TickTimer::ExElapsed(uint32 delay)
{
	return this->Elapsed(this->m_Delay + delay);
}

bool TickTimer::Elapsed(uint32 delay)
{
	if (MyGetTickCount() > (this->m_Timer + delay))
	{
		this->m_Timer = MyGetTickCount();
		return true;
	}

	return false;
}

uint32 TickTimer::GetElapsed() const
{
	return uint32(MyGetTickCount() - this->m_Timer);
}

uint32 TickTimer::GetRemain() const
{
	uint32 diff = uint32(MyGetTickCount() - this->m_Timer);

	if (diff < this->m_Delay)
	{
		diff = this->m_Delay - diff;
	}
	else
	{
		diff = 0;
	}

	return diff;
}

bool TickTimer::Finished() const
{
	return this->GetElapsed() >= this->m_Delay;
}
