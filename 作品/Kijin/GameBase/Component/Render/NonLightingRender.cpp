#include "NonLightingRender.h"
#include <DxLib.h>

void NonLightingRender::Draw(void)
{
	SetUseLighting(false);
	MV1DrawModel(*handle_);
	SetUseLighting(true);
}

void NonLightingRender::Draw(int shadowMap, int buff)
{
	SetUseLighting(false);
	MV1DrawModel(*handle_);
	SetUseLighting(true);
}

void NonLightingRender::SetUpShadowMap(void)
{
	SetUseLighting(false);
	MV1DrawModel(*handle_);
	SetUseLighting(true);
}
