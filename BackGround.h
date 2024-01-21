#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"
#include "Utility/CollisionManager/Collider.h"

class BackGround
{
public:

	void Initialize();

	void Update();

	void Draw(const ViewProjection viewProjection);

private:
	Input* input_ = nullptr;

	std::unique_ptr<Model>skyDomeModel_ = nullptr;

	WorldTransform skyDomeWorldTransform_;

	std::unique_ptr<Model>stageModel_ = nullptr;

	WorldTransform stageWorldTransform_;

	uint32_t textureHandle_ = 0u;
};

