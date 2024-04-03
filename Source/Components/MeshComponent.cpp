#include "MeshComponent.h"

#include "Transform.h"
#include "Camera/Camera.h"
#include "Games/Game.h"
#include "Graphics/Mesh.h"

MeshComponent::MeshComponent(Entity e) : IComponent(e)
{
        
}
    
void MeshComponent::SetMesh(Mesh* mesh)
{
    this->mesh = mesh;
}

void MeshComponent::Init()
{
    tr = Game::getInstance().GetComponent<Transform>(entity);
    Game::getInstance().GetRenderPipeline().AddMesh(mesh);
}
    
void MeshComponent::Draw()
{

}

void MeshComponent::Update()
{
    mesh->UpdateMeshData(tr->GetWorldMatrix(), tr->GetWorldTranslation());
}

void MeshComponent::Destroy()
{
        
}

Mesh* MeshComponent::GetMesh() const
{
    return mesh;
}