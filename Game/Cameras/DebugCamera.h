#pragma once
#include "../../Engine/GameObject/GameObject.h"

class DebugCamera : public GameObject
{
	XMFLOAT2 angle_;
	float sensitivity_;
	float center_To_camTargetDistance_;
public:
	DebugCamera(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void Move(XMVECTOR dir, float speed);
};

