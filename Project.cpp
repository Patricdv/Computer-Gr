/////////////////////////////////////////////////////////////////////////
///////////// Componente Curricular: Computação Gráfica /////////////////
/////////////////////////////////////////////////////////////////////////
//// Docente: José Bins Filho                                        ////
//// Trabalho 2 – Labirinto Com Iluminação, NURBS e Transparência    ////
//// Discentes: Matheus Henrique Trichez e Patric Dalcin Venturini   ////
/////////////////////////////////////////////////////////////////////////

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
int changeCamera = 0, cameraAngle = 10, legRotation = 0, changeWalking = 0, changeHeadMovement = 0, robotHeadAngle = 0, robotAngle = 0;

int maze[MazeHeight][MazeWidth] = {
        {1, 1, 1, 1, 1, 1, 1, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 1, 1, 0, 1, 1, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
        {1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 0, 1, 1, 1, 1}
};

GLfloat robotBody[8][3] = { {-1, 0, -1}, {1, 0, -1}, {1, 2, -1}, {-1, 2, -1},
                            {1, 0, 1}, {-1, 0, 1}, {-1, 2, 1}, {1, 2, 1} };

GLfloat tableFoots[4][4][3] = {
        { { 0.0, 0.0, 0.0 },{ 1.0, 1.3, 0.0 },{ -1.5, 2.9, 0.0 },{ 0.1, 4.0, 0.0 } },
        { { 0.5, 0.0, 0.0 },{ 1.5, 1.3, 0.0 },{ -1.0, 2.9, 0.0 },{ 0.55, 4.0, 0.0 } },
        { { 0.25, 0.0, 0.5 },{ 1.25, 1.3, 0.5 },{ -1.25, 2.9, 0.5 },{ 0.3, 4.0, 0.5 } },
        { { 0.0, 0.0, 0.0 },{ 1.0, 1.3, 0.0 },{ -1.5, 2.9, 0.0 },{ 0.1, 4.0, 0.0 } }
};

GLfloat postCtrlPoints[4][4][3] = {
        { { 0.0, 0.0, 0.0 },{ 2.0, 2.0, 0.0 },{ -2.5, 3.2, 0.0 },{ -3.8, 1.45, 0.0 } },
        { { 0.8, 0.0, 0.6 },{ 2.8, 2.0, 0.6 },{ -1.7, 3.2, 0.6 },{ -3.0, 1.45, 0.6 } },
        { { 0.8, 0.0, -0.6 },{ 2.8, 2.0, -0.6 },{ -1.7, 3.2, -0.6 },{ -3.0, 1.45, -0.6 } },
        { { 0.0, 0.0, 0.0 },{ 2.0, 2.0, 0.0 },{ -2.5, 3.2, 0.0 },{ -3.8, 1.45, 0.0 } }
};

void nurbs(GLfloat cp[4][4][3], GLint un, GLint vn) {
    int u, v;

    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 12, 4, 0, 1, 3, 4, &cp[0][0][0]);
    glEnable(GL_MAP2_VERTEX_3);
    glEnable(GL_AUTO_NORMAL);

    glBegin(GL_QUADS);
        for (u = 0; u < un; u++) {
            for (v = 0; v < vn; v++) {
                glEvalCoord2f((GLfloat)v / vn, (GLfloat)(u+1) / un);
                glEvalCoord2f((GLfloat)(v+1) / vn, (GLfloat)(u+1) / un);
                glEvalCoord2f((GLfloat)(v+1) / vn, (GLfloat)u / un);
                glEvalCoord2f((GLfloat)v / vn, (GLfloat)u / un);
            }
        }
    glEnd();
}

void drawCube(float size) {
    glColor3f(0.48, 0.80, 0.12);
    glutSolidCube(size);
}

void drawFloor() {
    float size = 500 / 2;

    GLUquadricObj * quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glPushMatrix();
        glRotatef(-90, 1.0, 0.0, 0.0);
        gluDisk(quadric, 0, size, 100, 100);
    glPopMatrix();
}

void drawPost() {
    GLUquadricObj * quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glTranslatef(0, 0, 2.0f);

    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        gluCylinder(quadric, 0.3, 0.3, 8, 100, 100);
    glPopMatrix();

    glTranslatef(0, 8, 0);
    glPushMatrix();
        glColor3f(0.35, 0.30, 0.0);
        glRotatef(-90, 1, 0, 0);
        gluDisk(quadric, 0, 0.5, 100, 100);
    glPopMatrix();

    glTranslatef(-0.2, 0, -0.2);
    glRotatef(-45, 0, 1, 0);
    nurbs(postCtrlPoints, 30, 30);

    glTranslatef(-3.2, 1, 0);
    glPushMatrix();
        glColor3f(0.75, 0.30, 0.5);
        glRotatef(-90, 1, 0, 0);
        glutSolidCone(2, 0.8, 100, 100);
    glPopMatrix();
}

void drawBear() {
    GLUquadricObj * quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glPushMatrix();
        glTranslatef(0.0, 0.1, 0.0);
        glPushMatrix();
            glRotatef(50, 0.0, 1.0, 0.0);
            glRotatef(180, 1.0, 0.0, 0.0);
            gluCylinder(quadric, 0.1, 0.1, 0.5, 20, 20);
            glTranslatef(0.0, 0.0, 0.5);
            gluDisk(quadric, 0.0, 0.1, 100, 100);
        glPopMatrix();
        glPushMatrix();
            glRotatef(-40, 0.0, 1.0, 0.0);
            glRotatef(180, 1.0, 0.0, 0.0);
            gluCylinder(quadric, 0.1, 0.1, 0.5, 20, 20);
            glTranslatef(0.0, 0.0, 0.5);
            gluDisk(quadric, 0.0, 0.1, 100, 100);
        glPopMatrix();

        glTranslatef(0.0, 0.2, 0.0);
        glutSolidSphere(0.3, 100, 100);
    glPopMatrix();
}

void drawOutsideThings() {
    //Draw of the post aside of the bear
    glPushMatrix();
        drawPost();
    glPopMatrix();

    drawBear();
    // nurbs(postCtrlPoints, 30, 30);
}

void drawWindowedWall(float size) {
    float brickSize = size/4;
    int i, j;

    glPushMatrix();
        glTranslatef(8*Scale, 0, 0);
        drawOutsideThings();
    glPopMatrix();

    glPushMatrix();
        glTranslatef((size/2 - brickSize/2), brickSize/2, (-size/2 + brickSize/2));
        for(i = 0; i<=3; i++) {
            for(j = 0; j<=3; j++) {
                glPushMatrix();
                    if((i == 0 || i == 3) || (j == 0 || j == 3)) {
                        glTranslatef(0, brickSize*i, brickSize*j);
                        glColor3f(0.0f, 0.0f, 0.0f);
                        glutSolidCube(brickSize);
                    }
                glPopMatrix();
            }
        }

        for(i = 2; i>=1; i--) {
            for(j = 2; j>=1; j--) {
                glPushMatrix();
                    glColor4f(0.5f, 0.8f, 0.1f, 0.15f);
                    glTranslatef(0, brickSize*i, brickSize*j);
                    glutSolidCube(brickSize);
                glPopMatrix();
            }
        }
    glPopMatrix();
}

void drawRobotLegs() {
    GLUquadricObj * quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    // Left Legs
    glPushMatrix();
        glTranslatef(-0.5, 0, 0);
        glRotatef(legRotation, 0.0, 1.0, 0.0);

        glPushMatrix();
            glRotatef(50, 0.0, 1.0, 0.0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(-60, 0.0, 1.0, 0.0);
            glColor3f(0.2, 0.2, 0.2);
            gluCylinder(quadric, 0.2, 0.2, 2, 100, 100);
            glTranslatef(0, 0, 2);
            glColor3f(0.8, 0.4, 0.4);
            glutSolidTorus(0.2, 0.1, 100, 100);

            glRotatef(60, 0.0, 1.0, 0.0);
            glColor3f(0.0, 0.4, 0.4);
            gluCylinder(quadric, 0.2, 0.2, 2, 100, 100);
            glTranslatef(0, 0, 2);
            glColor3f(0.8, 0.4, 0.4);
            glutSolidTorus(0.2, 0.1, 100, 100);
        glPopMatrix();

        glPushMatrix();
            glRotatef(-50, 0.0, 1.0, 0.0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(-60, 0.0, 1.0, 0.0);
            glColor3f(0.2, 0.2, 0.2);
            gluCylinder(quadric, 0.2, 0.2, 2, 100, 100);
            glTranslatef(0, 0, 2);
            glColor3f(0.8, 0.4, 0.4);
            glutSolidTorus(0.2, 0.1, 100, 100);

            glRotatef(60, 0.0, 1.0, 0.0);
            glColor3f(0.0, 0.4, 0.4);
            gluCylinder(quadric, 0.2, 0.2, 2, 100, 100);
            glTranslatef(0, 0, 2);
            glColor3f(0.8, 0.4, 0.4);
            glutSolidTorus(0.2, 0.1, 100, 100);
        glPopMatrix();
    glPopMatrix();

    // Right Legs
    glPushMatrix();
        glTranslatef(0.5, 0, 0);
        glRotatef(legRotation, 0.0, 1.0, 0.0);

        glPushMatrix();
            glRotatef(50, 0.0, 1.0, 0.0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(60, 0.0, 1.0, 0.0);
            glColor3f(0.2, 0.2, 0.2);
            gluCylinder(quadric, 0.2, 0.2, 2, 100, 100);
            glTranslatef(0, 0, 2);
            glColor3f(0.8, 0.4, 0.4);
            glutSolidTorus(0.2, 0.1, 100, 100);

            glRotatef(-60, 0.0, 1.0, 0.0);
            glColor3f(0.0, 0.4, 0.4);
            gluCylinder(quadric, 0.2, 0.2, 2, 100, 100);
            glTranslatef(0, 0, 2);
            glColor3f(0.8, 0.4, 0.4);
            glutSolidTorus(0.2, 0.1, 100, 100);
        glPopMatrix();

        glPushMatrix();
            glRotatef(-50, 0.0, 1.0, 0.0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(60, 0.0, 1.0, 0.0);
            glColor3f(0.2, 0.2, 0.2);
            gluCylinder(quadric, 0.2, 0.2, 2, 100, 100);
            glTranslatef(0, 0, 2);
            glColor3f(0.8, 0.4, 0.4);
            glutSolidTorus(0.2, 0.1, 100, 100);

            glRotatef(-60, 0.0, 1.0, 0.0);
            glColor3f(0.0, 0.4, 0.4);
            gluCylinder(quadric, 0.2, 0.2, 2, 100, 100);
            glTranslatef(0, 0, 2);
            glColor3f(0.8, 0.4, 0.4);
            glutSolidTorus(0.2, 0.1, 100, 100);
        glPopMatrix();
    glPopMatrix();
}

void drawRobotArms() {
    GLUquadricObj * quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glPushMatrix();
        glRotatef(90, 1.0, 0.0, 0.0);

        // Left Arm
        glPushMatrix();
          glTranslatef(-1, 0, 0);
          glRotatef(-20, 0.0, 1.0, 0.0);
          glRotatef(legRotation, 1.0, 0.0, 0.0);

          glColor3f(0.2, 0.2, 0.2);
          glutSolidSphere(0.3, 100,   100);
          glColor3f(0.6, 0.8, 0.6);
          gluCylinder(quadric, 0.1, 0.1, 1, 100, 100);

          glTranslatef(0, 0, 1);
          glRotatef(50, 1.0, 0.0, 0.0);
          glColor3f(0.2, 0.2, 0.2);
          glutSolidSphere(0.3, 100, 100);
          glColor3f(0.6, 0.8, 0.6);
          gluCylinder(quadric, 0.2, 0.2, 1, 100, 100);
          glTranslatef(0, 0, 1);
          glColor3f(0.2, 0.2, 0.2);
          glutSolidSphere(0.3, 100, 100);
        glPopMatrix();

        // Right Arm
        glPushMatrix();
          glTranslatef(1, 0, 0);
          glRotatef(20, 0.0, 1.0, 0.0);
          glRotatef(-legRotation, 1.0, 0.0, 0.0);

          glColor3f(0.2, 0.2, 0.2);
          glutSolidSphere(0.3, 100, 100);
          glColor3f(0.6, 1.0, 0.6);
          gluCylinder(quadric, 0.1, 0.1, 1, 100, 100);

          glTranslatef(0, 0, 1);
          glRotatef(50, 1.0, 0.0, 0.0);
          glColor3f(0.2, 0.2, 0.2);
          glutSolidSphere(0.3, 100, 100);
          glColor3f(0.6, 0.8, 0.6);
          gluCylinder(quadric, 0.2, 0.2, 1, 100, 100);
          glTranslatef(0, 0, 1);
          glColor3f(0.2, 0.2, 0.2);
          glutSolidSphere(0.3, 100, 100);
        glPopMatrix();
    glPopMatrix();
}

void drawRobotBody() {
    GLUquadricObj * quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glPushMatrix();
        glColor3f(0.0, 0.2, 0.9);
        glutSolidSphere(1, 100, 100);
    glPopMatrix();

    glTranslatef(0.0f, 1.5f, 0.0f);
    glPushMatrix();
        glColor3f(0.4, 0.4, 0.4);
        glRotatef(90, 1, 0, 0);
        glutSolidCone(0.5, 2, 20, 20);
    glPopMatrix();

    glTranslatef(0.0f, 0.5f, 0.0f);
    glPushMatrix();
        glColor3f(0.0, 0.2, 0.9);
        glutSolidSphere(0.8, 100, 100);
    glPopMatrix();

    glTranslatef(0.0f, 0.4f, 0.5f);

    glPushMatrix();
        glTranslatef(0.25f, 0.0f, 0.0f);
        glRotatef(50, 1, 0, 0);

        glColor3f(0.8, 0.3, 0.4);
        gluCylinder(quadric, 0.1, 0.2, 0.8, 100, 100);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-0.25f, 0.0f, 0.0f);
        glRotatef(50, 1, 0, 0);

        glColor3f(0.8, 0.3, 0.4);
        gluCylinder(quadric, 0.1, 0.2, 0.8, 100, 100);
    glPopMatrix();

    glTranslatef(0.0f, 0.0f, -0.5f);
}

void drawRobotHead() {
    GLUquadricObj * quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glRotatef(robotHeadAngle, 0.0, 1.0, 0.0);

    glColor3f(0.4, 0.4, 0.4);
    glPushMatrix();
        glRotatef(90, 1.0, 0.0, 0.0);
        gluCylinder(quadric, 0.5, 0.5, 1, 100, 100);
    glPopMatrix();

    glutSolidSphere(0.5, 100, 100);

    glPushMatrix();
        glTranslatef(0, 0.1,-0.35);
        glColor3f(0.3, 0.3, 0.3);
        glutSolidCube(0.4);

        glBegin(GL_QUADS);
            glColor3f(1.0, 0.8, 0.4);

            glVertex3f(0.15, -0.15, -0.201);
            glVertex3f(-0.15, -0.15, -0.201);
            glVertex3f(-0.15, 0.15, -0.201);
            glVertex3f(0.15, 0.15, -0.201);
        glEnd();
    glPopMatrix();

    glTranslatef(0.0f, 0.4f, 0.0f);
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

    glTranslatef(0.0, 3.2, 0.0);
    drawRobotLegs();
    drawRobotBody();
    drawRobotArms();

    glutSolidTorus(0.4, 0.5, 100, 100);
    glTranslatef(0.0, 1.3, 0.0);
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

        glTranslatef(1, 0, 1);
        glPushMatrix();
            glRotatef(135, 0, 1, 0);
            nurbs(tableFoots, 30, 30);
        glPopMatrix();

        glTranslatef(0, 0, -2);
        glPushMatrix();
            glRotatef(45, 0, 1, 0);
            nurbs(tableFoots, 30, 30);
        glPopMatrix();

        glTranslatef(-2, 0, 0);
        glPushMatrix();
            glRotatef(-45, 0, 1, 0);
            nurbs(tableFoots, 30, 30);
        glPopMatrix();

        glTranslatef(0, 0, 2);
        glPushMatrix();
            glRotatef(-135, 0, 1, 0);
            nurbs(tableFoots, 30, 30);
        glPopMatrix();
    glPopMatrix();

    glTranslatef(0.0, 4.0, 0.0);
    glRotatef(90, 1, 0, 0);

    glColor3f(0.30, 0.27, 0.0);
    gluDisk(quadric, 0.0, 1.0, 100, 100);

    glColor3f(0.35, 0.30, 0.0);
    gluDisk(quadric, 1, 2.5, 100, 100);

    glRotatef(-90, 1, 0, 0);
    glTranslatef(0.0, 0.8, 0.0);
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

void applyLights() {
  ////////////////////////////////////////////////
  //////////////////  First Light ////////////////
  ////////////////////////////////////////////////
  GLfloat light1Diffuse[] = {0.3, 0.3, 0.3, 1.0};
  GLfloat light1Specular[] = {0.3, 0.3, 0.3, 1.0};
  GLfloat light1Position[] = {0, 50, -5*Scale, 1.0};

  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light1Specular);
  glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
  /////////////////////////////////////////////////

  ////////////////////////////////////////////////
  /////////////////  Second Light ////////////////
  ////////////////////////////////////////////////
  GLfloat light2Diffuse[] = {0.5, 0.3, 0.3, 1.0};
  GLfloat light2Specular[] = {0.5, 0.3, 0.3, 1.0};
  GLfloat light2Position[] = {12*Scale, 10, -3*Scale, 1.0};
  GLfloat light2SpotDirection[] = {0, -1, 0};

  glLightfv(GL_LIGHT2, GL_DIFFUSE, light2Diffuse);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light2Specular);
  glLightfv(GL_LIGHT2, GL_POSITION, light2Position);
  glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2SpotDirection);
  /////////////////////////////////////////////////
}

void draw(void) {
    int i, j, positionX, positionY;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    ///////////////////////////////////////////////////
    /////  All the 10 program cameras            //////
    /////  0 - The maze's top camera             //////
    /////  1 - The Camera that follow the Robot  //////
    /////  2 to 5 - Camera to Objects            //////
    /////  6 to 9 - Robor side Cameras           //////
    /////  'c' - to change between them          //////
    ///////////////////////////////////////////////////
    if (changeCamera == 0) {
        gluLookAt(3,100 + cameraAngle,-4*Scale, 3,0,-5*Scale, 0,1,0);
  	} else if (changeCamera == 1) {
        gluLookAt(cameraX,20 + cameraAngle,cameraZ, robotWalkingSide,0,robotWalkingUp, 0,1,0);
    } else if (changeCamera == 2) {
        gluLookAt(-2*Scale,10 + cameraAngle,-Scale + 1, -4*Scale,4,-Scale, 0,1,0);
    } else if (changeCamera == 3) {
        gluLookAt(-3*Scale,6 + cameraAngle,-3*Scale, -3*Scale,4,-5*Scale, 0,1,0);
    } else if (changeCamera == 4) {
        gluLookAt(3*Scale,3 + cameraAngle,-3*Scale, 4*Scale,3,-3*Scale, 0,1,0);
    } else if (changeCamera == 5) {
        gluLookAt(8*Scale,10 + cameraAngle,-3*Scale, 10*Scale,3,-3*Scale, 0,1,0);
    } else if (changeCamera == 6) {
        gluLookAt(robotWalkingSide, 20 + cameraAngle,robotWalkingUp-10, robotWalkingSide,0,robotWalkingUp, 0,1,0);
    } else if (changeCamera == 7) {
        gluLookAt(robotWalkingSide+10, 20 + cameraAngle,robotWalkingUp, robotWalkingSide,0,robotWalkingUp, 0,1,0);
    } else if (changeCamera == 8) {
        gluLookAt(robotWalkingSide-10, 20 + cameraAngle,robotWalkingUp, robotWalkingSide,0,robotWalkingUp, 0,1,0);
    } else if (changeCamera == 9) {
        gluLookAt(robotWalkingSide, 20 + cameraAngle,robotWalkingUp+10, robotWalkingSide,0,robotWalkingUp, 0,1,0);
    }

    applyLights();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawFloor();
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
                    if (i == 1 && j == 1) {
                        glTranslatef(positionY*Scale, -Scale/2, positionX*Scale);
                        drawTable();
                    }
                    if (i == 5 && j == 2) {
                        glTranslatef(positionY*Scale, -Scale/2, positionX*Scale);
                        glColor3f(0.0,0.0,0.0);
                        drawFountain();
                    }

                    if (i == 3 && j == 9) {
                        glTranslatef(positionY*Scale, -Scale/2, positionX*Scale);
                        glColor3f(0.0,0.0,0.0);
                        drawWindowedWall(Scale);
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
		changeCamera++;
        if (changeCamera == 10) {
            changeCamera = 0;
        }
    }

    if (key == '+') {
        cameraAngle -= 1;
    }

    if (key == '-') {
        cameraAngle += 1;
    }

    if (key == '0' or key == '1' or key == '2' or key == '3' or key == '4' or key == '5' or key == '6' or key == '7' or key == '8' or key == '9') {
        cameraAngle = 0;
        changeCamera = (int)key - 48;
    }
}

// Start rendering params
void start(void) {
    ////////////////////////////////////////////////
    /////////////////  Enable light ////////////////
    ////////////////////////////////////////////////
    GLfloat ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat especularity[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLint materialEspecularity = 10;

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
  	glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especularity);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, materialEspecularity);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
