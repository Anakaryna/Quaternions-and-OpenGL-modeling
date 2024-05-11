#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <cstdio>
#include <cmath>

#include "include/Camera.h"
#include "include/Map.h"
#include "include/Quaternion.h"


// Objet Camera
Camera *cam = new Camera();
// Objet Scène
Map *m = new Map();



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
            std::cout << "Conjugué de Q1: " << conjugated << std::endl;
            std::cout << "Norme Q1: " << q1.norm() << std::endl;
            std::cout << "Quaternion unitaire de Q1: " << q1.normalize() << std::endl;
            float dotProduct = q1.dot(q2);
            std::cout << "Produit scalaire de Q1 et Q2: " << dotProduct << std::endl;
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

/** AFFICHAGE **/
void renderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // Définition de la caméra
    gluLookAt(  cam->posx, cam->posy, cam->posz,
                cam->posx+cam->dirx, cam->posy+cam->diry,  cam->posz+cam->dirz,
                0.0f, 1.0f,  0.0f
    );

    m->DrawGround();
    m->DrawSkybox(cam);
    glutSwapBuffers();
}

void LoadTextures()
{
    m->LoadTextures();
}

int main(int argc, char **argv)
{
    /** CREATION FENETRE **/
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(320,320);
    glutCreateWindow("Quaternions");

    /** FONCTIONS GLUT **/
    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshapeWindow);
    glutIdleFunc(renderScene);
    glutTimerFunc(10, computePos, 0);

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


