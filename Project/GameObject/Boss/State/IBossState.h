#pragma once
#include "3D/Matrix/ViewProjection.h"

class Boss;

class IBossState {
public:
	virtual ~IBossState() {};
	virtual void Initialize(Boss* pBoss) = 0;
	virtual void Update(Boss* pBoss) = 0;
	virtual void Draw(Boss* pBoss, const ViewProjection& viewProjection) = 0;
	virtual void ApplyGlobalVariables() = 0;
};