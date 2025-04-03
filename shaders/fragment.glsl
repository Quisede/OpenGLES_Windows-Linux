#version 100
precision mediump float;
uniform vec3 objColor;

void main() {
    gl_FragColor = vec4(objColor, 1.0);
}
