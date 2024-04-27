#pragma once
#include "../../../Engine/GameObject/GameObject.h"

class Player : public GameObject
{
	int modelHandle_;
	bool hasKey_;
public:
	Player(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void Move(XMVECTOR dir, float speed);

	void GetKey() { hasKey_ = true; }
	void LostKey() { hasKey_ = false; }
};

