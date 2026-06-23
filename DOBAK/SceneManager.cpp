#include "SceneManager.h"
SceneManager* SceneManager::m_inst = nullptr;

void SceneManager::Update(GameState& state)
{
    if (m_curScene)
        m_curScene->Update(state);

    if (state.requestEndGame)
    {
        state.requestEndGame = false;
        ChangeScene("EndingScene", state);
        return;
    }
    if (state.requestQuotaCheck)
    {
        state.requestQuotaCheck = false;
        ChangeScene("QuotaScene", state);
        return;
    }
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
void SceneManager::ChangeScene(const std::string& _sceneName, GameState& state, bool _pushHistory)
{
    auto iter = m_mapScenes.find(_sceneName);
    if (iter == m_mapScenes.end())
        return;
    if (m_curScene)
    {
        m_curScene->Release();
        if (_pushHistory)
            m_sceneHistory.push_back(m_curScene);
    }
    system("cls");
    m_curScene = iter->second.get();
    m_curScene->Init(state);
}
void SceneManager::ChangeSceneToPrev(GameState& state)
{
    if (m_sceneHistory.empty())
        return;
    if (m_curScene)
        m_curScene->Release();
    system("cls");
    m_curScene = m_sceneHistory.back();
    m_sceneHistory.pop_back();
    m_curScene->Init(state);
}
void SceneManager::RegisterScene(const std::string& _sceneName, std::unique_ptr<AbstractScene> _scene)
{
	m_mapScenes[_sceneName] = std::move(_scene);
}