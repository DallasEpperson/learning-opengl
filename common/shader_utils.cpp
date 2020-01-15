#include <SDL2/SDL.h>

char* file_read(const char* filename) {
    SDL_RWops *readWrite = SDL_RWFromFile(filename, "rb"); // see https://wiki.libsdl.org/SDL_RWFromFile#Remarks
    if (readWrite == NULL) return NULL;

    Sint64 dataStreamSize = SDL_RWsize(readWrite);
    char* res = (char*)malloc(dataStreamSize + 1);

    Sint64 nb_read_total = 0, nb_read = 1;
    char* buf = res;
    while (nb_read_total < dataStreamSize && nb_read != 0) {
        nb_read = SDL_RWread(readWrite, buf, 1, (dataStreamSize - nb_read_total));
        nb_read_total += nb_read;
        buf += nb_read;
    }
    SDL_RWclose(readWrite);
    if (nb_read_total != dataStreamSize) {
        free(res);
        return NULL;
    }
    res[nb_read_total] = '\0';
    return res;
}