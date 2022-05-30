#pragma once

#define DotProduct(x,y)			((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
#define VectorSubtract(a,b,c)	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define VectorAdd(a,b,c)		((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define VectorCopy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define	VectorScale(v, s, o)	((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))
#define	VectorMA(v, s, b, o)	((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))
#define VectorSub(a,b,c)		((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define DEG2RAD( a )			( ( (a) * M_PI ) / 180.0F )
#define RAD2DEG( a )			( ( (a) * 180.0f ) / M_PI )

#define	PITCH					0		// up / down
#define	YAW						1		// left / right
#define	ROLL					2		// fall over
#define M_PI					3.14159265358979323846f
#define M_RADPI					57.295779513082f
#define	ANGLE2SHORT(x)			((int)((x)*65536/360) & 65535)

#define	CONTENTS_SOLID				0x00000001
#define	CONTENTS_LIGHTGRID			0x00000004
#define	CONTENTS_LAVA				0x00000008
#define	CONTENTS_SLIME				0x00000010
#define	CONTENTS_WATER				0x00000020
#define	CONTENTS_FOG				0x00000040
#define	CONTENTS_MISSILECLIP		0x00000080
#define CONTENTS_ITEM				0x00000100
#define CONTENTS_BULLETCLIP1		0x00000400
#define CONTENTS_COD2_3				0x00000800
#define CONTENTS_COD2_2				0x00001000
#define CONTENTS_COD2_4				0x00002000
#define CONTENTS_MOVER				0x00004000
#define	CONTENTS_AREAPORTAL			0x00008000
#define	CONTENTS_PLAYERCLIP			0x00010000
#define	CONTENTS_MONSTERCLIP		0x00020000
#define	CONTENTS_TELEPORTER			0x00040000
#define	CONTENTS_JUMPPAD			0x00080000
#define CONTENTS_CLUSTERPORTAL		0x00100000
#define CONTENTS_DONOTENTER			0x00200000
#define CONTENTS_DONOTENTER_LARGE	0x00400000
#define CONTENTS_COD2_1				0x00800000
#define	CONTENTS_ORIGIN				0x01000000	// removed before bsping an entity
#define	CONTENTS_BODY				0x02000000	// should never be on a brush, only in game
#define	CONTENTS_CORPSE				0x04000000
#define	CONTENTS_DETAIL				0x08000000	// brushes not used for the bsp					/* FOG? */

#define	CONTENTS_STRUCTURAL		0x10000000	// brushes used for the bsp
#define	CONTENTS_TRANSLUCENT	0x20000000	// don't consume surface fragments inside
#define	CONTENTS_TRIGGER		0x40000000
#define	CONTENTS_NODROP			0x80000000	// don't leave bodies or items (death fog, lava)

#define	MASK_ALL				(-1)
#define	MASK_SOLID				(CONTENTS_SOLID)
#define	MASK_PLAYERSOLID		(0x02810011)//(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_BODY)
#define	MASK_DEADSOLID			(CONTENTS_SOLID|CONTENTS_PLAYERCLIP)
#define	MASK_WATER				(CONTENTS_WATER|CONTENTS_LAVA|CONTENTS_SLIME)
#define	MASK_OPAQUE				(CONTENTS_SOLID|CONTENTS_SLIME|CONTENTS_LAVA)
#define	MASK_SHOT				(0x02802091)//(CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE)

#define MASK_TARGET				(CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_SLIME|CONTENTS_BULLETCLIP1)/*0x02802411*/

float GetDistance( vec3_t origin );
bool IsVisible( vec3_t point );
void GetAngleToOrigin( vec3_t origin, vec3_t angles );