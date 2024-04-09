#pragma once
#include "../../../Engine/GameObject/GameObject.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

class StageObjectComponent;

// ComponentType‚Ì’è‹`
enum ComponentType {
	COMP_TESTMOVE,
	COMP_INPUTMOVE,
	COMP_GRAVITY,
	COMP_MAX
};

// ComponentType‚ğ•¶š—ñ‚É•ÏŠ·‚·‚éŠÖ”
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
