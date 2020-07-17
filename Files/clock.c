#include "game_libs.h"
#include "game_structs.h"


//uint64_t now, then;

clock_t *clock_init(uint8_t fps)
{
    clock_t *new_clock = malloc(sizeof(clock_t));
    new_clock->fps = fps;
    new_clock->now = SDL_GetPerformanceCounter();
    new_clock->then = new_clock->now;
    new_clock->deltatime = 0;

    return new_clock;
}

void tick(clock_t *clock)
{
    clock->deltatime = (double)((clock->now - clock->then)) / SDL_GetPerformanceFrequency();
    //printf("%llu / %llu = %f\n", ((clock->now - clock->then)), SDL_GetPerformanceFrequency(), (double)((clock->now - clock->then)) / (SDL_GetPerformanceFrequency()));
}

double get_deltatime(clock_t *clock)
{
    return clock->deltatime;
}

void set_now(clock_t *clock)
{
    clock->now = SDL_GetPerformanceCounter();
}

void set_then(clock_t *clock)
{
    clock->then = clock->now;
}

void clock_destroy(clock_t *clock)
{
    free(clock);
}