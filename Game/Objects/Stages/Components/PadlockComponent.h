#pragma once
#include "StageObjectComponent.h"

class PadlockComponent : public StageObjectComponent
{
public:
	PadlockComponent(StageObject* myObj);
	void Initialize() override;
	void Update() override;
	json Save() override;
	void OnCollision(GameObject* pTarget) override;
};

