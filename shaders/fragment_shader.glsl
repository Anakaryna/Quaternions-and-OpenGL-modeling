#version 120

varying vec2 vTexCoord;
uniform sampler2D uTexture;

void main()
{
    gl_FragColor = texture2D(uTexture, vTexCoord);
}
