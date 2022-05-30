#include "dllmain.h"

vec3_t vec3_origin = { 0.0f, 0.0f, 0.0f };

int CG_Trace( trace_t *result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int skipNumber, int mask )
{
    return ( ( int( *)( trace_t *result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int skipNumber, int mask ) )0x004DE690 )( result, start, mins, maxs, end, skipNumber, mask );
}

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
    VectorSubtract( origin, set.cg->refdef.vieworg, entity );
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