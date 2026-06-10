#pragma once
#include "Scene.h"
#include "SceneManager.h"
class InfoScene : public AbstractScene
{
public:
	void Init(GameState& state) override;
	void Update(GameState& state) override;
	void Render(const GameState& state) override;
};