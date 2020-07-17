#include "game_structs.h"
#include "game_libs.h"

bullet_t *bullet_init(SDL_Renderer *renderer, int pos_x, int pos_y, float lifespan)
{
    bullet_t *new_bullet = malloc(sizeof(bullet_t));
    new_bullet->sprite = sprite_init(renderer, pos_x, pos_y, "Assets\\bullet.png", 2);
    new_bullet->cloud_sprite = sprite_init(renderer, pos_x, pos_y, "Assets\\cloud.png", 3);
    new_bullet->pos_x = pos_x;
    new_bullet->pos_y = pos_y;

    new_bullet->sprite->rect->x = pos_x;
    new_bullet->sprite->rect->y = pos_y;
    new_bullet->cloud_sprite->rect->x = pos_x;
    new_bullet->cloud_sprite->rect->y = pos_y;

    new_bullet->start_cloud_height = new_bullet->cloud_sprite->rect->h;
    new_bullet->lifespan = lifespan;

    return new_bullet;
}

void spawn(bullet_t *bullet, game_object_t *ref_obj)
{
    sprite_set_active(bullet->sprite, 1);
    sprite_set_active(bullet->cloud_sprite, 1);
    bullet->sprite->rect->x = ref_obj->sprite->pos_x + 13;
    bullet->cloud_sprite->rect->x = ref_obj->sprite->pos_x + 13;
    bullet->sprite->rect->y = ref_obj->sprite->pos_y - 40;
    bullet->cloud_sprite->rect->y = ref_obj->sprite->pos_y;
    bullet->cloud_sprite->rect->h = bullet->start_cloud_height;
    bullet->lifespan = 1.5;
}


void bullet_update(bullet_t *bullet, clock_t *clock)
{
    game_object_move_rect_v(bullet->sprite, -4);
    bullet->lifespan -= get_deltatime(clock);

    game_object_move_rect_v(bullet->cloud_sprite, -4);
    game_object_scale(bullet->cloud_sprite, -3);
}

void bullet_destroy(bullet_t *bullet)
{
    if (bullet->sprite)
        sprite_destroy(bullet->sprite);
    free(bullet);
}
