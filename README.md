# Quaternions-and-OpenGL-modeling

## Description
This project demonstrates the implementation and use of quaternions in 3D graphics using OpenGL. It covers various quaternion operations, their application in 3D transformations, and comparison with traditional matrix transformations. Additionally, it includes a quaternion camera system and a solar system simulation with textured objects.
Done with OpenGl, GLUT and SOIL

## Implemented Features
1. **Quaternion Class Implementation**:
   - Addition
   - Multiplication
   - Conjugation
   - Norm
   - Unit quaternion
   - Dot product
   - Cross product
   - Matrix product

2. **Quaternion-Matrix Transformations**:
   - Convert a quaternion \( q = a + bi + cj + dk \) to a matrix.
   - Inverse transformation from matrix to quaternion.
   - Verify compatibility of quaternion operations with matrix operations.

3. **Rotation Transformations**:
   - Convert rotation matrices to quaternions and vice versa.
   - Implement rotations using both matrix and quaternion formulas.
   - Perform rotations similarly to `glRotatef`.

4. **Advanced Rotations and Transformations**:
   - Implement complex rotations using quaternions.
   - Apply shear, scale, and translate transformations.

5. **Camera Systems**:
   - Adapt camera rotations using quaternions.

6. **Textures and Materials**:
   - Use a skybox to enhance the visual appeal of the scene.
   - Use a variety of textures.

7. **Custom Animated Scene**:
   - Create a personalized animated scene using quaternion rotation.
   - Example: A solar system with planets orbiting a sun, where each planet is textured.

## Controls
### Keyboard
- **m**: Toggle between rendering modes.
- **z**: Move forward.
- **s**: Move backward.
- **q**: Strafe left.
- **d**: Strafe right.
- **p**: Test quaternion operations (prints results to the console).

### Special Keys
- **UP Arrow**: Move forward.
- **DOWN Arrow**: Move backward.
- **RIGHT Arrow**: Strafe right.
- **LEFT Arrow**: Strafe left.

### Mouse
- **Left Click**: Control camera orientation.
- **Mouse Movement**: Rotate camera.

## Video showcasing the project 

Cube scene : 

![ezgif-3-a62426fbbb](https://github.com/Anakaryna/Quaternions-and-OpenGL-modeling/assets/78374876/d52efdcc-738a-4cb5-bcaa-08c67090457b)

Solar system scene :

https://github.com/Anakaryna/Quaternions-and-OpenGL-modeling/assets/78374876/fed1674d-1e2f-46f7-afe5-2155d657d11d

