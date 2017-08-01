//
// Simple wrapper class for OpenGL fragment programs
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#include    "FragmentProgram.h"
#include    "Data.h"

ParamArray  FragmentProgram :: env ( GL_FRAGMENT_PROGRAM_ARB );
int         FragmentProgram :: activeProgram    = 0;

///////////////////////////// FragmentProgram methods /////////////////////////////

FragmentProgram :: FragmentProgram  () : local ( GL_FRAGMENT_PROGRAM_ARB )
{
    id          = 0;
    errorCode   = 0;
    errorString = "";
}

FragmentProgram :: FragmentProgram  ( const char * fileName )  : local ( GL_FRAGMENT_PROGRAM_ARB )
{
    id = 0;

    load ( fileName );
}

FragmentProgram :: ~FragmentProgram ()
{
    glDeleteProgramsARB ( 1, &id );
}

bool    FragmentProgram :: load ( const char * fileName )
{
    Data    data ( fileName );

    return load ( &data );
}

bool    FragmentProgram :: load ( Data * data )
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

    glProgramStringARB ( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
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

void    FragmentProgram :: enable ()
{
    glEnable ( GL_FRAGMENT_PROGRAM_ARB );
}

void    FragmentProgram :: disable ()
{
    glDisable ( GL_FRAGMENT_PROGRAM_ARB );
}

void    FragmentProgram :: bind ()
{
     glBindProgramARB ( GL_FRAGMENT_PROGRAM_ARB, activeProgram = id );
}

void    FragmentProgram :: unbind ()
{
     glBindProgramARB ( GL_FRAGMENT_PROGRAM_ARB, activeProgram = 0 );
}

//////////////////////////// static query methods ////////////////////////////////////

bool    FragmentProgram :: isSupported ()
{
    return isExtensionSupported ( "GL_ARB_fragment_program" );
}

int FragmentProgram :: maxAluInstructions ()
{
    GLint maxAluInstructions;

    glGetProgramivARB ( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB, &maxAluInstructions );

    return maxAluInstructions;
}

int FragmentProgram :: maxTexInstructions ()
{
    GLint maxTexInstructions;

    glGetProgramivARB ( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB, &maxTexInstructions );

    return maxTexInstructions;
}

int FragmentProgram :: maxTexIndirections ()
{
    GLint maxTexIndirections;

    glGetProgramivARB ( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB, &maxTexIndirections );

    return maxTexIndirections;
}

int FragmentProgram :: maxInstructions ()
{
    GLint maxInstructions;

    glGetProgramivARB ( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_INSTRUCTIONS_ARB, &maxInstructions );

    return maxInstructions;
}

int FragmentProgram :: maxLocalParams ()
{
    GLint maxLocalParams;

    glGetProgramivARB ( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB, &maxLocalParams );

    return maxLocalParams;
}

int FragmentProgram :: maxEnvParams ()
{
    GLint maxEnvParams;

    glGetProgramivARB ( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ENV_PARAMETERS_ARB, &maxEnvParams );

    return maxEnvParams;
}

int FragmentProgram :: maxTemporaries ()
{
    GLint maxTemporaries;

    glGetProgramivARB ( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEMPORARIES_ARB, &maxTemporaries );

    return maxTemporaries;
}

int FragmentProgram :: maxProgramAttribs ()
{
    GLint maxAttribs;

    glGetProgramivARB ( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ATTRIBS_ARB, &maxAttribs );

    return maxAttribs;
}

