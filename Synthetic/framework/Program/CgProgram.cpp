//
// Object wrapper for Cg programs
//
// Author: Alex V. Boreskoff <steps3d@narod.ru>, <alexboreskoff@mtu-net.ru>
//

#include	"CgProgram.h"

CGcontext	CgProgram :: context         = NULL;
CGprofile	CgProgram :: vertexProfile   = CG_PROFILE_UNKNOWN;
CGprofile	CgProgram :: fragmentProfile = CG_PROFILE_UNKNOWN;

CgProgram :: CgProgram  ( int theType )
{
	type = theType;
}

CgProgram :: ~CgProgram ()
{
	cgDestroyProgram ( program );
}

bool	CgProgram :: load ( const char * fileName )
{
    								// create context
	if ( context == NULL )
		context = cgCreateContext ();

									// check for success
	if ( context == NULL )
	{
		errorDesc = "Cannot create context";

		return false;
	}

									// create vertex profile
	if ( type == CgVertexProgram )
	{
		if ( vertexProfile == CG_PROFILE_UNKNOWN )
		{
			vertexProfile = cgGLGetLatestProfile ( CG_GL_VERTEX );

			if ( vertexProfile == CG_PROFILE_UNKNOWN )
			{
				errorDesc = "Invalid vertex profile type";

				return false;
			}

			cgGLSetOptimalOptions ( vertexProfile );
		}
	}
	else
	if ( type == CgFragmentProgram )
	{									// create fragment profile
		if ( fragmentProfile == CG_PROFILE_UNKNOWN )
		{
			fragmentProfile = cgGLGetLatestProfile ( CG_GL_FRAGMENT );

			if ( fragmentProfile == CG_PROFILE_UNKNOWN )
			{
				errorDesc = "Invalid fragment profile type";

				return false;
			}

			cgGLSetOptimalOptions ( fragmentProfile );
		}
	}
	else
	{
		errorDesc = "Unknown profile type";

		return false;
	}

	program = cgCreateProgramFromFile ( context, CG_SOURCE, fileName, getProfile (), "main", 0 );

	if ( !checkCgError () )
		return false;

	if ( program == NULL )
	{
		CGerror	error = cgGetError ();

		errorDesc  = "Error loading vertex program:\n";
		errorDesc += cgGetErrorString ( error );

		return false;
	}

	listing = cgGetProgramString ( program, CG_COMPILED_PROGRAM );

	cgGLLoadProgram ( program );

	return checkCgError ();
}

bool	CgProgram :: checkCgError ()
{
	CGerror error = cgGetError();

	if ( error != CG_NO_ERROR )
    	errorDesc = cgGetErrorString ( error );
    else
    	errorDesc = "";

	return error == CG_NO_ERROR;
}

