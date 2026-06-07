#pragma once
#include<map>
#include<memory>
#include<string>
#include "Defines.h"
#include "Scene.h"
class SceneManager
{
private:
	SceneManager() = default;
public:
	// Lazy Initialization
	static SceneManager* GetInst()
	{
		if (nullptr == m_inst)
			m_inst = new SceneManager();
		return m_inst;
	}
	static void DestroyInst()
	{
		if(m_inst)
		{
			if (m_inst->m_curScene)
				m_inst->m_curScene->Release();

			for (auto& pair : m_inst->m_mapScenes)
				pair.second->Release();
		}
		SAFE_DELETE(m_inst);
	}
public:
	void Update(GameState& state);
	void Render(const GameState& state);
	void ChangeScene(const std::string& _sceneName, GameState& state);
	void RegisterScene(const std::string& _sceneName, std::unique_ptr<AbstractScene> _scene);
private:
	static SceneManager* m_inst;
	std::map<std::string, std::unique_ptr<AbstractScene>> m_mapScenes;
	AbstractScene* m_curScene = nullptr;
};