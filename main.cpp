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
    rotationQuaternion.toRotationMatrix3x3(rotationMatrix);

    glPushMatrix();
    glTranslatef(-4.0f, 0.0f, -5.0f); // Position du Cube
    //glTranslatef(0.5f, 0.5f, 0.5f); // Translation du centre du cube à l'origine
    glTranslatef(1.0f, 0.0f, 0.0f); // Translation du centre du cube decentré
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
        rotationQuaternion.rotatePointWithQuaternion(vertices[i], rotationQuaternion);
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
    //zglTranslatef(-0.5f, -0.5f, -0.5f);
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
        rotationQuaternion.shearAndScalePointandTranslate(shearScaleTranslateVertices[i], 0.5f, 0.0f, 0.0f, 1.5f, 1.5f, 1.5f, 0.0f, 3.0f, 0.0f);
        rotationQuaternion.rotatePointWithQuaternion(shearScaleTranslateVertices[i], rotationQuaternion);

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


// Function to create a rotation matrix around the Y-axis

void createOrbitMatrix(float angle, float matrix[16])
{
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);

    matrix[0] = cosAngle; matrix[1] = 0.0f; matrix[2] = sinAngle; matrix[3] = 0.0f;
    matrix[4] = 0.0f;     matrix[5] = 1.0f; matrix[6] = 0.0f;     matrix[7] = 0.0f;
    matrix[8] = -sinAngle; matrix[9] = 0.0f; matrix[10] = cosAngle; matrix[11] = 0.0f;
    matrix[12] = 0.0f;     matrix[13] = 0.0f; matrix[14] = 0.0f;     matrix[15] = 1.0f;
}


void drawSolarSys() {
    float time = 150.0f;

    // Cube avec rotation par matrice
    float rotationMatrix[9];
    rotationQuaternion.toRotationMatrix3x3(rotationMatrix);

    float matrix4x4[16] = {
            rotationMatrix[0], rotationMatrix[1], rotationMatrix[2], 0.0f,
            rotationMatrix[3], rotationMatrix[4], rotationMatrix[5], 0.0f,
            rotationMatrix[6], rotationMatrix[7], rotationMatrix[8], 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };

    // Define positions and speeds for each sphere
    float posX = 0.0f; // Example X position
    float posY = 0.0f; // Example Y position
    float posZ = 0.0f; // Example Z position
    float speed1 = 1.0f; // Speed for first sphere
    float angle1 = time * speed1;
    float orbitMatrix1[16];
    createOrbitMatrix(angle1, orbitMatrix1);
    cubeMatrix.DrawSphere2(textures[SPHERE], posX, posY, posZ, matrix4x4, orbitMatrix1, 5.0f, 10.0f);

    float posX1 = 7.0f; // Example X position
    float posY1 = 0.0f; // Example Y position
    float posZ1 = 0.0f; // Example Z position
    float speed2 = 20.0f; // Speed for second sphere
    float angle2 = time * speed2;
    float orbitMatrix2[16];
    createOrbitMatrix(angle2, orbitMatrix2);
    cubeMatrix.DrawSphere2(textures[SPHERE], posX1, posY1, posZ1, matrix4x4, orbitMatrix2, 1.0f, 10.0f);

    float posX2 = 15.0f; // Example X position
    float posY2 = 0.0f; // Example Y position
    float posZ2 = 0.0f; // Example Z position
    float speed3 = 1.5f; // Speed for third sphere
    float angle3 = time * speed3;
    float orbitMatrix3[16];
    createOrbitMatrix(angle3, orbitMatrix3);
    cubeMatrix.DrawSphere2(textures[SPHERE], posX2, posY2, posZ2, matrix4x4, orbitMatrix3, 7.0f, 10.0f);

    float posX3 = 30.0f; // Example X position
    float posY3 = 0.0f; // Example Y position
    float posZ3 = 0.0f; // Example Z position
    float speed4 = 0.5f; // Speed for fourth sphere
    float angle4 = time * speed4;
    float orbitMatrix4[16];
    createOrbitMatrix(angle4, orbitMatrix4);
    cubeMatrix.DrawSphere2(textures[SPHERE], posX3, posY3, posZ3, matrix4x4, orbitMatrix4, 3.0f, 10.0f);
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
            q1.to4x4Matrix(matrix1);
            q2.to4x4Matrix(matrix2);
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
    rotationQuaternion = qIncrement * rotationQuaternion;
    rotationQuaternion = rotationQuaternion.normalize();

    glutTimerFunc(10, updateRotations, 0);
}



/** AFFICHAGE **/
void renderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(cam->posx, cam->posy, cam->posz,
              cam->posx + cam->dirx, cam->posy + cam->diry, cam->posz + cam->dirz,
              0.0f, 1.0f, 0.0f);

    m->DrawGround();
    m->DrawSkybox(cam);

    if(swapRender ==1) {
        // Dessiner les cubes
        drawCubes();
        cubeMatrix.DrawSphere(textures[SPHERE],10);
    }
    else {
        drawSolarSys() ;
    }
    // Dessiner la sphère planète


    glutSwapBuffers();
}




void LoadTextures()
{
    m->LoadTextures();

    // Charge la texture de la sphère
    textures[SPHERE] = SOIL_load_OGL_texture(
            "img/terre.jpg",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if (textures[SPHERE] == 0)
    {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
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
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
    }

    // Assigner la même texture pour chaque face du cube (vous pouvez charger d'autres textures pour chaque face si vous le souhaitez)
    for (int i = 0; i < 6; i++)
    {
        cubeMatrix.SetTexture(i, textures[0]);
        cubeQuaternion.SetTexture(i, textures[0]);
        cubeGLRotate.SetTexture(i, textures[0]);
    }
}





int main(int argc, char **argv)
{
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

    return (1);
}




