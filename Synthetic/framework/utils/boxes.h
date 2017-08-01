//
// Simple functions to draw various alix aligned box-like shapes
//

#ifndef __BOXES__
#define __BOXES__

void    drawBox            ( const Vector3D& pos, const Vector3D& size, unsigned texture, bool cull = true );
void    drawBoxNoBottom    ( const Vector3D& pos, const Vector3D& size, unsigned texture, bool cull = true );
void    drawBox2           ( const Vector3D& pos, const Vector3D& size, unsigned texture );
void    drawBox3           ( const Vector3D& pos, const Vector3D& size, unsigned texture );
void    drawBox4           ( const Vector3D& pos, const Vector3D& size, unsigned texture );
void    drawBottom         ( const Vector3D& pos, const Vector3D& size, unsigned texture );
void    drawCubicMappedBox ( const Vector3D& size, unsigned texture );
void	drawQuad           ( const Vector3D& org, const Vector3D& side1, const Vector3D& side2 );

#endif
