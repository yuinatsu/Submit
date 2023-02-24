#include <algorithm>
#include "../Camera.h"
#include "../../../Object/Obj.h"
#include "TraceModeCamera.h"

TraceModeCamera::TraceModeCamera(const std::weak_ptr<Obj>& traceTarget) :
	traceTarget_{traceTarget}
{
}

TraceModeCamera::~TraceModeCamera()
{
}

void TraceModeCamera::Update(Camera& camera, float delta)
{
	camera.cameraTime_ += delta;
	if (!traceTarget_.expired())
	{
		camera.pos_ = { Math::Clamp(traceTarget_.lock()->GetPos(),camera.rect_.leftTop,camera.rect_.rightBottom) };
	}
}

void TraceModeCamera::Draw(Camera& camera)
{
}
