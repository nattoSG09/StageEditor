#pragma once
#include "StageObjectComponent.h"

class KeyComponent : public StageObjectComponent
{
public:
	KeyComponent(StageObject* myObj);
	void Initialize() override;
	void Update() override;
	json Save() override;
};

