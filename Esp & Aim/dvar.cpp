#include "dllmain.h"

enum_type aimtype[] = { "Off", "Auto aim", "Silent aim", NULL };
enum_type aimkey[] = { "Off", "Left mouse", "Right Mouse", "Shift", "Alt", NULL };
enum_type aimfilter[] = { "Min distance", "Max distance", "Min FOV", NULL };

dvar_t *dv_aimbot;
dvar_t *dv_aimbot_fov;
dvar_t *dv_aimbot_key;
dvar_t *dv_aimbot_filter;
dvar_t *dv_aimbot_norecoil;
dvar_t *dv_aimbot_autoshoot;

void register_dvars()
{
    dv_aimbot = RegisterEnum( "zombot_aimbot", aimtype, 0, 0 );
    dv_aimbot_key = RegisterEnum( "zombot_aimkey", aimkey, 0, 0 );
    dv_aimbot_filter = RegisterEnum( "zombot_filter", aimfilter, 0, 0 );

    CmdConsole( "dvar_bool zombot_norecoil 0 " );
    CmdConsole( "dvar_bool zombot_autoshoot 0 " );

    dv_aimbot_norecoil = Dvar_FindDvar( "zombot_norecoil" );
    dv_aimbot_autoshoot = Dvar_FindDvar( "zombot_norecoil" );
}

dvar_t * Dvar_FindDvar( const char * name )
{
    static dvar_t *dvarList = (dvar_t *)0x00C5C9D0;

    for ( int i = 0; i < 0x500; i++ )
    {
        if ( dvarList[ i ].name )
        {
            if ( _stricmp( dvarList[ i ].name, name ) == 0 )
                return &( dvarList[ i ] );
        }
    }

    return NULL;
}