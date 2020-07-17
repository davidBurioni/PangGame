#include "game_structs.h"
#include "game_libs.h"

ball_t *ball_init(SDL_Renderer *renderer, int pos_x, int pos_y, int direction, int life, float scale, float invincible)
{
    ball_t *new_ball = malloc(sizeof(ball_t));
    new_ball->sprite = sprite_init(renderer, pos_x, pos_y, "Assets\\ball.png", 4);
    new_ball->pos_x = pos_x;
    new_ball->pos_y = pos_y;
    new_ball->life = life;
    new_ball->invincible = invincible;

    srand(SDL_GetPerformanceCounter());
    int i = rand() % 10;

    new_ball->vel_x = (direction != 0 ? direction : i < 5 ? 1 : -1) * 4;
    new_ball->vel_y = -2;
    new_ball->scale = 1;

    return new_ball;
}

void ball_update(ball_t *ball, float deltatime)
{
    if (ball->sprite->is_active)
    {
        if (ball->invincible > 0)
            ball->invincible -= deltatime;
        if (ball->sprite->rect->y > 430 - ball->sprite->rect->h)
        {
            ball->sprite->rect->y -= 3;
            ball_velocity_multiplier_y(ball, -.9);
        }

        if (ball->sprite->rect->x > 720 - ball->sprite->rect->w || ball->sprite->rect->x < 0)
        {
            ball->vel_x = -ball->vel_x;
        }
        float vel = ball->vel_y += 9.81 * deltatime;

        game_object_move_rect_h(ball->sprite, ball->vel_x);
        game_object_move_rect_v(ball->sprite, vel);

    }
}

void ball_rescale(ball_t *ball, float new_scale)
{
    ball->scale = new_scale;
    ball->sprite->rect->h *= new_scale;
    ball->sprite->rect->w *= new_scale;
}

void ball_set_velocity_y(ball_t *ball, float vel_y)
{
    ball->vel_y = vel_y;
}

void ball_set_velocity_x(ball_t *ball, float vel_x)
{
    ball->vel_x = vel_x;
}

void ball_velocity_multiplier_y(ball_t *ball, float scale)
{
    ball->vel_y *= scale;
}

void ball_destroy(ball_t *ball)
{
    if (ball->sprite)
        sprite_destroy(ball->sprite);
    free(ball);
}