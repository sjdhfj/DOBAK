#include "SceneManager.h"
SceneManager* SceneManager::m_inst = nullptr;

void SceneManager::Update(GameState& state)
{
	if (m_curScene)
		m_curScene->Update(state);
    if (state.requestNextDay)
    {
        state.requestNextDay = false;
        ChangeScene("NextDayScene", state);
    }
}
void SceneManager::Render(const GameState& state)
{
	if(m_curScene)
		m_curScene->Render(state);
}
void SceneManager::ChangeScene(const std::string& _sceneName, GameState& state)
{
    auto iter = m_mapScenes.find(_sceneName);
    if (iter == m_mapScenes.end())
        return;
    if (m_curScene)
        m_curScene->Release();
    system("cls");
    m_curScene = iter->second.get();
    m_curScene->Init(state);
}

void SceneManager::RegisterScene(const std::string& _sceneName, std::unique_ptr<AbstractScene> _scene)
{
	m_mapScenes[_sceneName] = std::move(_scene);
}