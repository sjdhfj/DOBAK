#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "Console.h"

class QuotaScene : public AbstractScene
{
public:
    void Init(GameState& state) override;
    void Update(GameState& state) override;
    void Render(const GameState& state) override;
    void Release() override {}

private:
    void PlayOpenTransition(const GameState& state, unsigned long delayMs);
    void PlayCloseTransition(const GameState& state, unsigned long delayMs);
    void PlaySubmitAnimation(const GameState& state);
    void DrawPanel(const GameState& state, int px, int py) const;
};