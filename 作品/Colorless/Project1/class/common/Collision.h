#pragma once
#include <list>
#include "Vector2.h"
#include "ColorID.h"

using Collision = std::pair<Vector2F, Vector2F>;							// 左上頂点, (幅, 高さ)
using CollisionColor = std::tuple<Vector2F, Vector2F, Color>;				// 左上頂点, (幅, 高さ, 色情報)
using ColList = std::list<CollisionColor>;									// 左上頂点, Vector2F(幅, 高さ)