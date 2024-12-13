#include "gun.h"
#include <math.h>

// 총을 렌더링하는 함수
// 입력: cameraX, cameraY, cameraZ - 카메라의 위치
//       yaw, pitch - 카메라의 회전 각도 (yaw: 좌우, pitch: 상하)
void DrawGun(float cameraX, float cameraY, float cameraZ, float yaw, float pitch) {
    // 플레이어의 중심 위치 계산
    float playerCenterX = cameraX;                    // 플레이어 중심의 X 좌표
    float playerCenterY = cameraY * 3.0f / 4.0f;      // 플레이어 중심의 Y 좌표 (카메라보다 낮음)
    float playerCenterZ = cameraZ;                    // 플레이어 중심의 Z 좌표

    // yaw를 기준으로 총의 위치 계산 (플레이어 중심에서 일정 거리)
    float gunDistance = 2.0f;                         // 중심으로부터 총의 거리
    float radYaw = yaw * M_PI / 180.0f;               // yaw를 라디안으로 변환

    // 총의 초기 위치 설정
    float gunX = playerCenterX + cosf(radYaw) * gunDistance;
    float gunY = playerCenterY;
    float gunZ = playerCenterZ - sinf(radYaw) * gunDistance;

    // 총의 재질 설정 (어두운 회색 계열)
    GLfloat mat_gun_diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };   // 확산 반사
    GLfloat mat_gun_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // 반사광 없음
    GLfloat mat_gun_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };   // 환경광 (어두운 회색)
    GLfloat mat_gun_shininess[] = { 32.0f };                  // 광택 정도

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_gun_diffuse);   // 확산 반사 설정
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_gun_specular); // 반사광 설정
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_gun_ambient);   // 환경광 설정
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_gun_shininess); // 광택 설정

    // 총의 변환 적용
    glPushMatrix();
    glTranslatef(playerCenterX, playerCenterY, playerCenterZ); // 플레이어 중심으로 이동
    glRotatef(yaw, 0.0f, 1.0f, 0.0f);                         // yaw 기준 Y축 회전
    glRotatef(pitch, 0.0f, 0.0f, 1.0f);                       // pitch 기준 Z축 회전
    glTranslatef(gunDistance, 0.0f, 0.0f);                    // X축 방향으로 총 이동

    // 1. 손잡이 그리기
    glPushMatrix();
    glTranslatef(-0.5f, -0.3f, 0.2f); // 총 손잡이를 몸체 아래로 이동
    glScalef(0.25f, 0.4f, 0.25f);     // 손잡이 크기 조정
    glutSolidCube(1.0f);              // 손잡이를 큐브로 생성
    glPopMatrix();

    // 2. 몸체 그리기
    glPushMatrix();
    glTranslatef(-0.1f, 0.0f, 0.2f); // 총 몸체의 위치 조정
    glScalef(1.4f, 0.3f, 0.3f);      // 몸체 크기 조정 (길이, 높이, 너비)
    glutSolidCube(1.0f);             // 몸체를 큐브로 생성
    glPopMatrix();

    // 3. 총구 그리기
    glPushMatrix();
    glTranslatef(0.6f, 0.0f, 0.2f);    // 몸체 끝으로 이동
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Y축 기준으로 회전
    GLUquadric* quad = gluNewQuadric(); // 실린더 생성
    gluCylinder(quad, 0.08f, 0.08f, 0.15f, 12, 3); // 총구 실린더
    gluDeleteQuadric(quad); // 메모리 해제
    glPopMatrix();

    glPopMatrix(); // 변환 복원
}

// 총구 끝 위치를 계산하는 함수
// 입력: 카메라 위치 (cameraX, cameraY, cameraZ), 회전 각도 (yaw, pitch)
// 출력: 총구 끝 위치 (tipX, tipY, tipZ)
void GetGunTipPosition(float cameraX, float cameraY, float cameraZ, float yaw, float pitch, float* tipX, float* tipY, float* tipZ) {
    // 플레이어 중심 위치 계산
    float playerCenterX = cameraX;                   // 플레이어 중심 X 좌표
    float playerCenterY = cameraY * 3.0f / 4.0f;     // 플레이어 중심 Y 좌표
    float playerCenterZ = cameraZ;                   // 플레이어 중심 Z 좌표

    // yaw와 pitch를 라디안으로 변환
    float radYaw = yaw * M_PI / 180.0f;
    float radPitch = pitch * M_PI / 180.0f;

    // 총구 끝까지의 거리
    float gunTipDistance = 1.2f;

    // 총구 끝 위치 계산
    float baseTipX = playerCenterX + cosf(radYaw - 0.15f) * cosf(radPitch) * gunTipDistance;
    float baseTipY = playerCenterY + sinf(radPitch + 0.15f) * gunTipDistance;
    float baseTipZ = playerCenterZ - sinf(radYaw) * cosf(radPitch) * gunTipDistance;

    // 변위 적용
    *tipX = baseTipX; // X축 위치
    *tipY = baseTipY; // Y축 위치
    *tipZ = baseTipZ; // Z축 위치
}
