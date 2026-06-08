#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "UIAsciiObjs.h"
#include "Console.h"
class TitleScene : public AbstractScene
{
    void Init(GameState& state) override;
    void Update(GameState& state) override;
    void Render(const GameState& state) override;
private:
    void PlayTransition();
    void FlashAnimation(COORD resolution, int count, int delayMs);
    void CrossAnimation(COORD resolution, int delayMs);
    void Circle(float prevAngle, float targetAngle, const string& fillChar);
private:
    UIAsciiObjs _objs;
    Menu _curMenu = Menu::START;
};
