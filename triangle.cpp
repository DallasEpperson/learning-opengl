#include <cstdlib>
#include <iostream>
using namespace std;

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "./common/shader_utils.h"

GLuint program;
GLint attribute_coord2d;

void print_log(GLuint object) {
    GLint log_length = 0;
    if (glIsShader(object)) {
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
    } else if (glIsProgram(object)) {
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    } else {
        cerr << "printlog: Not a shader or a program" << endl;
        return;
    }

    char* log = (char*)malloc(log_length);

    if (glIsShader(object))
        glGetShaderInfoLog(object, log_length, NULL, log);
    else if (glIsProgram(object))
        glGetProgramInfoLog(object, log_length, NULL, log);

    cerr << log;
    free(log);
}

GLuint create_shader(const char* filename, GLenum type) {
    const GLchar* shaderCode = file_read(filename);
    if (shaderCode == NULL) {
        cerr << "Error opening " << filename << ": " << SDL_GetError() << endl;
        return 0;
    }
    GLuint res = glCreateShader(type);
    const GLchar* sources[] = {
#ifdef GL_ES_VERSION_2_0
        "#version 100\n"
#else
        "#version 120\n"
#endif
        ,
        shaderCode
    };
    glShaderSource(res, 2, sources, NULL);
    free((void*)shaderCode);
    glCompileShader(res);
    GLint compile_ok = GL_FALSE;
    glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
    if (compile_ok == GL_FALSE){
        cerr << filename << ":";
        print_log(res);
        glDeleteShader(res);
        return 0;
    }
    return res;
}

bool init_resources() {
    GLuint vs, fs;
    GLint link_ok;
    if ((vs = create_shader("triangle.v.glsl", GL_VERTEX_SHADER)) == 0)
        return false;
    if ((fs = create_shader("triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0)
        return false;

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        cerr << "Error in glLinkProgram:";
        print_log(program);
        return false;
    }

    const char* attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation(program, attribute_name);
    if (attribute_coord2d == -1) {
        cerr << "Could not bind attribute " << attribute_name << endl;
        return false;
    }
    return true;
}

void render (SDL_Window* window) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glEnableVertexAttribArray(attribute_coord2d);
    GLfloat triangle_vertices[] = {
        0.0, 0.8,
        -0.8, -0.8,
        0.8, -0.8
    };

    glVertexAttribPointer(
        attribute_coord2d,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        triangle_vertices
    );

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(attribute_coord2d);

    SDL_GL_SwapWindow(window);
}

void free_resources() {
    glDeleteProgram(program);
}

void mainLoop(SDL_Window* window) {
    while (true) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT)
                return;
        }
        render(window);
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "I made a triangle",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(window);

    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        cerr << "Error: glewInit: " << glewGetErrorString(glew_status) << endl;
        return EXIT_FAILURE;
    }

    if (!init_resources()){
        return EXIT_FAILURE;
    }

    mainLoop(window);

    free_resources();
    return EXIT_SUCCESS;
}