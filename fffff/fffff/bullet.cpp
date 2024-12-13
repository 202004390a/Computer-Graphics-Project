#include "bullet.h"
#include "target.h"
#include "particle.h"
#include <math.h>
#include <stdio.h>

// 최대 총알 개수 배열
Bullet bullets[MAX_BULLETS];

// 벽의 위치와 크기 정의
#define WALL_X_MIN -5.0f        // 벽의 X 최소값
#define WALL_X_MAX 5.0f         // 벽의 X 최대값
#define WALL_Z -3.0f            // 벽의 Z 위치
#define WALL_THICKNESS 0.5f     // 벽 두께
#define WALL_HEIGHT 4.0f        // 벽 높이

// 총알 초기화 함수
// 모든 총알을 비활성화 상태로 설정
void InitBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0; // 비활성화
    }
}

// 총알 발사 함수
// 사용 가능한 총알을 찾아 활성화하고 방향을 설정
void FireBullet(float startX, float startY, float startZ, float directionX, float directionY, float directionZ) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) { // 비활성화된 총알만 사용
            // 총알 시작 위치 설정
            bullets[i].x = startX;
            bullets[i].y = startY;
            bullets[i].z = startZ;

            // 방향 벡터를 단위 벡터로 정규화
            float magnitude = sqrtf(directionX * directionX + directionY * directionY + directionZ * directionZ);
            bullets[i].dx = directionX / magnitude;
            bullets[i].dy = directionY / magnitude;
            bullets[i].dz = directionZ / magnitude;

            // 총알 활성화
            bullets[i].active = 1;
            break;
        }
    }
}

// 벽 충돌 판정 함수
// 벽의 위치와 크기를 기반으로 충돌 여부를 반환
bool CheckWallCollision(float bulletX, float bulletY, float bulletZ) {
    // 벽의 Z 축 충돌 여부 확인
    if (fabs(bulletZ - WALL_Z) <= WALL_THICKNESS / 2.0f) {
        // 벽의 X 및 Y 범위 내에 있는지 확인
        if (bulletX >= WALL_X_MIN && bulletX <= WALL_X_MAX &&
            bulletY >= 0.0f && bulletY <= WALL_HEIGHT) {
            return true; // 충돌
        }
    }
    return false; // 충돌하지 않음
}

// 총알 업데이트 함수
// 총알의 위치를 업데이트하고 충돌을 처리
void UpdateBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) { // 활성화된 총알만 업데이트
            // 총알 이동 (속도 0.5f)
            bullets[i].x += bullets[i].dx * 0.5f;
            bullets[i].y += bullets[i].dy * 0.5f;
            bullets[i].z += bullets[i].dz * 0.5f;

            // 총알이 바닥 아래로 내려가면 비활성화
            if (bullets[i].y < 0.0f) {
                bullets[i].active = 0;
                continue;
            }

            // 총알이 화면 밖으로 나가면 비활성화
            if (fabs(bullets[i].x) > 50.0f || fabs(bullets[i].y) > 50.0f || fabs(bullets[i].z) > 50.0f) {
                bullets[i].active = 0;
                continue;
            }

            // 과녁 충돌 체크
            if (CheckBulletCollision(bullets[i].x, bullets[i].y, bullets[i].z)) {
                // 충돌 시 총알 비활성화 및 파티클 생성
                CreateParticleExplosion(bullets[i].x, bullets[i].y, bullets[i].z);
                bullets[i].active = 0;
                continue;
            }

            // 벽 충돌 체크
            if (CheckWallCollision(bullets[i].x, bullets[i].y, bullets[i].z)) {
                // 충돌 시 총알 비활성화 및 파티클 생성
                CreateParticleExplosion(bullets[i].x, bullets[i].y, bullets[i].z);
                bullets[i].active = 0;
                continue;
            }
        }
    }
}

// 총알 렌더링 함수
// 활성화된 총알을 화면에 구 형태로 그린다
void RenderBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) { // 활성화된 총알만 렌더링
            glPushMatrix();
            glTranslatef(bullets[i].x, bullets[i].y, bullets[i].z); // 총알 위치로 이동
            glutSolidSphere(0.1f, 16, 16); // 반지름 0.1의 구체로 렌더링
            glPopMatrix();
        }
    }
}
