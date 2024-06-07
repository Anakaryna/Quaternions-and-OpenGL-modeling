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


void drawCubes()
{
    // Cube avec rotation par matrice
    // Create a 3x3 rotation matrix from the quaternion
    float rotationMatrix[9];
    rotationQuaternion.toRotationMatrix3x3(rotationMatrix);

    glPushMatrix();
    glTranslatef(-4.0f, 0.0f, -5.0f); // Translation pour décentrer
    glTranslatef(1.0f, 0.0f, 0.0f); // Mise à l'origine

    // Apply the 3x3 rotation matrix
    float matrix4x4[16] = {
            rotationMatrix[0], rotationMatrix[1], rotationMatrix[2], 0.0f,
            rotationMatrix[3], rotationMatrix[4], rotationMatrix[5], 0.0f,
            rotationMatrix[6], rotationMatrix[7], rotationMatrix[8], 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
    glMultMatrixf(matrix4x4);

    glTranslatef(-1.0f, 0.0f, 0.0f); // Repositionnement
    cubeMatrix.Draw();
    glPopMatrix();

    // Cube avec rotation par quaternion
    // Cube avec rotation par quaternion
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f); // Translation pour décentrer

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

    // Apply quaternion rotation to each vertex
    for (int i = 0; i < 8; i++) {
        rotationQuaternion.rotatePointWithQuaternion(vertices[i], rotationQuaternion);
    }

    glBegin(GL_QUADS);

    // Front face
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertices[0]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertices[1]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertices[2]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertices[3]);

    // Back face
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertices[4]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertices[5]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertices[6]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertices[7]);

    // Top face
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertices[3]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertices[2]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertices[6]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertices[7]);

    // Bottom face
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertices[0]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertices[1]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertices[5]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertices[4]);

    // Right face
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertices[1]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertices[5]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertices[6]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertices[2]);

    // Left face
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertices[0]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertices[4]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertices[7]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertices[3]);

    glEnd();
    glPopMatrix();

    // Cube avec rotation par glRotatef
    glPushMatrix();
    glTranslatef(4.0f, 0.0f, -5.0f); // Translation pour décentrer
    glTranslatef(1.0f, 0.0f, 0.0f); // Mise à l'origine
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f); // Rotation
    glTranslatef(-1.0f, 0.0f, 0.0f); // Repositionnement
    cubeGLRotate.Draw();
    glPopMatrix();

    // Cube avec rotation par glRotatef
    glPushMatrix();
    glTranslatef(4.0f, 0.0f, -5.0f); // Translation pour décentrer
    glTranslatef(1.0f, 0.0f, 0.0f); // Mise à l'origine
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f); // Rotation
    glTranslatef(-1.0f, 0.0f, 0.0f); // Repositionnement
    cubeGLRotate.Draw();
    glPopMatrix();
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

    // Dessiner les cubes
    drawCubes();

    // Dessiner la sphère planète
    cubeMatrix.DrawSphere(textures[SPHERE]);

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




