#include <GL/glut.h>
#include <math.h>
#include "target.h"
#include "particle.h"
#include <stdio.h>
#include <stdlib.h> // rand()와 srand()를 위해
#include <time.h>   // time()을 위해

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


extern float cameraX, cameraY, cameraZ; // 카메라 좌표

bool g_TargetHit = false;
bool g_TargetRotating = false;
float g_TargetRotationZ = 0.0f;
int g_WaitCount = -1;

static GLfloat targetX = 0.0f;
static GLfloat targetY = 0.0f;
static GLfloat targetZ = -20.0f;
static GLfloat pillarHeight = 3.0f;
static GLfloat pillarWidth = 0.5f;
static GLfloat pillarDepth = 0.5f;
static GLfloat sphereRadius = 0.75f;

static bool g_TargetInitialized = false;
static float g_TargetInitialAngle = 0.0f;

// 이동 관련 변수
static GLfloat currentTargetX = 0.0f;      // 현재 X 위치
static GLfloat newTargetX = 0.0f;          // 새로운 타겟 X 위치
static float movementDuration = 0.0f;      // 이동 지속 시간 (초)
static float movementElapsedTime = 0.0f;   // 이동 시작 후 경과 시간 (초)
static int lastUpdateTime = 0;             // 마지막 업데이트 시간 (밀리초)

bool canCount = true;

float cx = targetX; // 초기 X 위치
float cy = targetY + (pillarHeight + sphereRadius - 0.2f); // 초기 Y 위치
float cz = targetZ; // 초기 Z 위치

float newcx = cx;
float newcy = cy;
float newcz = cz; // 초기 Z 위치;

// min과 max 사이의 무작위 float 값을 생성
float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

// 바닥 렌더링 함수
void RenderTargetBase() {
    GLfloat baseDiffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f }; // 바닥 색상
    GLfloat baseSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat baseShininess[] = { 10.0f };
    GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, baseDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, baseSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, baseShininess);

    glTranslatef(targetX, targetY - 0.01f, targetZ); // 업데이트된 targetX 사용
    glScalef(4.0f, 0.1f, 4.0f);
    glutSolidCube(1.0f);

    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission); // 기본값 복구
    glPopMatrix();
}

// 초기화 함수
void InitializeTarget() {
    if (!g_TargetInitialized) {
        // 난수 생성기 시드 설정
        srand(static_cast<unsigned int>(time(NULL)));

        // 카메라 위치를 기반으로 초기 각도 계산
        float dx = cameraX - targetX;
        float dz = cameraZ - targetZ;
        g_TargetInitialAngle = atan2(dz, dx) * 180.0f / M_PI;

        // 이동 변수 초기화
        currentTargetX = targetX;
        newTargetX = targetX;
        movementDuration = 0.0f;
        movementElapsedTime = 0.0f;
        lastUpdateTime = glutGet(GLUT_ELAPSED_TIME); // 마지막 업데이트 시간 초기화

        g_TargetInitialized = true;
    }
}

// 타겟 그리기 함수
void DrawTarget() {
    InitializeTarget();

    glPushMatrix();
    glTranslatef(targetX, targetY, targetZ); // 업데이트된 targetX 사용
    glRotatef(g_TargetInitialAngle, 0.0f, 1.0f, 0.0f);
    glRotatef(g_TargetRotationZ, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, pillarHeight / 2.0f, 0.0f);

    // 기둥의 재질 속성
    GLfloat mat_pillar_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat mat_pillar_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat mat_pillar_shininess[] = { 1.0f };
    GLfloat mat_pillar_emission[] = { 0.1f, 0.1f, 0.1f, 1.0f };

    // 구체(과녁)의 재질 속성
    GLfloat mat_sphere_diffuse[4];
    GLfloat mat_sphere_emission[4];

    if (g_TargetHit) {
        mat_sphere_diffuse[0] = 0.0f;
        mat_sphere_diffuse[1] = 0.0f;
        mat_sphere_diffuse[2] = 0.0f;
        mat_sphere_diffuse[3] = 1.0f;

        mat_sphere_emission[0] = 0.05f;
        mat_sphere_emission[1] = 0.05f;
        mat_sphere_emission[2] = 0.05f;
        mat_sphere_emission[3] = 1.0f;
    }
    else {
        mat_sphere_diffuse[0] = 0.0f;
        mat_sphere_diffuse[1] = 0.0f;
        mat_sphere_diffuse[2] = 1.0f;
        mat_sphere_diffuse[3] = 1.0f;

        mat_sphere_emission[0] = 0.1f;
        mat_sphere_emission[1] = 0.1f;
        mat_sphere_emission[2] = 0.3f;
        mat_sphere_emission[3] = 1.0f;
    }

    GLfloat mat_sphere_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_sphere_shininess[] = { 50.0f };

    // 기둥 그리기
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_pillar_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_pillar_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_pillar_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_pillar_emission);
    glPushMatrix();
    glScalef(pillarWidth, pillarHeight, pillarDepth);
    glutSolidCube(1.0f);
    glPopMatrix();
    GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    glPopMatrix();

    // 구체(과녁) 그리기
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_sphere_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_sphere_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_sphere_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_sphere_emission);
    glTranslatef(0.0f, pillarHeight / 2 + sphereRadius - 0.2f, 0.0f);
    glutSolidSphere(sphereRadius, 30, 30);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    glPopMatrix();

    glPopMatrix();

    // 바닥 렌더링
    RenderTargetBase();
}

// 타겟의 중심 위치 업데이트 함수 (회전 관련)
void UpdateTargetPosition() {
    float angleRad = g_TargetRotationZ * M_PI / 180.0f;
    float displacement = (pillarHeight + sphereRadius - 0.2f);

    // 새로운 중심 위치 계산
    newcx = targetX;                             // X축 변위 없음
    newcy = displacement * cosf(angleRad);      // Y 변위
    newcz = targetZ - displacement * sinf(angleRad); // Z 변위

    // 디버깅 출력 (선택 사항)
    // printf("New Center Position: (%f, %f, %f)\n", newcx, newcy, newcz);
}

// 총알과 타겟의 충돌 확인 함수
// 전역 변수 추가
int hitCount = 0;         // 타겟이 맞은 횟수
int exitTimerStart = -1;  // 종료 타이머 시작 시간 (밀리초)
bool showExitMessage = false; // 종료 메시지 표시 여부

// 텍스트 출력 함수
void RenderText(float x, float y, const char* text) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); // 2D 화면 좌표 설정 (윈도우 크기와 일치)
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST); // 3D 깊이 버퍼 비활성화
    glColor3f(1.0f, 1.0f, 1.0f); // 흰색 텍스트

    // 텍스트 출력
    float currentY = y;
    glRasterPos2f(x, currentY); // 텍스트 시작 위치
    while (*text) {
        if (*text == '\n') {
            // 개행 문자 처리: Y 좌표를 줄이고 새로운 줄 시작
            currentY -= 20; // 줄 간격
            glRasterPos2f(x, currentY); // 새로운 줄 위치 설정
        }
        else {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        }
        text++;
    }

    glEnable(GL_DEPTH_TEST); // 깊이 버퍼 복원
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// 총알과 타겟의 충돌 확인 함수 수정
bool CheckBulletCollision(float bulletX, float bulletY, float bulletZ) {
    float dx = bulletX - newcx;
    float dy = bulletY - newcy;
    float dz = bulletZ - newcz;
    float dist = sqrtf(dx * dx + dy * dy + dz * dz);

    if (dist <= sphereRadius) {
        g_TargetHit = true;
        g_TargetRotating = true;
        g_WaitCount = 120; // 대기 시간 설정
        CreateParticleExplosion(bulletX, bulletY, bulletZ);

        // 타겟을 맞췄을 때 카운트 증가
        if (canCount) { hitCount++; canCount = false; }

        // 맞힌 횟수가 3이면 종료 타이머 시작 및 메시지 활성화
        if (hitCount >= 3 && exitTimerStart == -1) {
            showExitMessage = true; // 메시지 표시 활성화
            exitTimerStart = glutGet(GLUT_ELAPSED_TIME); // 현재 시간 저장
        }

        return true;
    }
    return false;
}


// 타겟의 상태 업데이트 함수
void UpdateTarget() {
    UpdateTargetPosition(); // 회전 관련 위치 업데이트

    if (exitTimerStart != -1) {
        int currentTime = glutGet(GLUT_ELAPSED_TIME);
        if ((currentTime - exitTimerStart) >= 3000) { // 5초 경과 확인
            printf("Exiting program after 3 seconds.\n");
            exit(0); // 프로그램 종료
        }
    }

    // **이동 로직 시작**

    // 현재 시간 (밀리초) 가져오기
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    // deltaTime을 초 단위로 계산
    float deltaTime = (currentTime - lastUpdateTime) / 1000.0f;
    lastUpdateTime = currentTime;

    if (movementDuration > 0.0f) {
        // 경과 시간 업데이트
        movementElapsedTime += deltaTime;

        // 보간 인자 계산 (0.0에서 1.0 사이로 제한)
        float t = movementElapsedTime / movementDuration;
        if (t > 1.0f) t = 1.0f;

        // targetX를 현재 위치에서 새로운 타겟 위치로 선형 보간
        targetX = currentTargetX + t * (newTargetX - currentTargetX);

        // 이동이 완료되었는지 확인
        if (t >= 1.0f) {
            // 현재 위치를 새로운 타겟 위치로 설정
            currentTargetX = newTargetX;

            // -10.0f에서 10.0f 사이의 새로운 타겟 위치 선택
            newTargetX = randomFloat(-10.0f, 10.0f);

            // 이동 지속 시간을 1.0f에서 2.0f 사이로 무작위 설정
            movementDuration = randomFloat(1.0f, 2.0f);

            // 경과 시간 리셋
            movementElapsedTime = 0.0f;
        }
    }
    else {
        // 현재 이동 중이 아니면 첫 이동 초기화
        currentTargetX = targetX;
        newTargetX = randomFloat(-10.0f, 10.0f);
        movementDuration = randomFloat(1.0f, 2.0f); // 이동 지속 시간을 1~2초로 설정
        movementElapsedTime = 0.0f;
    }

    // **이동 로직 종료**

    // 기존 회전 및 히트 처리 로직
    if (g_TargetRotating) {
        if (g_TargetHit) {
            if (g_TargetRotationZ > -90.0f) {
                g_TargetRotationZ -= 1.0f;
                if (g_TargetRotationZ <= -90.0f) {
                    g_TargetRotationZ = -90.0f;
                    g_TargetRotating = false;
                    g_WaitCount = 120;
                }
            }
        }
        else {
            if (g_TargetRotationZ < 0.0f) {
                g_TargetRotationZ += 1.0f;
                if (g_TargetRotationZ >= 0.0f) {
                    g_TargetRotationZ = 0.0f;
                    g_TargetRotating = false;
                    canCount = true;
                }
            }
        }
    }
    else {
        if (g_WaitCount > 0) {
            g_WaitCount--;
            if (g_WaitCount == 0) {
                g_TargetHit = false;
                g_TargetRotating = true;
            }
        }
    }
}