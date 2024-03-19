#include "Transform.h"

void Transform::Init()
{

}

void Transform::Update()
{

}

void Transform::Draw()
{

}

void Transform::Destroy()
{

}

void Transform::SetParent(Transform* parent)
{
	this->parent = parent;
}

matrix GetDefaultWorldMatrix()
{
	return
		matrix::CreateScale(1.0f) *
		matrix::CreateFromYawPitchRoll({ .0f, .0f, .0f }) *
		matrix::CreateTranslation({ .0f, .0f, .0f });
}

matrix Transform::GetParentWorldMatrix() const
{
	return parent != nullptr ? parent->GetWorldMatrix() : GetDefaultWorldMatrix();
}

void Transform::SetLocalTranslation(const float3& translation)
{
	this->translation = translation;
}

void Transform::SetLocalEuler(const float3& euler)
{
	this->rotation = quaternion::CreateFromYawPitchRoll({euler.x, euler.y, euler.z});
}

void Transform::SetLocalRotation(const quaternion& rotation)
{
	this->rotation = rotation;
}

void Transform::SetLocalScale(const float3& scale)
{
	this->scale = scale;
}

void Transform::SetWorldTranslation(const float3& translation)
{
	auto world = matrix::CreateTranslation(translation) * GetParentWorldMatrix().Invert();

	float3 t, s;
	quaternion r;

	world.Decompose(s, r, t);
	this->translation = t;
}

void Transform::SetWorldEuler(const float3& euler)
{
	auto world = matrix::CreateFromYawPitchRoll(euler) * GetParentWorldMatrix().Invert();

	float3 t, s;
	quaternion r;

	world.Decompose(s, r, t);
	this->rotation = r;
}

void Transform::SetWorldRotation(const quaternion& rotation)
{
	auto world = matrix::CreateFromQuaternion(rotation) * GetParentWorldMatrix().Invert();

	float3 t, s;
	quaternion r;

	world.Decompose(s, r, t);
	this->rotation = r;
}

void Transform::SetWorldScale(const float3& scale)
{
	auto world = matrix::CreateScale(scale) * GetParentWorldMatrix().Invert();

	float3 t, s;
	quaternion r;

	world.Decompose(s, r, t);
	this->scale = s;
}

void Transform::SetLocalMatrix(matrix wtl)
{
	wtl.Decompose(scale, rotation, translation);
}

void Transform::SetWorldMatrix(matrix ltw)
{
	matrix wtl = ltw * GetParentWorldMatrix().Invert();
	SetLocalMatrix(wtl);
}

void Transform::Translate(const float3& translation)
{
	this->translation += translation;
}

void Transform::RotateEuler(const float3& euler)
{
	rotation *= quaternion::CreateFromYawPitchRoll({ euler.x, euler.y, euler.z });
}

void Transform::RotateEuler(float x, float y, float z)
{
	rotation *= quaternion::CreateFromYawPitchRoll(x, y, z);
}

void Transform::Rotate(const quaternion& rotation)
{
	this->rotation *= rotation;
}

Transform* Transform::GetParent() const
{
	return parent;
}

matrix Transform::GetLocalMatrix() const
{
	auto value =
		matrix::CreateScale(scale) *
		matrix::CreateFromQuaternion(rotation) *
		matrix::CreateTranslation(translation);
	return value;
}

matrix Transform::GetWorldMatrix() const
{
	return GetLocalMatrix() * GetParentWorldMatrix();
}

float3 Transform::GetLocalTranslation() const
{
	return translation;
}

float3 Transform::GetLocalEuler() const
{
	return rotation.ToEuler();
}

quaternion Transform::GetLocalRotation() const
{
	return rotation;
}

float3 Transform::GetLocalScale() const
{
	return scale;
}

float3 Transform::GetWorldTranslation() const
{
	float3 s, t;
	quaternion r;

	GetWorldMatrix().Decompose(s, r, t);
	return t;
}

float3 Transform::GetWorldEuler() const
{
	float3 s, t;
	quaternion r;

	GetWorldMatrix().Decompose(s, r, t);
	return r.ToEuler();
}

quaternion Transform::GetWorldRotation() const
{
	float3 s, t;
	quaternion r;

	GetWorldMatrix().Decompose(s, r, t);
	return r;
}

float3 Transform::GetWorldScale() const
{
	float3 s, t;
	quaternion r;

	GetWorldMatrix().Decompose(s, r, t);
	return s;
}

float3 Transform::GetForward() const
{
	return GetWorldMatrix().Forward();
}

float3 Transform::GetRight() const
{
	return GetWorldMatrix().Right();
}

float3 Transform::GetUp() const
{
	return GetWorldMatrix().Up();
}
