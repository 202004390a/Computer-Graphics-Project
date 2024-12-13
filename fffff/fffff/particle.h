#ifndef PARTICLE_H
#define PARTICLE_H

#define MAX_PARTICLES 1000

typedef struct {
    float x, y, z;       // 위치
    float vx, vy, vz;    // 속도
    float ax, ay, az;    // 가속도
    float life;          // 남은 수명
    int active;          // 활성 상태
} Particle;

void InitParticles();
void GenerateNewParticles();
void CreateParticleExplosion(float x, float y, float z);
void UpdateParticles(float deltaTime);
void RenderParticles();

#endif // PARTICLE_H
