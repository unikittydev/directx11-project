#include "Game.h"

Game* Game::s_instance = nullptr;

Game::Game()
{
    Game::s_instance = this;
}

void Game::PreDrawFrame()
{
    RECT winRect;
    GetClientRect(app.GetWindow().GetHWND(), &winRect);

    //device.GetContext()->ClearState();

    D3D11_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<FLOAT>(Application::WindowWidth()), static_cast<FLOAT>(Application::WindowHeight()), 0.0f, 1.0f };

    Application::GetDeviceContext()->RSSetViewports(1, &viewport);

    const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    Application::GetDeviceContext()->OMSetRenderTargets(1, &Application::GetSwapchain().GetRenderTargetView(), Application::GetSwapchain().GetDepthStencilView());
    Application::GetDeviceContext()->ClearRenderTargetView(Application::GetSwapchain().GetRenderTargetView(), clearColor);
    Application::GetDeviceContext()->ClearDepthStencilView(Application::GetSwapchain().GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Game::DrawFrame()
{
    Application::GetSwapchainPtr()->Present(1, 0);
}

Game& Game::getInstance()
{
    return *s_instance;
}
	
void Game::Run()
{
    Application::GetWindow().Show();

    for (auto&& component : components)
        component->Init();

    while (!Application::GetWindow().ShouldClose())
    {
        time.UpdateTime();

        Application::GetWindow().Update();

        PreDrawFrame();

        // Init phase
        while (!adding.empty())
        {
            adding.front()->Init();
            adding.pop();
        }

        // Update phase
        for (auto&& component : components)
            component->Update();

        for (auto&& component : components)
            component->Draw();

        // Destroy phase
        while (!destroying.empty())
        {
            auto* component = destroying.front();
            component->Destroy();

            for (auto it = components.begin(); it < components.end(); it++)
                if ((*it).get() == component)
                {
                    components.erase(it);
                    break;
                }
            destroying.pop();
        }

        DrawFrame();
        time.Finish();
    }

    for (auto&& component : components)
        component->Destroy();
}

Entity Game::CreateEntity()
{
    const Entity e { ++lastId };
    entityComponentMap.insert(std::make_pair(e, nullptr));
    return e;
}