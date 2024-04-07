#pragma once
#include "StageObjectComponent.h"

class InputMove : public StageObjectComponent
{
	float angle;
public:
	InputMove(StageObject* myObj);
	void Initialize() override;
	void Update() override;
	json Save() override;
};

