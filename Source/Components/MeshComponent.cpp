#include "MeshComponent.h"

#include "Transform.h"
#include "Camera/Camera.h"
#include "Games/Game.h"
#include "Graphics/Mesh.h"

MeshComponent::MeshComponent(Entity e) : IComponent(e)
{
        
}

void MeshComponent::SetCamera(Camera* camera)
{
    this->camera = camera;
}
    
void MeshComponent::SetMesh(Mesh* mesh)
{
    this->mesh = mesh;
}

void MeshComponent::Init()
{
    tr = Game::getInstance().GetComponent<Transform>(entity);
}
    
void MeshComponent::Draw()
{
    Application::GetDeviceContext()->PSSetShaderResources(0, 1, mainTexture.GetSRV());
    Application::GetDeviceContext()->PSSetSamplers(0, 1, mainTexture.GetSampler());
    
    mesh->Draw(camera->GetViewProjectionMatrix(), tr->GetWorldMatrix());
}

void MeshComponent::Update()
{
        
}

void MeshComponent::Destroy()
{
        
}

const Texture2D& MeshComponent::GetMainTexture() const
{
    return mainTexture;
}

void MeshComponent::SetMainTexture(const Texture2D& texture)
{
    mainTexture = texture;
}