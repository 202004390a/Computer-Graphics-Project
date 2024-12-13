// grass.h
#ifndef GRASS_H
#define GRASS_H

#include <GL/glut.h>

// 풀의 개수 정의
#define NUM_GRASS 10000

// 풀의 위치와 색상, 각도를 저장할 구조체
typedef struct {
    GLfloat x;
    GLfloat z;
    GLfloat green; // 풀의 초록색 값
    GLfloat angle; // 풀의 회전 각도 (도 단위)
} Grass;

// 풀의 위치와 색상을 저장할 배열 선언
extern Grass grassPositions[NUM_GRASS];

// 풀의 위치와 색상을 초기화하는 함수 선언
void InitGrass(GLfloat floorSize);

// 풀을 그리는 함수 선언
void DrawGrass();

#endif // GRASS_H
