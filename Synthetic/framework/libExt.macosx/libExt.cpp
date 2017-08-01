//
// Simple library to load basic extensions
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#include    "libExt.h"

#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>

#ifdef	MACOSX
	#include	<dlfcn.h>
	#include	<stdlib.h>
	#include	<string.h>
	#include	<alloc.h>
#endif

#ifndef	GL_VERSION_1_3										// ARB_transpose matrix
PFNGLLOADTRANSPOSEMATRIXFPROC					glLoadTransposeMatrixfARB                = NULL;
PFNGLLOADTRANSPOSEMATRIXDPROC					glLoadTransposeMatrixdARB                = NULL;
PFNGLMULTTRANSPOSEMATRIXFPROC					glMultTransposeMatrixfARB                = NULL;
PFNGLMULTTRANSPOSEMATRIXDPROC					glMultTransposeMatrixdARB                = NULL;
#endif

////////////////////////// functions ////////////////////////////

static  bool    isExtensionSupported ( const char * ext, const char * extList )
{
    const char * start = extList;
    const char * ptr;

    while ( ( ptr = strstr ( start, ext ) ) != NULL )
    {
                                            // we've found, ensure name is exactly ext
        const char * end = ptr + strlen ( ext );

        if ( isspace ( *end ) || *end == '\0' )
            return true;

        start = end;
    }

    return false;
}

bool    isExtensionSupported ( const char * ext )
{
    initExtensions ();                      // since we will need it for platform-specific extensions

    const char * extensions = (const char *) glGetString ( GL_EXTENSIONS );

    if ( isExtensionSupported ( ext, extensions ) )
	    return true;

	return false;
}

const char * getGeneralExtensions ()
{
    initExtensions ();

	return (const char *) glGetString ( GL_EXTENSIONS );
}

const char * getPlatformExtensions ()
{
    initExtensions ();                      // since we will need it for platform-specific extensions

	return "";
}

void    assertExtensionsSupported ( const char * extList )
{
    char * str = strdup ( extList );

    for ( char * ptr = strtok ( str, " ,;\t" ); ptr != NULL; ptr = strtok ( NULL, " ,;\t" ) )
        if ( !isExtensionSupported ( ptr ) )
        {
            fprintf ( stderr, "Error - extension %s not supported.\n", ptr );

            free ( str );
            exit ( 1   );
        }

    free ( str );
}

//
// hide platform specific in one place
//

void * NSGetProcAddress ( const char * name )
{
	return  dlsym ( RTLD_DEFAULT /* RTLD_NEXT */, name );
}

static void * getProcAddress ( const char * name )
{
	return NSGetProcAddress ( name );			// Mac OS X code
}

void    initExtensions ()
{
    static  bool    initialized = false;                    // whether initExtensions has already been called

    if ( initialized )
        return;

#ifndef	GL_VERSION_1_3										// ARB_transpose matrix
	glLoadTransposeMatrixfARB	              = (PFNGLLOADTRANSPOSEMATRIXFPROC                 ) getProcAddress ( "glLoadTransposeMatrixfARB" );
	glLoadTransposeMatrixdARB	              = (PFNGLLOADTRANSPOSEMATRIXDPROC                 ) getProcAddress ( "glLoadTransposeMatrixdARB" );
	glMultTransposeMatrixfARB	              = (PFNGLMULTTRANSPOSEMATRIXFPROC                 ) getProcAddress ( "glMultTransposeMatrixfARB" );
	glMultTransposeMatrixdARB	              = (PFNGLMULTTRANSPOSEMATRIXDPROC                 ) getProcAddress ( "glMultTransposeMatrixdARB" );
#endif	

    initialized = true;
}

void    printfInfo ()               // print info about card, driver, version & etc
{
    const char * vendor    = (const char *)glGetString ( GL_VENDOR   );
    const char * renderer  = (const char *)glGetString ( GL_RENDERER );
    const char * version   = (const char *)glGetString ( GL_VERSION  );

    printf ( "Graphics card and driver information:\n" );
    printf ( "Vendor:   %s\nRenderer: %s\nVersion:  %s\n", vendor, renderer, version );

    if ( isExtensionSupported ( "GL_ARB_multitexture" ) )
    {
        int maxTextureUnits;

        glGetIntegerv ( GL_MAX_TEXTURE_UNITS_ARB,    &maxTextureUnits );

        printf ( "ARB_multitexture supported.\nMax texture units %d.\n", maxTextureUnits );


    }
    if ( isExtensionSupported ( "GL_NV_register_combiners" ) )
    {
        int maxCombiners;

        glGetIntegerv ( GL_MAX_GENERAL_COMBINERS_NV, &maxCombiners    );

        printf ( "NV_register_combiners supported.\nMax general combiners %d.\n", maxCombiners );
    }

    if ( isExtensionSupported ( "GL_ARB_vertex_buffer_object" ) )
        printf ( "VBO supported.\n" );

    if ( isExtensionSupported ( "GL_NV_occlusion_query" ) )
        printf ( "NV_occlusion_query extension supported.\n" );

    if ( isExtensionSupported ( "GL_SGIS_generate_mipmap" ) )
        printf ( "GL_SGIS_generate_mipmap extension supported.\n" );

    if ( isExtensionSupported ( "GL_ARB_vertex_program" ) )
        printf ( "GL_ARB_vertex_program extension supported.\n" );
}

