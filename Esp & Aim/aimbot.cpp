#include "dllmain.h"

cAimbot aim;
std::vector<BotTarget_t> Targets;
std::vector<model_t> models;

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

void cAimbot::AddZombie( refEntity_t * ent, shaderInfo_t * si, centity_t * cent )
{
    BotTarget_t zom;

    zom.ent = ent;
    zom.si = si;
    zom.cent = cent;

    vec3_t origin;
    VectorCopy( zom.ent->origin, origin );
    origin[ 2 ] += zom.si->point2[ 2 ] - zom.si->point2[2] / 3;

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