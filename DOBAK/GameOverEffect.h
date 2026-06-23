#pragma once
#include "Console.h"
#include <vector>
#include <utility>

struct ExplosionParticle
{
	float x = 0.0f;
	float y = 0.0f;
	float vx = 0.0f;
	float vy = 0.0f;
	float life = 0.0f;
	float maxLife = 0.0f;
	Color color = Color::LIGHT_RED;
};

class GameOverEffect
{
public:
	void Trigger(int centerX, int centerY);

	void Update();
	void Draw();
	void Erase();
	void Clear();

	bool IsTriggered() const { return m_triggered; }
	bool IsFinished() const;

private:
	char GetParticleChar(const ExplosionParticle& p) const;

	std::vector<ExplosionParticle> m_particles;
	std::vector<std::pair<int, int>> m_prevDrawPositions;
	ULONGLONG m_lastUpdateTime = 0;
	bool m_triggered = false;
};