#include <GL/glew.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <cstdio>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdint>
#include <filesystem>
#include "include/Camera.h"
#include "include/Map.h"
#include "include/Quaternion.h"
#include "include/Block.h"
#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include "include/tinyobj_loader_c.h"
#include "include/matrices.h"


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
int swapScenes = 0 ;

struct Vertex {
    float position[3];
    float normal[3];
    float texcoords[2];
};

struct Mesh {
    Vertex* vertices;
    uint32_t vertexCount;
};

Mesh suzanneMesh;
bool isMeshLoaded = false;


class attrib_t;

// Code for OBJ laoder and Shader Application
void generateProjectionMatrix(float* matrix, float fov, float aspect, float zNear, float zFar) {
    float f = 1.0f / tan(fov / 2.0f);
    matrix[0] = f / aspect;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;

    matrix[4] = 0.0f;
    matrix[5] = f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;

    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = (zFar + zNear) / (zNear - zFar);
    matrix[11] = -1.0f;

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = (2.0f * zFar * zNear) / (zNear - zFar);
    matrix[15] = 0.0f;
}
void generateTranslationMatrix(float* matrix, float x, float y, float z) {
    matrix[0] = 1.0f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;

    matrix[4] = 0.0f;
    matrix[5] = 1.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;

    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;

    matrix[12] = x;
    matrix[13] = y;
    matrix[14] = z;
    matrix[15] = 1.0f;
}
void generateRotationMatrix(float* matrix, float angle, float x, float y, float z) {
    float c = cos(angle);
    float s = sin(angle);
    float omc = 1.0f - c;

    matrix[0] = x * x * omc + c;
    matrix[1] = y * x * omc + z * s;
    matrix[2] = x * z * omc - y * s;
    matrix[3] = 0.0f;

    matrix[4] = x * y * omc - z * s;
    matrix[5] = y * y * omc + c;
    matrix[6] = y * z * omc + x * s;
    matrix[7] = 0.0f;

    matrix[8] = x * z * omc + y * s;
    matrix[9] = y * z * omc - x * s;
    matrix[10] = z * z * omc + c;
    matrix[11] = 0.0f;

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
}

#include <fstream>

static void fileReaderCallback(void *ctx, const char *filename, int is_mtl, const char *obj_filename, char **data, size_t *len) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "File open failed in callback: " << filename << std::endl;
        *data = NULL;
        *len = 0;
        return;
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[size];
    if (file.read(buffer, size)) {
        *data = buffer;
        *len = size;
    } else {
        std::cerr << "Error reading file: " << filename << std::endl;
        delete[] buffer;
        *data = NULL;
        *len = 0;
    }
}




Mesh loadObj(const char* filename) {
    tinyobj_attrib_t attrib;
    tinyobj_shape_t* shapes = NULL;
    size_t num_shapes;
    tinyobj_material_t* materials = NULL;
    size_t num_materials;

    unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;

    int result = tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials, &num_materials, filename, fileReaderCallback, NULL, flags);


    if (result != TINYOBJ_SUCCESS) {
        std::cerr << "Failed to load/parse .obj file: " << filename << std::endl;
        exit(1);
    } else {
        std::cout << "Successfully loaded .obj file: " << filename << std::endl;
    }

    std::vector<Vertex> vertices;

    for (size_t i = 0; i < attrib.num_faces; i++) {
        tinyobj_vertex_index_t idx = attrib.faces[i];

        Vertex vertex;
        vertex.position[0] = attrib.vertices[3 * idx.v_idx + 0];
        vertex.position[1] = attrib.vertices[3 * idx.v_idx + 1];
        vertex.position[2] = attrib.vertices[3 * idx.v_idx + 2];

        if (idx.vn_idx >= 0) {
            vertex.normal[0] = attrib.normals[3 * idx.vn_idx + 0];
            vertex.normal[1] = attrib.normals[3 * idx.vn_idx + 1];
            vertex.normal[2] = attrib.normals[3 * idx.vn_idx + 2];
        }

        if (idx.vt_idx >= 0) {
            vertex.texcoords[0] = attrib.texcoords[2 * idx.vt_idx + 0];
            vertex.texcoords[1] = attrib.texcoords[2 * idx.vt_idx + 1];
        }

        vertices.push_back(vertex);
    }

    Mesh mesh;
    mesh.vertexCount = vertices.size();
    mesh.vertices = new Vertex[mesh.vertexCount];
    memcpy(mesh.vertices, vertices.data(), mesh.vertexCount * sizeof(Vertex));

    tinyobj_attrib_free(&attrib);
    tinyobj_shapes_free(shapes, num_shapes);
    tinyobj_materials_free(materials, num_materials);

    return mesh;
}











// Fonction de rendu du mesh
void renderMesh(const Mesh& mesh) {
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertexCount * sizeof(Vertex), mesh.vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoords));
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}



void drawText(const char* text, float x, float y, float z) {
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


void drawSolarSysMatrix() {
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

    // Orbit rotations as quaternions
    Quaternion orbitRotationMercury(cos(angleMercury / 2), 0.0f, sin(angleMercury / 2), 0.0f);
    Quaternion orbitRotationVenus(cos(angleVenus / 2), 0.0f, sin(angleVenus / 2), 0.0f);
    Quaternion orbitRotationEarth(cos(angleEarth / 2), 0.0f, sin(angleEarth / 2), 0.0f);
    Quaternion orbitRotationMars(cos(angleMars / 2), 0.0f, sin(angleMars / 2), 0.0f);
    Quaternion orbitRotationJupiter(cos(angleJupiter / 2), 0.0f, sin(angleJupiter / 2), 0.0f);
    Quaternion orbitRotationSaturn(cos(angleSaturn / 2), 0.0f, sin(angleSaturn / 2), 0.0f);
    Quaternion orbitRotationUranus(cos(angleUranus / 2), 0.0f, sin(angleUranus / 2), 0.0f);
    Quaternion orbitRotationNeptune(cos(angleNeptune / 2), 0.0f, sin(angleNeptune / 2), 0.0f);

    // Convert quaternion rotations to 4x4 matrices
    float matrixMercury[16];
    float matrixVenus[16];
    float matrixEarth[16];
    float matrixMars[16];
    float matrixJupiter[16];
    float matrixSaturn[16];
    float matrixUranus[16];
    float matrixNeptune[16];

    orbitRotationMercury.to4x4Matrix(matrixMercury);
    orbitRotationVenus.to4x4Matrix(matrixVenus);
    orbitRotationEarth.to4x4Matrix(matrixEarth);
    orbitRotationMars.to4x4Matrix(matrixMars);
    orbitRotationJupiter.to4x4Matrix(matrixJupiter);
    orbitRotationSaturn.to4x4Matrix(matrixSaturn);
    orbitRotationUranus.to4x4Matrix(matrixUranus);
    orbitRotationNeptune.to4x4Matrix(matrixNeptune);

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

    // Draw the Sun
    cubeMatrix.DrawSphere2(textures[SUN], centerX, centerY, centerZ, Quaternion(1, 0, 0, 0), Quaternion(1, 0, 0, 0), radiusSun, 0.0f);

    // Draw the planets with the updated positions and rotations using matrix multiplication
    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);
    glMultMatrixf(matrixMercury);
    cubeMatrix.DrawSphere2(textures[MERCURY], 0.0f, 0.0f, orbitRadiusMercury, Quaternion(1, 0, 0, 0), Quaternion(1, 0, 0, 0), radiusMercury, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);
    glMultMatrixf(matrixVenus);
    cubeMatrix.DrawSphere2(textures[VENUS], 0.0f, 0.0f, orbitRadiusVenus, Quaternion(1, 0, 0, 0), Quaternion(1, 0, 0, 0), radiusVenus, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);
    glMultMatrixf(matrixEarth);
    cubeMatrix.DrawSphere2(textures[SPHERE], 0.0f, 0.0f, orbitRadiusEarth, Quaternion(1, 0, 0, 0), Quaternion(1, 0, 0, 0), radiusEarth, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);
    glMultMatrixf(matrixMars);
    cubeMatrix.DrawSphere2(textures[MARS], 0.0f, 0.0f, orbitRadiusMars, Quaternion(1, 0, 0, 0), Quaternion(1, 0, 0, 0), radiusMars, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);
    glMultMatrixf(matrixJupiter);
    cubeMatrix.DrawSphere2(textures[JUPITER], 0.0f, 0.0f, orbitRadiusJupiter, Quaternion(1, 0, 0, 0), Quaternion(1, 0, 0, 0), radiusJupiter, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);
    glMultMatrixf(matrixSaturn);
    cubeMatrix.DrawSphere2(textures[SATURN], 0.0f, 0.0f, orbitRadiusSaturn, Quaternion(1, 0, 0, 0), Quaternion(1, 0, 0, 0), radiusSaturn, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);
    glMultMatrixf(matrixUranus);
    cubeMatrix.DrawSphere2(textures[URANUS], 0.0f, 0.0f, orbitRadiusUranus, Quaternion(1, 0, 0, 0), Quaternion(1, 0, 0, 0), radiusUranus, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);
    glMultMatrixf(matrixNeptune);
    cubeMatrix.DrawSphere2(textures[NEPTUNE], 0.0f, 0.0f, orbitRadiusNeptune, Quaternion(1, 0, 0, 0), Quaternion(1, 0, 0, 0), radiusNeptune, 0.0f);
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
        case '9' :
            swapScenes = (swapScenes + 1) % 3 ;
            break;
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



GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    } else {
        printf("Impossible to open %s. Are you in the right directory?\n", vertex_file_path);
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(cam->posx, cam->posy, cam->posz,
              cam->posx + cam->dirx, cam->posy + cam->diry, cam->posz + cam->dirz,
              0.0f, 1.0f, 0.0f);

    if (swapScenes == 0) {
        m->DrawGround();
        m->DrawSkybox(cam);
        // Draw cubes
        drawCubes();
        cubeMatrix.DrawSphere(textures[SPHERE], 10);
    } else if (swapScenes == 1) {
        drawSolarSys();
        m->DrawSolarSystemSkybox(cam);
    } else if (swapScenes == 2) {
        if (!isMeshLoaded) {
            suzanneMesh = loadObj("suzanne.obj");
            isMeshLoaded = true;
        }

        float projectionMatrix[16];
        generateProjectionMatrix(projectionMatrix, 45.0f * M_PI / 180.0f, 1.0f, 0.1f, 100.0f);

        float modelMatrix[16];
        generateTranslationMatrix(modelMatrix, 0.0f, 0.0f, -5.0f);

        GLuint program = LoadShaders("C:/Users/Anamaria/Documents/ESGI/3rdYear/Semestre 2/Maths/Projet/Quaternions-and-OpenGL-modeling/shaders/vertex_shader.glsl",
                                     "C:/Users/Anamaria/Documents/ESGI/3rdYear/Semestre 2/Maths/Projet/Quaternions-and-OpenGL-modeling/shaders/fragment_shader.glsl");

        glUseProgram(program);

        GLuint projLoc = glGetUniformLocation(program, "uProjection");
        GLuint modelLoc = glGetUniformLocation(program, "uModel");

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projectionMatrix);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);

        renderMesh(suzanneMesh);

        glUseProgram(0);
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
    std::cout << "Current path is " << std::filesystem::current_path() << '\n';
    /** CREATION FENETRE **/
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(320, 320);
    glutCreateWindow("Quaternions");

    /** INIT GLEW **/
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return -1;
    }

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





