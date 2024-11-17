#include <GL/glut.h> // GLUT ��� ����
#include <cmath>

int windowWidth = 800;
int windowHeight = 600;
bool wireframeMode = false; // ���̾������� ��� ����
float sphereY = 3.0f;       // ���� y�� ��ġ (�ʱⰪ)
float velocity = 0.0f;      // ���� �ʱ� �ӵ�
float gravity = -9.8f;      // �߷� ���ӵ� (m/s^2)
float deltaTime = 0.016f;   // �����Ӵ� �ð� (��)
bool isFalling = true;      // ���� �������� ����
float scaleX = 1.0f;        // x�� ������
float scaleY = 1.0f;        // y�� ������
float scaleZ = 1.0f;        // z�� ������
bool isCompressing = false; // ���� ������ ����
bool isBouncing = false;    // ���� ƨ��� ����
float compressionSpeed = 0.1f; // ���� �ӵ�
float bounceFactor = 0.8f;  // ƨ�� �� �ӵ� ���� ���
bool isRestoring = false;

// ���÷��� �ݹ� �Լ�
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // ī�޶� ��ġ ����
    gluLookAt(0.0, 0.0, 10.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    // ���̾������� ��� ����
    if (wireframeMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Ÿ�� ���� (��)
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0.0f, sphereY, 0.0f);
    glScalef(scaleX, scaleY, scaleZ);
    glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    // ť�� ���� (��)
    glPushMatrix();
    glColor3f(0.3f, 0.7f, 0.3f);
    glTranslatef(0.0f, -1.0f, 0.0f);
    glScalef(5.0f, 0.5f, 5.0f);
    glutSolidCube(1.0);
    glPopMatrix();

    glutSwapBuffers();
}

// �ִϸ��̼� ������Ʈ �Լ�
void update(int value) {
    // �ּ� �ӵ� ����
    const float minBounceVelocity = 1.0f; // �ּ� ƨ�� �ӵ�
    static float compressionFactor = 1.0f; // ���� ������ �����ϴ� ����

    if (isFalling) {
        // �߷� ���ӵ� ���� ����
        velocity += gravity * deltaTime; // �ӵ� ������Ʈ (v = v0 + a * t)
        sphereY += velocity * deltaTime; // ��ġ ������Ʈ (y = y0 + v * t)

        // ���� ���� ������
        if (sphereY <= -0.45f) {
            sphereY = -0.45f;
            velocity = -velocity * bounceFactor; // �ӵ� ���� �� ����

            // ƨ�� �ӵ��� �ּ� �ӵ����� ������ ���� �ܰ�� �̵�
            if (fabs(velocity) < minBounceVelocity) {
                velocity = 0.0f;
                isFalling = false;
                isCompressing = false;
                isBouncing = false;
                isRestoring = true; // ���� �ܰ� ����
            }
            else {
                isFalling = false;
                isCompressing = true;
                compressionFactor *= 0.9f; // ���� ���� ���� (ƨ����� �� ����)
            }
        }
    }

    if (isCompressing) {
        // ���� ���� (y���� �پ���, x��� z���� ����)
        scaleY -= compressionSpeed * compressionFactor;
        scaleX += compressionSpeed * compressionFactor;
        scaleZ += compressionSpeed * compressionFactor;

        // ����� �������� ƨ��� ����
        if (scaleY <= 0.5f * (2-compressionFactor)) {
            isCompressing = false;
            isBouncing = true;
            isFalling = true;
        }
    }
    else if (isBouncing) {
        // ���� ƨ�� ���� (x��� z���� �پ���, y���� �þ)
        scaleY += compressionSpeed;
        scaleX -= compressionSpeed;
        scaleZ -= compressionSpeed;

        // ���� �ٽ� ���� ũ��� ���ƿ�
        if (scaleY >= 1.2f * compressionFactor) {
            scaleX = 1.0f;
            scaleY = 1.0f;
            scaleZ = 1.0f;
            isBouncing = false;
        }
    }
    else if (isRestoring) {
        // ���� ���������� ���� ũ��� ������
        float restoreSpeed = 0.05f; // �ε巯�� ���� �ӵ�
        scaleX += (1.0f - scaleX) * restoreSpeed;
        scaleY += (1.0f - scaleY) * restoreSpeed;
        scaleZ += (1.0f - scaleZ) * restoreSpeed;

        // ���� ũ��� �����Ǹ� �ִϸ��̼� ����
        if (fabs(scaleX - 1.0f) < 0.01f && fabs(scaleY - 1.0f) < 0.01f && fabs(scaleZ - 1.0f) < 0.01f) {
            scaleX = 1.0f;
            scaleY = 1.0f;
            scaleZ = 1.0f;
            isRestoring = false;
            isFalling = false;
            compressionFactor = 1.0f; // ���� ���� �ʱ�ȭ
        }
    }

    glutPostRedisplay();         // ȭ�� ���� ��û
    glutTimerFunc(16, update, 0); // 16ms �Ŀ� �ٽ� ������Ʈ (�� 60FPS)
}





// ������ �������� �ݹ� �Լ�
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Ű���� �Է� �ݹ� �Լ�
void keyboard(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') {
        wireframeMode = true;
    }
    else if (key == 's' || key == 'S') {
        wireframeMode = false;
    }
    glutPostRedisplay();
}

// �ʱ�ȭ �Լ�
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

// ���� �Լ�
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Bouncing Ball with Horizontal Deformation");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0); // �ִϸ��̼� ������Ʈ ����
    glutMainLoop();
    return 0;
}
