#include "input.h"
#include "bullet.h"
#include "gun.h"
#include <GL/glut.h>
#include <math.h>
#include "target.h"

// 외부 변수 정의
float cameraX = 0.0f, cameraZ = 5.0f; // 카메라 X, Z 위치 (Y는 고정)
float cameraY = 1.75f;                // 카메라 Y 위치 (플레이어 높이 기준)
float yaw = 90.0f, pitch = 30.0f;     // 카메라의 회전 각도 (yaw: 좌우, pitch: 상하)
float sensitivity = 0.1f;             // 마우스 감도
float moveSpeed = 0.2f;               // 카메라 이동 속도

int windowWidth = 800, windowHeight = 600; // 창 크기
bool isWarping = false;                    // 마우스 포인터 워핑 중인지 여부
bool firstMouse = true;                    // 첫 마우스 움직임 여부
bool pressH = false;                       // 'H' 키가 눌렸는지 여부

bool keys[256] = { false }; // 키 상태 배열


// 마우스를 창 중앙으로 이동시키는 함수
void CenterMouse() {
    glutWarpPointer(windowWidth / 2, windowHeight / 2);
}

// 키가 눌렸을 때 호출되는 함수
// 입력: key - 눌린 키, x, y - 마우스 위치
void MyKeyboardDown(unsigned char key, int x, int y) {
    keys[key] = true; // 눌린 키를 활성화

    // 'H' 키가 눌렸을 경우 pressH 플래그 활성화
    if (key == 'h' || key == 'H') {
        pressH = true;
    }
}

// 키가 떼어졌을 때 호출되는 함수
// 입력: key - 떼어진 키, x, y - 마우스 위치
void MyKeyboardUp(unsigned char key, int x, int y) {
    keys[key] = false; // 떼어진 키를 비활성화

    // 'H' 키가 떼어졌을 경우 pressH 플래그 비활성화
    if (key == 'h' || key == 'H') {
        pressH = false;
    }

    // 프로그램 종료 시 마우스 커서 복원
    if (key == 'q' || key == 'Q' || key == 27) { // 'q', 'Q', ESC
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }
}

// 마우스 움직임 처리 함수
// 입력: x, y - 마우스 위치
void MyPassiveMouseMove(int x, int y) {
    if (isWarping) {
        // 워핑으로 인한 호출은 무시
        isWarping = false;
        return;
    }

    // 화면 중앙 좌표
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;

    // 중앙과의 차이를 계산
    int dx = x - centerX;
    int dy = centerY - y; // Y 방향은 반전

    // 첫 마우스 움직임일 경우 델타 무시
    if (firstMouse) {
        firstMouse = false;
        dx = 0;
        dy = 0;
    }

    // yaw와 pitch 업데이트
    yaw += (-dx) * sensitivity; // 좌우 회전
    pitch += dy * sensitivity;  // 상하 회전

    // pitch 제한 (90도 초과 방지)
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // 마우스 포인터를 중앙으로 되돌림
    isWarping = true;
    CenterMouse();

    // 화면 다시 그리기 요청
    glutPostRedisplay();
}

// 카메라 이동 업데이트 함수
void UpdateCamera() {
    // 현재 바라보는 방향 벡터 계산
    float radYaw = yaw * M_PI / 180.0f;
    float forwardX = cosf(radYaw);
    float forwardZ = -sinf(radYaw);

    // 오른쪽 방향 벡터 계산
    float rightX = sinf(radYaw);
    float rightZ = cosf(radYaw);

    // W/S/A/D 키를 이용한 이동 처리
    if (keys['w'] || keys['W']) { // 앞으로 이동
        cameraX += forwardX * moveSpeed;
        cameraZ += forwardZ * moveSpeed;
    }
    if (keys['s'] || keys['S']) { // 뒤로 이동
        cameraX -= forwardX * moveSpeed;
        cameraZ -= forwardZ * moveSpeed;
    }
    if (keys['a'] || keys['A']) { // 왼쪽 이동
        cameraX -= rightX * moveSpeed;
        cameraZ -= rightZ * moveSpeed;
    }
    if (keys['d'] || keys['D']) { // 오른쪽 이동
        cameraX += rightX * moveSpeed;
        cameraZ += rightZ * moveSpeed;
    }

    // X-Z 평면 상의 이동 범위를 제한
    if (cameraX > 10.0f) cameraX = 10.0f;
    if (cameraX < -10.0f) cameraX = -10.0f;
    if (cameraZ > 10.0f) cameraZ = 10.0f;
    if (cameraZ < -10.0f) cameraZ = -10.0f;

    // 벽 충돌 처리
    float wallXMin = -5.0f;  // 벽의 X 최소값
    float wallXMax = 5.0f;   // 벽의 X 최대값
    float wallZ = -3.0f;     // 벽의 Z 위치
    float wallThickness = 0.5f; // 벽 두께

    if (fabs(cameraZ - wallZ) < wallThickness) { // Z 축 근처에서
        if (cameraX > wallXMin && cameraX < wallXMax) {
            if (keys['w'] || keys['W']) cameraZ = wallZ + wallThickness; // 벽 앞쪽
            if (keys['s'] || keys['S']) cameraZ = wallZ - wallThickness; // 벽 뒤쪽
        }
    }

    // 화면 다시 그리기 요청
    glutPostRedisplay();
}

// 타이머 콜백 함수
void Timer(int value) {
    UpdateCamera();   // 카메라 업데이트
    UpdateBullets();  // 총알 업데이트
    glutPostRedisplay(); // 화면 갱신 요청
    glutTimerFunc(16, Timer, 0); // 약 60FPS로 타이머 설정
}

// 입력 초기화 함수
void InitInput() {
    CenterMouse(); // 마우스를 중앙으로 설정
    glutPostRedisplay();
}

// 마우스 클릭 처리 함수
// 입력: button - 클릭된 버튼, state - 버튼 상태, x, y - 클릭 위치
void MyMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // 총구 끝 위치 계산
        float gunTipX, gunTipY, gunTipZ;
        GetGunTipPosition(cameraX, cameraY, cameraZ, yaw, pitch, &gunTipX, &gunTipY, &gunTipZ);

        // 총알의 발사 방향 계산
        float dirX = cosf(yaw * M_PI / 180.0f) * cosf(pitch * M_PI / 180.0f);
        float dirY = sinf(pitch * M_PI / 180.0f);
        float dirZ = -sinf(yaw * M_PI / 180.0f) * cosf(pitch * M_PI / 180.0f);

        // 총알 발사
        FireBullet(gunTipX, gunTipY, gunTipZ, dirX, dirY, dirZ);
    }
}
