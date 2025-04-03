/*#define GL_SILENCE_DEPRECATION //�� ���������� �������������� �� ���������� �������� OpenGL
#define GLFW_INCLUDE_NONE //�� �������� ������������ ����� �������������*/
#define GLFW_INCLUDE_ES2 //OPENGL ES 2.0


#include <iostream>

#include <GLFW/glfw3.h>
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

/*/#include <GL/glew.h> //����������� glew
#include <GLFW/glfw3.h> //����������� GLFW
#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#endif //��� MacOS
#include <glm/glm.hpp> //����������� glm
*/
#include <glm/glm.hpp>

#include "shaders_loader.h" //������������ ���� � �������� ��������� ��������

enum Primitives { SQUARE, TRIANGLE, POINTS, LINES }; //������������ � ����� ���������
Primitives currentPrimitive = TRIANGLE;//��������� �������� - �����������
bool showMenu = false;
glm::vec3 objColor(0.6f, 0.0f, 1.0f);//��������� ���������� ����
float pointSize = 10.0f;  //��������� ������ �����

//��������� �������
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);//���������� ������
void Menu();//����� ����������� ����

int main() {
    std::cout << "Starting GLFW context, OpenGL" << std::endl;
    // �������������� GLFW
    //�������� �� ������ ��� ������������ ���������� GLFW
    if (!glfwInit()) {
        std::cerr << "Failed with initialization GLFW\n"; //����� ������
        return -1;
    }

    //������� ������ OpenGL
    glfwWindowHint(GLFW_SAMPLES, 4); //����������� 4x MSAA
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0); 
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API); 

    GLFWwindow* window = glfwCreateWindow(800, 600, "Open GL ES Emulator", NULL, NULL); //��������� �� ��������� GLFWwindow(����)

    //�������� ���������� �������� ����
    if (!window) {
        std::cerr << "Failed with create window\n";
        glfwTerminate();//��������� ������ GLFW � ������ �������
        return -1;
    }
    
    glfwMakeContextCurrent(window); //������ OpenGl �������� ���������� ���� �������� (�������) ��� ����������� ������
    glfwSetKeyCallback(window, key_callback);//������������ ������� ��������� ������ � ����

    // ��������� ������ OpenGL ES
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "OpenGL ES version: " << version << std::endl;

    //��������� �����������
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    //��������� ������� �� �����
    GLuint shaderProgram = createShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");
    if (!shaderProgram) {
        std::cerr << "Failed to create shader program\n";
        glfwTerminate();
        return -1;
    }

   

    GLfloat vertices[] = {
        //����� (0)
        0.0f, 0.0f, 0.0f,

        //������� (1-2)
        -0.8f, 0.0f, 0.0f,
        0.8f, 0.0f, 0.0f,

        //����������� (3-5)
        -0.5f, -0.5f, 0.0f,  //���� ���
        0.5f, -0.5f, 0.0f,   //����� ���
        0.0f, 0.5f, 0.0f,    //����

        //������� (6-9)
        0.5f,  0.5f, 0.0f,   //���� �����
        0.5f, -0.5f, 0.0f,   //��� �����
        -0.5f,-0.5f, 0.0f,   //��� ����
        -0.5f, 0.5f, 0.0f    //���� ����
    };

    //��� ��������
    GLuint indices[] = {
        6, 7, 9,  //������ �����������
        7, 8, 9   //������ �����������
    };

    GLuint VBO, EBO; //������ ���������� �������, ����� ��������
    glGenBuffers(1, &VBO); //��������� 1-�� ������, ��������� �� ����������
    glGenBuffers(1, &EBO);

    //�������� ��� ������ ������ � ����� ��� OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //����������� �������� � GL_ARRAY_BUFFER ����� ����������� � ����� VBO
    //�������� ������ � GPU � �������� ���� ���� �������� VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //���������� ��� EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //����������� �������� � GL_ARRAY_BUFFER ����� ����������� � ����� VBO
    //�������� ������ � GPU � �������� ���� ���� �������� VBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    //������������� ��������� ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); //location 0, ���-�� ������������ (x,y,z), ��� ������, �� �����������, ��� ����� ����������, �������� ������ � ������ ������
    glEnableVertexAttribArray(0);//���������� ��������� ������� � location = 0

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //���� ���������
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();//��������� �������

        //������� ������
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);//�����
        glClear(GL_COLOR_BUFFER_BIT); //������� ����� �����

        //��� ������ ���� ���� ���
        static bool firstRun = true;
        if (firstRun || showMenu) {
            Menu();
            firstRun = false;
        }

        //���������� �������
        glUseProgram(shaderProgram);

        //�������� ���� � ������
        GLint objColorLoc = glGetUniformLocation(shaderProgram, "objColor");
        glUniform3f(objColorLoc, objColor.r, objColor.g, objColor.b);

        GLint pointSizeLoc = glGetUniformLocation(shaderProgram, "pointSize");
        glUniform1f(pointSizeLoc, pointSize);

        switch (currentPrimitive) {
        case POINTS:
            glDrawArrays(GL_POINTS, 0, 1); //��������� �����
            break;
        case LINES:
            glDrawArrays(GL_LINES, 1, 2); //��������� �����
            break;
        case TRIANGLE:
            glDrawArrays(GL_TRIANGLES, 3, 3); //��������� ������������
            break;
        case SQUARE:
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //��������� ��������
            break;
        }

        //����� �������
        glfwSwapBuffers(window);
    }
    //����������� �������
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_1:
            currentPrimitive = SQUARE;
            //std::cout << "������ ������� (������ ������ 1)" << std::endl;
            break;
        case GLFW_KEY_2:
            currentPrimitive = TRIANGLE;
            //std::cout << "������ ����������� (������ ������ 2)" << std::endl;
            break;
        case GLFW_KEY_3:
            currentPrimitive = POINTS;
            //std::cout << "������� ����� (������ ������ 3)" << std::endl;
            break;
        case GLFW_KEY_4:
            currentPrimitive = LINES;
            //std::cout << "������ ������� (������ ������ 4)" << std::endl;
            break;
        case GLFW_KEY_G:
            objColor.r = static_cast<float>(rand()) / RAND_MAX;
            objColor.g = static_cast<float>(rand()) / RAND_MAX;
            objColor.b = static_cast<float>(rand()) / RAND_MAX;
            //std::cout <<  << std::endl;
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        }
    }
}

void Menu() {
    std::cout << "------------------------" << std::endl;
    std::cout << "The Primitive display menu" << std::endl;
    std::cout << "1 - Show a square" << std::endl;
    std::cout << "2 - Show the triangle" << std::endl;
    std::cout << "3 - Show points" << std::endl;
    std::cout << "4 - Show a segment" << std::endl;
    std::cout << "G - Change the color of the primitive" << std::endl;
    std::cout << "ESC - stop program" << std::endl;
}
