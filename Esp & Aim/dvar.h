#pragma once

typedef struct
{
    const char *	name;
    int				flags;
    //LPVOID		value;
    union
    {
        float		fValue;
        float *		pVectorValue;
        int			iValue;
        ULONG		ulValue;
        char *		pStringValue;
        bool		boolValue;
    };
    LPVOID			value2;
    LPVOID			value3;
    LPVOID			min;
    LPVOID			max;
    int				pad[ 2 ];
} dvar_t;

typedef const char *enum_type;

void register_dvars();
dvar_t * Dvar_FindDvar( const char * name );

extern dvar_t *dv_aimbot;
extern dvar_t *dv_aimbot_fov;
extern dvar_t *dv_aimbot_key;
extern dvar_t *dv_aimbot_filter;
extern dvar_t *dv_aimbot_norecoil;
extern dvar_t *dv_aimbot_autoshoot;