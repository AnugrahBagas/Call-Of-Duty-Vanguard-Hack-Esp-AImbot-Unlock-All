// =====================================================
// Header
// =====================================================
#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

#define VectorCopy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define VectorSub(a,b,c)		((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define RAD2DEG( a )			( ( (a) * 180.0f ) / M_PI )

#define	PITCH					0		// up / down
#define	YAW						1		// left / right
#define	ROLL					2		// fall over
#define M_PI					3.14159265358979323846f
#define	ANGLE2SHORT(x)			((int)((x)*65536/360) & 65535)

#define	MASK_SHOT				(0x02802091)//(CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE)
#define RT_MODEL				0

#define CG_Trace ( ( int( *)( trace_t *result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int skipNumber, int mask ) )0x004DE690 )
#define RegisterEnum ( ( dvar_t * ( * )( const char * var_name, enum_type enum_values[], int initial, int flags ) )0x004383A0 )
#define CmdConsole  ( ( void (*)( char * ) )0x004214C0 )

typedef float vec3_t[ 3 ];
typedef int qhandle_t;
typedef int qbool;
typedef const char *enum_type;

typedef struct
{
    float		fraction;
    int			pad1[ 9 ];
} trace_t;

typedef struct
{
    int reType;
    int pad1[ 14 ];
    vec3_t origin;
    int pad2[ 11 ];
} refEntity_t;

typedef struct
{
    int render[ 7 ];
    qhandle_t *model;
    int pad2[ 11 ];
    vec3_t point1;
    vec3_t point2;
    int pad3[ 5 ];
} shaderInfo_t;

typedef struct
{
    int ServerTime;
    int Buttons;
    byte weapon;
    char pad1[ 3 ];
    int ViewAngle[ 3 ];
    byte forwardmove;
    byte rightmove;
    char pad2[ 2 ];
} userCmd_t; //Size=28, dw=1C

typedef struct
{
    userCmd_t usercmds[ 128 ];
    int currentCmdNum;
    userCmd_t *GetUserCmd( int cmdNum )
    {
        int id = cmdNum & 0x7F;
        return &usercmds[ id ];
    }
} input_t;

typedef struct
{
    int pad0[ 6 ];
    vec3_t vieworg;
    int pas1[ 13 ];
} refdef_t;

typedef struct
{
    int padding00[ 42 ];
    int clientNum;
    int pading01[ 2422 ];
} playerState_t;

typedef struct
{
    int padding00[ 12 ];
    playerState_t	ps;
    int	entities[ 61442 ];
} snapshot_t;

typedef struct
{
    int padding01[ 9 ];
    snapshot_t * snap;
    int padding03[ 38631 ];
    playerState_t predictedPlayerState;
    int padding5[ 202 ];
    refdef_t refdef; //22
    vec3_t refdefViewAngles;
    int padding4[ 209139 ];
} cg_t;

typedef struct
{
    refEntity_t * ent;
    shaderInfo_t * si;
    bool visible;
    int fov_x;
    int fov_y;
    float distance;
    vec3_t aimangles;
} BotTarget_t;

typedef struct
{
    refEntity_t ent;
    shaderInfo_t si;
} model_t;

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

typedef struct
{
    DWORD gfx_dll;
    float *viewangles;
    cg_t *cg;
    input_t *input;
    bool Aimkeypress;
    bool Shoot;
} set_t;

typedef void( *oStereoFrame ) ( );
typedef int( WINAPIV *t_AddRefEntityToScene )( refEntity_t * ent, shaderInfo_t * si, int * cent );

class cAimbot
{
private:
    static bool     SortMinDist( BotTarget_t lhs, BotTarget_t rhs );
    static bool     SortMaxDist( BotTarget_t lhs, BotTarget_t rhs );
    static bool     SortMinFov( BotTarget_t lhs, BotTarget_t rhs );

    void            SilentAim( int num_zombie );
    void            AutoAim();

    vec3_t          aimangles;
    bool            findzom;
public:
    void            AddZombie( refEntity_t * ent, shaderInfo_t * si );
    int             GetFirstZombie();
    void            Aiming( int num_zombie );
};

// =====================================================
// Source
// =====================================================
set_t set;
oStereoFrame pStereoFrame;
t_AddRefEntityToScene o_AddRefEntityToScene;

enum_type aimtype[] = { "Off", "Auto aim", "Silent aim", NULL };
enum_type aimkey[] = { "Off", "Left mouse", "Right Mouse", "Shift", "Alt", NULL };
enum_type aimfilter[] = { "Min distance", "Max distance", "Min FOV", NULL };

vec3_t vec3_origin = { 0.0f, 0.0f, 0.0f };

dvar_t *dv_aimbot;
dvar_t *dv_aimbot_fov;
dvar_t *dv_aimbot_key;
dvar_t *dv_aimbot_filter;
dvar_t *dv_aimbot_norecoil;
dvar_t *dv_aimbot_autoshoot;

cAimbot aim;
std::vector<BotTarget_t> Targets;
std::vector<model_t> models;

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

// =====================================================
// Memory
// =====================================================
static void MemoryWrite( void *adr, void *ptr, int size )
{
	DWORD OldProtection;
	VirtualProtect( adr, size, PAGE_EXECUTE_READWRITE, &OldProtection );
	memcpy( adr, ptr, size );
	VirtualProtect( adr, size, OldProtection, &OldProtection );
}

static void * DetourCreate( BYTE *src, const BYTE *dst, const int len )
{
	BYTE *jmp = (BYTE*)malloc( len + 5 );
	DWORD dwback;

	VirtualProtect( src, len, PAGE_READWRITE, &dwback );

	memcpy( jmp, src, len ); jmp += len;

	jmp[ 0 ] = 0xE9;
	*(DWORD*)( jmp + 1 ) = (DWORD)( src + len - jmp ) - 5;

	src[ 0 ] = 0xE9;
	*(DWORD*)( src + 1 ) = (DWORD)( dst - src ) - 5;

	VirtualProtect( src, len, dwback, &dwback );

	return ( jmp - len );
}

// =====================================================
// Dvars
// =====================================================
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

// =====================================================
// Math
// =====================================================
bool IsVisible( vec3_t point )
{
    trace_t t;

    CG_Trace( &t, set.cg->refdef.vieworg, vec3_origin, vec3_origin, point, set.cg->snap->ps.clientNum, MASK_SHOT );

    return ( t.fraction == 1.0f );
}

float GetDistance( vec3_t origin )
{
    vec3_t vector;

    VectorSub( origin, set.cg->refdef.vieworg, vector );
    return ( sqrt( vector[ 0 ] * vector[ 0 ] + vector[ 1 ] * vector[ 1 ] + vector[ 2 ] * vector[ 2 ] ) / 48 );
}

void vectoangles( const vec3_t value1, vec3_t angles )
{
    float forward, yaw, pitch;

    if ( value1[ 0 ] == 0 && value1[ 1 ] == 0 )
    {
        yaw = 0;

        if ( value1[ 2 ] > 0 )
            pitch = 90;
        else
            pitch = 270;
    }
    else
    {
        if ( value1[ 0 ] )
            yaw = RAD2DEG( atan2( value1[ 1 ], value1[ 0 ] ) );
        else if ( value1[ 1 ] > 0 )
            yaw = 90.0f;
        else
            yaw = 270.0f;

        if ( yaw < 0.0f )
            yaw += 360.0f;

        forward = sqrt( ( value1[ 0 ] * value1[ 0 ] ) + ( value1[ 1 ] * value1[ 1 ] ) );
        pitch = RAD2DEG( atan2( value1[ 2 ], forward ) );

        if ( pitch < 0.0f )
            pitch += 360;
    }

    angles[ PITCH ] = -pitch;
    angles[ YAW ] = yaw;
    angles[ ROLL ] = 0;
}

void GetAngleToOrigin( vec3_t origin, vec3_t angles )
{
    vec3_t entity;
    VectorSub( origin, set.cg->refdef.vieworg, entity );
    vectoangles( entity, angles );

    if ( angles[ PITCH ] > 180.0f )				angles[ PITCH ] -= 360.0f;
    else if ( angles[ PITCH ] < -180.0f )		angles[ PITCH ] += 360.0f;
    if ( angles[ YAW ] > 180.0f )				angles[ YAW ] -= 360.0f;
    else if ( angles[ YAW ] < -180.0f )		angles[ YAW ] += 360.0f;
    angles[ PITCH ] -= set.cg->refdefViewAngles[ PITCH ];
    angles[ YAW ] -= set.cg->refdefViewAngles[ YAW ];
    if ( angles[ PITCH ] > 180.0f )			angles[ PITCH ] -= 360.0f;
    else if ( angles[ PITCH ] < -180.0f )		angles[ PITCH ] += 360.0f;
    if ( angles[ YAW ] > 180.0f )				angles[ YAW ] -= 360.0f;
    else if ( angles[ YAW ] < -180.0f )		angles[ YAW ] += 360.0f;
}

// =====================================================
// Aimbot
// =====================================================
bool cAimbot::SortMinDist( BotTarget_t lhs, BotTarget_t rhs )
{
    return ( lhs.distance < rhs.distance );
}

bool cAimbot::SortMaxDist( BotTarget_t lhs, BotTarget_t rhs )
{
    return ( lhs.distance > rhs.distance );
}

bool cAimbot::SortMinFov( BotTarget_t lhs, BotTarget_t rhs )
{
    float fLHS = ( lhs.aimangles[ 0 ] * lhs.aimangles[ 0 ] ) + ( lhs.aimangles[ 1 ] * lhs.aimangles[ 1 ] );
    float fRHS = ( rhs.aimangles[ 0 ] * rhs.aimangles[ 0 ] ) + ( rhs.aimangles[ 1 ] * rhs.aimangles[ 1 ] );

    return ( fLHS < fRHS );
}

void cAimbot::AddZombie( refEntity_t * ent, shaderInfo_t * si )
{
    BotTarget_t zom;

    zom.ent = ent;
    zom.si = si;

    vec3_t origin;
    VectorCopy( zom.ent->origin, origin );
    origin[ 2 ] += zom.si->point2[ 2 ] - zom.si->point2[ 2 ] / 3;

    GetAngleToOrigin( origin, zom.aimangles );

    zom.distance = GetDistance( origin );

    if ( IsVisible( origin ) )
    {
        zom.visible = true;
        findzom = true;
    }
    else
    {
        zom.visible = false;
    }

    zom.fov_x = (int)fabs( zom.aimangles[ PITCH ] ) * 2;
    zom.fov_y = (int)fabs( zom.aimangles[ YAW ] ) * 2;

    Targets.push_back( zom );
}

int cAimbot::GetFirstZombie()
{
    if ( !dv_aimbot_filter || !findzom )
        return -1;

    switch ( dv_aimbot_filter->iValue )
    {
    case 0: // Min distance
        std::sort( Targets.begin(), Targets.end(), SortMinDist );
        break;
    case 1:	// Max distance
        std::sort( Targets.begin(), Targets.end(), SortMaxDist );
        break;
    case 2: // Min angle
        std::sort( Targets.begin(), Targets.end(), SortMinFov );
        break;
    };

    for ( size_t i = 0; i < Targets.size(); i++ )
    {
        if ( !Targets[ i ].visible )
            continue;

        aimangles[ PITCH ] = Targets[ i ].aimangles[ PITCH ];
        aimangles[ YAW ] = Targets[ i ].aimangles[ YAW ];
        aimangles[ ROLL ] = Targets[ i ].aimangles[ ROLL ];

        return i;
    }

    return -1;
}

void cAimbot::AutoAim()
{
    set.viewangles[ PITCH ] += aimangles[ PITCH ];
    set.viewangles[ YAW ] += aimangles[ YAW ];
    set.viewangles[ ROLL ] += aimangles[ ROLL ];

    if ( dv_aimbot_autoshoot && dv_aimbot_autoshoot->iValue == 1 )
        set.Shoot = true;
}

void cAimbot::SilentAim( int num_player )
{
    userCmd_t *cmd = set.input->GetUserCmd( set.input->currentCmdNum );
    userCmd_t *oldcmd = set.input->GetUserCmd( set.input->currentCmdNum - 1 );
    *oldcmd = *cmd;
    cmd->ServerTime++;
    set.input->currentCmdNum++;

    cmd->ViewAngle[ 0 ] += ANGLE2SHORT( aimangles[ 0 ] );
    cmd->ViewAngle[ 1 ] += ANGLE2SHORT( aimangles[ 1 ] );
    cmd->ViewAngle[ 2 ] += ANGLE2SHORT( aimangles[ 2 ] );

    if ( dv_aimbot_autoshoot && dv_aimbot_autoshoot->iValue == 1 )
        set.Shoot = true;
}

void cAimbot::Aiming( int num_player )
{
    if ( num_player < 0 || !dv_aimbot || dv_aimbot->iValue == 0 )
        return;

    if ( ( dv_aimbot_key->iValue > 0 && set.Aimkeypress ) || !dv_aimbot_key->iValue )
    {
        switch ( dv_aimbot->iValue )
        {
        case 1:
            AutoAim();
            break;
        case 2:
            SilentAim( num_player );
            break;
        }
    }

    Targets.clear();
    findzom = false;
}

// =====================================================
// Detours
// =====================================================
int RE_AddRefEntityToScene( refEntity_t * ent, shaderInfo_t * si, int * cent )
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

                    models.push_back( model );
                }
            }
        }
    }

    return o_AddRefEntityToScene( ent, si, cent );
}

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
                aim.AddZombie( &models[ i ].ent, &models[ i ].si );

            int num = aim.GetFirstZombie();
            aim.Aiming( num );
            models.clear();
        }
    }
}

// =====================================================
// Main
// =====================================================
void __stdcall mymain()
{
	while ( GetModuleHandleA( "gfx_d3d_mp_x86_s.dll" ) == 0 || GetModuleHandleA( "mssmp3.asi" ) == 0 || GetModuleHandleA( "d3d9.dll" ) == 0 )
		Sleep( 100 );

	set.gfx_dll = (DWORD)LoadLibraryA( "gfx_d3d_mp_x86_s.dll" );
	set.cg = (cg_t*)0x014EE080;
	set.viewangles = (float *)0x0098FDEC;
	set.input = (input_t *)0x009CFE2C;

	register_dvars();

	o_AddRefEntityToScene = (t_AddRefEntityToScene)DetourCreate( (PBYTE)( set.gfx_dll + 0x241A0 ), (PBYTE)RE_AddRefEntityToScene, 5 );
    pStereoFrame = (oStereoFrame)DetourCreate( (PBYTE)( 0x004CBCA0 ), (PBYTE)hStereoFrame, 8 );
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

bool __stdcall DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	switch ( fdwReason )
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls( hinstDLL );

		CreateThread( NULL, NULL, (LPTHREAD_START_ROUTINE)mymain, NULL, NULL, NULL );
		CreateThread( NULL, NULL, (LPTHREAD_START_ROUTINE)KeyHook, NULL, NULL, NULL );
		break;
	}

	return true;
}