#version 100
attribute vec3 position;

uniform float pointSize;  //для управления размером точки

void main() {
    gl_Position = vec4(position, 1.0);
    gl_PointSize = pointSize;
}