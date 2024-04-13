#pragma once
#include "Game/Objects/Stages/StageObject.h"

class StageColision
{
public:
	StageColision(float len,vector<StageObject*> objects);
	~StageColision();

	bool LoadData();



};

