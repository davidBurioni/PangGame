#include "game_structs.h"
#include "game_libs.h"
// #include "vector.h"


game_object_t *game_object_init(SDL_Renderer *renderer, int pos_x, int pos_y, const char * directory,int tag)
{
    game_object_t *new_go = malloc(sizeof(game_object_t));
    new_go->sprite = sprite_init(renderer, pos_x, pos_y, directory,tag);
    new_go->pos_x = pos_x;
    new_go->pos_y = pos_y;

    new_go->sprite->rect->x = pos_x;
    new_go->sprite->rect->y = pos_y;

    return new_go;
}

game_object_t *game_object_init2(sprite_t *sprite, int pos_x, int pos_y)
{
    game_object_t *new_go = malloc(sizeof(game_object_t));
    new_go->sprite = sprite;
    new_go->pos_x = pos_x;
    new_go->pos_y = pos_y;

    new_go->sprite->rect->x = pos_x;
    new_go->sprite->rect->y = pos_y;

    return new_go;
}

void game_object_draw(SDL_Renderer *renderer, game_object_t *game_object)
{    
    sprite_draw(renderer, game_object->sprite);
}

void game_object_destroy(game_object_t *game_object)
{
    if (game_object->sprite)
        sprite_destroy(game_object->sprite);
    free(game_object);
}

void game_object_move_h(sprite_t *sprite, float h_velocity)
{
    sprite->pos_x += h_velocity;
}

void game_object_move_v(sprite_t *sprite, float v_velocity)
{
    sprite->pos_y += v_velocity;
}

void game_object_move_rect_h(sprite_t *sprite, float h_velocity)
{
    sprite->rect->x += h_velocity;
}

void game_object_move_rect_v(sprite_t *sprite, float v_velocity)
{
    sprite->rect->y += v_velocity;
}

void game_object_scale(sprite_t *sprite, int scale)
{
    sprite->rect->h -= scale;
}

