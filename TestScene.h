#pragma once
#include "Engine/GameObject/GameObject.h"
#include <vector>

//テストシーンを管理するクラス
class TestScene : public GameObject
{
	int hModel;
	std::vector<XMFLOAT3> vs_;
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	TestScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};