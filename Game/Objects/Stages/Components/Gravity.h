#pragma once
#include "StageObjectComponent.h"

class Gravity : public StageObjectComponent
{
	float speed_;
	float time_;
public:
	Gravity(StageObject* myObj);
	void Initialize() override;
	void Update() override;
	json Save() override;
	void DrawDatails() override;
};

