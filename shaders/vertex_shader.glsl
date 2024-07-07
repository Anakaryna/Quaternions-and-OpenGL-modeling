#version 120

attribute vec3 aPosition;
attribute vec2 aTexCoord;
uniform mat4 uModelViewProjectionMatrix;

varying vec2 vTexCoord;

void main()
{
    gl_Position = uModelViewProjectionMatrix * vec4(aPosition, 1.0);
    vTexCoord = aTexCoord;
}
