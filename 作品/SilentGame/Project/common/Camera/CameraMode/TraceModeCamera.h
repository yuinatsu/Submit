#pragma once
#include <memory>
#include "CameraModeBase.h"

class Obj;

class TraceModeCamera :
    public CameraModeBase
{
public:
    TraceModeCamera(const std::weak_ptr<Obj>& traceTarget);
    ~TraceModeCamera();
    void Update(Camera& camera, float delta) final;
    void Draw(Camera& camera) final;
private:
    std::weak_ptr<Obj> traceTarget_;
};

