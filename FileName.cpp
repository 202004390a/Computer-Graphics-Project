#include <GL/glut.h> // GLUT 헤더 파일
#include <cmath>

int windowWidth = 800;
int windowHeight = 600;
bool wireframeMode = false; // 와이어프레임 모드 상태
float sphereY = 3.0f;       // 구의 y축 위치 (초기값)
float velocity = 0.0f;      // 구의 초기 속도
float gravity = -9.8f;      // 중력 가속도 (m/s^2)
float deltaTime = 0.016f;   // 프레임당 시간 (초)
bool isFalling = true;      // 구가 떨어지는 상태
float scaleX = 1.0f;        // x축 스케일
float scaleY = 1.0f;        // y축 스케일
float scaleZ = 1.0f;        // z축 스케일
bool isCompressing = false; // 구가 눌리는 상태
bool isBouncing = false;    // 구가 튕기는 상태
float compressionSpeed = 0.1f; // 압축 속도
float bounceFactor = 0.8f;  // 튕길 때 속도 감소 계수
bool isRestoring = false;

// 디스플레이 콜백 함수
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 카메라 위치 설정
    gluLookAt(0.0, 0.0, 10.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    // 와이어프레임 모드 설정
    if (wireframeMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // 타원 생성 (구)
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0.0f, sphereY, 0.0f);
    glScalef(scaleX, scaleY, scaleZ);
    glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    // 큐브 생성 (땅)
    glPushMatrix();
    glColor3f(0.3f, 0.7f, 0.3f);
    glTranslatef(0.0f, -1.0f, 0.0f);
    glScalef(5.0f, 0.5f, 5.0f);
    glutSolidCube(1.0);
    glPopMatrix();

    glutSwapBuffers();
}

// 애니메이션 업데이트 함수
void update(int value) {
    // 최소 속도 조건
    const float minBounceVelocity = 1.0f; // 최소 튕김 속도
    static float compressionFactor = 1.0f; // 압축 정도를 조절하는 변수

    if (isFalling) {
        // 중력 가속도 공식 적용
        velocity += gravity * deltaTime; // 속도 업데이트 (v = v0 + a * t)
        sphereY += velocity * deltaTime; // 위치 업데이트 (y = y0 + v * t)

        // 구가 땅에 닿으면
        if (sphereY <= -0.45f) {
            sphereY = -0.45f;
            velocity = -velocity * bounceFactor; // 속도 반전 및 감속

            // 튕김 속도가 최소 속도보다 작으면 복원 단계로 이동
            if (fabs(velocity) < minBounceVelocity) {
                velocity = 0.0f;
                isFalling = false;
                isCompressing = false;
                isBouncing = false;
                isRestoring = true; // 복원 단계 시작
            }
            else {
                isFalling = false;
                isCompressing = true;
                compressionFactor *= 0.9f; // 압축 정도 감소 (튕길수록 덜 눌림)
            }
        }
    }

    if (isCompressing) {
        // 구가 눌림 (y축이 줄어들고, x축과 z축이 퍼짐)
        scaleY -= compressionSpeed * compressionFactor;
        scaleX += compressionSpeed * compressionFactor;
        scaleZ += compressionSpeed * compressionFactor;

        // 충분히 눌렸으면 튕기기 시작
        if (scaleY <= 0.5f * (2-compressionFactor)) {
            isCompressing = false;
            isBouncing = true;
            isFalling = true;
        }
    }
    else if (isBouncing) {
        // 구가 튕겨 오름 (x축과 z축이 줄어들고, y축이 늘어남)
        scaleY += compressionSpeed;
        scaleX -= compressionSpeed;
        scaleZ -= compressionSpeed;

        // 구가 다시 원래 크기로 돌아옴
        if (scaleY >= 1.2f * compressionFactor) {
            scaleX = 1.0f;
            scaleY = 1.0f;
            scaleZ = 1.0f;
            isBouncing = false;
        }
    }
    else if (isRestoring) {
        // 구가 점진적으로 원래 크기로 복원됨
        float restoreSpeed = 0.05f; // 부드러운 복원 속도
        scaleX += (1.0f - scaleX) * restoreSpeed;
        scaleY += (1.0f - scaleY) * restoreSpeed;
        scaleZ += (1.0f - scaleZ) * restoreSpeed;

        // 원래 크기로 복원되면 애니메이션 종료
        if (fabs(scaleX - 1.0f) < 0.01f && fabs(scaleY - 1.0f) < 0.01f && fabs(scaleZ - 1.0f) < 0.01f) {
            scaleX = 1.0f;
            scaleY = 1.0f;
            scaleZ = 1.0f;
            isRestoring = false;
            isFalling = false;
            compressionFactor = 1.0f; // 압축 정도 초기화
        }
    }

    glutPostRedisplay();         // 화면 갱신 요청
    glutTimerFunc(16, update, 0); // 16ms 후에 다시 업데이트 (약 60FPS)
}





// 윈도우 리사이즈 콜백 함수
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// 키보드 입력 콜백 함수
void keyboard(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') {
        wireframeMode = true;
    }
    else if (key == 's' || key == 'S') {
        wireframeMode = false;
    }
    glutPostRedisplay();
}

// 초기화 함수
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

// 메인 함수
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Bouncing Ball with Horizontal Deformation");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0); // 애니메이션 업데이트 시작
    glutMainLoop();
    return 0;
}
