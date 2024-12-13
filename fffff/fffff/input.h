#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

// M_PI가 정의되어 있지 않은 경우를 대비하여 수동 정의
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 외부 변수 선언 (input.c에 정의됨)
extern float cameraX, cameraY, cameraZ;  // 카메라 위치
extern float yaw, pitch;                // 카메라 회전 각도
extern float sensitivity;               // 마우스 감도
extern float moveSpeed;                 // 카메라 이동 속도
extern bool keys[256];                  // 키 상태 배열
extern int windowWidth, windowHeight;   // 창 크기
extern bool isWarping;                  // 마우스 워핑 상태
extern bool firstMouse;                 // 첫 마우스 움직임 여부

extern bool pressH;                     // 'h' 키 상태 플래그

// 입력 초기화
void InitInput();

// 키보드 이벤트 핸들러
void MyKeyboardDown(unsigned char key, int x, int y);
void MyKeyboardUp(unsigned char key, int x, int y);

// 마우스 이벤트 핸들러
void MyPassiveMouseMove(int x, int y);

// 타이머 함수
void Timer(int value);

// 카메라 이동 업데이트
void UpdateCamera();

// 마우스를 중앙으로 이동시키는 함수
void CenterMouse();

// 마우스 클릭 이벤트 핸들러
void MyMouseClick(int button, int state, int x, int y);

#endif // INPUT_H
