#pragma once
#include <list>
#include "Vector2.h"
#include "ColorID.h"

using Collision = std::pair<Vector2F, Vector2F>;							// ���㒸�_, (��, ����)
using CollisionColor = std::tuple<Vector2F, Vector2F, Color>;				// ���㒸�_, (��, ����, �F���)
using ColList = std::list<CollisionColor>;									// ���㒸�_, Vector2F(��, ����)