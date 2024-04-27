#pragma once
#include "../StageObject.h"
#include "../../../../Engine/Json/JsonReader.h"

class StageObjectComponent
{
protected:
	string name_;
	StageObject* myObject;
	friend class Stage;
public:
	StageObjectComponent(StageObject* myObj,string name);
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual json Save() = 0;
	virtual void DrawDatails(){}
	virtual void OnCollision(GameObject* pTarget){}
};

