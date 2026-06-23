#pragma once
#include <vector>
#include <utility>
#include <Windows.h>
#include "Console.h"
class FireworkSystem
{
public:
    void Launch(int x, int apexY);
    void Update();
    void Erase();
    void Draw();
    void Clear();

    size_t GetParticleCount() const { return m_particles.size(); }

private:
    struct Particle
    {
        float x = 0, y = 0;
        float vx = 0, vy = 0;
        float life = 0;      
        float maxLife = 1;   
        Color color = Color::WHITE;
        bool isRocket = false; 
        int apexY = 0;         
    };

    std::vector<Particle> m_particles;
    std::vector<std::pair<int, int>> m_prevDrawPositions;
    ULONGLONG m_lastUpdateTime = 0;

    void Explode(float x, float y);
    char GetParticleChar(const Particle& p) const;
};