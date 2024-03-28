#include "Game.h"

Game* Game::s_instance = nullptr;

Game::Game()
{
    Game::s_instance = this;
}

Game& Game::getInstance()
{
    return *s_instance;
}
	
void Game::Run()
{
    Application::GetWindow().Show();

    while (!Application::GetWindow().ShouldClose())
    {
        time.UpdateTime();

        Application::GetWindow().Update();

        // Init phase
        while (!adding.empty())
        {
            adding.front()->Init();
            adding.pop();
        }

        renderPipeline.Prepare();

        // Update phase
        for (auto&& component : components)
            component->Update();

        // Destroy phase
        while (!destroying.empty())
        {
            auto* component = destroying.front();
            component->Destroy();

            for (auto it = components.begin(); it < components.end(); ++it)
                if (it->get() == component)
                {
                    components.erase(it);
                    break;
                }
            destroying.pop();
        }

        renderPipeline.Draw();
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

RenderPipeline& Game::GetRenderPipeline()
{
    return renderPipeline;
}
