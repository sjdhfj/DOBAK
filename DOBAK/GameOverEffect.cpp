#include "GameOverEffect.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>
#undef min

namespace
{
	constexpr float GRAVITY = 20.0f;  
	constexpr int   DEBRIS_COUNT = 70;
	constexpr float DEBRIS_SPEED_MIN = 8.0f;
	constexpr float DEBRIS_SPEED_MAX = 24.0f;
	constexpr float DEBRIS_LIFE_MIN = 0.6f;
	constexpr float DEBRIS_LIFE_MAX = 1.6f;
	constexpr float MAX_DT = 0.1f;
	constexpr float TWO_PI = 6.2831853f;

	const Color PALETTE[] =
	{
		Color::LIGHT_RED, Color::RED, Color::GRAY, Color::LIGHT_GRAY, Color::WHITE
	};

	float RandRange(float lo, float hi)
	{
		return lo + (hi - lo) * (rand() / (float)RAND_MAX);
	}
	Color RandColor()
	{
		return PALETTE[rand() % (sizeof(PALETTE) / sizeof(PALETTE[0]))];
	}
}

void GameOverEffect::Trigger(int centerX, int centerY)
{
	m_particles.clear();
	m_particles.reserve(DEBRIS_COUNT);
	m_triggered = true;
	m_lastUpdateTime = 0;

	for (int i = 0; i < DEBRIS_COUNT; ++i)
	{
		float angle = RandRange(0.0f, TWO_PI);
		float speed = RandRange(DEBRIS_SPEED_MIN, DEBRIS_SPEED_MAX);

		ExplosionParticle p;
		p.x = (float)centerX;
		p.y = (float)centerY;
		p.vx = std::cos(angle) * speed;
		p.vy = std::sin(angle) * speed * 0.5f;
		p.maxLife = RandRange(DEBRIS_LIFE_MIN, DEBRIS_LIFE_MAX);
		p.life = p.maxLife;
		p.color = RandColor();
		m_particles.push_back(p);
	}

	ShakeConsoleWindow(6, 250, 25);
}

void GameOverEffect::Update()
{
	if (!m_triggered)
		return;

	ULONGLONG curTime = GetTickCount64();
	if (m_lastUpdateTime == 0)
		m_lastUpdateTime = curTime;
	float dt = (curTime - m_lastUpdateTime) / 1000.0f;
	m_lastUpdateTime = curTime;
	dt = std::min(dt, MAX_DT);

	for (auto& p : m_particles)
	{
		p.vy += GRAVITY * dt;
		p.x += p.vx * dt;
		p.y += p.vy * dt;
		p.life -= dt;
	}

	m_particles.erase(
		std::remove_if(m_particles.begin(), m_particles.end(),
			[](const ExplosionParticle& p) { return p.life <= 0.0f; }),
		m_particles.end());
}

char GameOverEffect::GetParticleChar(const ExplosionParticle& p) const
{
	float ratio = p.life / p.maxLife;
	if (ratio > 0.66f) return '#';
	if (ratio > 0.33f) return '*';
	return '.';
}

void GameOverEffect::Erase()
{
	if (m_prevDrawPositions.empty())
		return;

	for (auto& pos : m_prevDrawPositions)
	{
		GotoXY(pos.first, pos.second);
		cout << ' ';
	}
	m_prevDrawPositions.clear();
}

void GameOverEffect::Draw()
{
	COORD res = GetConsoleResolution();
	m_prevDrawPositions.reserve(m_particles.size());

	for (const auto& p : m_particles)
	{
		int sx = (int)(p.x + 0.5f);
		int sy = (int)(p.y + 0.5f);
		if (sx < 0 || sx >= res.X || sy < 0 || sy >= res.Y)
			continue;

		SetColor(p.color);
		GotoXY(sx, sy);
		cout << GetParticleChar(p);
		m_prevDrawPositions.emplace_back(sx, sy);
	}
	SetColor();
}

void GameOverEffect::Clear()
{
	Erase();
	m_particles.clear();
	m_triggered = false;
}

bool GameOverEffect::IsFinished() const
{
	return m_triggered && m_particles.empty();
}