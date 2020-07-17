#include "game_structs.h"
#include "game_libs.h"

sprite_t *sprite_init(SDL_Renderer *renderer, int pos_x, int pos_y, const char *directory, int tag)
{
    sprite_t *new_sprite = malloc(sizeof(sprite_t));
    new_sprite->rect = malloc(sizeof(SDL_Rect));
    int width, height;

    // Load Image and set Image data
    int comp;
    unsigned char *image = stbi_load(directory, &width, &height, &comp, 4);
    if (!image)
    {
        return NULL;
    }

    // Set Texture data
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture)
    {
        return NULL;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    // Pitch & pixels
    Uint8 *pixels = NULL;
    int pitch = 0;
    if (SDL_LockTexture(texture, NULL, (void **)&pixels, &pitch))
    {
        return NULL;
    }

    int opt_pitch = pitch >> 2;

    SDL_memcpy(pixels, image, pitch * height);
    free(image);
    SDL_SetTextureAlphaMod(texture, 100);
    SDL_UnlockTexture(texture);

    new_sprite->sprite_texture[0] = texture;
    new_sprite->anim_to_draw = 0;

    new_sprite->rect->x = pos_x;
    new_sprite->rect->y = pos_y;
    new_sprite->rect->w = width;
    new_sprite->rect->h = height;

    new_sprite->pos_x = pos_x;
    new_sprite->pos_y = pos_y;
    new_sprite->tag = tag;
    new_sprite->is_active = 1;

    return new_sprite;
}

// Animated Init
sprite_t *sprite_animated_init(SDL_Renderer *renderer, int pos_x, int pos_y, const char *directory, const char *directory1, const char *directory2,const char *directory3, int index, int check, int anim_start, int tag)
{
    sprite_t *new_sprite = malloc(sizeof(sprite_t));
    new_sprite->rect = malloc(sizeof(SDL_Rect));

    // Set Texture 0
    int width, height;

    // Load Image and set Image data
    int comp;
    unsigned char *image = stbi_load(directory, &width, &height, &comp, 4);
    if (!image)
    {
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture)
    {
        return NULL;
    }
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    // Pitch & pixels
    Uint8 *pixels = NULL;
    int pitch = 0;
    if (SDL_LockTexture(texture, NULL, (void **)&pixels, &pitch))
    {
        return NULL;
    }

    int opt_pitch = pitch >> 2;

    SDL_memcpy(pixels, image, pitch * height);
    free(image);
    SDL_UnlockTexture(texture);
    new_sprite->sprite_texture[0] = texture;
    new_sprite->sprite_texture[1] = texture;

    // Set Texture 2
    int width2, height2;
    int comp2;
    unsigned char *image2 = stbi_load(directory2, &width2, &height2, &comp2, 4);
    if (!image2)
    {
        return NULL;
    }

    SDL_Texture *texture2 = malloc(sizeof(size_t));
    texture2 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width2, height2);
    if (!texture2)
    {
        return NULL;
    }

    SDL_SetTextureBlendMode(texture2, SDL_BLENDMODE_BLEND);
    // Pitch & pixels
    Uint8 *pixels2 = NULL;
    int pitch2 = 0;
    if (SDL_LockTexture(texture2, NULL, (void **)&pixels2, &pitch2))
    {
        return NULL;
    }

    SDL_memcpy(pixels2, image2, pitch2 * height2);
    free(image2);
    SDL_UnlockTexture(texture2);
    new_sprite->sprite_texture[2] = texture2;


// Set Texture 3
    int width3, height3;
    int comp3;
    unsigned char *image3 = stbi_load(directory3, &width3, &height3, &comp3, 4);
    if (!image3)
    {
        return NULL;
    }

    SDL_Texture *texture3 = malloc(sizeof(size_t));
    texture3 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width3, height3);
    if (!texture3)
    {
        return NULL;
    }

    SDL_SetTextureBlendMode(texture3, SDL_BLENDMODE_BLEND);
    // Pitch & pixels
    Uint8 *pixels3 = NULL;
    int pitch3 = 0;
    if (SDL_LockTexture(texture3, NULL, (void **)&pixels3, &pitch3))
    {
        return NULL;
    }

    SDL_memcpy(pixels3, image3, pitch3 * height3);
    free(image3);
    SDL_UnlockTexture(texture3);
    new_sprite->sprite_texture[3] = texture3;
    //END texture

    new_sprite->anim_to_draw = anim_start;

    new_sprite->rect->x = pos_x;
    new_sprite->rect->y = pos_y;
    new_sprite->rect->w = width;
    new_sprite->rect->h = height;

    new_sprite->pos_x = pos_x;
    new_sprite->pos_y = pos_y;
    new_sprite->index = index;
    new_sprite->check = check;
    new_sprite->tag = tag;
    new_sprite->is_active = 1;

    return new_sprite;
}

void sprite_draw(SDL_Renderer *renderer, sprite_t *sprite)
{
    if (sprite->is_active == 1)
        SDL_RenderCopy(renderer, sprite->sprite_texture[0], NULL, sprite->rect);
}

// Animated Draw
void sprite_draw_animated(SDL_Renderer *renderer, sprite_t *sprite_to_draw, uint8_t row, uint8_t colums)
{
    uint32_t ticks;
    uint32_t seconds;
    uint32_t sprite;
    //Calculate the width and height for one cell to draw
    // float_t player_width_animated = sprite_to_draw->rect->w / row;
    // float_t player_height_animated = sprite_to_draw->rect->h / colums;
    float_t player_width_animated = 268 / row;
    float_t player_height_animated = 488 / colums;
    //Get ticks
    ticks = SDL_GetTicks();
    //divide for 100  get unit. second
    sprite = (ticks / 100) % row;
    // fprintf(stderr, "sprite %d \n", sprite);
    //Check the value of animation sprite is the first sprite
    if (sprite <= 0)
    {
        //increase index for raw
        if (sprite_to_draw->check == 0)
        {
            sprite_to_draw->index++;
            sprite_to_draw->check = 1;
        }
        else
        {
            //when the index is greater the row animation will be reset
            if (sprite_to_draw->index > 4)
            {
                sprite_to_draw->index = 0;
            }
        }
    }
    else
    {
        sprite_to_draw->check = 0;
    }

    //Source and destination rect for single frame to anim, change the offset that texture and set the right animation
    SDL_Rect srcrect = {sprite * player_width_animated, sprite_to_draw->index * player_height_animated, player_width_animated, player_height_animated};
    SDL_Rect dstrect = {sprite_to_draw->pos_x, sprite_to_draw->pos_y, player_width_animated, player_height_animated};

    //if the case press left flip the animation walk
    if (sprite_to_draw->anim_to_draw == 1)
    {
        //fprintf(stderr, "INDEX ANIM: %d \n", sprite_to_draw->anim_to_draw);
        SDL_RenderCopyEx(renderer, sprite_to_draw->sprite_texture[sprite_to_draw->anim_to_draw], &srcrect, &dstrect, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    else
    {
        SDL_RenderCopy(renderer, sprite_to_draw->sprite_texture[sprite_to_draw->anim_to_draw], &srcrect, &dstrect); 
    }
}

void sprite_set_active(sprite_t *sprite, int active)
{
    sprite->is_active = active;
    // visibile o invisibile
}

void sprite_destroy(sprite_t *sprite)
{
    if (sprite->rect)
    {
        free(sprite->rect);
    }
    free(sprite);
}