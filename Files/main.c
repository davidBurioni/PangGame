#define SDL_MAIN_HANDLED
#include <SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "game_structs.h"
#include "vector.h"

#define MAXLIFESPAN 1.5

SDL_Window *window;
SDL_Renderer *renderer;
clock_t *clock;
vector_t *sprites_vec;
vector_t *bullet_pool;
vector_t *ball_pool;
vector_t *ball_pool_active;
sprite_t *bg_sprite;
sprite_t *player_sprite;
game_object_t *player;
SDL_Event event;

float velocity = 0;
uint8_t check_ball_pool = 0;

//Audio
SDL_AudioSpec wavSpec;
Uint32 wavLength;
Uint8 *wavBuffer;
SDL_AudioDeviceID deviceId;

bullet_t *get_first_bullet();
ball_t *get_first_ball();
void Init_audio();

vector2_t *vector2_add(vector2_t *vect1, vector2_t *vect2);
vector2_t *vector2_init(float x, float y);

float end_time = 6000;
int counter_ball_spawn = 0;
int exit_game;
int ball_spawn = 1;
int time_spawn_ready = 0;

int coun_ball_destroy = 0;
int count_end_ball = 4;

//
int level = 1;

int Init();

void check_Physics();

void Update();

void Draw();

void check_level();

void spawn_ball(int balls_to_spawn);

void check_level()
{
    switch (level)
    {
    case 1:
        ball_spawn = 1;
        spawn_ball(ball_spawn);
        break;

    case 2:
        ball_spawn = 2;
        spawn_ball(ball_spawn);
        break;

    case 3:
        ball_spawn = 3;
        spawn_ball(ball_spawn);
        break;

    default:
        break;
    }
}

int die(const char *message)
{
    SDL_Log("%s: %s", message, SDL_GetError());
    SDL_Quit();
    return 1;
}

int end()
{
    SDL_Quit();
    return 0;
}

void Init_audio()
{

    SDL_LoadWAV("Assets\\audo_backCopia.wav", &wavSpec, &wavBuffer, &wavLength);
    // open audio device

    deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
}

vector2_t *vector2_add(vector2_t *vect1, vector2_t *vect2)
{
    vector2_t *new_vector;
    new_vector->x = vect1->x + vect2->x;
    new_vector->y = vect1->y + vect2->y;

    return new_vector;
}

vector2_t *vector3_init(float x, float y, float z)
{
    vector2_t *new_vector;
    new_vector->x = x;
    new_vector->y = y;

    return new_vector;
}

int main(int argc, char **argv)
{
    // Initialization
    if (Init())
        die("Unable to initialize");

    int lock = 0;
    int pressed = 0;

    //AUDIO
    printf("initittve");

    // Loop
    // int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);

    for (;;)
    {
        //AUDIO BACKGROUND
        SDL_QueueAudio(deviceId, wavBuffer, wavLength);
        SDL_PauseAudioDevice(deviceId, 0);

        if (exit_game)
        {
            end_time -= (SDL_GetTicks() / 100);
            if (end_time <= 0)
            {
                SDL_Delay(2000);
                goto end;
            }
        }
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                goto end;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                pressed = 0;
                if (!lock && !pressed)
                {
                    pressed = 1;
                    lock = 1;

                    switch (event.key.keysym.sym)
                    {
                    case SDLK_RIGHT:
                        player_sprite->anim_to_draw = 0;
                        velocity = 200;
                        break;

                    case SDLK_LEFT:
                        player_sprite->anim_to_draw = 1;
                        velocity = -200;
                        break;

                    case SDLK_SPACE:
                        bullet_t *bullet = get_first_bullet();
                        if (bullet)
                            spawn(bullet, player);
                        break;

                    default:
                        break;
                    }
                }
            }
            else if (event.type == SDL_KEYUP)
            {
                lock = 0;
                if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_LEFT)
                {
                    if (exit_game)
                    {
                        player_sprite->anim_to_draw = 3;
                    }
                    else
                    {
                        player_sprite->anim_to_draw = 2;
                        velocity = 0;
                    }
                }
            }
        }

        Update();
        Draw();
    }

end:
    game_object_destroy(player);
    vector_destroy(sprites_vec);
    vector_destroy(bullet_pool);
    clock_destroy(clock);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
    end();
}

int Init()
{
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | 0))
    {
        return die("Error initializing SDL2");
    }

    window = SDL_CreateWindow("Game", 100, 100, 720, 480, SDL_WINDOW_SHOWN);
    if (!window)
    {
        return die("Error creating window");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        return die("Unable to create renderer");
    }
    Init_audio();

    int check = 0;
    int temp = 0;

    clock = clock_init(60);
    temp = !clock;
    check |= temp;

    bg_sprite = sprite_init(renderer, 0, 0, "Assets\\background2.png", 0);
    temp = !bg_sprite;
    check |= temp;

    player_sprite = sprite_animated_init(renderer, 315, 330, "Assets\\Walk.png", "Assete\\Walk.png", "Assets\\Idle.png", "Assets\\Die.png", 0, 0, 2, 1);
    player = game_object_init2(player_sprite, 315, 310);
    printf("creoplayer");
    temp = !player_sprite;
    check |= temp;
    temp = !player;
    check |= temp;

    sprites_vec = vector_init(sizeof(sprite_t));
    vector_add(sprites_vec, bg_sprite);
    bullet_pool = vector_init(sizeof(bullet_t));
    bullet_t *tmp;
    for (size_t i = 0; i < 20; i++)
    {
        tmp = bullet_init(renderer, player->pos_x + 13, player->pos_y - 40, MAXLIFESPAN);
        sprite_set_active(tmp->sprite, 0);
        sprite_set_active(tmp->cloud_sprite, 0);

        vector_add(bullet_pool, tmp);
        vector_add(sprites_vec, tmp->sprite);
        vector_add(sprites_vec, tmp->cloud_sprite);
    }
    ball_pool = vector_init(sizeof(ball_t));

    //INIT BALL POOL
    ball_t *tmp_ball;
    for (size_t t = 0; t < 10; t++)
    {
        tmp_ball = ball_init(renderer, 400, 20, 0, 3, 1, 1);
        sprite_set_active(tmp_ball->sprite, 0);
        vector_add(ball_pool, tmp_ball);
        vector_add(sprites_vec, tmp_ball->sprite);
    }

    ball_pool_active = vector_init(sizeof(ball_t));

    SDL_Log("Initialization succesfull");
    return check;
}

void set_balls_disactive()
{
    for (size_t i = 0; i < ball_pool->size; i++)
    {
        bullet_t *bull = vector_get(bullet_pool, i);
        if (bull->sprite->is_active == 1)
            sprite_set_active(vector_get(ball_pool, i), 0);
    }
    counter_ball_spawn = 0;
}

void spawn_ball(int balls_to_spawn)
{
    if (counter_ball_spawn < balls_to_spawn)
    {
        if (check_ball_pool == 0)
        {
            check_ball_pool = 1;
            printf("counter_ball_spawn: %d, balls_to_spawn %d, check bool : %d\n", counter_ball_spawn, balls_to_spawn, check_ball_pool);

            ball_t *new_ball = get_first_ball();
            if (new_ball)
            {
                counter_ball_spawn++;
                sprite_set_active(new_ball->sprite, 1);
                vector_add(ball_pool_active, new_ball);
                printf("ball acrive: %zu \n", ball_pool_active->size);
            }
            check_ball_pool = 0;
        }
    }
}

void check_Physics()
{
    ball_t *curr_ball;
    bullet_t *curr_bullet;
    SDL_Rect *ball_rect, *bullet_rect;
    int magnitude_x = 1000;
    int ball_mid_w;
    for (size_t i = 0; i < ball_pool->size; i++)
    {
        curr_ball = vector_get(ball_pool, i);
        if (!curr_ball->sprite->is_active)
            continue;
        ball_rect = curr_ball->sprite->rect;
        ball_mid_w = ball_rect->w / 2;
        magnitude_x = abs((ball_rect->x + ball_mid_w) - (player->sprite->pos_x + 10));
        if (ball_rect->y + (ball_rect->h / 2) >= player->sprite->rect->y && magnitude_x <= ball_mid_w)
        {
            //printf("collisione col player!");
            // Game Over
             player_sprite->anim_to_draw = 3;
             exit_game = 1;
            return;
            end();
        }
        magnitude_x = 1000;
        for (size_t j = 0; j < bullet_pool->size; j++)
        {
            curr_bullet = vector_get(bullet_pool, j);
            if (curr_bullet->sprite->is_active)
            {
                bullet_rect = curr_bullet->sprite->rect;
                magnitude_x = abs((ball_rect->x + ball_mid_w) - (bullet_rect->x + bullet_rect->w / 2));
                if (ball_rect->y + ball_rect->h >= bullet_rect->y && magnitude_x <= ball_mid_w && curr_ball->invincible <= 0)
                {
                    //printf("collisione con bullet!\n");
                    curr_bullet->lifespan = 0;
                    sprite_set_active(curr_ball->sprite, 0);
                    game_object_scale(curr_ball->sprite, 1);
                    curr_ball->life--;
                    // printf("ball life: %d\n", curr_ball->life);
                    // vector_remove(ball_pool_active, curr_ball);

                    if (curr_ball->life > 0)
                    {
                        ball_t *new_ball1 = ball_init(renderer, curr_ball->sprite->rect->x, curr_ball->sprite->rect->y, -1, curr_ball->life, curr_ball->scale * .5, .5);
                        vector_add(ball_pool, new_ball1);
                        vector_add(ball_pool_active, new_ball1);
                        vector_add(sprites_vec, new_ball1->sprite);
                        ball_rescale(new_ball1, curr_ball->scale * .5);
                        ball_t *new_ball2 = ball_init(renderer, curr_ball->sprite->rect->x, curr_ball->sprite->rect->y, 1, curr_ball->life, curr_ball->scale * .5, .5);
                        vector_add(ball_pool, new_ball2);
                        vector_add(ball_pool_active, new_ball2);
                        vector_add(sprites_vec, new_ball2->sprite);
                        ball_rescale(new_ball2, curr_ball->scale * .5);
                        printf("ball acrive: %zu \n", ball_pool_active->size);
                    }
                    else
                    {
                        sprite_set_active(curr_ball->sprite, 0);
                        printf("ball acrive: %zu , ball destroy: %d\n", ball_pool_active->size, coun_ball_destroy);

                        if (ball_pool_active->size % 7 == 0)
                        {
                            if (coun_ball_destroy <= ball_spawn)
                            {
                                coun_ball_destroy++;
                            }
                        }
                        if (coun_ball_destroy > ball_spawn)
                        {
                            count_end_ball--;
                            printf("endball: %d\n", count_end_ball);
                            if (count_end_ball <= 1)
                            {
                                level++;
                                check_level();
                                // ball_spawn++;
                                counter_ball_spawn = 1;
                                vector_empty(ball_pool_active);
                                check_ball_pool = 0;
                                // set_balls_disactive();
                                printf("ball acrive: %zu \n", ball_pool_active->size);
                                printf("finishLevel\n");
                                coun_ball_destroy = 0;
                                count_end_ball = 4;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Update()
{
    set_then(clock);
    set_now(clock);
    tick(clock);

    float dt = get_deltatime(clock);

    bullet_t *first_b;

    for (size_t i = 0; i < bullet_pool->size; i++)
    {
        first_b = vector_get(bullet_pool, i);
        if (first_b->sprite->is_active)
        {
            bullet_update(first_b, clock);
            if (first_b->lifespan <= 0)
            {
                sprite_set_active(first_b->sprite, 0);
                sprite_set_active(first_b->cloud_sprite, 0);
            }
        }
    }

    check_level();

    for (size_t i = 0; i < ball_pool->size; i++)
    {
        ball_update(vector_get(ball_pool, i), dt);
    }

    check_Physics();

    game_object_move_h(player->sprite, velocity * dt);
}

void Draw()
{
    for (size_t i = 0; i < sprites_vec->size; i++)
    {
        sprite_draw(renderer, vector_get(sprites_vec, i));
    }
    sprite_draw_animated(renderer, player_sprite, 4, 5);
    SDL_RenderPresent(renderer);
}

bullet_t *get_first_bullet()
{
    bullet_t *tmp;
    for (size_t i = 0; i < bullet_pool->size; i++)
    {
        tmp = (bullet_t *)bullet_pool->data[i];
        sprite_t *spr = tmp->sprite;
        if (!spr->is_active)
        {
            goto taken;
        }
    }
    bullet_destroy(tmp);

    return NULL;

taken:
    return tmp;
}

ball_t *get_first_ball()
{
    ball_t *tmp;
    for (size_t i = 0; i < ball_pool->size; i++)
    {
        tmp = (ball_t *)ball_pool->data[i];
        sprite_t *spr = tmp->sprite;
        if (!spr->is_active)
        {
            goto taken;
        }
    }
    ball_destroy(tmp);

    return NULL;

taken:
    return tmp;
}