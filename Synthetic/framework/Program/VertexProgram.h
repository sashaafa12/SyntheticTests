//
// Simple wrapper class for OpenGL vertex programs
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#ifndef __VERTEX_PROGRAM__
#define __VERTEX_PROGRAM__

#include    <string>
#include    "libExt.h"
#include    "Vector3D.h"
#include    "Vector4D.h"
#include    "ParamProxy.h"

using namespace std;

class   Data;                               // class predeclaration

class   VertexProgram
{
protected:
    GLint               errorCode;
    string              errorString;
    GLuint				id;                 // program id

public:
    ParamArray          local;              // local parameters

    static ParamArray   env;                // environment params
    static int          activeProgram;
    static int          maxVertexAttribs ();
    static int          maxLocalParams   ();
    static int          maxEnvParams     ();
    static int          maxMatrices      ();
    static int          maxTemporaries   ();
    static int          maxParams        ();
    static int          maxAddressRegs   ();

    static  bool    isSupported ();

    VertexProgram  ();
    VertexProgram  ( const char * fileName );
    ~VertexProgram ();

    unsigned    getId () const
    {
        return id;
    }

    string  getErrorString () const
    {
        return errorString;
    }

    void    bind   ();
    void    unbind ();

    void    enable  ();
    void    disable ();

    bool    load ( Data * data );
    bool    load ( const char * fileName );
};

#endif
