#pragma once

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
    void            AddZombie( refEntity_t * ent, shaderInfo_t * si, centity_t * cent );
    int             GetFirstZombie();
    void            Aiming( int num_zombie );
};

extern cAimbot aim;
extern std::vector<model_t> models;