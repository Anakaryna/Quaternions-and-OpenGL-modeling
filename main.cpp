#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <cstdio>
#include <cmath>
#include <iomanip>

#include "include/Camera.h"
#include "include/Map.h"
#include "include/Quaternion.h"
#include "include/Block.h"

// Objet Camera
Camera *cam = new Camera();
// Objet Scène
Map *m = new Map();
// Objets Cube
Block cubeMatrix(1.0f, 1.0f, 1.0f);
Block cubeQuaternion(1.0f, 1.0f, 1.0f);
Block cubeGLRotate(1.0f, 1.0f, 1.0f);  // Cube utilisant glRotatef

GLuint textures[8]; // Déclaration des textures
Quaternion rotationQuaternion(1.0, 0.0, 0.0, 0.0);
Quaternion rotationQuaternion2(1.0, 0.0, 0.0, 0.0);
float rotationAngle = 0.0f;
bool swapRender = 1 ;


void drawText(const char* text, float x, float y, float z)
{
    glRasterPos3f(x, y, z);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}



void drawCubes()
{

    // Cube avec rotation par matrice
    float rotationMatrix[9];
    rotationQuaternion2.toRotationMatrix3x3(rotationMatrix);

    glPushMatrix();
    glTranslatef(-4.0f, 0.0f, -5.0f); // Position du Cube
    //glTranslatef(0.5f, 0.5f, 0.5f); // Translation du centre du cube à l'origine
    float matrix4x4[16] = {
            rotationMatrix[0], rotationMatrix[1], rotationMatrix[2], 0.0f,
            rotationMatrix[3], rotationMatrix[4], rotationMatrix[5], 0.0f,
            rotationMatrix[6], rotationMatrix[7], rotationMatrix[8], 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
    glMultMatrixf(matrix4x4);
    //glTranslatef(-0.5f, -0.5f, -0.5f); // Translation du cube à sa position initiale
    glTranslatef(1.0f, 0.0f, 0.0f); // Translation du centre du cube decentré
    cubeMatrix.Draw();
    glPopMatrix();
    drawText("Matrix Rotation", -4.5f, 1.5f, -5.0f);

    // Cube avec rotation par quaternion
    glPushMatrix();
    glTranslatef(-1.0f, 0.0f, -5.0f); // Position du Cube au centre
    glTranslatef(0.5f, 0.5f, 0.5f); // Translation du centre du cube à l'origine
    //glTranslatef(1.0f, 0.0f, 0.0f); // Translation du centre du cube decentré

    float vertices[8][3] = {
            {-0.5f, -0.5f, -0.5f},
            {0.5f, -0.5f, -0.5f},
            {0.5f, 0.5f, -0.5f},
            {-0.5f, 0.5f, -0.5f},
            {-0.5f, -0.5f, 0.5f},
            {0.5f, -0.5f, 0.5f},
            {0.5f, 0.5f, 0.5f},
            {-0.5f, 0.5f, 0.5f}
    };

    for (int i = 0; i < 8; i++) {
        vertices[i][0] -= 1.5f; // Translation du centre du cube à l'origine
    }

    // Appliquer la rotation quaternion à chaque sommet
    for (int i = 0; i < 8; i++) {
        rotationQuaternion2.rotatePointWithQuaternion(vertices[i], rotationQuaternion2);
    }

    for (int i = 0; i < 8; i++) {
        vertices[i][0] += 1.5f; // Repositionner les sommets après la rotation
    }


    glBegin(GL_QUADS);

    // Face avant
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertices[0]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertices[1]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertices[2]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertices[3]);

    // Face arrière
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertices[4]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertices[5]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertices[6]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertices[7]);

    // Face supérieure
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertices[3]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertices[2]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertices[6]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertices[7]);

    // Face inférieure
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertices[0]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertices[1]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertices[5]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertices[4]);

    // Face droite
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertices[1]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertices[5]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertices[6]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertices[2]);

    // Face gauche
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertices[0]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertices[4]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertices[7]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertices[3]);

    glEnd();

    glPopMatrix();

    drawText("Quaternion Rotation", 1.0f, 1.5f, -5.0f);

    // Cube avec rotation par glRotatef
    glPushMatrix();
    glTranslatef(4.0f, 0.0f, -5.0f); // Translation pour décentrer
    //glTranslatef(-0.5f, -0.5f, -0.5f);
    glTranslatef(1.0f, 0.0f, 0.0f); // Translation du centre du cube decentré
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f); // Rotation
    //glTranslatef(-0.5f, -0.5f, -0.5f); // Translation du cube à sa position initiale
    glTranslatef(1.0f, 0.0f, 0.0f); // Translation du centre du cube decentré
    cubeGLRotate.Draw();
    glPopMatrix();
    drawText("glRotatef Rotation", 5.0f, 1.5f, -5.0f);




    // Shearing et Scaling
    glPushMatrix();
    glTranslatef(-12.0f, 1.0f, -5.0f); // Position du Cube
    applyShear(0.5f, 0.0f, 0.0f); // Applique un cisaillement sur l'axe X
    glScalef(1.5f, 1.5f, 1.5f); // Appliquer un scaling
    glMultMatrixf(matrix4x4); // Appliquer la rotation
    glTranslatef(-2.0f, 0.0f, 0.0f); // Translation du centre du cube décéntré
    cubeMatrix.Draw();
    glPopMatrix();
    drawText("Sclaed and Sheared Rotation", -12.0f, 1.5f, -5.0f);


    // Shearing,Translation et Scaling avec Quaternions
    glPushMatrix();
    glTranslatef(-18.0f, 1.0f, -5.0f); // Position du Cube

    // Prendre les vertex du cube
    float shearScaleTranslateVertices[8][3] = {
            {-0.5f, -0.5f, -0.5f},
            {0.5f, -0.5f, -0.5f},
            {0.5f, 0.5f, -0.5f},
            {-0.5f, 0.5f, -0.5f},
            {-0.5f, -0.5f, 0.5f},
            {0.5f, -0.5f, 0.5f},
            {0.5f, 0.5f, 0.5f},
            {-0.5f, 0.5f, 0.5f}
    };

    for (int i = 0; i < 8; i++) {
        rotationQuaternion2.shearAndScalePointandTranslate(shearScaleTranslateVertices[i], 0.5f, 0.0f, 0.0f, 1.5f, 1.5f, 1.5f, 0.0f, 3.0f, 0.0f);
        rotationQuaternion2.rotatePointWithQuaternion(shearScaleTranslateVertices[i], rotationQuaternion2);

    }

    glBegin(GL_QUADS);

    // Face avant
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(shearScaleTranslateVertices[0]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(shearScaleTranslateVertices[1]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(shearScaleTranslateVertices[2]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(shearScaleTranslateVertices[3]);

    // Face arrière
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(shearScaleTranslateVertices[4]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(shearScaleTranslateVertices[5]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(shearScaleTranslateVertices[6]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(shearScaleTranslateVertices[7]);

    // Face supérieure
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(shearScaleTranslateVertices[3]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(shearScaleTranslateVertices[2]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(shearScaleTranslateVertices[6]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(shearScaleTranslateVertices[7]);

    // Face inférieure
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(shearScaleTranslateVertices[0]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(shearScaleTranslateVertices[1]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(shearScaleTranslateVertices[5]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(shearScaleTranslateVertices[4]);

    // Face droite
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(shearScaleTranslateVertices[1]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(shearScaleTranslateVertices[5]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(shearScaleTranslateVertices[6]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(shearScaleTranslateVertices[2]);

    // Face gauche
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(shearScaleTranslateVertices[0]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(shearScaleTranslateVertices[4]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(shearScaleTranslateVertices[7]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(shearScaleTranslateVertices[3]);

    glEnd();

    glPopMatrix();
    drawText("Scaled and Sheared Rotation with Quaternion", -18.0f, 3.0f, -5.0f);

}



void drawSolarSys() {
    float time = glutGet(GLUT_ELAPSED_TIME) / 10.0f;

    // Define speeds and sizes for each planet
    float speedMercury = 0.004f;
    float speedVenus = 0.0015f;
    float speedEarth = 0.001f;
    float speedMars = 0.0008f;
    float speedJupiter = 0.0002f;
    float speedSaturn = 0.0001f;
    float speedUranus = 0.00005f;
    float speedNeptune = 0.00003f;

    // Initial angles for each planet
    float initialAngleMercury = 0.0f;
    float initialAngleVenus = 45.0f;  // 45 degrees in radians
    float initialAngleEarth = 90.0f;  // 90 degrees in radians
    float initialAngleMars = 135.0f;  // 135 degrees in radians
    float initialAngleJupiter = 180.0f;  // 180 degrees in radians
    float initialAngleSaturn = 225.0f;  // 225 degrees in radians
    float initialAngleUranus = 270.0f;  // 270 degrees in radians
    float initialAngleNeptune = 315.0f;  // 315 degrees in radians

    // Angles based on the elapsed time and initial angles
    float angleMercury = time * speedMercury + initialAngleMercury;
    float angleVenus = time * speedVenus + initialAngleVenus;
    float angleEarth = time * speedEarth + initialAngleEarth;
    float angleMars = time * speedMars + initialAngleMars;
    float angleJupiter = time * speedJupiter + initialAngleJupiter;
    float angleSaturn = time * speedSaturn + initialAngleSaturn;
    float angleUranus = time * speedUranus + initialAngleUranus;
    float angleNeptune = time * speedNeptune + initialAngleNeptune;

    // Orbit rotations
    Quaternion orbitRotationMercury(cos(angleMercury / 2), 0.0f, sin(angleMercury / 2), 0.0f);
    Quaternion orbitRotationVenus(cos(angleVenus / 2), 0.0f, sin(angleVenus / 2), 0.0f);
    Quaternion orbitRotationEarth(cos(angleEarth / 2), 0.0f, sin(angleEarth / 2), 0.0f);
    Quaternion orbitRotationMars(cos(angleMars / 2), 0.0f, sin(angleMars / 2), 0.0f);
    Quaternion orbitRotationJupiter(cos(angleJupiter / 2), 0.0f, sin(angleJupiter / 2), 0.0f);
    Quaternion orbitRotationSaturn(cos(angleSaturn / 2), 0.0f, sin(angleSaturn / 2), 0.0f);
    Quaternion orbitRotationUranus(cos(angleUranus / 2), 0.0f, sin(angleUranus / 2), 0.0f);
    Quaternion orbitRotationNeptune(cos(angleNeptune / 2), 0.0f, sin(angleNeptune / 2), 0.0f);

    // Center position for all spheres (the Sun)
    float centerX = 0.0f;
    float centerY = 0.0f;
    float centerZ = 0.0f;

    // Distances of the planets from the center (not to scale)
    float orbitRadiusMercury = 10.0f;
    float orbitRadiusVenus = 15.0f;
    float orbitRadiusEarth = 20.0f;
    float orbitRadiusMars = 30.0f;
    float orbitRadiusJupiter = 50.0f;
    float orbitRadiusSaturn = 70.0f;
    float orbitRadiusUranus = 90.0f;
    float orbitRadiusNeptune = 110.0f;

    // Sizes of the planets
    float radiusMercury = 0.3f;
    float radiusVenus = 0.9f;
    float radiusEarth = 1.0f;
    float radiusMars = 0.5f;
    float radiusJupiter = 2.5f;
    float radiusSaturn = 2.0f;
    float radiusUranus = 1.5f;
    float radiusNeptune = 1.4f;
    float radiusSun = 3.0f;

    // Rotation quaternion for the planets (rotation around the Y-axis)
    float rotationSpeed = 0.00001f;
    float angleRotation = time * rotationSpeed;
    Quaternion rotationIncrement(cos(angleRotation / 2), 0.0f, sin(angleRotation / 2), 0.0f);

    // Rotation quaternion for the Sun
    static Quaternion rotationSun(1, 0, 0, 0);
    rotationSun = rotationIncrement * rotationSun;
    rotationSun = rotationSun.normalize();

    // Draw the Sun
    cubeMatrix.DrawSphere2(textures[SUN], centerX, centerY, centerZ, rotationSun, Quaternion(1, 0, 0, 0), radiusSun, 0.0f);

    // Draw the planets with the updated positions and rotations
    static Quaternion rotationMercury(1, 0, 0, 0);
    rotationMercury = rotationIncrement * rotationMercury;
    rotationMercury = rotationMercury.normalize();
    cubeMatrix.DrawSphere2(textures[MERCURY], centerX, centerY, centerZ, rotationMercury, orbitRotationMercury, radiusMercury, orbitRadiusMercury);

    static Quaternion rotationVenus(1, 0, 0, 0);
    rotationVenus = rotationIncrement * rotationVenus;
    rotationVenus = rotationVenus.normalize();
    cubeMatrix.DrawSphere2(textures[VENUS], centerX, centerY, centerZ, rotationVenus, orbitRotationVenus, radiusVenus, orbitRadiusVenus);

    static Quaternion rotationEarth(1, 0, 0, 0);
    rotationEarth = rotationIncrement * rotationEarth;
    rotationEarth = rotationEarth.normalize();
    cubeMatrix.DrawSphere2(textures[SPHERE], centerX, centerY, centerZ, rotationEarth, orbitRotationEarth, radiusEarth, orbitRadiusEarth);

    static Quaternion rotationMars(1, 0, 0, 0);
    rotationMars = rotationIncrement * rotationMars;
    rotationMars = rotationMars.normalize();
    cubeMatrix.DrawSphere2(textures[MARS], centerX, centerY, centerZ, rotationMars, orbitRotationMars, radiusMars, orbitRadiusMars);

    static Quaternion rotationJupiter(1, 0, 0, 0);
    rotationJupiter = rotationIncrement * rotationJupiter;
    rotationJupiter = rotationJupiter.normalize();
    cubeMatrix.DrawSphere2(textures[JUPITER], centerX, centerY, centerZ, rotationJupiter, orbitRotationJupiter, radiusJupiter, orbitRadiusJupiter);

    static Quaternion rotationSaturn(1, 0, 0, 0);
    rotationSaturn = rotationIncrement * rotationSaturn;
    rotationSaturn = rotationSaturn.normalize();
    cubeMatrix.DrawSphere2(textures[SATURN], centerX, centerY, centerZ, rotationSaturn, orbitRotationSaturn, radiusSaturn, orbitRadiusSaturn);

    static Quaternion rotationUranus(1, 0, 0, 0);
    rotationUranus = rotationIncrement * rotationUranus;
    rotationUranus = rotationUranus.normalize();
    cubeMatrix.DrawSphere2(textures[URANUS], centerX, centerY, centerZ, rotationUranus, orbitRotationUranus, radiusUranus, orbitRadiusUranus);

    static Quaternion rotationNeptune(1, 0, 0, 0);
    rotationNeptune = rotationIncrement * rotationNeptune;
    rotationNeptune = rotationNeptune.normalize();
    cubeMatrix.DrawSphere2(textures[NEPTUNE], centerX, centerY, centerZ, rotationNeptune, orbitRotationNeptune, radiusNeptune, orbitRadiusNeptune);
}



/** GESTION FENETRE **/
void reshapeWindow(int w, int h)
{
    if (h == 0)
        h = 1;

    float ratio =  w * 1.0 / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 0.1f, 348.0f);
    glMatrixMode(GL_MODELVIEW);
}

/** FONCTIONS DE GESTION CLAVIER **/
void KeyboardDown(unsigned char key, int xx, int yy)
{
    switch(key)
    {
        case 'm' :
            swapRender = !swapRender ;
            break ;
        case 'e': // Unlock Camera
            cam->locked = (cam->locked)?0:1;
            break;
        case 'z':
            cam->deltaForward = 1;
            break;
        case 's':
            cam->deltaForward = -1;
            break;
        case 'd':
            cam->deltaStrafe = -1;
            break;
        case 'q':
            cam->deltaStrafe = 1;
            break;
        case 'p': // Test Quaternion operations
        {
            Quaternion q1(1, 2, 3, 4);
            Quaternion q2(2, 3, 4, 1);
            Quaternion sum = q1 + q2;
            Quaternion product = q1 * q2;
            Quaternion conjugated = q1.conjugate();
            std::cout << "Q1: " << q1 << std::endl;
            std::cout << "Q2: " << q2 << std::endl;
            std::cout << "Somme: " << sum << std::endl;
            std::cout << "Produit: " << product << std::endl;
            std::cout << "Conjugue de Q1: " << conjugated << std::endl;
            std::cout << "Norme Q1: " << q1.norm() << std::endl;
            std::cout << "Quaternion unitaire de Q1: " << q1.normalize() << std::endl;
            float dotProduct = q1.dot(q2);
            std::cout << "Produit scalaire de Q1 et Q2: " << dotProduct << std::endl;
            float matrix1[16];
            float matrix2[16];
            float resultMatrix[16];
            q1.to4x4Matrix2(matrix1);
            q2.to4x4Matrix2(matrix2);
            Quaternion::multiplyMatrices4x4(matrix1, matrix2, resultMatrix);

            std::cout << "Matrice 4x4 pour q1:" << std::endl;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    std::cout << std::setw(10) << matrix1[i * 4 + j] << " ";
                }
                std::cout << std::endl;
            }

            std::cout << "Matrice 4x4 pour q2:" << std::endl;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    std::cout << std::setw(10) << matrix2[i * 4 + j] << " ";
                }
                std::cout << std::endl;
            }

            std::cout << "Produit des matrices 4x4 de q1 et q2:" << std::endl;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    std::cout << std::setw(10) << resultMatrix[i * 4 + j] << " ";
                }
                std::cout << std::endl;
            }

            Quaternion q_from_matrix = Quaternion::from4x4Matrix(resultMatrix);
            std::cout << "Quaternion reconstruit a partir de la matrice resultat: " << q_from_matrix << std::endl;

            // Test de conversion quaternion <-> matrice de rotation 3x3
            float rotationMatrix[9];
            q1.toRotationMatrix3x3(rotationMatrix);
            std::cout << "Matrice de rotation 3x3 pour q1:" << std::endl;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    std::cout << std::setw(10) << rotationMatrix[i * 3 + j] << " ";
                }
                std::cout << std::endl;
            }

            Quaternion q_from_rotation_matrix = Quaternion::fromRotationMatrix3x3(rotationMatrix);
            std::cout << "Quaternion reconstruit a partir de la matrice de rotation: " << q_from_rotation_matrix << std::endl;

            break;
        }


    }
}
void KeyboardUp(unsigned char key, int xx, int yy)
{
    switch(key)
    {
        case 'z':
        case 's':
            cam->deltaForward = 0;
            break;
        case 'q':
        case 'd':
            cam->deltaStrafe = 0;
            break;
    }
}
void SpecialDown(int key, int xx, int yy)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            cam->deltaForward = 1;
            break;
        case GLUT_KEY_DOWN:
            cam->deltaForward = -1;
            break;
        case GLUT_KEY_RIGHT:
            cam->deltaStrafe = -1;
            break;
        case GLUT_KEY_LEFT:
            cam->deltaStrafe = 1;
            break;
    }
}
void SpecialUp(int key, int xx, int yy)
{
    switch(key)
    {
        case GLUT_KEY_UP:
        case GLUT_KEY_DOWN:
            cam->deltaForward = 0;
            break;
        case GLUT_KEY_RIGHT:
        case GLUT_KEY_LEFT:
            cam->deltaStrafe = 0;
            break;
    }
}

/** FONCTIONS DE GESTION SOURIS (ORIENTATION CAMERA) **/
void mouseMove(int x, int y)
{
    // Rentres uniquement lors du clic
    cam->orienterCam(x, y);
}
void mouseButton(int button, int state, int x, int y)
{
    // Gestion camera en fonction du clic souris
    if (button == GLUT_LEFT_BUTTON)
    {
        // Relacher la camera
        if (state == GLUT_UP)
        {
            cam->releaseCam();
        }
            // Mise à jour origine du clic
        else
        {
            cam->grabCam(x, y);
        }
    }
}

/** GESTION DEPLACEMENT CAMERA **/
void computePos(int inutile)
{
    cam->updatePos();
    glutTimerFunc(10, computePos, 0);
}

void updateRotations(int value)
{
    rotationAngle += 1.0f;
    if (rotationAngle >= 360.0f)
        rotationAngle -= 360.0f;

    Quaternion qIncrement(std::cos(0.5 * 0.0174533), 0.0f, std::sin(0.5 * 0.0174533), 0.0f); // 1 degree around y-axis
    rotationQuaternion2 = qIncrement * rotationQuaternion2;
    rotationQuaternion2 = rotationQuaternion2.normalize();

    glutTimerFunc(10, updateRotations, 0);
}




/** AFFICHAGE **/
void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(cam->posx, cam->posy, cam->posz,
              cam->posx + cam->dirx, cam->posy + cam->diry, cam->posz + cam->dirz,
              0.0f, 1.0f, 0.0f);



    if(swapRender == 1) {
        m->DrawGround();
        m->DrawSkybox(cam);
        // Draw cubes
        drawCubes();
        cubeMatrix.DrawSphere(textures[SPHERE], 10);

    } else {
        drawSolarSys();
        m->DrawSolarSystemSkybox(cam);
    }

    glutSwapBuffers();
    glutPostRedisplay(); // Ensure continuous rendering
}




void LoadTextures()
{
    m->LoadTextures();

    // Charger les textures
    textures[SPHERE] = SOIL_load_OGL_texture(
            "img/terre.jpg",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    if (textures[SPHERE] == 0)
    {
        printf("SOIL loading error for SPHERE: '%s'\n", SOIL_last_result());
    }

    textures[SUN] = SOIL_load_OGL_texture(
            "img/sun.jpg",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    if (textures[SUN] == 0)
    {
        printf("SOIL loading error for SUN: '%s'\n", SOIL_last_result());
    }

    textures[MARS] = SOIL_load_OGL_texture(
            "img/mars.jpg",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    if (textures[MARS] == 0)
    {
        printf("SOIL loading error for MARS: '%s'\n", SOIL_last_result());
    }



    textures[JUPITER] = SOIL_load_OGL_texture(
            "img/jupiter.jpg",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    if (textures[JUPITER] == 0)
    {
        printf("SOIL loading error for JUPITER: '%s'\n", SOIL_last_result());
    }

    textures[MERCURY] = SOIL_load_OGL_texture(
            "img/mercury.jpg",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    if (textures[MERCURY] == 0)
    {
        printf("SOIL loading error for MERCURY: '%s'\n", SOIL_last_result());
    }

    textures[SATURN] = SOIL_load_OGL_texture(
            "img/saturn.jpg",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    if (textures[SATURN] == 0)
    {
        printf("SOIL loading error for SATURN: '%s'\n", SOIL_last_result());
    }

    textures[URANUS] = SOIL_load_OGL_texture(
            "img/uranus.jpg",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    if (textures[URANUS] == 0)
    {
        printf("SOIL loading error for URANUS: '%s'\n", SOIL_last_result());
    }

    textures[VENUS] = SOIL_load_OGL_texture(
            "img/venus.png",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    if (textures[VENUS] == 0)
    {
        printf("SOIL loading error for VENUS: '%s'\n", SOIL_last_result());
    }

    textures[NEPTUNE] = SOIL_load_OGL_texture(
            "img/neptune.jpg",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    if (textures[NEPTUNE] == 0)
    {
        printf("SOIL loading error for NEPTUNE: '%s'\n", SOIL_last_result());
    }

    // Charger les textures pour les cubes
    textures[0] = SOIL_load_OGL_texture(
            "img/graybricktiles.bmp",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    if (textures[0] == 0)
    {
        printf("SOIL loading error for graybricktiles: '%s'\n", SOIL_last_result());
    }

    // Assigner la même texture pour chaque face du cube (vous pouvez charger d'autres textures pour chaque face si vous le souhaitez)
    for (int i = 0; i < 6; i++)
    {
        cubeMatrix.SetTexture(i, textures[0]);
        cubeQuaternion.SetTexture(i, textures[0]);
        cubeGLRotate.SetTexture(i, textures[0]);
    }
}







int main(int argc, char **argv) {
    /** CREATION FENETRE **/
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(320, 320);
    glutCreateWindow("Quaternions");

    /** FONCTIONS GLUT **/
    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshapeWindow);
    glutIdleFunc(renderScene);
    glutTimerFunc(10, computePos, 0);
    glutTimerFunc(10, updateRotations, 0); // Appel à updateRotations

    /** GESTION CLAVIER **/
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(KeyboardDown);
    glutKeyboardUpFunc(KeyboardUp);
    glutSpecialFunc(SpecialDown);
    glutSpecialUpFunc(SpecialUp);

    /** GESTION SOURIS **/
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);

    /** INIT TEXTURES **/
    LoadTextures();

    /** INIT GL STATES **/
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glutMainLoop();

    return 1;
}




