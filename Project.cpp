/////////////////////////////////////////
//// Componente Curricular: Computação Gráfica
//// Docente: José Bins Filho
//// Trabalho 1 – Labirinto
//// Discentes: Matheus Henrique Trichez e Patric Dalcin Venturini  
/////////////////////////////////////////

#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>

#define MazeHeight 10
#define MazeWidth 10
#define Scale 6
#define StartPosition 12

const double pi = 3.1415926;
GLdouble p[3] = {0, 0, 0};

float robotWalkingUp = 0, robotWalkingSide = StartPosition;
float cameraX = StartPosition, cameraZ = 12;
int changeCamera = 1, cameraAngle = 10, legRotation = 0, changeWalking = 0, changeHeadMovement = 0, robotHeadAngle = 0, robotAngle = 0;

int maze[MazeHeight][MazeWidth] = {
        {1, 1, 1, 1, 1, 1, 1, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 1, 1, 0, 1, 1, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
        {1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
        {1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 1, 0, 1, 1, 1, 1}
};

GLfloat robotBody[8][3] = { {-1, 0, -1}, {1, 0, -1}, {1, 2, -1}, {-1, 2, -1},
                            {1, 0, 1}, {-1, 0, 1}, {-1, 2, 1}, {1, 2, 1} };

GLfloat robotEyes[8][3] = { {-0.5, 0.0, -0.5}, {0.5, 0.0, -0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
                            {0.5, 0.0, 0.2} , {-0.5, 0.0, 0.2}, {-0.5, 0.5, 0.2}, {0.5, 0.5, 0.2}};

void drawCube(float size) {
    size = size / 2;

    glBegin(GL_QUADS);
        // TRASEIRA
        glColor3f(0.28, 0.20, 0.0);
        glVertex3f(size, -size, size);
        glVertex3f(size,  size, size);
        glVertex3f(-size,  size, size);
        glVertex3f(-size, -size, size);

        // FRENTE
        glColor3f(0.12, 0.08, 0.0);
        glVertex3f(size, -size, -size);
        glVertex3f(size, size, -size);
        glVertex3f(-size, size, -size);
        glVertex3f(-size, -size, -size);

        // DIREITA
        glColor3f(0.20, 0.14, 0.0);
        glVertex3f(size, -size, -size);
        glVertex3f(size, size, -size);
        glVertex3f(size, size, size);
        glVertex3f(size, -size, size);

        // ESQUERDA
        glColor3f(0.22, 0.17, 0.0);
        glVertex3f(-size, -size, size);
        glVertex3f(-size, size, size);
        glVertex3f(-size, size, -size);
        glVertex3f(-size, -size, -size);

        // TOPO
        glColor3f(0.18, 0.10, 0.0);
        glVertex3f(size, size, size);
        glVertex3f(size, size, -size);
        glVertex3f(-size, size, -size);
        glVertex3f(-size, size, size);

        // BASE
        glColor3f(0.28, 0.20, 0.0);
        glVertex3f(size, -size, -size);
        glVertex3f(size, -size, size);
        glVertex3f(-size, -size, size);
        glVertex3f(-size, -size, -size);
    glEnd();
}

void drawFloor(float size) {
    size = size / 2;

    glBegin(GL_QUADS);
        glColor3f(0.14, 0.08, 0.0);

        glVertex3f(size, -size, -size);
        glVertex3f(size, -size, size);
        glVertex3f(-size, -size, size);
        glVertex3f(-size, -size, -size);
    glEnd();
}

void drawRobotLegs() {
    GLUquadricObj * quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glPushMatrix();
        glColor3f(0.2, 0.2, 0.2);
        glTranslatef(0.0, 2.0, 0.0);
        glRotatef(90, 1.0, 0.0, 0.0);

        glTranslatef(-0.5, 0, 0);
        glRotatef(-legRotation, 1.0, 0.0, 0.0);
        gluCylinder(quadric, 0.3, 0.3, 2, 100, 100);
        glRotatef(legRotation, 1.0, 0.0, 0.0);

        glTranslatef(1, 0, 0);

        glRotatef(legRotation, 1.0, 0.0, 0.0);
        gluCylinder(quadric, 0.3, 0.3, 2, 100, 100);
    glPopMatrix();
}

void drawRobotArms() {
    GLUquadricObj * quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glPushMatrix();
        glColor3f(0.2, 0.2, 0.2);
        glTranslatef(0.0, 1.5, 0.0);
        glRotatef(90, 1.0, 0.0, 0.0);

        glTranslatef(-1, 0, 0);
        glRotatef(-20, 0.0, 1.0, 0.0);
        glRotatef(legRotation, 1.0, 0.0, 0.0);
        glutSolidSphere(0.3,100,100);
        gluCylinder(quadric, 0.3, 0.3, 1.5, 100, 100);
        glRotatef(-legRotation, 1.0, 0.0, 0.0);
        glRotatef(20, 0.0, 1.0, 0.0);

        glTranslatef(2, 0, 0);
        glRotatef(20, 0.0, 1.0, 0.0);
        glRotatef(-legRotation, 1.0, 0.0, 0.0);
        glutSolidSphere(0.3,100,100);
        gluCylinder(quadric, 0.3, 0.3, 1.5, 100, 100);
    glPopMatrix();
}

void drawRobotBody() {
    glColor3f(0.4, 0.4, 0.4);

    glBegin(GL_QUADS);
        glVertex3fv(robotBody[0]); //frente
        glVertex3fv(robotBody[1]);
        glVertex3fv(robotBody[2]);
        glVertex3fv(robotBody[3]);

        glVertex3fv(robotBody[4]); // fundos
        glVertex3fv(robotBody[5]);
        glVertex3fv(robotBody[6]);
        glVertex3fv(robotBody[7]);

        glColor3f(0.3, 0.3, 0.3);
        glVertex3fv(robotBody[0]); // esq
        glVertex3fv(robotBody[3]);
        glVertex3fv(robotBody[6]);
        glVertex3fv(robotBody[5]);

        glVertex3fv(robotBody[3]); // topo
        glVertex3fv(robotBody[2]);
        glVertex3fv(robotBody[7]);
        glVertex3fv(robotBody[6]);

        glVertex3fv(robotBody[1]); // dir
        glVertex3fv(robotBody[4]);
        glVertex3fv(robotBody[7]);
        glVertex3fv(robotBody[2]);

        glVertex3fv(robotBody[1]); // base
        glVertex3fv(robotBody[0]);
        glVertex3fv(robotBody[5]);
        glVertex3fv(robotBody[4]);
    glEnd();
}

void drawRobotHead() {
    GLUquadricObj * quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glRotatef(robotHeadAngle, 0.0, 1.0, 0.0);

    glColor3f(0.4, 0.4, 0.4);
    glutSolidSphere(1, 100, 100);

    glPushMatrix();
        glTranslatef(0, 0.2,-0.5);
        glColor3f(0.2, 0.2, 0.2);
        glBegin(GL_QUADS);
            glVertex3fv(robotEyes[0]); //frente
            glVertex3fv(robotEyes[1]);
            glVertex3fv(robotEyes[2]);
            glVertex3fv(robotEyes[3]);

            glVertex3fv(robotEyes[4]); // fundos
            glVertex3fv(robotEyes[5]);
            glVertex3fv(robotEyes[6]);
            glVertex3fv(robotEyes[7]);

            glVertex3fv(robotEyes[0]); // esq
            glVertex3fv(robotEyes[3]);
            glVertex3fv(robotEyes[6]);
            glVertex3fv(robotEyes[5]);

            glVertex3fv(robotEyes[3]); // topo
            glVertex3fv(robotEyes[2]);
            glVertex3fv(robotEyes[7]);
            glVertex3fv(robotEyes[6]);

            glVertex3fv(robotEyes[1]); // dir
            glVertex3fv(robotEyes[4]);
            glVertex3fv(robotEyes[7]);
            glVertex3fv(robotEyes[2]);

            glVertex3fv(robotEyes[1]); // base
            glVertex3fv(robotEyes[0]);
            glVertex3fv(robotEyes[5]);
            glVertex3fv(robotEyes[4]);
        glEnd();

        glBegin(GL_QUADS);
            glColor3f(1.0, 0.0, 0.0);

            glVertex3f(0.3, 0.1, -0.501); //Olhos
            glVertex3f(-0.3, 0.1, -0.501);
            glVertex3f(-0.3, 0.4, -0.501);
            glVertex3f(0.3, 0.4, -0.501);
        glEnd();
    glPopMatrix();

    glTranslatef(0.0f, 0.9f, 0.0f);
    glRotatef(-90, 1, 0, 0);
    glColor3f(0.3, 0.3, 0.3);
    glutSolidCone(0.1, 0.8, 20, 20);

    glColor3f(0.4, 0.8, 0.4);
    glTranslatef(0.0f, 0.0f, 0.6f);
    gluDisk(quadric, 0.1, 0.15, 100, 100);

    glColor3f(0.4, 0.0, 0.8);
    glTranslatef(0.0f, 0.0f, 0.2f);
    glutSolidSphere(0.1, 50, 50);

    if (changeHeadMovement == 0) {
        robotHeadAngle += 2;
        if (robotHeadAngle == 20) {
            changeHeadMovement = 1;
        }
    } else {
        robotHeadAngle -= 2;
        if (robotHeadAngle == -20) {
            changeHeadMovement = 0;
        }
    }
}

void drawRobot() {
    glRotatef(robotAngle, 0.0, 1.0, 0.0);

    drawRobotLegs();

    glTranslatef(0.0, 2.0, 0.0);
    drawRobotBody();

    drawRobotArms();

    glTranslatef(0.0, 3.0, 0.0);
    drawRobotHead();

    if (changeWalking == 0) {
        legRotation += 3;
        if (legRotation == 45) {
            changeWalking = 1;
        }
    } else {
        legRotation -= 3;
        if (legRotation == -45) {
            changeWalking = 0;
        }
    }
}

void drawTable() {
    GLUquadricObj * quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glRotatef(45, 0, 1, 0);

    glPushMatrix();
        glColor3f(0.18, 0.10, 0.0);

        glRotatef(-90, 1, 0, 0);
        glTranslatef(1, 1, 0);
        gluCylinder(quadric, 0.2, 0.2, 3, 100, 100);
        glTranslatef(0, -2, 0);
        gluCylinder(quadric, 0.2, 0.2, 3, 100, 100);
        glTranslatef(-2, 0, 0);
        gluCylinder(quadric, 0.2, 0.2, 3, 100, 100);
        glTranslatef(0, 2, 0);
        gluCylinder(quadric, 0.2, 0.2, 3, 100, 100);
    glPopMatrix();

    glTranslatef(0.0, 3.0, 0.0);
    glRotatef(90, 1, 0, 0);

    glColor3f(0.30, 0.27, 0.0);
    gluDisk(quadric, 0.0, 1.0, 100, 100);

    glColor3f(0.35, 0.30, 0.0);
    gluDisk(quadric, 1, 2, 100, 100);

    glRotatef(-90, 1, 0, 0);
    glTranslatef(0.0, 1, 0.0);
    glColor3f(0.8, 0.8, 0.2);
    glutSolidTeapot(1);
}

void drawFountain() {
    GLUquadricObj * quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glPushMatrix();
        glColor3f(0.87, 0.8, 0.9);

        glTranslatef(0.0, 0.30, 0.0);
        glRotatef(-90, 1, 0, 0);
        glutSolidTorus(0.5, 1.9, 50, 50);
        glRotatef(90, 1, 0, 0);
        glTranslatef(0.0, -2.0, 0.0);

        glColor3f(0.0, 0.0, 0.8);
        glTranslatef(0.0, 2, 0.0);
        glRotatef(-90, 1, 0, 0);
        gluDisk(quadric, 0.0, 2.0, 100, 100);

        glColor3f(0.7, 0.7, 0.7);
        glutSolidCone(0.4, 2, 20, 20);

        glColor3f(0.5, 0.5, 0.5);
        glTranslatef(0,0,1.5);
        glutSolidSphere(0.3,50,50);

        glColor3f(0.5, 0.5, 0.5);
        glTranslatef(0,0,0.5);
        glutSolidSphere(0.5,50,50);

        glColor3f(0.7, 0.7, 0.7);
        glTranslatef(0,0,0.8);
        glRotatef(-180, 1,0,0);
        glutSolidCone(0.8, 1, 20, 20);

        glColor3f(0.0, 0.0, 0.8);
        glTranslatef(0.0, 0, -0.01);
        gluDisk(quadric, 0.0, 0.6, 100, 100);
    glPopMatrix();
}

void draw(void) {
    int i, j, positionX, positionY;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (changeCamera == 1) {
		gluLookAt(robotWalkingSide,110 + cameraAngle,robotWalkingUp+50, robotWalkingSide,0,robotWalkingUp, 0,1,0);
	} else {
        gluLookAt(cameraX,20 + cameraAngle,cameraZ, robotWalkingSide,0,robotWalkingUp, 0,1,0);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTranslatef(0.0, Scale/2, 0.0);
    for (i = 0; i < MazeWidth; i++) {
        for (j = 0; j < MazeHeight; j++) {
            if (maze[i][j] == 1) {  // Means there is a cube there
                glPushMatrix();
                    positionX = -i;
                    positionY = j - (MazeHeight / 2);
                    glTranslatef(positionY*Scale, 0, positionX*Scale);
                    drawCube(Scale);
                glPopMatrix();
            } else {
                glPushMatrix();
                    positionX = -i;
                    positionY = j - (MazeHeight / 2);
                    glTranslatef(positionY*Scale, 0, positionX*Scale);
                    drawFloor(Scale);
                    if (i == 1 && j == 1) {
                        glTranslatef(0.0, -Scale/2, 0.0);
                        drawTable();
                    }
                    if (i == 5 && j == 2) {
                        glTranslatef(0.0, -Scale/2, 0.0);
                        glColor3f(0.0,0.0,0.0);
                        drawFountain();
                    }
                glPopMatrix();
            }
        }
    }
    glTranslatef(0.0, -Scale/2, 0.0);

    glTranslatef(robotWalkingSide, 0, robotWalkingUp);
    drawRobot();

    // Executa os comandos OpenGL
    glFlush();
    glutSwapBuffers();

    if (robotWalkingUp > (-1*Scale)) {
	    robotWalkingUp -= 0.5;
        cameraZ -= 0.5;
	} else if ((robotWalkingUp == (-1*Scale)) && (robotWalkingSide > (-3*Scale))) {
        if (robotAngle < 90) {
            robotAngle += 6;
            legRotation = 0;
            cameraX += 0.8;
            cameraZ -= 0.8;
        } else {
            robotWalkingSide -= 0.5;
            cameraX -= 0.5;
        }
	} else if ((robotWalkingUp <= (-1*Scale) && robotWalkingUp > (-3*Scale)) && (robotWalkingSide == (-3*Scale))) {
        if (robotAngle > 0) {
            robotAngle -= 6;
            legRotation = 0;
            cameraX -= 0.8;
            cameraZ += 0.8;
        } else {
            robotWalkingUp -= 0.5;
            cameraZ -= 0.5;
        }
	} else if ((robotWalkingUp == (-3*Scale)) && (robotWalkingSide < Scale)) {
        if (robotAngle > -90) {
            robotAngle -= 6;
            legRotation = 0;
            cameraX -= 0.8;
            cameraZ -= 0.8;
        } else {
            robotWalkingSide += 0.5;
            cameraX += 0.5;
        }
	} else if ((robotWalkingUp <= (-3*Scale) && robotWalkingUp > (-6*Scale)) && (robotWalkingSide == Scale)) {
        if (robotAngle < 0) {
            robotAngle += 6;
            legRotation = 0;
            cameraX += 0.8;
            cameraZ += 0.8;
        } else {
            robotWalkingUp -= 0.5;
            cameraZ -= 0.5;
        }
	} else if ((robotWalkingUp == (-6*Scale)) && (robotWalkingSide > 0)) {
        if (robotAngle < 90) {
            robotAngle += 6;
            legRotation = 0;
            cameraX += 0.8;
            cameraZ -= 0.8;
        } else {
            robotWalkingSide -= 0.5;
            cameraX -= 0.5;
        }
	} else if ((robotWalkingUp <= (-6*Scale) && robotWalkingUp > (-10*Scale)) && (robotWalkingSide == 0)) {
        if (robotAngle > 0) {
            robotAngle -= 6;
            legRotation = 0;
            cameraX -= 0.8;
            cameraZ += 0.8;
        } else {
            robotWalkingUp -= 0.5;
            cameraZ -= 0.5;
        }
	}
}

void redraw(int) {
    glutPostRedisplay();
    draw();
    glutTimerFunc(50, redraw, 1);
}

void keyPressed(unsigned char key, int x, int y) {
	if (key == 'c') {
        cameraAngle = 0;
		changeCamera = (changeCamera) ? 0 : 1;
    } else if (key == '+') {
        cameraAngle -= 1;
    } else if (key == '-') {
        cameraAngle += 1;
    }

//	glutPostRedisplay();
}

// Start rendering params
void start(void) {
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

// Callback function of window size alteration
void changeWindowSize(GLsizei w, GLsizei h) {
    // Avoid zero's division
    if (h == 0) h = 1;

    // Specify viewport dimensions
    glViewport(0, 0, w, h);

    // Initialize coordinates system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, w/h, 0.5, 200);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 700);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Study Test");
    glutTimerFunc(40, redraw, 1);
    glutDisplayFunc(draw);
    glutReshapeFunc(changeWindowSize);
    glutKeyboardFunc(keyPressed);
    start();
    glutMainLoop();
}
