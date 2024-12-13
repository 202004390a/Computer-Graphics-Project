#include <GL/glut.h>
#include <stdlib.h>
#include "particle.h"

// 파티클 배열 정의
Particle particles[MAX_PARTICLES];

// 파티클 시스템 초기화 함수
void InitParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].active = 0; // 모든 파티클을 비활성 상태로 초기화
    }
}

// 새로운 파티클 생성 함수
// 입력: x, y, z - 생성 위치, count - 생성할 파티클 개수
void GenerateNewParticles(float x, float y, float z, int count) {
    for (int i = 0; i < MAX_PARTICLES && count > 0; i++) {
        if (!particles[i].active) { // 비활성 상태의 파티클만 초기화
            particles[i].x = x;      // 파티클의 생성 위치 설정
            particles[i].y = y;
            particles[i].z = z;

            // 속도를 랜덤하게 설정 (-1.5 ~ 1.5 범위)
            particles[i].vx = ((float)rand() / RAND_MAX - 0.5f) * 3.0f;
            particles[i].vy = ((float)rand() / RAND_MAX - 0.5f) * 3.0f;
            particles[i].vz = ((float)rand() / RAND_MAX - 0.5f) * 3.0f;

            // 가속도 설정 (중력 효과 적용)
            particles[i].ax = 0.0f;   // X축 가속도
            particles[i].ay = -9.8f;  // Y축 방향 중력
            particles[i].az = 0.0f;   // Z축 가속도

            particles[i].life = 0.3f; // 파티클의 초기 수명 (초 단위)
            particles[i].active = 1;  // 활성화
            count--;                  // 남은 생성 개수 감소
        }
    }
}

// 파티클 업데이트 함수
// 입력: deltaTime - 이전 프레임과의 시간 간격 (초 단위)
void UpdateParticles(float deltaTime) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].active) { // 활성화된 파티클만 업데이트
            // 가속도에 의한 속도 변화
            particles[i].vx += particles[i].ax * deltaTime;
            particles[i].vy += particles[i].ay * deltaTime;
            particles[i].vz += particles[i].az * deltaTime;

            // 속도에 의한 위치 변화
            particles[i].x += particles[i].vx * deltaTime;
            particles[i].y += particles[i].vy * deltaTime;
            particles[i].z += particles[i].vz * deltaTime;

            // 파티클 수명 감소
            particles[i].life -= deltaTime;
            if (particles[i].life <= 0.0f) {
                particles[i].active = 0; // 수명이 다하면 비활성화
            }
        }
    }
}

// 파티클 렌더링 함수
void RenderParticles() {
    glPushMatrix();
    glDisable(GL_LIGHTING); // 파티클은 자체 발광이므로 조명 비활성화
    glEnable(GL_BLEND);     // 블렌딩 활성화 (반투명 효과)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 알파 블렌딩 설정
    glPointSize(5.0f);      // 파티클의 크기 설정

    glBegin(GL_POINTS);
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].active) { // 활성화된 파티클만 렌더링
            // 파티클의 색상 설정 (수명에 따라 투명도 조정)
            glColor4f(1.0f, 0.0f, 0.0f, particles[i].life); // 빨간색
            glVertex3f(particles[i].x, particles[i].y, particles[i].z); // 파티클 위치
        }
    }
    glEnd();

    glDisable(GL_BLEND);    // 블렌딩 비활성화
    glEnable(GL_LIGHTING);  // 조명 다시 활성화
    glPopMatrix();
}

// 폭발 효과를 생성하는 함수
// 입력: x, y, z - 폭발 중심 위치
void CreateParticleExplosion(float x, float y, float z) {
    GenerateNewParticles(x, y, z, 200); // 폭발 위치에서 200개의 파티클 생성
}
