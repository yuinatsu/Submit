#include "Raycast.h"
#include "../../_debug/_DebugDispOut.h"

bool Raycast::CheckCollision(const Ray& ray, std::pair<Vector2, Vector2> col)
{
	bool reFlag = false;
	Vector2Flt colF = static_cast<Vector2Flt>(col.first);
	Vector2Flt colS = static_cast<Vector2Flt>(col.second);
	reFlag |= CheckLine(ray, Line(colF, Vector2Flt{ colF.x + colS.x,colF.y }));
	reFlag |= CheckLine(ray, Line(Vector2Flt{ colF.x + colS.x,colF.y }, static_cast<Vector2Flt>(colF + colS)));
	reFlag |= CheckLine(ray, Line(colF + colS, Vector2Flt{ colF.x,colF.y + colS.y }));
	reFlag |= CheckLine(ray, Line(Vector2Flt{ colF.x,colF.y + colS.y }, colF));
	return reFlag;
}

bool Raycast::CheckLine(Ray ray, Line line)
{
	const Vector2Flt v = line.point - ray.point;
	float crsA = static_cast<float>(Cross(ray.vec, line.vec()));
	// ïΩçsÇ©Ç«Ç§Ç©
	if (crsA == 0.0f)
	{
		return false;
	}

	float crsB = static_cast<float>(Cross(v, line.vec()));
	float crsC = static_cast<float>(Cross(v, ray.vec));

	float tA = crsB / crsA;
	float tB = crsC / crsA;

	const float esp = 0.00001f;

	if (tA + esp < 0.0f || tA - esp > 1.0f || tB + esp < 0.0f || tB - esp > 1.0f)
	{
		return false;
	}

	//_dbgDrawLine(ray.point.x, ray.point.y, ray.point.x + ray.vec.x, ray.point.y + ray.vec.y, 0xffff00);
	return true;
}
