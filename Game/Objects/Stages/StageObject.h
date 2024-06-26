#pragma once
#include "../../../Engine/GameObject/GameObject.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

class StageObjectComponent;

// ComponentTypeの定義
enum ComponentType {
	COMP_TESTMOVE,
	COMP_INPUTMOVE,
	COMP_GRAVITY,
	COMP_MAX
};

// ComponentTypeを文字列に変換する関数
const string ComponentTypeToString(ComponentType type);


class StageObject : public GameObject
{
	string modelFilePath_;
	int modelHandle_;
	vector<StageObjectComponent*> components_;
	friend class Stage;
public:
	StageObject(string name,string modelFilePath,GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void Move(XMVECTOR dir, float speed);

	void AddComponent(StageObjectComponent* comp);
	void DeleteComponent(StageObjectComponent* comp);
	int GetModelHandle() { return modelHandle_; }
};

StageObject* CreateStageObject(string name, string modelFilePath, GameObject* parent);
