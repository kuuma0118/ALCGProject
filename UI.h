#pragma once
#include "Engine/Utility/MathFunction.h"
#include "2D/Sprite.h"

struct UIStruct {
	bool isDraw_ = true;
	uint32_t textureHandle_ = 0;
	Vector2 position_{};
	float rotation_{};
	Vector2 size_{};
	Sprite* sprite_ = nullptr;;
};