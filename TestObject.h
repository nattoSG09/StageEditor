#pragma once
#include "Engine/GameObject/GameObject.h"

class TestObject : public GameObject
{
	int hModel_;
public:
	TestObject(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

