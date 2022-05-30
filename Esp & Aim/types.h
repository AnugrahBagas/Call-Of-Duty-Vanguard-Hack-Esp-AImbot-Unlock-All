#pragma once

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

typedef struct
{
    centity_t *	    cent;
    refEntity_t *   ent;
    shaderInfo_t *  si;
    bool			visible;
    int				fov_x;
    int				fov_y;
    float			distance;
    vec3_t			aimangles;
} BotTarget_t;

typedef struct
{
    refEntity_t ent;
    shaderInfo_t si;
    centity_t cent;
} model_t;

typedef struct
{
    DWORD gfx_dll;
    float *viewangles;
    cg_t *cg;
    input_t *input;
    bool Aimkeypress;
    bool Shoot;
} set_t;

extern set_t set;
extern cg_t * cg;
extern float * viewangles;