#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "input.h"
#include "floor.h"
#include "grass.h"
#include "gun.h"
#include "bullet.h"
#include "target.h"
#include "particle.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 바닥 크기와 태양 설정
GLfloat floorSize = 10.0f;                         // 바닥 크기
GLfloat sunPosition[] = { 0.0f, 100.0f, 0.0f, 1.0f }; // 태양 위치
GLfloat sunRadius = 5.0f;                          // 태양 크기

// 태양을 그리면서 광원을 설정하는 함수
void DrawSun() {
    // 태양의 재질 속성
    GLfloat mat_sun_emission[] = { 0.5f, 0.5f, 0.4f, 1.0f }; // 발광 색상
    GLfloat mat_sun_diffuse[] = { 1.0f, 1.0f, 0.8f, 1.0f };
    GLfloat mat_sun_specular[] = { 1.0f, 1.0f, 0.8f, 1.0f };
    GLfloat mat_sun_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    // 태양 재질 설정
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_sun_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_sun_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_sun_ambient);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_sun_emission);

    // 태양을 광원으로 사용
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, sunPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_sun_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, mat_sun_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, mat_sun_ambient);

    // 태양 구체 렌더링
    glPushMatrix();
    glTranslatef(sunPosition[0], sunPosition[1], sunPosition[2]);
    glutSolidSphere(sunRadius, 50, 50); // 태양 크기와 분할
    glPopMatrix();

    // 발광 속성 초기화
    GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
}

// 벽을 렌더링하는 함수
void RenderWall() {
    // 벽의 재질 속성
    GLfloat wallDiffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat wallSpecular[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat wallShininess[] = { 50.0f };

    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wallDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wallSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, wallShininess);

    // 벽 위치와 크기 설정
    glTranslatef(0.0f, 2.0f, -3.0f); // 벽 위치
    glScalef(10.0f, 4.0f, 0.5f);     // 벽 크기
    glutSolidCube(1.0f);             // 벽 생성
    glPopMatrix();
}

// 메인 디스플레이 함수
// 씬을 렌더링하고 상태에 따라 텍스트를 표시
void MyDisplay() {
    // 프레임 간 시간 계산
    static float previousTime = 0.0f;
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - previousTime;
    previousTime = currentTime;

    // 화면 초기화
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 카메라 설정
    float radYaw = yaw * M_PI / 180.0f;
    float radPitch = pitch * M_PI / 180.0f;
    float lookAtX = cameraX + cosf(radYaw) * cosf(radPitch);
    float lookAtY = cameraY + sinf(radPitch);
    float lookAtZ = cameraZ - sinf(radYaw) * cosf(radPitch);
    gluLookAt(cameraX, cameraY, cameraZ, lookAtX, lookAtY, lookAtZ, 0.0, 1.0, 0.0);

    // 씬 렌더링
    DrawFloor(floorSize);            // 바닥 렌더링
    DrawGrass();                     // 풀 렌더링
    DrawSun();                       // 태양 렌더링
    RenderWall();                    // 벽 렌더링
    DrawGun(cameraX, cameraY, cameraZ, yaw, pitch); // 총 렌더링
    RenderBullets();                 // 총알 렌더링
    UpdateTarget();                  // 과녁 상태 업데이트
    DrawTarget();                    // 과녁 렌더링
    UpdateParticles(deltaTime);      // 파티클 상태 업데이트
    RenderParticles();               // 파티클 렌더링

    // 종료 메시지 렌더링
    if (showExitMessage) {
        RenderText(300.0f, 300.0f, "Clear!!!"); // "Clear" 메시지
    }

    // 도움말 텍스트 렌더링
    if (pressH) {
        RenderText(300.0f, 300.0f, "WASD to move\nMouse Left click to shout\nYou have to shout target three times to clear");
    }

    // 화면 버퍼 교환
    glutSwapBuffers();
}

// 윈도우 크기 변경 처리
void MyReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0); // 원근 투영
}

// 메인 함수
int main(int argc, char* argv[]) {
    // 난수 생성기 초기화
    srand((unsigned int)time(NULL));

    // 풀 초기화
    InitGrass(floorSize);

    // GLUT 초기화
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // RGB 모드, 더블 버퍼링, 깊이 버퍼 활성화
    glutInitWindowSize(800, 600); // 윈도우 크기 설정
    glutCreateWindow("OpenGL FPS Camera with Wall");

    // 깊이 테스트 활성화
    glEnable(GL_DEPTH_TEST);

    // 조명 설정
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); // 기본 광원 활성화

    // 배경 색상 설정
    glClearColor(0.4, 0.4, 0.4, 1.0); // 회색 배경

    // 마우스 커서를 숨김
    glutSetCursor(GLUT_CURSOR_NONE);

    // 입력 초기화
    InitInput();

    // 총알 초기화
    InitBullets();

    // 이벤트 핸들러 등록
    glutDisplayFunc(MyDisplay);
    glutKeyboardFunc(MyKeyboardDown);
    glutKeyboardUpFunc(MyKeyboardUp);
    glutPassiveMotionFunc(MyPassiveMouseMove);
    glutMouseFunc(MyMouseClick); // 마우스 클릭 이벤트 핸들러
    glutReshapeFunc(MyReshape);
    glutTimerFunc(0, Timer, 0); // 타이머 설정

    // 마우스를 창 중앙으로 이동
    CenterMouse();

    // 메인 루프 실행
    glutMainLoop();

    return 0;
}
