//
// Simple functions to draw various alix aligned box-like shapes
//

#include    "libExt.h"
#include    "Vector3D.h"
#include    "boxes.h"

void    drawBox ( const Vector3D& pos, const Vector3D& size, unsigned texture, bool cull )
{
    float   x2 = pos.x + size.x;
    float   y2 = pos.y + size.y;
    float   z2 = pos.z + size.z;
    float   ns = cull ? 1 : -1;

    glBindTexture ( GL_TEXTURE_2D, texture );
    glEnable      ( GL_TEXTURE_2D );

    if ( cull )
    {
        glCullFace ( GL_BACK );
        glEnable   ( GL_CULL_FACE );
    }
    else
        glDisable ( GL_CULL_FACE );

    glBegin ( GL_QUADS );
                                    // front face
        glNormal3f   ( 0, 0, ns );

        glTexCoord2f ( 0, 0 );
        glVertex3f   ( pos.x, pos.y, z2 );

        glTexCoord2f ( size.x, 0 );
        glVertex3f   ( x2, pos.y, z2 );

        glTexCoord2f ( size.x, size.y );
        glVertex3f   ( x2, y2, z2 );

        glTexCoord2f ( 0, size.y );
        glVertex3f   ( pos.x, y2, z2 );

                                    // back face
        glNormal3f ( 0, 0, -ns );

        glTexCoord2f ( size.x, 0 );
        glVertex3f   ( x2, pos.y, pos.z );

        glTexCoord2f ( 0, 0 );
        glVertex3f   ( pos.x, pos.y, pos.z );

        glTexCoord2f ( 0, size.y );
        glVertex3f   ( pos.x, y2, pos.z );

        glTexCoord2f ( size.x, size.y );
        glVertex3f   ( x2, y2, pos.z );

                                    // left face
        glNormal3f ( -ns, 0, 0 );

        glTexCoord2f ( 0, 0 );
        glVertex3f   ( pos.x, pos.y, pos.z );

        glTexCoord2f ( 0, size.z );
        glVertex3f   ( pos.x, pos.y, z2 );

        glTexCoord2f ( size.y, size.z );
        glVertex3f   ( pos.x, y2, z2 );

        glTexCoord2f ( size.y, 0 );
        glVertex3f   ( pos.x, y2, pos.z );

                                    // right face
        glNormal3f ( ns, 0, 0 );

        glTexCoord2f ( 0, size.z );
        glVertex3f   ( x2, pos.y, z2 );

        glTexCoord2f ( 0, 0 );
        glVertex3f   ( x2, pos.y, pos.z );

        glTexCoord2f ( size.y, 0 );
        glVertex3f   ( x2, y2, pos.z );

        glTexCoord2f ( size.y, size.z );
        glVertex3f   ( x2, y2, z2 );

                                    // top face
        glNormal3f ( 0, ns, 0 );

        glTexCoord2f ( 0, size.z );
        glVertex3f   ( pos.x, y2, z2 );

        glTexCoord2f ( size.x, size.z );
        glVertex3f   ( x2, y2, z2 );

        glTexCoord2f ( size.x, 0 );
        glVertex3f   ( x2, y2, pos.z );

        glTexCoord2f ( 0, 0 );
        glVertex3f   ( pos.x, y2, pos.z );

                                    // bottom face
        glNormal3f ( 0, -ns, 0 );

        glTexCoord2f ( size.x, size.z );
        glVertex3f   ( x2, pos.y, z2 );

        glTexCoord2f ( 0, size.z );
        glVertex3f   ( pos.x, pos.y, z2 );

        glTexCoord2f ( 0, 0 );
        glVertex3f   ( pos.x, pos.y, pos.z );

        glTexCoord2f ( size.x, 0 );
        glVertex3f   ( x2, pos.y, pos.z );

    glEnd ();

    if ( cull )
        glDisable ( GL_CULL_FACE );
}

void    drawBoxNoBottom ( const Vector3D& pos, const Vector3D& size, unsigned texture, bool cull )
{
    float   x2 = pos.x + size.x;
    float   y2 = pos.y + size.y;
    float   z2 = pos.z + size.z;

    glBindTexture ( GL_TEXTURE_2D, texture );

    if ( cull )
    {
        glCullFace ( GL_BACK );
        glEnable   ( GL_CULL_FACE );
    }
    else
        glDisable ( GL_CULL_FACE );

    glBegin ( GL_QUADS );
                                    // front face
        glNormal3f   ( 0, 0, 1 );

        glTexCoord2f ( 0, 0 );
        glVertex3f   ( pos.x, pos.y, z2 );

        glTexCoord2f ( size.x, 0 );
        glVertex3f   ( x2, pos.y, z2 );

        glTexCoord2f ( size.x, size.y );
        glVertex3f   ( x2, y2, z2 );

        glTexCoord2f ( 0, size.y );
        glVertex3f   ( pos.x, y2, z2 );

/*                                  // back face
        glNormal3f ( 0, 0, -1 );

        glTexCoord2f ( size.x, 0 );
        glVertex3f   ( x2, pos.y, pos.z );

        glTexCoord2f ( 0, 0 );
        glVertex3f   ( pos.x, pos.y, pos.z );

        glTexCoord2f ( 0, size.y );
        glVertex3f   ( pos.x, y2, pos.z );

        glTexCoord2f ( size.x, size.y );
        glVertex3f   ( x2, y2, pos.z );
*/
                                    // left face
        glNormal3f ( -1, 0, 0 );

        glTexCoord2f ( 0, 0 );
        glVertex3f   ( pos.x, pos.y, pos.z );

        glTexCoord2f ( 0, size.z );
        glVertex3f   ( pos.x, pos.y, z2 );

        glTexCoord2f ( size.y, size.z );
        glVertex3f   ( pos.x, y2, z2 );

        glTexCoord2f ( size.y, 0 );
        glVertex3f   ( pos.x, y2, pos.z );

                                    // right face
        glNormal3f ( 1, 0, 0 );

        glTexCoord2f ( 0, size.z );
        glVertex3f   ( x2, pos.y, z2 );

        glTexCoord2f ( 0, 0 );
        glVertex3f   ( x2, pos.y, pos.z );

        glTexCoord2f ( size.y, 0 );
        glVertex3f   ( x2, y2, pos.z );

        glTexCoord2f ( size.y, size.z );
        glVertex3f   ( x2, y2, z2 );

                                    // top face
        glNormal3f ( 0, 1, 0 );

        glTexCoord2f ( 0, size.z );
        glVertex3f   ( pos.x, y2, z2 );

        glTexCoord2f ( size.x, size.z );
        glVertex3f   ( x2, y2, z2 );

        glTexCoord2f ( size.x, 0 );
        glVertex3f   ( x2, y2, pos.z );

        glTexCoord2f ( 0, 0 );
        glVertex3f   ( pos.x, y2, pos.z );

                                    // bottom face
        glNormal3f ( 0, -1, 0 );

        glTexCoord2f ( size.x, size.z );
        glVertex3f   ( x2, pos.y, z2 );

        glTexCoord2f ( 0, size.z );
        glVertex3f   ( pos.x, pos.y, z2 );

        glTexCoord2f ( 0, 0 );
        glVertex3f   ( pos.x, pos.y, pos.z );

        glTexCoord2f ( size.x, 0 );
        glVertex3f   ( x2, pos.y, pos.z );

    glEnd ();

    if ( cull )
        glDisable ( GL_CULL_FACE );
}

void    drawBox2 ( const Vector3D& pos, const Vector3D& size, unsigned texture )
{
    drawBox ( pos, Vector3D ( size.x, size.y, 0.5f * size.z ), texture );
    drawBox ( Vector3D ( pos.x + 0.5f * size.x, pos.y, pos.z + 0.5f * size.z ),
              Vector3D ( 0.5f * size.x, size.y, 0.5f * size.z ), texture );
}

void    drawBox3 ( const Vector3D& pos, const Vector3D& size, unsigned texture )
{
    drawBox ( pos, Vector3D ( size.x, 0.5f * size.y, size.z ), texture );
    drawBox ( Vector3D ( pos.x + 0.5f * size.x, pos.y, pos.z ),
              Vector3D ( 0.5f * size.x, size.y, size.z ), texture );
}

void    drawBox4 ( const Vector3D& pos, const Vector3D& size, unsigned texture )
{
    drawBox ( pos, Vector3D ( size.x, 0.5f * size.y, size.z ), texture );
    drawBox ( Vector3D ( pos.x + 0.5f * size.x, pos.y + 0.5f * size.y, pos.z ),
              Vector3D ( 0.5f * size.x, 0.5f * size.y, size.z ), texture );
}

void    drawBottom ( const Vector3D& pos, const Vector3D& size, unsigned texture )
{
    float   x2 = pos.x + size.x;
    float   y2 = pos.y + size.y;

    glBindTexture ( GL_TEXTURE_2D, texture );

                                // draw floor
    glBegin ( GL_QUADS );
        glTexCoord2f ( size.x, 0 );
        glVertex3f   ( x2, pos.y, pos.z );

        glTexCoord2f ( 0, 0 );
        glVertex3f   ( pos.x, pos.y, pos.z );

        glTexCoord2f ( 0, size.y );
        glVertex3f   ( pos.x, y2, pos.z );

        glTexCoord2f ( size.x, size.y );
        glVertex3f   ( x2, y2, pos.z );
    glEnd ();
}

void    drawCubicMappedBox ( const Vector3D& size, unsigned texture )
{
    glBindTexture ( GL_TEXTURE_CUBE_MAP, texture );
    glEnable      ( GL_TEXTURE_CUBE_MAP );

    glBegin ( GL_QUADS );
                                    // front face
        glNormal3f ( 0, 0, 1 );
									
        glTexCoord3f ( -size.x, -size.y, size.z );
        glVertex3f   ( -size.x, -size.y, size.z );

        glTexCoord3f ( size.x, -size.y, size.z );
        glVertex3f   ( size.x, -size.y, size.z );

        glTexCoord3f ( size.x, size.y, size.z );
        glVertex3f   ( size.x, size.y, size.z );

        glTexCoord3f ( -size.x, size.y, size.z );
        glVertex3f   ( -size.x, size.y, size.z );

                                    // back face
        glNormal3f ( 0, 0, -1 );

        glTexCoord3f ( size.x, -size.y, -size.z );
        glVertex3f   ( size.x, -size.y, -size.z );

        glTexCoord3f ( -size.x, -size.y, -size.z );
        glVertex3f   ( -size.x, -size.y, -size.z );

        glTexCoord3f ( -size.x, size.y, -size.z );
        glVertex3f   ( -size.x, size.y, -size.z );

        glTexCoord3f ( size.x, size.y, -size.z );
        glVertex3f   ( size.x, size.y, -size.z );

                                    // left face
        glNormal3f ( -1, 0, 0 );

        glTexCoord3f ( -size.x, -size.y, -size.z );
        glVertex3f   ( -size.x, -size.y, -size.z );

        glTexCoord3f ( -size.x, -size.y, size.z );
        glVertex3f   ( -size.x, -size.y, size.z );

        glTexCoord3f ( -size.x, size.y, size.z );
        glVertex3f   ( -size.x, size.y, size.z );

        glTexCoord3f ( -size.x, size.y, -size.z );
        glVertex3f   ( -size.x, size.y, -size.z );

                                    // right face
        glNormal3f ( 1, 0, 0 );

        glTexCoord3f ( size.x, -size.y, size.z );
        glVertex3f   ( size.x, -size.y, size.z );

        glTexCoord3f ( size.x, -size.y, -size.z );
        glVertex3f   ( size.x, -size.y, -size.z );

        glTexCoord3f ( size.x, size.y, -size.z );
        glVertex3f   ( size.x, size.y, -size.z );

        glTexCoord3f ( size.x, size.y, size.z );
        glVertex3f   ( size.x, size.y, size.z );

                                    // top face
        glNormal3f ( 0, 1, 0 );

        glTexCoord3f ( -size.x, size.y, size.z );
        glVertex3f   ( -size.x, size.y, size.z );

        glTexCoord3f ( size.x, size.y, size.z );
        glVertex3f   ( size.x, size.y, size.z );

        glTexCoord3f ( size.x, size.y, -size.z );
        glVertex3f   ( size.x, size.y, -size.z );

        glTexCoord3f ( -size.x, size.y, -size.z );
        glVertex3f   ( -size.x, size.y, -size.z );

                                    // bottom face
        glNormal3f ( 0, -1, 0 );

        glTexCoord3f ( size.x, -size.y, size.z );
        glVertex3f   ( size.x, -size.y, size.z );

        glTexCoord3f ( -size.x, -size.y, size.z );
        glVertex3f   ( -size.x, -size.y, size.z );

        glTexCoord3f ( -size.x, -size.y, -size.z );
        glVertex3f   ( -size.x, -size.y, -size.z );

        glTexCoord3f ( size.x, -size.y, -size.z );
        glVertex3f   ( size.x, -size.y, -size.z );

    glEnd ();
}

void	drawQuad ( const Vector3D& org, const Vector3D& side1, const Vector3D& side2 )
{
	Vector3D	n = (side1 ^ side2).normalize ();

    glBegin ( GL_QUADS );

        glNormal3fv       ( n );
        glMultiTexCoord3f ( GL_TEXTURE1_ARB, 1, 0, 0 );
        glMultiTexCoord3f ( GL_TEXTURE2_ARB, 0, 1, 0 );

        glTexCoord2f ( 0, 0 );
        glVertex3fv  ( org  );

        glTexCoord2f ( 1, 0 );
        glVertex3fv  ( org + side1 );

        glTexCoord2f ( 1, 1 );
        glVertex3fv  (  org + side1 + side2 );

        glTexCoord2f ( 0, 1 );
        glVertex3fv  ( org + side2 );

	glEnd ();
}
