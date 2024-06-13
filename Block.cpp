#include "include/Block.h"

Block::Block()
{
    posx = 0.0f;
    posy = 0.0f;
    posz = 0.0f;

    x = 1.0f;
    y = 1.0f;
    z = 1.0f;

    for (int i = 0; i < 6; i++)
    {
        textures[i] = 0;
    }
    for (int j = 0; j < 3; j ++)
    {
        anchor[j] = 0;
    }
}

Block::Block(float x, float y, float z)
{
    posx = 0.0f;
    posy = 0.0f;
    posz = 0.0f;

    this->x = x;
    this->y = y;
    this->z = z;

    for (int i = 0; i < 6; i++)
    {
        textures[i] = 0;
    }
    for (int j = 0; j < 3; j ++)
    {
        anchor[j] = 0;
    }
}

void Block::Draw()
{
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();

    if (anchor[0]) glTranslatef(-x/2, 0.0f, 0.0f);
    if (anchor[1]) glTranslatef(0.0f, -y/2, 0.0f);
    if (anchor[2]) glTranslatef(0.0f, 0.0f, -z/2);

    /** FRONT **/
    glBindTexture(GL_TEXTURE_2D, textures[FRONT]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, y / TEXTURE_SCALE);
    glVertex3f(0.0f, y, 0.0f);
    glTexCoord2f(x / TEXTURE_SCALE, y / TEXTURE_SCALE);
    glVertex3f(x, y, 0.0f);
    glTexCoord2f(x / TEXTURE_SCALE, 0.0f);
    glVertex3f(x, 0.0f, 0.0f);
    glEnd();

    /** BACK **/
    glBindTexture(GL_TEXTURE_2D, textures[BACK]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, z);
    glTexCoord2f(0.0f, y / TEXTURE_SCALE);
    glVertex3f(0.0f, y, z);
    glTexCoord2f(x / TEXTURE_SCALE, y / TEXTURE_SCALE);
    glVertex3f(x, y, z);
    glTexCoord2f(x / TEXTURE_SCALE, 0.0f);
    glVertex3f(x, 0.0f, z);
    glEnd();

    /** TOP **/
    glBindTexture(GL_TEXTURE_2D, textures[TOP]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, y, 0.0f);
    glTexCoord2f(0.0f, y / TEXTURE_SCALE);
    glVertex3f(0.0f, y, z);
    glTexCoord2f(x / TEXTURE_SCALE, y / TEXTURE_SCALE);
    glVertex3f(x, y, z);
    glTexCoord2f(x / TEXTURE_SCALE, 0.0f);
    glVertex3f(x, y, 0.0f);
    glEnd();

    /** BOT **/
    glBindTexture(GL_TEXTURE_2D, textures[BOT]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, z / TEXTURE_SCALE);
    glVertex3f(0.0f, 0.0f, z);
    glTexCoord2f(x / TEXTURE_SCALE, z / TEXTURE_SCALE);
    glVertex3f(x, 0.0f, z);
    glTexCoord2f(x / TEXTURE_SCALE, 0.0f);
    glVertex3f(x, 0.0f, 0.0f);
    glEnd();

    /** LEFT **/
    glBindTexture(GL_TEXTURE_2D, textures[LEFT]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, z / TEXTURE_SCALE);
    glVertex3f(0.0f, 0.0f, z);
    glTexCoord2f(y / TEXTURE_SCALE, z / TEXTURE_SCALE);
    glVertex3f(0.0f, y, z);
    glTexCoord2f(y / TEXTURE_SCALE, 0.0f);
    glVertex3f(0.0f, y, 0.0f);
    glEnd();

    /** RIGHT **/
    glBindTexture(GL_TEXTURE_2D, textures[RIGHT]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x, 0.0f, 0.0f);
    glTexCoord2f(0.0f, z / TEXTURE_SCALE);
    glVertex3f(x, 0.0f, z);
    glTexCoord2f(y / TEXTURE_SCALE, z / TEXTURE_SCALE);
    glVertex3f(x, y, z);
    glTexCoord2f(y / TEXTURE_SCALE, 0.0f);
    glVertex3f(x, y, 0.0f);
    glEnd();

    glPopMatrix();
}

void Block::DrawSphere(GLuint texture, float radius)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    GLUquadric* params = gluNewQuadric();
    gluQuadricTexture(params, GL_TRUE);
    glPushMatrix();
    glTranslatef(10, 10, 10); // Positionner la sph�re

    // Ajoutez une rotation initiale ici
    glRotatef(-90, 1.0f, 0.0f, 0.0f); // Ajustez l'angle et l'axe selon vos besoins

    gluSphere(params, radius, 100, 100); // Dessiner la sph�re
    glPopMatrix();
    gluDeleteQuadric(params);
}


void Block::SetTexture(int face, GLuint texture)
{
    textures[face] = texture;
}





void Block::DrawSphere2(GLuint texture, float posX, float posY, float posZ, const Quaternion& rotation, const Quaternion& orbitRotation, float radius, float orbitRadius) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    GLUquadric* params = gluNewQuadric();
    gluQuadricTexture(params, GL_TRUE);

    glPushMatrix();

    // Translate to the orbit center position
    glTranslatef(posX, posY, posZ);

    // Apply orbit rotation
    float orbitCenter[3] = {orbitRadius, 0.0f, 0.0f};
    orbitRotation.rotatePoint(orbitCenter);

    // Translate by the rotated orbit radius to position the sphere in orbit
    glTranslatef(orbitCenter[0], orbitCenter[1], orbitCenter[2]);

    // Apply the initial rotation to the sphere
    float sphereCenter[3] = {0.0f, 0.0f, 0.0f};
    rotation.rotatePoint(sphereCenter);

    glTranslatef(sphereCenter[0], sphereCenter[1], sphereCenter[2]);

    // Draw the sphere
    gluSphere(params, radius, 100, 100);

    glPopMatrix();

    gluDeleteQuadric(params);
}






