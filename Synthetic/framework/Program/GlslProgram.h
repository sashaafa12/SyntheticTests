//
// Class to encapsulate GLSL program and shader objects and working with them
//
// Author: Alex V. Boreskoff <steps3d@gmail.com>, <steps3d@narod.ru>
//

#ifndef __GLSL_PROGRAM__
#define __GLSL_PROGRAM__

#include    "../libExt/libExt.h"

#include    <string>

using namespace std;

class   Vector2D;
class   Vector3D;
class   Vector4D;
class   Matrix3D;
class   Matrix4x4;
class   Data;

class   GlslProgram
{
protected:
    GLuint 		program;                            // program object handle
    GLuint 		vertexShader;
    GLuint 		fragmentShader;
	GLuint		geometryShader;
    bool        ok;                                 // whether program is loaded and ready to be used
    string      glError;
    string      log;

public:
    GlslProgram  ();
    ~GlslProgram ();

                                                    // load shaders
    bool    loadShaders ( const char * vertexFileName, const char * fragmentFileName,
                          const char * geometryFileName = NULL );
    bool    loadShaders ( Data * vertexShaderData, Data * fragmentShaderData, Data * geometryShaderData = NULL );

                                                    // remove all shaders and free all objects
    void    clear ();

	GLuint	getProgram () const
	{
		return program;
	}

    string  getLog     () const                         // get current log
    {
        return log;
    }

	bool	getLinkStatus () const;
	
    bool    isOk () const                               // whether shader is ok
    {
        return ok;
    }

    string  getGlError () const
    {
        return glError;
    }

    void    bind   ();
    void    unbind ();
    bool    relink ();
	void	addActiveVaryings ( const string& name );
	
	Data  * getBinary  ( GLenum * binaryFormat ) const;	// get program binary 
	bool	saveBinary ( const char * fileName, GLenum * binaryFormat ) const;
	bool	loadBinary ( Data * data, GLenum binaryFormat );
	bool	loadBinary ( const char * fileName, GLenum binaryFormat );
	
                                                        // uniform variables handling methods
    bool        setUniformVector  ( const char * name, const Vector4D& value  );
    bool        setUniformVector  ( int loc,            const Vector4D& value );
    bool        setUniformVector  ( const char * name, const Vector3D& value  );
    bool        setUniformVector  ( int loc,            const Vector3D& value );
    bool        setUniformVector  ( const char * name, const Vector2D& value  );
    bool        setUniformVector  ( int loc,            const Vector2D& value );
    bool        setUniformFloat   ( const char * name, float value            );
    bool        setUniformFloat   ( int loc,            float value           );
    bool        setUniformMatrix  ( const char * name, const Matrix4x4& value );
    bool        setUniformMatrix  ( const char * name, const Matrix3D&  value );
    bool        setUniformMatrix  ( const char * name, float value [16]       );
    bool        setUniformInt     ( const char * name, int value              );
    bool        setUniformInt     ( int loc,            int value             );
    Vector4D    getUniformVector  ( const char * name );
    Vector4D    getUniformVector  ( int loc            );
    int         locForUniformName ( const char * name );

														// bind uniform to VBO
	bool		bindUniform       ( const char * name, GLuint bufId );
	
                                                        // attribute variables handling methods
    bool        setAttribute     ( const char * name, const Vector4D& value );
    bool        setAttribute     ( int index,         const Vector4D& value );
    Vector4D    getAttribute     ( const char * name );
    Vector4D    getAttribute     ( int index );
    int         indexForAttrName ( const char * name );
    bool        bindAttributeTo  ( int no, const char * name );

    bool    	setTexture ( const char * name, int texUnit );
    bool    	setTexture ( int loc,           int texUnit );

	bool		bindFragOut ( const char * name, int no )
	{
		glBindFragDataLocationEXT ( program, no, name );
		
		return true;
	}
                                                        // check whether there is a support for GLSL
    static  bool    isSupported               ();
    static  string  version                   ();
	static	bool	isGeometryShaderSupported ();
                                                        // some limitations on program
    static  int maxVertexUniformComponents   ();
    static  int maxVertexAttribs             ();
    static  int maxFragmentTextureUnits      ();
    static  int maxVertexTextureUnits        ();
    static  int maxCombinedTextureUnits      ();
    static  int maxVaryingFloats             ();
    static  int maxFragmentUniformComponents ();
    static  int maxTextureCoords             ();

protected:
    bool    loadShader     ( GLuint shader, Data * data );
    bool    checkGlError   ();
    void    loadProgramLog ( GLuint object );
    void    loadShaderLog  ( GLuint object );
};

#endif
