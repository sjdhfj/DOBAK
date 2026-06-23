#include "Firework.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>
#undef min
namespace
{
	constexpr float GRAVITY = 14.0f;         // 파편을 아래로 끌어당기는 가속도
	constexpr float ROCKET_SPEED = 20.0f;    // 로켓 상승 속도
	constexpr int   DEBRIS_COUNT = 26;       // 한 번 터질 때 생기는 파편 개수
	constexpr float DEBRIS_SPEED_MIN = 6.0f;
	constexpr float DEBRIS_SPEED_MAX = 13.0f;
	constexpr float DEBRIS_LIFE_MIN = 0.8f;
	constexpr float DEBRIS_LIFE_MAX = 1.4f;
	constexpr float MAX_DT = 0.1f;
	constexpr float TWO_PI = 6.2831853f;

	const Color PALETTE[] =
	{
		Color::LIGHT_RED, Color::LIGHT_YELLOW, Color::LIGHT_GREEN,
		Color::CYAN, Color::LIGHT_VIOLET, Color::LIGHT_BLUE
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

void FireworkSystem::Launch(int x, int apexY)
{
	COORD res = GetConsoleResolution();

	Particle rocket;
	rocket.x = (float)x;
	rocket.y = (float)(res.Y - 1);
	rocket.vx = 0.0f;
	rocket.vy = -ROCKET_SPEED;
	rocket.isRocket = true;
	rocket.apexY = apexY;
	rocket.color = Color::WHITE;
	rocket.life = 10.0f;
	rocket.maxLife = 10.0f;

	m_particles.push_back(rocket);
}

void FireworkSystem::Explode(float x, float y)
{
	Color baseColor = RandColor();
	for (int i = 0; i < DEBRIS_COUNT; ++i)
	{
		float angle = RandRange(0.0f, TWO_PI);
		float speed = RandRange(DEBRIS_SPEED_MIN, DEBRIS_SPEED_MAX);

		Particle p;
		p.x = x;
		p.y = y;
		p.vx = std::cos(angle) * speed;
		p.vy = std::sin(angle) * speed * 0.6f; 
		p.isRocket = false;
		p.maxLife = RandRange(DEBRIS_LIFE_MIN, DEBRIS_LIFE_MAX);
		p.life = p.maxLife;
		p.color = (rand() % 4 == 0) ? Color::WHITE : baseColor;

		m_particles.push_back(p);
	}
}

void FireworkSystem::Update()
{
	ULONGLONG curTime = GetTickCount64();
	if (m_lastUpdateTime == 0)
		m_lastUpdateTime = curTime;

	float dt = (curTime - m_lastUpdateTime) / 1000.0f;
	m_lastUpdateTime = curTime;
	dt = std::min(dt, MAX_DT);

	std::vector<Particle> exploded;

	for (auto& p : m_particles)
	{
		if (p.isRocket)
		{
			p.y += p.vy * dt;
			if (p.y <= (float)p.apexY)
			{
				exploded.push_back(p);
				p.life = -1.0f;
			}
		}
		else
		{
			p.vy += GRAVITY * dt;
			p.x += p.vx * dt;
			p.y += p.vy * dt;
			p.life -= dt;
		}
	}

	for (auto& r : exploded)
		Explode(r.x, r.y);

	m_particles.erase(
		std::remove_if(m_particles.begin(), m_particles.end(),
			[](const Particle& p) { return p.life <= 0.0f; }),
		m_particles.end());
}

char FireworkSystem::GetParticleChar(const Particle& p) const
{
	if (p.isRocket)
		return '|';

	float ratio = p.life / p.maxLife;
	if (ratio > 0.66f) return '*';
	if (ratio > 0.33f) return '+';
	return '.';
}

void FireworkSystem::Erase()
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

void FireworkSystem::Draw()
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

void FireworkSystem::Clear()
{
	Erase();
	m_particles.clear();
}