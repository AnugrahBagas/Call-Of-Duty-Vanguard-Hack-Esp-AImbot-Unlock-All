#pragma once

#define RegisterEnum ( ( dvar_t * ( * )( const char * var_name, enum_type enum_values[], int initial, int flags ) )0x004383A0 )
#define CmdConsole  ( ( void (*)( char * ) )0x004214C0 )

typedef float vec_t;
typedef vec_t vec2_t[ 2 ];
typedef vec_t vec3_t[ 3 ];
typedef vec_t vec4_t[ 4 ];

typedef int qhandle_t;
typedef int qbool;

typedef enum
{
    RT_MODEL,
    RT_POLY,
    RT_SPRITE,
    RT_ORIENTED_QUAD,
    RT_LINE,
    RT_ELECTRICITY,
    RT_CYLINDER,
    RT_LATHE,
    RT_BEAM,
    RT_SABER_GLOW,
    RT_PORTALSURFACE,		// doesn't draw anything, just info for portals
    RT_CLOUDS,

    RT_MAX_REF_ENTITY_TYPE
} refEntityType_t;

typedef struct
{
    float		fraction;
    vec3_t		endpos;
    int			surfaceFlags;
    qbool		allSolid;
    int			contents;
    short		entityNum;
    short		pad0;
    int			pad1;
} trace_t;

typedef struct
{
    refEntityType_t reType;
    int renderfx;
    vec3_t lightingOrigin;
    vec3_t axis[ 3 ];
    float axisScale; //normally 1.0
    vec3_t origin;
    long padding1[ 4 ];
    BYTE shaderRGBA[ 4 ];
    long padding2[ 6 ];
} refEntity_t; //size=116, dw=29

typedef struct
{
    int pad0;
    int pointer;
    int frame;
    int pad1;
    int num;
    int pointer1;
    int render;
    qhandle_t *model;
    int pad2[ 11 ];
    vec3_t point1;
    vec3_t point2;
    int pad3[ 5 ];
} shaderInfo_t;

typedef struct
{
    int		trType;
    int		trTime;
    int		trDuration;
    vec3_t	trBase;
    vec3_t	trDelta;
} trajectory_t;

typedef struct
{
    int				number;
    int				eType;
    int				eFlags;
    trajectory_t	pos;
    trajectory_t	apos;

    int				pad[ 8 ];

    int				otherEntityNum;
    int				otherEntityNum2;

    int				pad4[ 5 ];

    int				clientNum;

    int				pad2[ 2 ];

    int				solid;
    int				eventParm;

    int				pad5[ 9 ];

    int				weapon;
    int				legsAnim;
    int				torsoAnim;

    int				pad3[ 7 ];
} entityState_t; //size=240, dw=60

typedef struct
{
    entityState_t	currentState;
    entityState_t	nextState;
    qbool			currentValid;
    int				pad[ 2 ];
    vec3_t			lerpOrigin;
    vec3_t			lerpAngles;
    int				pad2[ 8 ];
} centity_t; //size=548, dw=137

typedef struct
{
    int			ServerTime;
    int			Buttons;
    byte		weapon;
    char		pad1[ 3 ];
    int			ViewAngle[ 3 ];
    byte		forwardmove;
    byte		rightmove;
    char		pad2[ 2 ];
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
    int				x;
    int				y;
    int				width;
    int				height;
    float			fov_x;
    float			fov_y;
    vec3_t			vieworg;
    vec3_t			viewaxis[ 3 ];
    int				time;
    int				rdflags;
    BYTE			areamask[ 8 ]; //?
} refdef_t; //size=88, dw=22

typedef struct
{
    int			commandTime;	// cmd->serverTime of last executed command
    int			pm_type;
    int			bobCycle;		// for view bobbing and footstep generation
    int			pm_flags;		// ducked, jump_held, etc
    int			pm_time;
    vec3_t		origin;
    vec3_t		velocity;

    int			padding00[ 31 ];

    int			clientNum;

    int			pading01[ 2422 ];
} playerState_t; //size=10404, dw=2465

typedef struct
{
    int				padding00[ 12 ];

    playerState_t	ps;
    int				numEntities;

    int				padding01;

    entityState_t	entities[ 256 ];
} snapshot_t;

typedef struct
{
    int				clientFrame;
    int				clientNum;

    int				padding01[ 7 ];

    snapshot_t *	snap;
    snapshot_t *	nextSnap;

    int				padding02[ 57 ];

    int				sight;

    int				padding03[ 38566 ];

    int				frametime;
    int				time;

    int				padding05[ 4 ];

    playerState_t	predictedPlayerState;

    int				padding5[ 202 ];

    //41308
    refdef_t		refdef; //22
    vec3_t			refdefViewAngles;

    int				padding2[ 3582 ];
    int				crosshairClientNum;
    int				crosshairClientTime;
    int				padding3[ 3 ];
    int				crosshairClientHealth; //?
    int				padding4[ 173 ];

    vec3_t			kick_angles;
    int				padding1[ 205375 ];

    //int				padding1[209139];
} cg_t; //size=1001888, dw=250472