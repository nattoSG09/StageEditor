#pragma once
#include "../../../Engine/GameObject/GameObject.h"
#include <vector>
#include "StageObject.h"

using std::vector;

class Stage : public GameObject
{
	int selectedIndex_;
	vector<StageObject*> objects_;
	ComponentType selectedComponentType_;
public:
	Stage(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void DrawWorldOutliner();
	void DrawDatails();
	void DrawDebugTexts();

	void Add(StageObject* obj);
	bool Save(string filePath);
	bool Load(string filePath);
	void Delete(StageObject* obj);
	void AllDelete();

	vector<StageObject*> GetObjects() { return objects_; }
};