#include <SDL.h>

typedef struct sprite
{
    int index, check;
    SDL_Rect *rect;
    SDL_Texture *sprite_texture[4];
    int pos_x, pos_y;
    int anim_to_draw;
    int tag;
    int is_active;

} sprite_t;

typedef struct game_object
{
    sprite_t *sprite;
    float pos_x, pos_y;
} game_object_t;

typedef struct clock
{
    uint8_t fps;
    Uint64 then;
    Uint64 now;
    double deltatime;
} clock_t;

typedef struct vector2_t
{
    float x;
    float y;
} vector2_t;

typedef struct bullet
{
    sprite_t *sprite, *cloud_sprite;
    float pos_x, pos_y;
    float lifespan;
    int start_cloud_height;
} bullet_t;

typedef struct ball
{
    sprite_t *sprite;
    float pos_x, pos_y;
    float vel_x, vel_y;
    float scale;
    int life;
    float invincible;

} ball_t;

sprite_t *sprite_init(SDL_Renderer *renderer, int pos_x, int pos_y, const char *directory, int tag);

sprite_t *sprite_animated_init(SDL_Renderer *renderer, int pos_x, int pos_y, const char *directory, const char *directory1, const char *directory2, const char *directory3, int index, int check, int anim_to_draw, int tag);

void sprite_draw(SDL_Renderer *renderer, sprite_t *sprite);

void sprite_draw_animated(SDL_Renderer *renderer, sprite_t *sprite_to_draw, uint8_t row, uint8_t colums);

void sprite_set_active(sprite_t *sprite, int active);

void sprite_destroy(sprite_t *sprite);

game_object_t *game_object_init(SDL_Renderer *renderer, int pos_x, int pos_y, const char *directory, int tag);

game_object_t *game_object_init2(sprite_t *sprite, int pos_x, int pos_y);

void game_object_draw(SDL_Renderer *renderer, game_object_t *game_object);

void game_object_move_h(sprite_t *sprite, float h_velocity);

void game_object_move_v(sprite_t *sprite, float v_velocity);

void game_object_move_rect_h(sprite_t *sprite, float h_velocity);

void game_object_move_rect_v(sprite_t *sprite, float v_velocity);

void game_object_scale(sprite_t *sprite, int scale);

int is_colliding(sprite_t *first, sprite_t *second);

void game_object_destroy(game_object_t *game_object);

clock_t *clock_init(uint8_t fps);

void tick(clock_t *clock);

void set_now(clock_t *clock);

void set_then(clock_t *clock);

double get_deltatime(clock_t *clock);

void clock_destroy(clock_t *clock);

bullet_t *bullet_init(SDL_Renderer *renderer, int pos_x, int pos_y, float lifespan);

bullet_t *shoot(SDL_Renderer *renderer, sprite_t *sprite);

void spawn(bullet_t *bullet, game_object_t *ref_obj);

void bullet_update(bullet_t *bullet, clock_t *clock);

void bullet_destroy(bullet_t *bullet);

ball_t *ball_init(SDL_Renderer *renderer, int pos_x, int pos_y, int direction, int life, float scale, float invincible);

void ball_update(ball_t *ball, float deltatime);

void ball_set_velocity_x(ball_t *ball, float vel_x);

void ball_set_velocity_y(ball_t *ball, float vel_y);

void ball_rescale(ball_t *ball, float new_scale);

void ball_velocity_multiplier_y(ball_t *ball, float scale);

void ball_destroy(ball_t *ball);