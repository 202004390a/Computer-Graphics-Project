#ifndef BULLET_H
#define BULLET_H

#include <GL/glut.h>

// 구체 구조체 정의
typedef struct {
    float x, y, z;         // 구체의 현재 위치
    float dx, dy, dz;      // 구체의 이동 방향
    int active;            // 구체가 활성 상태인지 (1: 활성, 0: 비활성)
} Bullet;

#define MAX_BULLETS 10 // 최대 구체 수

// 구체 배열 선언
extern Bullet bullets[MAX_BULLETS];

// 구체 초기화 함수
void InitBullets();

// 구체 발사 함수
void FireBullet(float startX, float startY, float startZ, float directionX, float directionY, float directionZ);

// 구체 업데이트 함수
void UpdateBullets();

// 구체 렌더링 함수
void RenderBullets();

#endif // BULLET_H
