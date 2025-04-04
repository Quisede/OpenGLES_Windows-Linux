# OpenGLES_Windows-Linux
Решение тестового задания (реализация под системы Windows и Linux)

Разработка приложений с использованием OpenGL ES на ПК с архитектурой x64/x86
, через стандартный opengl. Использовать glfw3, glew/glut,  Linux/windows, приложение может отображать произвольный примитив или набор примитивов. Основное направление имитация/эмуляция opengles на ПК. 
Составить план,
Задачи по заданию (любой бактрекер или текстовый файл)
Репозиторий гит,
Код.

## Возможности
- Отображение 4-х примитивов (квадрат, треугольник, точка, отрезок)
- Динамическое изменение цвета (`клавиша G`)
- Консольное меню, управление клавишами

## Поддерживаемые ОС
- Windows
- Linux

## Какие библиотеки скачать и как настроить компилятор
- Windows
    - GLEW, GLFW, GLM, OpenGL ES, MesaOpenGL (для таких библиотек как libGLESv2.lib, libEGL.dll)
        - https://glew.sourceforge.net/index.html
        - https://www.glfw.org
        - https://registry.khronos.org/OpenGL/index_es.php
        - https://github.com/pal1000/mesa-dist-win?ysclid=m92fjuam2x15805089
    - Visual Studio - указать в настройках проекта пути к библиотекам (glf, glm, mesaopengl)
- Linux
    - Проверить базовые инструменты (при необходимости установить)
    g++ --version
    make --version
    - Библиотеки
    sudo apt update
    sudo apt install -y libglfw3-dev libgles2-mesa-dev
    sudo apt install -y libglm-dev libxinerama-dev libxcursor-dev
    - Сборка
    g++ -std=c++11 -I/usr/include main.cpp shaders_loader.cpp \
    -o opengl_es_emulator \
    -lglfw -lGLESv2 -ldl -lpthread -lX11 -lXrandr
    - Запуск
    ./opengl_es_emulator

