#include <cstdlib>
#include <iostream>
using namespace std;

#include <GL/glew.h>
#include <SDL2/SDL.h>

bool init_resources(void) {
    return true;
}

void render (SDL_Window* window) {

}

void free_resources() {

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
        "I'm making a triangle",
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