#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "Enums.h"
class InfoScene : public AbstractScene
{
public:
	void Init(GameState& state) override;
	void Update(GameState& state) override;
	void Render(const GameState& state) override;
private:
	void RenderSoundTab(int cx, int cy);
	void RenderInfoTab(int cx, int cy);
private:
	InfoTab _curTab = InfoTab::INFO;
	int _cursor = 0;
};