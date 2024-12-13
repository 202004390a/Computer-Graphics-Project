#ifndef GUN_H
#define GUN_H

#include <GL/glut.h>

// M_PI가 정의되어 있지 않은 경우를 대비하여 수동 정의
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 총을 그리는 함수 선언
void DrawGun(float cameraX, float cameraY, float cameraZ, float yaw, float pitch);

// 총구 끝 위치 계산 함수 선언
void GetGunTipPosition(float cameraX, float cameraY, float cameraZ, float yaw, float pitch, float* tipX, float* tipY, float* tipZ);

#endif // GUN_H
