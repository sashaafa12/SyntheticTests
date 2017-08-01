//
// Simple wrapper class for OpenGL vertex programs
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#include    "VertexProgram.h"
#include    "Data.h"

ParamArray  VertexProgram :: env ( GL_VERTEX_PROGRAM_ARB );
int         VertexProgram :: activeProgram    = 0;

///////////////////////////// VertexProgram methods /////////////////////////////

VertexProgram :: VertexProgram  () : local ( GL_VERTEX_PROGRAM_ARB )
{
    id          = 0;
    errorCode   = 0;
    errorString = "";
}

VertexProgram :: VertexProgram  ( const char * fileName )  : local ( GL_VERTEX_PROGRAM_ARB )
{
    id = 0;

    load ( fileName );
}

VertexProgram :: ~VertexProgram ()
{
    glDeleteProgramsARB ( 1, &id );
}

bool    VertexProgram :: load ( const char * fileName )
{
    Data    data ( fileName );

    return load ( &data );
}

bool    VertexProgram :: load ( Data * data )
{
    if ( id == 0 )                  // allocate an id for program
        glGenProgramsARB ( 1, &id );

                                    // set correct pointers to functions, because at this time extension
                                    // should be initialized
    local.setPointers ( glProgramLocalParameter4fvARB, glGetProgramLocalParameterfvARB );
    local.setId       ( id );
    env.  setPointers ( glProgramEnvParameter4fvARB, glGetProgramEnvParameterfvARB     );

    if ( !data -> isOk () || data -> isEmpty () )
        return false;

    bind ();

    glProgramStringARB ( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
                         data -> getLength (), data -> getPtr () );

    if ( glGetError () == GL_INVALID_OPERATION )
    {
        glGetIntegerv ( GL_PROGRAM_ERROR_POSITION_ARB, &errorCode );

        errorString = (const char *)glGetString ( GL_PROGRAM_ERROR_STRING_ARB );

        return false;
    }

    errorCode   = 0;
    errorString = "";

    return true;
}

void    VertexProgram :: enable ()
{
    glEnable ( GL_VERTEX_PROGRAM_ARB );
}

void    VertexProgram :: disable ()
{
    glDisable ( GL_VERTEX_PROGRAM_ARB );
}

void    VertexProgram :: bind ()
{
     glBindProgramARB ( GL_VERTEX_PROGRAM_ARB, activeProgram = id );
}

void    VertexProgram :: unbind ()
{
     glBindProgramARB ( GL_VERTEX_PROGRAM_ARB, activeProgram = 0 );
}

//////////////////////////// static query methods ////////////////////////////////////

bool    VertexProgram :: isSupported ()
{
    return isExtensionSupported ( "GL_ARB_vertex_program" );
}

int VertexProgram :: maxVertexAttribs ()
{
    GLint maxVertexAttribs;

    glGetProgramivARB ( GL_VERTEX_PROGRAM_ARB, GL_MAX_VERTEX_ATTRIBS_ARB, &maxVertexAttribs );

    return maxVertexAttribs;
}

int VertexProgram :: maxLocalParams ()
{
    GLint maxLocalParams;

    glGetProgramivARB ( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB, &maxLocalParams   );

    return maxLocalParams;
}

int VertexProgram :: maxEnvParams ()
{
    GLint maxEnvParams;

    glGetProgramivARB ( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_ENV_PARAMETERS_ARB,   &maxEnvParams     );

    return maxEnvParams;
}

int VertexProgram :: maxMatrices ()
{
    GLint maxMatrices;

    glGetProgramivARB ( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_MATRICES_ARB,         &maxMatrices      );

    return maxMatrices;
}

int VertexProgram :: maxTemporaries ()
{
    GLint maxTemporaries;

    glGetProgramivARB ( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_TEMPORARIES_ARB,      &maxTemporaries   );

    return maxTemporaries;
}

int VertexProgram :: maxParams ()
{
    GLint maxParams;

    glGetProgramivARB ( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_PARAMETERS_ARB,       &maxParams        );

    return maxParams;
}

int VertexProgram :: maxAddressRegs ()
{
    GLint maxAddressRegs;

    glGetProgramivARB ( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB, &maxAddressRegs  );

    return maxAddressRegs;
}
