#pragma once
#include "../../../Engine/GameObject/GameObject.h"
class SkySphere : public GameObject
{
	int modelHandle_;
public:
	SkySphere(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

