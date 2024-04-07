#pragma once
#include "StageObjectComponent.h"

class TestMove : public StageObjectComponent
{
	float angle;
public:
	TestMove(StageObject* myObj);
	void Initialize() override;
	void Update() override;
	json Save() override;
};

