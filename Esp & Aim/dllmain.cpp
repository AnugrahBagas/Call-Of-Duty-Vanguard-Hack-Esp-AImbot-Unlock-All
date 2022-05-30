#include "dllmain.h"

set_t set;

typedef void( *oStereoFrame ) ( );
oStereoFrame pStereoFrame;

typedef int( WINAPIV *t_AddRefEntityToScene )( refEntity_t * ent, shaderInfo_t * si, centity_t * cent );
t_AddRefEntityToScene o_AddRefEntityToScene;

char *zombie_list[] = {
    "iwx_nazi",
    "wolfrun",
    "wolfattack",
    "horrortulajdona_pssf",
    "horror_arab_rotu",
    "horror_rotu",
    "horror_bigblood",
    "inak_dog",
    "oma_diana2",
    "oma_zombie",
    "oma_zom_cellbreaker",
    "van_mon",
    NULL
};

void hStereoFrame()
{
    pStereoFrame();

    if ( set.cg->snap )
    {
        if ( dv_aimbot_norecoil )
        {
            if ( dv_aimbot_norecoil->iValue )
                MemoryWrite( (void*)0x004D7115, ( void* )"\x75", 1 );
            else
                MemoryWrite( (void*)0x004D7115, ( void* )"\x74", 1 );
        }

        if ( dv_aimbot && dv_aimbot->iValue > 0 )
        {
            for ( int i = 0; i < models.size(); i++ )
                aim.AddZombie( &models[i].ent, &models[ i ].si, &models[ i ].cent );

            int num = aim.GetFirstZombie();
            aim.Aiming( num );
            models.clear();
        }
    }
}

int RE_AddRefEntityToScene( refEntity_t * ent, shaderInfo_t * si, centity_t * cent )
{
    if ( ent->reType == RT_MODEL )
    {
        if ( dv_aimbot && dv_aimbot->iValue > 0 )
        {
            for ( int i = 0; zombie_list[ i ]; i++ )
            {
                if ( strstr( *(char **)( si->model + 2 ), zombie_list[ i ] ) )
                {
                    model_t model;

                    model.ent = *ent;
                    model.si = *si;
                    model.cent = *cent;

                    models.push_back( model );
                }
            }
        }
    }

    return o_AddRefEntityToScene( ent, si, cent );
}

void __stdcall KeyHook()
{
    DWORD keys[] = { 0, VK_RBUTTON, VK_LBUTTON, VK_LSHIFT, VK_MENU };

    for ( ;; Sleep( 20 ) )
    {
        if ( dv_aimbot_key && KEY_DOWN( keys[ dv_aimbot_key->iValue ] ) )
            set.Aimkeypress = true;
        else
            set.Aimkeypress = false;

        if ( set.Shoot )
        {
            keybd_event( VK_LBUTTON, 0, 0, 0 );
            keybd_event( VK_LBUTTON, 0, KEYEVENTF_KEYUP, 0 );
            set.Shoot = false;
        }
    }
}

void mymain()
{
    while ( GetModuleHandleA( "gfx_d3d_mp_x86_s.dll" ) == 0 || GetModuleHandleA( "mssmp3.asi" ) == 0 || GetModuleHandleA( "d3d9.dll" ) == 0 )
        Sleep( 100 );

    set.gfx_dll = (DWORD)LoadLibraryA( "gfx_d3d_mp_x86_s.dll" );
    set.cg = (cg_t*)0x014EE080;
    set.viewangles = (float *)0x0098FDEC;
    set.input = (input_t *)0x009CFE2C;

    Draw.initialization();
    register_dvars();

    o_AddRefEntityToScene = (t_AddRefEntityToScene)DetourFunction( (PBYTE)( set.gfx_dll + 0x241A0 ), (PBYTE)RE_AddRefEntityToScene );
    pStereoFrame = (oStereoFrame)DetourFunction( (PBYTE)( 0x004CBCA0 ), (PBYTE)hStereoFrame );
}

bool __stdcall DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
    DisableThreadLibraryCalls( hinstDLL );

    switch ( fdwReason )
    {
    case DLL_PROCESS_ATTACH:
        CreateThread( NULL, NULL, (LPTHREAD_START_ROUTINE)mymain, NULL, NULL, NULL );
        CreateThread( NULL, NULL, (LPTHREAD_START_ROUTINE)KeyHook, NULL, NULL, NULL );
        break;
    }

    return true;
}