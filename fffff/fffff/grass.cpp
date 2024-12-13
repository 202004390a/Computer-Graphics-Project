#include "grass.h"
#include <stdlib.h>
#include <math.h>

// 풀의 위치와 색상을 저장할 배열 정의
Grass grassPositions[NUM_GRASS];

// 풀의 위치와 색상을 초기화하는 함수 구현
// 입력: floorSize - 바닥 크기
void InitGrass(GLfloat floorSize) {
    for (int i = 0; i < NUM_GRASS; i++) {
        // 랜덤하게 X, Z 좌표를 생성 (-floorSize ~ floorSize 범위)
        grassPositions[i].x = ((float)rand() / RAND_MAX) * 2.0f * floorSize - floorSize;
        grassPositions[i].z = ((float)rand() / RAND_MAX) * 2.0f * floorSize - floorSize;

        // 풀의 초록색 계열 색상 (0.4 ~ 0.6 범위)
        grassPositions[i].green = 0.4f + ((float)rand() / RAND_MAX) * 0.2f;

        // 풀의 회전 각도 (0 ~ 360도)
        grassPositions[i].angle = ((float)rand() / RAND_MAX) * 360.0f;
    }
}

// 풀을 그리는 함수 구현
void DrawGrass() {
    GLfloat grassHeight = 0.3f; // 풀의 삼각형 높이
    GLfloat grassSize = 0.2f;   // 풀의 삼각형 크기

    // 풀의 재질 설정 (짙은 초록색 계열)
    GLfloat mat_grass_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // 약한 반사광
    GLfloat mat_grass_ambient[] = { 0.0f, 0.5f, 0.0f, 1.0f };   // 환경광 (초록색)
    GLfloat mat_grass_shininess[] = { 10.0f };                  // 낮은 광택

    // OpenGL 재질 설정
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_grass_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_grass_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_grass_shininess);

    // 풀(삼각형) 그리기 시작
    for (int i = 0; i < NUM_GRASS; i++) {
        // 현재 풀의 색상을 설정 (초록색 계열)
        GLfloat mat_grass_diffuse[] = { 0.0f, grassPositions[i].green, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_grass_diffuse);

        GLfloat x = grassPositions[i].x;    // 풀의 X 좌표
        GLfloat z = grassPositions[i].z;    // 풀의 Z 좌표
        GLfloat angle = grassPositions[i].angle; // 풀의 회전 각도

        // OpenGL 변환 및 그리기
        glPushMatrix();
        glTranslatef(x, 0.0f, z);            // 풀의 위치로 이동
        glRotatef(angle, 0.0f, 1.0f, 0.0f); // Y축을 기준으로 회전

        glNormal3f(0.0f, 1.0f, 0.0f); // 삼각형의 노멀 벡터 설정 (Y축 방향)

        // 풀을 삼각형으로 렌더링
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 0.0f);                // 삼각형의 첫 번째 꼭짓점
        glVertex3f(grassSize, 0.0f, 0.0f);           // 삼각형의 두 번째 꼭짓점
        glVertex3f(grassSize / 2.0f, grassHeight, grassSize / 2.0f); // 삼각형의 세 번째 꼭짓점
        glEnd();

        glPopMatrix(); // 변환 복원
    }
}
