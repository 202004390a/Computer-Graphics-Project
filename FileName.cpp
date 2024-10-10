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

	// ī�޶� ��ġ ����
	gluLookAt(1.0, 2.0, 2.0,  // ī�޶� ��ġ
		0.0, 0.0, 0.0,  // �ٶ󺸴� ����
		0.0, 1.0, 0.0); // ���� ���� (Y��)

	DrawTeapot();
	DrawTable();
	DrawCup();

	glFlush();
}


void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// ���� ���� ����
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
	// ���� ���� ����
	GLfloat table_diffuse[] = { 0.55f, 0.27f, 0.07f, 1.0f };  // ���� (RGB)
	GLfloat table_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };    // �ణ�� �ݻ籤
	GLfloat table_shininess[] = { 30.0f };                     // ������ ����



	// ���̺� ����
	glPushMatrix();
	glTranslatef(0.0f, -0.2f, 0.0f); // ���� ��ġ
	glScalef(0.6f, 0.05f, 0.6f);     // ���� ũ�� ����
	glutSolidCube(1.0f);             // ť��� ���� �׸���
	glPopMatrix();

	// ���̺� �ٸ� (�� ���� �ٸ�)
	float legHeight = 0.4f;
	float legWidth = 0.05f;

	// ù ��° �ٸ�
	glPushMatrix();
	glTranslatef(0.25f, -(0.2f + legHeight / 2), 0.25f); // �ٸ� ��ġ
	glScalef(legWidth, legHeight, legWidth);             // �ٸ� ũ�� ����
	glutSolidCube(1.0f);                                 // ť��� �ٸ� �׸���
	glPopMatrix();

	// �� ��° �ٸ�
	glPushMatrix();
	glTranslatef(-0.25f, -(0.2f + legHeight / 2), 0.25f);
	glScalef(legWidth, legHeight, legWidth);
	glutSolidCube(1.0f);
	glPopMatrix();

	// �� ��° �ٸ�
	glPushMatrix();
	glTranslatef(0.25f, -(0.2f + legHeight / 2), -0.25f);
	glScalef(legWidth, legHeight, legWidth);
	glutSolidCube(1.0f);
	glPopMatrix();

	// �� ��° �ٸ�
	glPushMatrix();
	glTranslatef(-0.25f, -(0.2f + legHeight / 2), -0.25f);
	glScalef(legWidth, legHeight, legWidth);
	glutSolidCube(1.0f);
	glPopMatrix();
}

void DrawCup() {
	// �� ���� ���� (���)
	GLfloat cup_diffuse[] = { 0.0f, 0.9f, 0.9f, 1.0f };  // ���
	GLfloat cup_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat cup_shininess[] = { 50.0f };

	// ���� ����
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cup_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, cup_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, cup_shininess);

	// GLUquadric ����
	GLUquadric* quadric = gluNewQuadric();

	// �� ��ü (����)
	glPushMatrix();
	glTranslatef(0.25f, -0.1f, 0.25f);  // ���� ��ġ�� ���̺� �𼭸��� �̵�
	glRotatef(-90, 1.0f, 0.0f, 0.0f);   // X���� �������� 90�� ȸ�� (���� ����)
	gluCylinder(quadric, 0.05f, 0.05f, 0.12f, 30, 30);  // ������ 0.1, ���� 0.15�� ����
	glPopMatrix();

	// �� �ٴ� (��ũ)
	glPushMatrix();
	glTranslatef(0.25f, -0.35f, 0.25f);  // �� �ٴ��� ���� �Ʒ��� �̵�
	glRotatef(-90, 1.0f, 0.0f, 0.0f);    // �� �ٴڵ� �����ϰ� ȸ��
	gluDisk(quadric, 0.05f, 0.05f, 30, 30);  // ������ 0.1�� ���� ��ũ
	glPopMatrix();

	// GLUquadric ����
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
	glTranslatef(0.0f, -0.1f, 0.0f); // �������ڸ� Ź�� ���� �̵�
	glutSolidTeapot(0.2);
	glPopMatrix();
}