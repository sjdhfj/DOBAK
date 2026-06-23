#pragma once
#include "Scene.h"
#include "Console.h"
#include "SceneManager.h"
#include "EncounterData.h"

class EncounterScene : public AbstractScene
{
public:
    void Init(GameState& state) override;
    void Update(GameState& state) override;
    void Render(const GameState& state) override;
    void Release() override {}

private:
    void DrawTextBox();
    void DrawBigAscii();
    void DrawShopLike();
    void DrawChoices();
    void DrawResult();
    string ApplyChoice(const EncounterChoice& choice, GameState& state);
};