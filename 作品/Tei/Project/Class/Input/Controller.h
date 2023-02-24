#pragma once
#include <array>
#include <list>
#include <utility>
#include <map>
#include "INPUT_ID.h"


using TrgBool = std::array<bool, static_cast<unsigned int>(TRG::MAX)>;
using TrgData = std::map<INPUT_ID, TrgBool>;
using KeyConfig = std::map<INPUT_ID, unsigned int>;

class Controller
{
public:
	struct RingBuf
	{
		RingBuf()
		{
			id_ = 0;
			num = 5;
			time_ = 0.0;
		};
		unsigned int id_;
		double time_;
		int num;
		RingBuf* befor_ = nullptr;
		RingBuf* next_ = nullptr;
	};
	Controller();
	virtual ~Controller();
	
	virtual bool Init(void) = 0;

	virtual void Update(void) = 0;

	void UpdateRingBuf(const double& delta);
	void DebugRingBuf(void);

	virtual const InputType GetInputType(void) = 0;
	const TrgData& GetCntData(void);
	const bool GetTrg(INPUT_ID id);
	const bool GetNow(INPUT_ID id);

	const RingBuf* GetRingBuf(void);

private:
protected:
	TrgData trgData_;
	KeyConfig config_;
	RingBuf* ringBuf_;
	std::map<CMD_ID,unsigned int> chengCMDtoINPUTMap_;
};

