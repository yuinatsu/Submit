#pragma once
#include <list>
#include <memory>

class ResultDraw;
class ResultData;

class ResultDrawCtrl
{
	using ResultDrawList = std::list<std::unique_ptr<ResultDraw>>;
public:

	/// <summary> ���U���g�̌��ʂ̕`����R���g���[������ </summary>
	/// <param name="data"> ���U���g�f�[�^ </param>
	ResultDrawCtrl(ResultData& data,float delta);

	~ResultDrawCtrl();
	void Update(float delta);
	void Draw(void);

	const bool IsStartedDraw(void) const;
private:

	// �`�悷�ׂ����ʂ̃��X�g
	ResultDrawList drawList_;

	// �o�ߎ���
	float time_;
};

