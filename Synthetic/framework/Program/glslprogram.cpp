//
// Class to encapsulate GLSL program and shader objects and working with them
//
// Author: Alex V. Boreskoff <steps3d@gmail.com>, <steps3d@narod.ru>
//

#include    "libExt.h"
#include    "libTexture.h"
#include    "Vector3D.h"
#include    "Vector2D.h"
#include    "Vector4D.h"
#include    "Matrix3D.h"
#include    "Matrix4x4.h"
#include    "Data.h"
#include    "GlslProgram.h"
#include	<malloc.h>

GlslProgram :: GlslProgram ()
{
    program        = 0;
    vertexShader   = 0;
    fragmentShader = 0;
	geometryShader = 0;
    ok             = false;
}

GlslProgram :: ~GlslProgram ()
{
    clear ();
}

bool    GlslProgram :: loadShaders ( const char * vertexFileName, const char * fragmentFileName, const char * geometryFileName )
{
    ok = false;

    Data * vertexData = getFile ( vertexFileName );

	if ( vertexData == NULL )
	{
		log += "Cannot open \"";
		log += vertexFileName;
		log += "\"";
	}
	else
    if ( !vertexData -> isOk () || vertexData -> isEmpty () )
    {
    	log += "Error loading vertex shader";

    	delete vertexData;

		vertexData = NULL;
    }

    Data * fragmentData = getFile ( fragmentFileName );

	if ( fragmentData == NULL )
	{
		log += "Cannot open \"";
		log += fragmentFileName;
		log += "\"";
	}
	else
    if ( !fragmentData -> isOk () || fragmentData -> isEmpty () )
    {
    	log += "Error loading fragment shader";

    	delete fragmentData;

    	fragmentData = NULL;
    }

	Data * geometryData = NULL;
	
	if ( geometryFileName != NULL )
	{
		geometryData = getFile ( geometryFileName );

		if ( geometryData == NULL )
		{
			log += "Cannot open \"";
			log += geometryFileName;
			log += "\"";
		}
		else
	    if ( !geometryData -> isOk () || geometryData -> isEmpty () )
	    {
	    	log += "Error loading geometry shader";

	    	delete geometryData;

	    	geometryData = NULL;
	    }
	}
	
    bool	result = loadShaders ( vertexData, fragmentData, geometryData );

	if ( fragmentData == NULL && fragmentFileName != NULL )		// file was specified but failed to open
		result = false;
		
	if ( geometryData == NULL && geometryFileName != NULL )		// file was specified but failed to open
		result = false;
		
    delete vertexData;
    delete fragmentData;
	delete geometryData;

    return result;
}

bool    GlslProgram :: loadShaders ( Data * vertexShaderData, Data * fragmentShaderData, Data * geometryShaderData )
{
    ok = false;
                                // check whether we should create program object
    if ( program == 0 )
        program = glCreateProgram ();

                                // check for errors
    if ( !checkGlError () )
        return false;
                                // create a vertex shader object and a fragment shader object
    if ( vertexShaderData != NULL )
    {
	    vertexShader = glCreateShader ( GL_VERTEX_SHADER_ARB   );

	    log += "Loading vertex shader\n";

    	if ( !loadShader ( vertexShader, vertexShaderData ) )
        	return false;

                                // attach shaders to program object
	    glAttachShader ( program, vertexShader );
    }

    if ( fragmentShaderData != NULL )
    {
	    fragmentShader = glCreateShader ( GL_FRAGMENT_SHADER_ARB );

	    log += "Loading fragment shader\n";

    	if ( !loadShader ( fragmentShader, fragmentShaderData ) )
        	return false;
                                // attach shaders to program object
	    glAttachShader ( program, fragmentShader );
    }

	
    if ( geometryShaderData != NULL )
    {
	    geometryShader = glCreateShader ( GL_GEOMETRY_SHADER_EXT );

	    log += "Loading geometry shader\n";

    	if ( !loadShader ( geometryShader, geometryShaderData ) )
        	return false;
                                // attach shaders to program object
	    glAttachShader ( program, geometryShader );
    }

	return relink ();
}

bool	GlslProgram :: getLinkStatus () const
{
    GLint   linked;

    glGetProgramiv ( program, GL_OBJECT_LINK_STATUS_ARB, &linked );

    if ( !linked )
        return false;
		
	return true;
}

bool	GlslProgram :: relink ()
{
    GLint   linked;

    log += "Linking programs\n";

	if ( glGetProgramBinary != NULL )
		glProgramParameteri ( program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE );
	
                                // link the program object and print out the info log
    glLinkProgram ( program );

/**/  if ( !checkGlError () )     // check for errors
/**/      return false;

    glGetProgramiv ( program, GL_OBJECT_LINK_STATUS_ARB, &linked );

    loadProgramLog ( program );

    if ( !linked )
        return false;

    return ok = true;
}

Data  * GlslProgram :: getBinary ( GLenum * binaryFormat ) const
{
	if ( program == 0 || !getLinkStatus () )
		return NULL;
		
	if ( glGetProgramBinary == NULL || glProgramBinary == NULL || glProgramParameteri == NULL )
		return NULL;

	GLint   binaryLength;
	void  * binary;

	glGetProgramiv ( program, GL_PROGRAM_BINARY_LENGTH, &binaryLength );
	
	if ( binaryLength < 1 )
		return NULL;
		
    binary = (GLvoid * )malloc ( binaryLength );
	
	if ( binary == NULL )
		return NULL;
		
    glGetProgramBinary ( program, binaryLength, NULL, binaryFormat, binary );
		
	return new Data ( binary, binaryLength );
}

bool	GlslProgram :: saveBinary ( const char * fileName, GLenum * binaryFormat ) const
{
	Data * data = getBinary ( binaryFormat );
	
	if ( data == NULL )
		return false;
		
	data -> saveToFile ( fileName );
	
	delete data;
	
	return true;
}

bool	GlslProgram :: loadBinary ( Data * data, GLenum binaryFormat )
{
	if ( program == 0 )
		return false;
		
	if ( glGetProgramBinary == NULL || glProgramBinary == NULL || glProgramParameteri == NULL )
		return false;

	if ( data == NULL || data -> getLength () < 1 )
		return false;
		
	glProgramBinary ( program, binaryFormat, data -> getPtr (), data -> getLength () );

	return getLinkStatus ();
}

bool	GlslProgram :: loadBinary ( const char * fileName, GLenum binaryFormat )
{
	Data * data = new Data ( fileName );
	bool   res  = loadBinary ( data, binaryFormat );
	
	delete data;
	
	return res;
}

void	GlslProgram :: addActiveVaryings ( const string& name )
{
	glActiveVaryingNV ( program, name.c_str () );
}

void    GlslProgram :: bind ()
{
    glUseProgram ( program );
}

void    GlslProgram :: unbind ()
{
    glUseProgram ( 0 );
}

bool    GlslProgram :: loadShader ( GLuint shader, Data * data )
{
    const char * body = (const char *) data -> getPtr ( 0 );
    GLint		 len  = data -> getLength ();
    GLint        compileStatus;

    glShaderSource ( shader, 1, &body,  &len );

                                        // compile the particle vertex shader, and print out
    glCompileShader ( shader );

    if ( !checkGlError() )              // check for OpenGL errors
        return false;

    glGetShaderiv ( shader, GL_OBJECT_COMPILE_STATUS_ARB, &compileStatus );

    loadShaderLog ( shader );

    return compileStatus != 0;

}

bool    GlslProgram :: checkGlError ()
{
    bool    retCode = true;
 	GLenum  glErr   = glGetError();

    if ( glErr == GL_NO_ERROR )
    	return retCode;

    glError = (const char *) gluErrorString ( glErr );		// XXX: what about gluErrorString errors ?

	log += glError;

    return false;
}

void    GlslProgram :: clear ()
{
    glDeleteProgram ( program        );                   // it will also detach shaders
    glDeleteShader  ( vertexShader   );
    glDeleteShader  ( fragmentShader );
	glDeleteShader  ( geometryShader );

    program        = 0;
    vertexShader   = 0;
    fragmentShader = 0;
	geometryShader = NULL;
    ok             = false;
}

void    GlslProgram :: loadProgramLog ( GLuint object )
{
    GLint       logLength     = 0;
    GLsizei     charsWritten  = 0;
    GLcharARB   buffer [2048];
    GLcharARB * infoLog;

    glGetProgramiv ( object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &logLength );

    if ( !checkGlError() )          // check for OpenGL errors
        return;

    if ( logLength < 1 )
        return;
                                    // try to avoid allocating buffer
    if ( logLength > (int) sizeof ( buffer ) )
    {
        infoLog = (GLcharARB*) malloc ( logLength );

        if ( infoLog == NULL )
        {
            log = "ERROR: Could not allocate log buffer";

            return;
        }
    }
    else
        infoLog = buffer;

    glGetProgramInfoLog ( object, logLength, &charsWritten, infoLog );

    log += infoLog;

    if ( infoLog != buffer )
        free ( infoLog );
}

void    GlslProgram :: loadShaderLog ( GLuint object )
{
    GLint       logLength     = 0;
    GLsizei     charsWritten  = 0;
    GLcharARB   buffer [2048];
    GLcharARB * infoLog;

    glGetShaderiv ( object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &logLength );

    if ( !checkGlError() )          // check for OpenGL errors
        return;

    if ( logLength < 1 )
        return;
                                    // try to avoid allocating buffer
    if ( logLength > (int) sizeof ( buffer ) )
    {
        infoLog = (GLcharARB*) malloc ( logLength );

        if ( infoLog == NULL )
        {
            log = "ERROR: Could not allocate log buffer";

            return;
        }
    }
    else
        infoLog = buffer;

    glGetShaderInfoLog ( object, logLength, &charsWritten, infoLog );

    log += infoLog;

    if ( infoLog != buffer )
        free ( infoLog );
}

bool    GlslProgram :: setUniformVector ( const char * name, const Vector4D& value )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniform4fvARB ( loc, 1, value );

    return true;
}

bool    GlslProgram :: setUniformVector  ( int loc, const Vector4D& value )
{
    glUniform4fv ( loc, 1, value );

    return true;
}

bool    GlslProgram :: setUniformVector ( const char * name, const Vector3D& value )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniform3fvARB ( loc, 1, value );

    return true;
}

bool    GlslProgram :: setUniformVector  ( int loc, const Vector3D& value )
{
    glUniform3fv ( loc, 1, value );

    return true;
}

bool    GlslProgram :: setUniformVector ( const char * name, const Vector2D& value )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniform2fv ( loc, 1, value );

    return true;
}

bool    GlslProgram :: setUniformVector  ( int loc, const Vector2D& value )
{
    glUniform2fv ( loc, 1, value );

    return true;
}

bool    GlslProgram :: setUniformFloat ( const char * name, float value )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniform1f ( loc, value );

    return true;
}

bool    GlslProgram :: setUniformFloat ( int loc, float value )
{
    glUniform1f ( loc, value );

    return true;
}

bool    GlslProgram :: setUniformInt ( const char * name, int value )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniform1i ( loc, value );

    return true;
}

bool    GlslProgram :: setUniformInt ( int loc, int value )
{
    glUniform1i ( loc, value );

    return true;
}

bool    GlslProgram :: setUniformMatrix  ( const char * name, const Matrix4x4& value )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniformMatrix4fv ( loc, 1, GL_FALSE, value [0] );

    return true;
}

bool    GlslProgram :: setUniformMatrix  ( const char * name, float value [16] )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniformMatrix4fv ( loc, 1, GL_FALSE, value );

    return true;
}

bool    GlslProgram :: setUniformMatrix  ( const char * name, const Matrix3D& value )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniformMatrix3fv ( loc, 1, GL_FALSE, value [0] );

    return true;
}

int     GlslProgram :: locForUniformName ( const char * name )
{
    return glGetUniformLocation ( program, name );
}

Vector4D    GlslProgram :: getUniformVector ( const char * name )
{
    float   values [4];

    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return Vector4D ( 0, 0, 0, 0 );

    glGetUniformfv ( program, loc, values );

    return Vector4D ( values [0], values [1], values [2], values [3] );
}

bool    GlslProgram :: setTexture ( const char * name, int texUnit )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc == -1 )
        return false;

    glUniform1i ( loc, texUnit );

    return true;
}

bool    GlslProgram :: setTexture ( int loc, int texUnit )
{
    if ( loc < 0 )
        return false;

    glUniform1i ( loc, texUnit );

    return true;
}

bool    GlslProgram :: bindAttributeTo  ( int loc, const char * name )
{
#ifdef	MACOSX
    glBindAttribLocation ( program, loc, name );		// ???, Mac OS X, Win32

#else
    glBindAttribLocationARB ( program, loc, name );		// ???, Mac OS X, Win32
#endif

    return true;
}

bool    GlslProgram :: setAttribute ( const char * name, const Vector4D& value )
{
    int index = glGetAttribLocation ( program, name );

    if ( index < 0 )
        return false;

    glVertexAttrib4fv ( index, value );

    return true;
}

bool    GlslProgram :: setAttribute ( int index, const Vector4D& value )
{
    glVertexAttrib4fv ( index, value );

    return true;
}

int     GlslProgram :: indexForAttrName ( const char * name )
{
    return glGetAttribLocation ( program, name );
}

Vector4D    GlslProgram :: getAttribute ( const char * name )
{
    int index = glGetAttribLocation ( program, name );

    if ( index < 0 )
        return Vector4D ( 0, 0, 0, 0 );

    float   buf [4];

    glGetVertexAttribfv ( index, GL_CURRENT_VERTEX_ATTRIB_ARB, buf );

    return Vector4D ( buf [0], buf [1], buf [2], buf [3] );
}

Vector4D    GlslProgram :: getAttribute ( int index )
{
    float   buf [4];

    glGetVertexAttribfv ( index, GL_CURRENT_VERTEX_ATTRIB_ARB, buf );

    return Vector4D ( buf [0], buf [1], buf [2], buf [3] );
}

bool    GlslProgram :: isSupported ()
{
    return isExtensionSupported ( "GL_ARB_shading_language_100" ) &&
           isExtensionSupported ( "GL_ARB_shader_objects"       ) &&
           isExtensionSupported ( "GL_ARB_vertex_shader"        ) &&
           isExtensionSupported ( "GL_ARB_fragment_shader"      );

}

bool	GlslProgram :: isGeometryShaderSupported ()
{
	return isExtensionSupported ( "GL_EXT_geometry_shader4" );
}

string  GlslProgram :: version ()
{
#ifdef	MACOSX
	return "1.051";
#else
    const char * slVer = (const char *) glGetString ( GL_SHADING_LANGUAGE_VERSION_ARB );

    if ( glGetError() != GL_NO_ERROR )
        return "1.051";

    return string ( slVer );
#endif
}

int     GlslProgram :: maxVertexUniformComponents ()
{
    GLint maxVertexUniformComponents;

    glGetIntegerv ( GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB, &maxVertexUniformComponents );

    return maxVertexUniformComponents;
}

int     GlslProgram :: maxVertexAttribs ()
{
    GLint maxVertexAttribs;

    glGetIntegerv ( GL_MAX_VERTEX_ATTRIBS_ARB, &maxVertexAttribs );

    return maxVertexAttribs;
}

int     GlslProgram :: maxFragmentTextureUnits ()
{
    GLint maxFragmentTextureUnits;

    glGetIntegerv ( GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &maxFragmentTextureUnits );

    return maxFragmentTextureUnits;
}

int     GlslProgram :: maxVertexTextureUnits ()
{
    GLint maxVertexTextureUnits;

    glGetIntegerv ( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB, &maxVertexTextureUnits );

    return maxVertexTextureUnits;
}

int     GlslProgram :: maxCombinedTextureUnits ()
{
    GLint maxCombinedTextureUnits;

    glGetIntegerv ( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB, &maxCombinedTextureUnits );

    return maxCombinedTextureUnits;
}

int     GlslProgram :: maxVaryingFloats ()
{
    GLint maxVaryingFloats;

    glGetIntegerv ( GL_MAX_VARYING_FLOATS_ARB, &maxVaryingFloats );

    return maxVaryingFloats;
}

int     GlslProgram :: maxFragmentUniformComponents ()
{
    GLint maxFragmentUniformComponents;

    glGetIntegerv ( GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB, &maxFragmentUniformComponents );

    return maxFragmentUniformComponents;
}

int     GlslProgram :: maxTextureCoords ()
{
    GLint maxTextureCoords;

    glGetIntegerv ( GL_MAX_TEXTURE_COORDS_ARB, &maxTextureCoords );

    return maxTextureCoords;
}
