#pragma once
#include "Scene.h"
#include "FireWork.h"
#include "GameOverEffect.h"
class EndingScene : public AbstractScene
{
	// AbstractScene을(를) 통해 상속됨
public:
	void Init(GameState& state) override;
	void Update(GameState& state) override;
	void Render(const GameState& state) override;
private:
	void WinUIUpdate(GameState& state);
	void LoseUIUpdate(GameState& state);
	void WinUIRender(const GameState& state);
	void LoseUIRender(const GameState& state);

private:
	bool m_showHappy = false;
	ULONGLONG m_lastSwapTime = 0;
	FireworkSystem m_fireworks;
	ULONGLONG m_nextFireworkTime = 0;
	GameOverEffect m_gameOverEffect;
	int  m_loseBurstCount = 0;          // 지금까지 터진 폭탄 수
	ULONGLONG m_nextBurstTime = 0;      // 다음 폭탄이 터질 시각
	bool m_loseSequenceFinished = false;
};

