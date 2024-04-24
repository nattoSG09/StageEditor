#include "TestObject.h"
#include "Engine/ResourceManager/Model.h"

TestObject::TestObject(GameObject* parent)
	:GameObject(parent,"TestObject")
{
}

void TestObject::Initialize()
{
	hModel_ = Model::Load("DebugCollision/BoxCollider.fbx");
}

// 現在の角度から目標の角度までのクォータニオンを計算する関数
XMVECTOR CalculateTargetQuaternion(float currentAngle, float targetAngle) {
	float halfTheta = (targetAngle - currentAngle) * 0.5f;
	XMVECTOR axis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // Y軸周りの回転を想定
	return XMQuaternionRotationAxis(axis, halfTheta);
}

// Slerpを使用してクォータニオンを補間する関数
XMVECTOR SlerpQuaternion(XMVECTOR startQuat, XMVECTOR endQuat, float t) {
	return XMQuaternionSlerp(startQuat, endQuat, t);
}

// クォータニオンから回転行列を計算する関数
XMMATRIX QuaternionToMatrix(XMVECTOR quat) {
	return XMMatrixRotationQuaternion(quat);
}

void TestObject::Update()
{
    static float currentAngle = 0.f;
    float targetAngle = 90.f;

    // 目標の角度までの回転を計算
    float rotationAmount = targetAngle - currentAngle;

    // 回転する行列を作成
    XMMATRIX rotationMatrix;

    if (rotationAmount > 0)
    {
        // 目標の角度が正の方向にある場合はY軸周りに回転
        rotationMatrix = XMMatrixRotationY(XMConvertToRadians(rotationAmount));
    }
    else if (rotationAmount < 0)
    {
        // 目標の角度が負の方向にある場合はZ軸周りに回転
        rotationMatrix = XMMatrixRotationZ(XMConvertToRadians(-rotationAmount));
    }
    else
    {
        // 目標の角度に到達している場合は回転しない
        rotationMatrix = XMMatrixIdentity();
    }

    // オブジェクトに回転を適用
    // ここで具体的なオブジェクトの回転処理を行う必要があります

    // 現在の角度を更新
    currentAngle += rotationAmount;
}

void TestObject::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void TestObject::Release()
{
}
