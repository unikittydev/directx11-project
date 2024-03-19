#pragma once

#include "Core/Types.h"
#include "IComponent.h"

class Transform : public IComponent
{
private:
	float3 translation{ .0f, .0f, .0f };
	quaternion rotation{.0f, .0f, .0f, 1.0f };
	float3 scale{ 1.0f, 1.0f, 1.0f };

	Transform* parent;

	matrix GetParentWorldMatrix() const;
public:
	Transform(Entity entity) : IComponent(entity), parent(nullptr)
	{

	}

	void Init() override;
	void Update() override;
	void Draw() override;
	void Destroy() override;

	void SetParent(Transform* parent);

	void SetLocalMatrix(matrix wtl);
	void SetWorldMatrix(matrix ltw);

	void SetLocalTranslation(const float3& translation);
	void SetLocalEuler(const float3& euler);
	void SetLocalRotation(const quaternion& rotation);
	void SetLocalScale(const float3& scale);

	void SetWorldTranslation(const float3& translation);
	void SetWorldEuler(const float3& euler);
	void SetWorldRotation(const quaternion& rotation);
	void SetWorldScale(const float3& scale);

	void Translate(const float3& translation);
	void RotateEuler(const float3& euler);
	void RotateEuler(float x, float y, float z);
	void Rotate(const quaternion& rotation);

	Transform* GetParent() const;

	matrix GetLocalMatrix() const;
	matrix GetWorldMatrix() const;

	float3 GetLocalTranslation() const;
	float3 GetLocalEuler() const;
	quaternion GetLocalRotation() const;
	float3 GetLocalScale() const;

	float3 GetWorldTranslation() const;
	float3 GetWorldEuler() const;
	quaternion GetWorldRotation() const;
	float3 GetWorldScale() const;

	float3 GetForward() const;
	float3 GetRight() const;
	float3 GetUp() const;
};