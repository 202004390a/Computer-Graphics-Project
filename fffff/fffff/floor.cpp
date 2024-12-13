#include "floor.h"

// 바닥 평면 그리기 함수 구현
// 입력: floorSize - 바닥의 크기 (X-Z 평면 상에서의 길이)
// 출력: 화면에 렌더링된 바닥 평면
void DrawFloor(GLfloat floorSize) {
    GLfloat floorY = 0.0f; // 바닥 평면의 Y 위치 (수평으로 렌더링)

    // 바닥의 재질 설정 (갈색 계열)
    GLfloat mat_floor_diffuse[] = { 0.545f, 0.271f, 0.075f, 1.0f }; // 확산 반사 (갈색)
    GLfloat mat_floor_specular[] = { 0.2f, 0.2f, 0.2f, 1.0f };       // 반사광 (약한 반사 효과)
    GLfloat mat_floor_ambient[] = { 0.545f, 0.271f, 0.075f, 1.0f };  // 환경광 (갈색)
    GLfloat mat_floor_shininess[] = { 10.0f };                       // 광택 (낮은 수준)

    // OpenGL 재질 속성 설정
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_floor_diffuse);   // 확산 반사 설정
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_floor_specular); // 반사광 설정
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_floor_ambient);   // 환경광 설정
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_floor_shininess); // 광택 설정

    // 바닥 평면 그리기 시작
    glBegin(GL_QUADS); // 사각형(QUADS)으로 바닥 평면을 생성

    glNormal3f(0.0f, 1.0f, 0.0f); // 바닥의 노멀 벡터 설정 (Y축 방향)

    // 사각형의 네 꼭짓점을 시계 방향으로 지정
    glVertex3f(-floorSize, floorY, -floorSize); // 왼쪽 뒤쪽 꼭짓점
    glVertex3f(-floorSize, floorY, floorSize);  // 왼쪽 앞쪽 꼭짓점
    glVertex3f(floorSize, floorY, floorSize);   // 오른쪽 앞쪽 꼭짓점
    glVertex3f(floorSize, floorY, -floorSize);  // 오른쪽 뒤쪽 꼭짓점

    glEnd(); // 바닥 평면 생성 종료
}
