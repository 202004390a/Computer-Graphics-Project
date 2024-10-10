#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

void DrawTable();
void DrawCup();
void DrawTeapot();

int FlatShaded = 0;	int Wireframed = 0;

int ViewX = 0, ViewY = 0;

void InitLight() {
	GLfloat mat_diffuse[] = { 1.0, 1.0, 0.3, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_shininess[] = { 15.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_position[] = { -3, 6, 3.0, 0.0 };
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void MyMouseMove(GLint X, GLint Y) {
	glutPostRedisplay();
}

void MyKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q': case 'Q': case '\033':
		exit(0);
		break;
	case 's':
		if (FlatShaded) {
			FlatShaded = 0;
			glShadeModel(GL_SMOOTH);
		}
		else {
			FlatShaded = 1;
			glShadeModel(GL_FLAT);
		}
		glutPostRedisplay();
		break;
	}
}

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 카메라 위치 조정
	gluLookAt(1.0, 2.0, 2.0,  // 카메라 위치
		0.0, 0.0, 0.0,  // 바라보는 지점
		0.0, 1.0, 0.0); // 위쪽 방향 (Y축)

	DrawTeapot();
	DrawTable();
	DrawCup();

	glFlush();
}


void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 투영 설정 조정
	if (w <= h)
		glOrtho(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-1.0 * (GLfloat)w / (GLfloat)h, 1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0, -10.0, 10.0);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Sample Drawing");

	glClearColor(0.4, 0.4, 0.4, 0.0);
	InitLight();
	glutDisplayFunc(MyDisplay);
	glutKeyboardFunc(MyKeyboard);
	glutMotionFunc(MyMouseMove);
	glutReshapeFunc(MyReshape);
	glutMainLoop();
}

void DrawTable() {
	// 갈색 재질 설정
	GLfloat table_diffuse[] = { 0.55f, 0.27f, 0.07f, 1.0f };  // 갈색 (RGB)
	GLfloat table_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };    // 약간의 반사광
	GLfloat table_shininess[] = { 30.0f };                     // 재질의 광택



	// 테이블 상판
	glPushMatrix();
	glTranslatef(0.0f, -0.2f, 0.0f); // 상판 위치
	glScalef(0.6f, 0.05f, 0.6f);     // 상판 크기 조정
	glutSolidCube(1.0f);             // 큐브로 상판 그리기
	glPopMatrix();

	// 테이블 다리 (네 개의 다리)
	float legHeight = 0.4f;
	float legWidth = 0.05f;

	// 첫 번째 다리
	glPushMatrix();
	glTranslatef(0.25f, -(0.2f + legHeight / 2), 0.25f); // 다리 위치
	glScalef(legWidth, legHeight, legWidth);             // 다리 크기 조정
	glutSolidCube(1.0f);                                 // 큐브로 다리 그리기
	glPopMatrix();

	// 두 번째 다리
	glPushMatrix();
	glTranslatef(-0.25f, -(0.2f + legHeight / 2), 0.25f);
	glScalef(legWidth, legHeight, legWidth);
	glutSolidCube(1.0f);
	glPopMatrix();

	// 세 번째 다리
	glPushMatrix();
	glTranslatef(0.25f, -(0.2f + legHeight / 2), -0.25f);
	glScalef(legWidth, legHeight, legWidth);
	glutSolidCube(1.0f);
	glPopMatrix();

	// 네 번째 다리
	glPushMatrix();
	glTranslatef(-0.25f, -(0.2f + legHeight / 2), -0.25f);
	glScalef(legWidth, legHeight, legWidth);
	glutSolidCube(1.0f);
	glPopMatrix();
}

void DrawCup() {
	// 컵 색상 설정 (흰색)
	GLfloat cup_diffuse[] = { 0.0f, 0.9f, 0.9f, 1.0f };  // 흰색
	GLfloat cup_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat cup_shininess[] = { 50.0f };

	// 재질 적용
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cup_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, cup_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, cup_shininess);

	// GLUquadric 생성
	GLUquadric* quadric = gluNewQuadric();

	// 컵 몸체 (원통)
	glPushMatrix();
	glTranslatef(0.25f, -0.1f, 0.25f);  // 컵의 위치를 테이블 모서리로 이동
	glRotatef(-90, 1.0f, 0.0f, 0.0f);   // X축을 기준으로 90도 회전 (컵을 눕힘)
	gluCylinder(quadric, 0.05f, 0.05f, 0.12f, 30, 30);  // 반지름 0.1, 높이 0.15의 원통
	glPopMatrix();

	// 컵 바닥 (디스크)
	glPushMatrix();
	glTranslatef(0.25f, -0.35f, 0.25f);  // 컵 바닥을 원통 아래로 이동
	glRotatef(-90, 1.0f, 0.0f, 0.0f);    // 컵 바닥도 동일하게 회전
	gluDisk(quadric, 0.05f, 0.05f, 30, 30);  // 반지름 0.1의 원형 디스크
	glPopMatrix();

	// GLUquadric 해제
	gluDeleteQuadric(quadric);
}

void DrawTeapot() {

	GLfloat mat_diffuse[] = { 0.5, 0.5, 0.3, 0.7 };
	GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_ambient);

	glPushMatrix();
	glTranslatef(0.0f, -0.1f, 0.0f); // 찻주전자를 탁자 위로 이동
	glutSolidTeapot(0.2);
	glPopMatrix();
}