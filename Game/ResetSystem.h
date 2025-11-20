#ifndef RESET_SYSTEM_CLASS_H
#define RESET_SYSTEM_CLASS_H



class ResetSystemFunction: public Unit
{
	int32 GetReset() const { return this->GetResetSystemData(RESET_TOTAL)->GetReset(); }

	//DECLARE_ARRAY_STRUCT(ResetSystem, ResetSystem, RESET_MAX);
	DECLARE_ARRAY_STRUCT(ResetSystemData, ResetSystemData, RESET_MAX);
};

struct ResetSystemData
{
	DECLARE_PROPERTY(int32, Reset);
	//DECLARE_PROPERTY(int32, )
	void Reset() {
		this->SetReset(0);
	}

	void Set(ResetSystemData const* Data)
	{
		this->SetReset(Data->GetReset());
	}
};

enum
{
	RESET_TOTAL,
	//RESET_LEVEL,
	//RESET_BLESS,
	//RESET_SOUL,
	//RESET_CHAOS,
	//RESET_LIFE,
	//RESET_CRE
	RESET_MAX
};

class ResetSystem
{
	SingletonInstance(ResetSystem);
public:

};

#endif