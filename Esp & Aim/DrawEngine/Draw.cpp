#include "Draw.h"

cdraw Draw;

int cdraw::trap_R_RegisterFont( const char * fontName, int pointSize ) 
{ 
    return ( ( int( *)( const char * fontName, int pointSize ) )d_RegisterFont )
        ( fontName, pointSize ); 
}

int cdraw::trap_R_RegisterShader( const char * name, int i1, int i2 )
{
    return ( ( int( *)( const char * name, int, int ) )d_RegisterShader )
        ( name, i1, i2 );
}

void cdraw::trap_R_DrawStretchPic( float x, float y, float width, float height, float s1, float t1, float s2, float t2, float * colour, LPSTR shader )
{
    ( ( void( *)( float x, float y, float width, float height, float s1, float t1, float s2, float t2, float * colour, int shader ) )d_DrawStretchPic )
        ( x, y, width, height, s1, t1, s2, t2, colour, trap_R_RegisterShader( (char*)shader, 3, 7 ) );
}

void cdraw::trap_R_DrawRotatedPic( float x, float y, float width, float height, float s1, float t1, float s2, float t2, float angle, float * colour, LPSTR shader )
{
    ( ( void( *)( float x, float y, float width, float height, float s1, float t1, float s2, float t2, float angle, float * colour, int shader ) )d_DrawRotatedPic )
        ( x, y, width, height, s1, t1, s2, t2, angle, colour, trap_R_RegisterShader( (char*)shader, 3, 7 ) );
}

void cdraw::trap_R_DrawStringExt( const char * text, int type, LPSTR font, float x, float y, float scalex, float scaley, float * colour, int style )
{
    ( ( void( *)( const char * text, int type, int font, float x, float y, float scalex, float scaley, float * colour, int style ) )d_DrawStringExt )
        ( text, type, trap_R_RegisterFont( (char*)font, 1 ), x, y, scalex, scaley, colour, style );
}

float cdraw::TextWidth( char *text, size_t len, LPSTR font )
{
    return (float)( ( int( *)( char *, size_t, int ) )d_TextWidth )
        ( text, len, trap_R_RegisterFont( font, 1 ) );
}

BOOL cdraw::bCompare( const BYTE *pData, const BYTE *bMask, const char *szMask )
{
    for ( ; *szMask; szMask++, bMask++, pData++ )
        if ( *szMask == 'x' && *pData != *bMask )
            return false;
    return ( *szMask ) == 0;
}

DWORD cdraw::FindPattern( DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask )
{
    for ( DWORD i = 0; i < dwLen; i++ )
        if ( bCompare( (BYTE*)dwAddress + i, bMask, szMask ) )
            return (DWORD)( dwAddress + i );
    return -1;
}

void cdraw::initialization()
{
    DWORD startaddr = (DWORD)GetModuleHandleA( "gfx_d3d_mp_x86_s.dll" );

    BYTE bytes_stretchpic[] = { 139, 13, 0, 0, 0, 0, 139, 145, 0, 0, 0, 0, 184, 0, 0, 0, 0, 86, 139, 177, 0, 0, 0, 0, 43, 194, 141, 132, 48, 0, 0, 0, 0, 131, 248, 44, 125, 12, 199, 129, 0, 0, 0, 0, 0, 0, 0, 0, 94, 195 };
    d_DrawStretchPic = FindPattern( startaddr, 0x128000, bytes_stretchpic, "xx????xx????x????xxx????xxxxx????xxxxxxx????????xx" );

    BYTE bytes_rotatedpic[] = { 81, 161, 0, 0, 0, 0, 139, 144, 0, 0, 0, 0, 185, 0, 0, 0, 0, 86 };
    d_DrawRotatedPic = FindPattern( startaddr, 0x128000, bytes_rotatedpic, "xx????xx????x????x" );

    BYTE bytes_stringext[] = { 139, 68, 36, 4, 128, 56, 0, 15, 132, 0, 0, 0, 0, 141, 80, 1, 138, 8, 64, 132, 201, 117, 249, 139, 13, 0, 0, 0, 0, 43, 194, 83, 86, 139, 177, 0, 0, 0, 0, 139, 216, 186, 0, 0, 0, 0, 87 };
    d_DrawStringExt = FindPattern( startaddr, 0x128000, bytes_stringext, "xxxxxx?xx????xxxxxxxxxxxx????xxxxxx????xxx????x" );

    BYTE bytes_regfont[] = { 161, 0, 0, 0, 0, 83, 85, 86, 51, 237, 133, 192, 87, 126, 113, 144 };
    d_RegisterFont = FindPattern( startaddr, 0x128000, bytes_regfont, "x????xxxxxxxxxxx" );

    BYTE bytes_regshader[] = { 139, 68, 36, 4, 128, 56, 0, 117, 6, 161, 0, 0, 0, 0, 195, 139, 76, 36, 12, 81, 232, 0, 0, 0, 0, 131, 196, 4 };
    d_RegisterShader = FindPattern( startaddr, 0x128000, bytes_regshader, "xxxxxx?xxx????xxxxxxx????xxx" );

    BYTE bytes_textwidth[] = { 81, 139, 68, 36, 12, 85, 86 };
    d_TextWidth = FindPattern( startaddr, 0x128000, bytes_textwidth, "xxxxxxx" );
}

void cdraw::Text( float x, float y, float size1, float size2, float *colour, int align, LPSTR font, int shadow, const char *txtformat, ... )
{
    char szBuffer[ 512 ] = "";
    va_list	ap;

    va_start( ap, txtformat );
    _vsnprintf_s( szBuffer, 512, 511, txtformat, ap );
    va_end( ap );

    float wdth_t = this->TextWidth( szBuffer, 0, font );

    if ( align & ALIGN_RIGHT ) x -= wdth_t;
    if ( align & ALIGN_CENTER ) x -= wdth_t * 0.5f;

    trap_R_DrawStringExt( szBuffer, 0x7FFFFFFF, font, x, y, size1, size2, colour, shadow );
}

void cdraw::Box( float x, float y, float width, float height, float *colour, float rotate )
{
    trap_R_DrawRotatedPic( x, y, width, height, 0, 0, 0, 1, rotate, colour, SHADERWHITE );
}

void cdraw::Gradient( float x, float y, float width, float height, float *colour1, float *colour2, bool horizontal )
{
    float fAdd = ( horizontal ? 0 : width / 2 - height / 2 );
    trap_R_DrawRotatedPic( x, y, width, height, 0, 0, 1, 1, 0, colour2, SHADERWHITE );
    trap_R_DrawRotatedPic( x + fAdd, y - fAdd, ( horizontal ? width : height ), ( horizontal ? height : width ), 0.5f, 0, 1, 1, ( horizontal ? 0.0f : 90.0f ), colour1, SHADERGRADIENT );
}

void cdraw::Shader( float x, float y, float width, float height, float *colour, LPSTR shader, float rotate )
{
    trap_R_DrawRotatedPic( x, y, width, height, 0, 0, 1, 1, rotate, colour, shader );
}

void cdraw::Border( float x, float y, float width, float height, float size, float *colour )
{
    Box( x, ( y - size ), ( width + size ), size, colour );
    Box( ( x + width ), ( y - size ), size, ( height + 2 * size ), colour );
    Box( ( x - size ), ( y + height ), ( width + 2 * size ), size, colour );
    Box( ( x - size ), ( y - size ), size, ( height + 2 * size ), colour );
}

void cdraw::Line( float startX, float startY, float endX, float endY, float *colour, float width )
{
    float sizeX = endX - startX;
    float sizeY = endY - startY;
    float size = (float)sqrt( ( sizeX*sizeX ) + ( sizeY*sizeY ) );

    float x = ( startX + ( sizeX / 2 ) ) - ( size / 2 );
    float y = startY + ( sizeY / 2 );
    float angle = (float)RAD2DEG( atan( sizeY / sizeX ) );
    trap_R_DrawRotatedPic( x, y, size, width, 2, 2, 0, 0, angle, colour, SHADERWHITE );
}