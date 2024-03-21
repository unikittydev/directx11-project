#include "SphereCollider.h"

#include "SphereComponent.h"
#include "Transform.h"
#include "Games/Game.h"

SphereCollider::SphereCollider(Entity e) : IComponent(e)
{
        
}

void SphereCollider::Init()
{
    tr = Game::getInstance().GetComponent<Transform>(entity);
    BS.Radius = .5f / tr->GetWorldScale().y;
}

void SphereCollider::Update()
{
    if (wireframe == nullptr)
        return;
    auto* wfTr = Game::getInstance().GetComponent<Transform>(wireframe->entity);
    wfTr->SetWorldTranslation(GetWorldBS().Center);
    wfTr->SetWorldScale(float3 {2 * GetWorldBS().Radius});
}

void SphereCollider::Destroy()
{
        
}

void SphereCollider::Draw()
{
    
}
    
boundingSphere SphereCollider::GetBS() const
{
    return BS;
}

boundingSphere SphereCollider::GetWorldBS() const
{
    DirectX::BoundingSphere wbs; 
    BS.Transform(wbs, tr->GetWorldMatrix());
    return wbs;
}

void SphereCollider::SetBS(const boundingSphere& bs)
{
    BS = bs;
}

bool SphereCollider::Intersects(const SphereCollider* other) const
{
    return GetWorldBS().Intersects(other->GetWorldBS());
}