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

// ���݂̊p�x����ڕW�̊p�x�܂ł̃N�H�[�^�j�I�����v�Z����֐�
XMVECTOR CalculateTargetQuaternion(float currentAngle, float targetAngle) {
	float halfTheta = (targetAngle - currentAngle) * 0.5f;
	XMVECTOR axis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // Y������̉�]��z��
	return XMQuaternionRotationAxis(axis, halfTheta);
}

// Slerp���g�p���ăN�H�[�^�j�I�����Ԃ���֐�
XMVECTOR SlerpQuaternion(XMVECTOR startQuat, XMVECTOR endQuat, float t) {
	return XMQuaternionSlerp(startQuat, endQuat, t);
}

// �N�H�[�^�j�I�������]�s����v�Z����֐�
XMMATRIX QuaternionToMatrix(XMVECTOR quat) {
	return XMMatrixRotationQuaternion(quat);
}

void TestObject::Update()
{
    static float currentAngle = 0.f;
    float targetAngle = 90.f;

    // �ڕW�̊p�x�܂ł̉�]���v�Z
    float rotationAmount = targetAngle - currentAngle;

    // ��]����s����쐬
    XMMATRIX rotationMatrix;

    if (rotationAmount > 0)
    {
        // �ڕW�̊p�x�����̕����ɂ���ꍇ��Y������ɉ�]
        rotationMatrix = XMMatrixRotationY(XMConvertToRadians(rotationAmount));
    }
    else if (rotationAmount < 0)
    {
        // �ڕW�̊p�x�����̕����ɂ���ꍇ��Z������ɉ�]
        rotationMatrix = XMMatrixRotationZ(XMConvertToRadians(-rotationAmount));
    }
    else
    {
        // �ڕW�̊p�x�ɓ��B���Ă���ꍇ�͉�]���Ȃ�
        rotationMatrix = XMMatrixIdentity();
    }

    // �I�u�W�F�N�g�ɉ�]��K�p
    // �����ŋ�̓I�ȃI�u�W�F�N�g�̉�]�������s���K�v������܂�

    // ���݂̊p�x���X�V
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
