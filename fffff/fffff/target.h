#ifndef TARGET_H
#define TARGET_H

extern bool g_TargetHit;         // 맞았는지 여부
extern float g_TargetRotationZ;  // Z축 회전 각도
extern bool g_TargetRotating;    // 회전 중인지 여부

extern bool showExitMessage;

void RenderTargetBase();
void RenderText(float x, float y, const char* text);
void MoveBase();
void DrawTarget();
void UpdateTargetPosition();
bool CheckBulletCollision(float bulletX, float bulletY, float bulletZ);
void UpdateTarget(); // 과녁 상태 업데이트 함수

#endif