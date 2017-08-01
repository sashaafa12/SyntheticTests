//
// Header file to libExt
//
// Author: Alex V. Boreskoff <steps3d@gmail.com>, <steps3d@narod.ru>
//

#ifndef __LIB_EXT__
#define __LIB_EXT__

#ifdef  _WIN32
    #include    <windows.h>
	#pragma warning (disable:4996)
#else
    #include	<stdint.h>
    #define GLX_GLXEXT_LEGACY
#endif

#include    <GL/gl.h>
#include    <GL/glu.h>
#include    "../glext.h"

#ifdef  _WIN32
    #include    "../wglext.h"
#else
    #include    "../glx.h"
    #include    "../glxext.h"
#endif

bool    isExtensionSupported ( const char * ext );
void    assertExtensionsSupported ( const char * extList );
void    initExtensions       ();
void    printfInfo           ();                // print info about card, driver, version & etc

const char * getGeneralExtensions  ();			// get list of platform-independant (GL) extensions
const char * getPlatformExtensions ();			// get list of extensions for current platform (WGL or GLX)

#ifdef  _WIN32
extern  PFNGLACTIVETEXTUREARBPROC               glActiveTextureARB;
extern  PFNGLCLIENTACTIVETEXTUREARBPROC         glClientActiveTextureARB;
extern  PFNGLMULTITEXCOORD1FARBPROC             glMultiTexCoord1f;
extern  PFNGLMULTITEXCOORD1FVARBPROC            glMultiTexCoord1fv;
extern  PFNGLMULTITEXCOORD2FARBPROC             glMultiTexCoord2f;
extern  PFNGLMULTITEXCOORD2FVARBPROC            glMultiTexCoord2fv;
extern  PFNGLMULTITEXCOORD3FARBPROC             glMultiTexCoord3f;
extern  PFNGLMULTITEXCOORD3FVARBPROC            glMultiTexCoord3fv;
extern  PFNGLMULTITEXCOORD4FARBPROC             glMultiTexCoord4f;
extern  PFNGLMULTITEXCOORD4FVARBPROC            glMultiTexCoord4fv;

	#define	glBindAttribLocation		glBindAttribLocationARB
#else
    #define  glActiveTextureARB         glActiveTexture
    #define  glClientActiveTextureARB   glClientActiveTexture
#endif
                                        // fog coord function
extern  PFNGLFOGCOORDFEXTPROC                      glFogCoordf;

                                        // secondary color functions
extern  PFNGLSECONDARYCOLOR3FPROC               glSecondaryColor3f;
extern  PFNGLSECONDARYCOLOR3FVPROC              glSecondaryColor3fv;
extern  PFNGLSECONDARYCOLORPOINTERPROC          glSecondaryColorPointer;

                                    // register combiners functions
extern  PFNGLCOMBINERPARAMETERFVNVPROC          glCombinerParameterfvNV;
extern  PFNGLCOMBINERPARAMETERIVNVPROC          glCombinerParameterivNV;
extern  PFNGLCOMBINERPARAMETERFNVPROC           glCombinerParameterfNV;
extern  PFNGLCOMBINERPARAMETERINVPROC           glCombinerParameteriNV;
extern  PFNGLCOMBINERINPUTNVPROC                glCombinerInputNV;
extern  PFNGLCOMBINEROUTPUTNVPROC               glCombinerOutputNV;
extern  PFNGLFINALCOMBINERINPUTNVPROC           glFinalCombinerInputNV;

                                    // VBO functions
extern  PFNGLBINDBUFFERARBPROC                  glBindBufferARB;
extern  PFNGLDELETEBUFFERSARBPROC               glDeleteBuffersARB;
extern  PFNGLGENBUFFERSARBPROC                  glGenBuffersARB;
extern  PFNGLISBUFFERARBPROC                    glIsBufferARB;
extern  PFNGLBUFFERDATAARBPROC                  glBufferDataARB;
extern  PFNGLBUFFERSUBDATAARBPROC               glBufferSubDataARB;
extern  PFNGLGETBUFFERSUBDATAARBPROC            glGetBufferSubDataARB;
extern  PFNGLMAPBUFFERARBPROC                   glMapBufferARB;
extern  PFNGLUNMAPBUFFERARBPROC                 glUnmapBufferARB;
extern  PFNGLGETBUFFERPARAMETERIVARBPROC        glGetBufferParameterivARB;
extern  PFNGLGETBUFFERPOINTERVARBPROC           glGetBufferPointervARB;

#ifdef  _WIN32
extern  PFNWGLGETEXTENSIONSSTRINGARBPROC        wglGetExtensionsStringARB;

                                    // p-buffer functions
extern  PFNWGLCREATEPBUFFERARBPROC              wglCreatePbufferARB;
extern  PFNWGLGETPBUFFERDCARBPROC               wglGetPbufferDCARB;
extern  PFNWGLRELEASEPBUFFERDCARBPROC           wglReleasePbufferDCARB;
extern  PFNWGLDESTROYPBUFFERARBPROC             wglDestroyPbufferARB;
extern  PFNWGLQUERYPBUFFERARBPROC               wglQueryPbufferARB;

                                    // WGL_ARB_pixel_format
extern  PFNWGLCHOOSEPIXELFORMATARBPROC          wglChoosePixelFormatARB;

                                    // WGL_ARB_render_texture
extern  PFNWGLBINDTEXIMAGEARBPROC               wglBindTexImageARB;
extern  PFNWGLRELEASETEXIMAGEARBPROC            wglReleaseTexImageARB;
extern  PFNWGLSETPBUFFERATTRIBARBPROC           wglSetPbufferAttribARB;

                                    // WGL_EXT_swap_control
extern  PFNWGLSWAPINTERVALEXTPROC               wglSwapIntervalEXT;
extern  PFNWGLGETSWAPINTERVALEXTPROC            wglGetSwapIntervalEXT;
#else
                                    // GLX_SGIX_pbuffer
extern  PFNGLXCREATEGLXPBUFFERSGIXPROC          glXCreateGLXPbufferSGIX;
extern  PFNGLXDESTROYGLXPBUFFERSGIXPROC         glXDestroyGLXPbufferSGIX;
extern  PFNGLXQUERYGLXPBUFFERSGIXPROC           glXQueryGLXPbufferSGIX;

                                    // GLX_SGIX_fbconfig
extern  PFNGLXCHOOSEFBCONFIGSGIXPROC            glXChooseFBConfigSGIX;
extern  PFNGLXCREATECONTEXTWITHCONFIGSGIXPROC   glXCreateContextWithConfigSGIX;
#endif

                                    // NV-occlusion query extension
extern  PFNGLGENOCCLUSIONQUERIESNVPROC          glGenOcclusionQueriesNV;
extern  PFNGLBEGINOCCLUSIONQUERYNVPROC          glBeginOcclusionQueryNV;
extern  PFNGLENDOCCLUSIONQUERYNVPROC            glEndOcclusionQueryNV;
extern  PFNGLGETOCCLUSIONQUERYIVNVPROC          glGetOcclusionQueryivNV;

                                    // ARB_vertex_program extension
extern  PFNGLGENPROGRAMSARBPROC                 glGenProgramsARB;
extern  PFNGLDELETEPROGRAMSARBPROC              glDeleteProgramsARB;
extern  PFNGLBINDPROGRAMARBPROC                 glBindProgramARB;
extern  PFNGLISPROGRAMARBPROC                   glIsProgramARB;

extern  PFNGLPROGRAMSTRINGARBPROC               glProgramStringARB;
extern  PFNGLGETPROGRAMIVARBPROC                glGetProgramivARB;

extern  PFNGLVERTEXATTRIB4FARBPROC              glVertexAttrib4fARB;
extern  PFNGLVERTEXATTRIB4FVARBPROC             glVertexAttrib4fvARB;
extern  PFNGLVERTEXATTRIB3FARBPROC              glVertexAttrib3fARB;
extern  PFNGLVERTEXATTRIB3FVARBPROC             glVertexAttrib3fvARB;

extern  PFNGLVERTEXATTRIBPOINTERARBPROC         glVertexAttribPointerARB;
extern  PFNGLENABLEVERTEXATTRIBARRAYARBPROC     glEnableVertexAttribArrayARB;
extern  PFNGLDISABLEVERTEXATTRIBARRAYARBPROC    glDisableVertexAttribArrayARB;

extern  PFNGLPROGRAMLOCALPARAMETER4FARBPROC     glProgramLocalParameter4fARB;
extern  PFNGLPROGRAMLOCALPARAMETER4FVARBPROC    glProgramLocalParameter4fvARB;
extern  PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC  glGetProgramLocalParameterfvARB;

extern  PFNGLPROGRAMENVPARAMETER4FARBPROC       glProgramEnvParameter4fARB;
extern  PFNGLPROGRAMENVPARAMETER4FVARBPROC      glProgramEnvParameter4fvARB;
extern  PFNGLGETPROGRAMENVPARAMETERFVARBPROC    glGetProgramEnvParameterfvARB;

                                    // EXT_texture3D
extern  PFNGLTEXIMAGE3DEXTPROC                  glTexImage3DEXT;

                                    // ARB_texture_compression
extern  PFNGLCOMPRESSEDTEXIMAGE3DARBPROC        glCompressedTexImage3DARB;
extern  PFNGLCOMPRESSEDTEXIMAGE2DARBPROC        glCompressedTexImage2DARB;
extern  PFNGLCOMPRESSEDTEXIMAGE1DARBPROC        glCompressedTexImage1DARB;
extern  PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC     glCompressedTexSubImage3DARB;
extern  PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC     glCompressedTexSubImage2DARB;
extern  PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC     glCompressedTexSubImage1DARB;
extern  PFNGLGETCOMPRESSEDTEXIMAGEARBPROC       glGetCompressedTexImageARB;

                                    // ARB_point_parameters
extern  PFNGLPOINTPARAMETERFARBPROC             glPointParameterfARB;
extern  PFNGLPOINTPARAMETERFVARBPROC            glPointParameterfvARB;

                                    // ARB_occlusion_query
extern  PFNGLGENQUERIESARBPROC                  glGenQueriesARB;
extern  PFNGLDELETEQUERIESARBPROC               glDeleteQueriesARB;
extern  PFNGLISQUERYARBPROC                     glIsQueryARB;
extern  PFNGLBEGINQUERYARBPROC                  glBeginQueryARB;
extern  PFNGLENDQUERYARBPROC                    glEndQueryARB;
extern  PFNGLGETQUERYIVARBPROC                  glGetQueryivARB;
extern  PFNGLGETQUERYOBJECTIVARBPROC            glGetQueryObjectivARB;
extern  PFNGLGETQUERYOBJECTUIVARBPROC           glGetQueryObjectuivARB;

                                    // GLSL-specific extensions
                                    // GL_ARB_shader_object
extern  PFNGLDELETEOBJECTARBPROC                glDeleteObjectARB;
extern  PFNGLGETHANDLEARBPROC                   glGetHandleARB;
extern  PFNGLDETACHOBJECTARBPROC                glDetachObjectARB;
extern  PFNGLCREATESHADEROBJECTARBPROC          glCreateShaderObjectARB;
extern  PFNGLSHADERSOURCEARBPROC                glShaderSourceARB;
extern  PFNGLCOMPILESHADERARBPROC               glCompileShaderARB;
extern  PFNGLCREATEPROGRAMOBJECTARBPROC         glCreateProgramObjectARB;
extern  PFNGLATTACHOBJECTARBPROC                glAttachObjectARB;
extern  PFNGLLINKPROGRAMARBPROC                 glLinkProgramARB;
extern  PFNGLUSEPROGRAMOBJECTARBPROC            glUseProgramObjectARB;
extern  PFNGLVALIDATEPROGRAMARBPROC             glValidateProgramARB;
extern  PFNGLUNIFORM1FARBPROC                   glUniform1fARB;
extern  PFNGLUNIFORM2FARBPROC                   glUniform2fARB;
extern  PFNGLUNIFORM3FARBPROC                   glUniform3fARB;
extern  PFNGLUNIFORM4FARBPROC                   glUniform4fARB;
extern  PFNGLUNIFORM1IARBPROC                   glUniform1iARB;
extern  PFNGLUNIFORM2IARBPROC                   glUniform2iARB;
extern  PFNGLUNIFORM3IARBPROC                   glUniform3iARB;
extern  PFNGLUNIFORM4IARBPROC                   glUniform4iARB;
extern  PFNGLUNIFORM1FVARBPROC                  glUniform1fvARB;
extern  PFNGLUNIFORM2FVARBPROC                  glUniform2fvARB;
extern  PFNGLUNIFORM3FVARBPROC                  glUniform3fvARB;
extern  PFNGLUNIFORM4FVARBPROC                  glUniform4fvARB;
extern  PFNGLUNIFORM1IVARBPROC                  glUniform1ivARB;
extern  PFNGLUNIFORM2IVARBPROC                  glUniform2ivARB;
extern  PFNGLUNIFORM3IVARBPROC                  glUniform3ivARB;
extern  PFNGLUNIFORM4IVARBPROC                  glUniform4ivARB;
extern  PFNGLUNIFORMMATRIX2FVARBPROC            glUniformMatrix2fvARB;
extern  PFNGLUNIFORMMATRIX3FVARBPROC            glUniformMatrix3fvARB;
extern  PFNGLUNIFORMMATRIX4FVARBPROC            glUniformMatrix4fvARB;
extern  PFNGLGETOBJECTPARAMETERFVARBPROC        glGetObjectParameterfvARB;
extern  PFNGLGETOBJECTPARAMETERIVARBPROC        glGetObjectParameterivARB;
extern  PFNGLGETINFOLOGARBPROC                  glGetInfoLogARB;
extern  PFNGLGETATTACHEDOBJECTSARBPROC          glGetAttachedObjectsARB;
extern  PFNGLGETUNIFORMLOCATIONARBPROC          glGetUniformLocationARB;
extern  PFNGLGETACTIVEUNIFORMARBPROC            glGetActiveUniformARB;
extern  PFNGLGETUNIFORMFVARBPROC                glGetUniformfvARB;
extern  PFNGLGETUNIFORMIVARBPROC                glGetUniformivARB;
extern  PFNGLGETSHADERSOURCEARBPROC             glGetShaderSourceARB;

                                        // GL_ARB_vertex_shader
extern  PFNGLBINDATTRIBLOCATIONARBPROC          glBindAttribLocationARB;
extern  PFNGLGETACTIVEATTRIBARBPROC             glGetActiveAttribARB;
extern  PFNGLGETATTRIBLOCATIONARBPROC           glGetAttribLocationARB;
extern  PFNGLGETVERTEXATTRIBFVARBPROC           glGetVertexAttribfvARB;

                                        // EXT_stencil_two_side
extern  PFNGLACTIVESTENCILFACEEXTPROC           glActiveStencilFaceEXT;

										// EXT_depth_bounds_test
extern	PFNGLDEPTHBOUNDSEXTPROC					glDepthBoundsEXT;

										// EXT_framebuffer_object
extern	PFNGLISRENDERBUFFEREXTPROC						glIsRenderbufferEXT;
extern	PFNGLBINDRENDERBUFFEREXTPROC					glBindRenderbufferEXT;
extern	PFNGLDELETERENDERBUFFERSEXTPROC					glDeleteRenderbuffersEXT;
extern	PFNGLGENRENDERBUFFERSEXTPROC					glGenRenderbuffersEXT;
extern	PFNGLRENDERBUFFERSTORAGEEXTPROC					glRenderbufferStorageEXT;
extern	PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC			glGetRenderbufferParameterivEXT;
extern	PFNGLISFRAMEBUFFEREXTPROC						glIsFramebufferEXT;
extern	PFNGLBINDFRAMEBUFFEREXTPROC						glBindFramebufferEXT;
extern	PFNGLDELETEFRAMEBUFFERSEXTPROC					glDeleteFramebuffersEXT;
extern	PFNGLGENFRAMEBUFFERSEXTPROC						glGenFramebuffersEXT;
extern	PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC				glCheckFramebufferStatusEXT;
extern	PFNGLFRAMEBUFFERTEXTURE1DEXTPROC				glFramebufferTexture1DEXT;
extern	PFNGLFRAMEBUFFERTEXTURE2DEXTPROC				glFramebufferTexture2DEXT;
extern	PFNGLFRAMEBUFFERTEXTURE3DEXTPROC				glFramebufferTexture3DEXT;
extern	PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC				glFramebufferRenderbufferEXT;
extern	PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC	glGetFramebufferAttachmentParameterivEXT;
extern	PFNGLGENERATEMIPMAPEXTPROC						glGenerateMipmapEXT;


#ifndef	GL_VERSION_1_3										// ARB_transpose matrix
extern	PFNGLLOADTRANSPOSEMATRIXFPROC					glLoadTransposeMatrixfARB;
extern	PFNGLLOADTRANSPOSEMATRIXDPROC					glLoadTransposeMatrixdARB;
extern	PFNGLMULTTRANSPOSEMATRIXFPROC					glMultTransposeMatrixfARB;
extern	PFNGLMULTTRANSPOSEMATRIXDPROC					glMultTransposeMatrixdARB;
#endif

															// ARB_color_buffer_float
extern	PFNGLCLAMPCOLORARBPROC							glClampColorARB;
															
#ifdef	_WIN32												// fresh Linux already has 2.0
															// stenccl ops from OpenGL 2.0
extern	PFNGLSTENCILFUNCSEPARATEPROC			glStencilFuncSeparate;
extern	PFNGLSTENCILOPSEPARATEPROC				glStencilOpSeparate;
extern	PFNGLSTENCILMASKSEPARATEPROC			glStencilMaskSeparate;

															// blend ops from OpenGL 2.0
extern	PFNGLBLENDEQUATIONPROC					glBlendEquation;
extern	PFNGLBLENDEQUATIONSEPARATEPROC		 	glBlendEquationSeparate;
extern	PFNGLBLENDFUNCSEPARATEPROC				glBlendFuncSeparate;
extern	PFNGLBLENDCOLORPROC						glBlendColor;
#endif

															// draw buffers from OpenGL 2.0
extern	PFNGLDRAWBUFFERSPROC					glDrawBuffers;
															// ATI_draw_buffers
extern	PFNGLDRAWBUFFERSATIPROC					glDrawBuffersATI;

															// EXT_texture_integer
extern	PFNGLTEXPARAMETERIIVEXTPROC				glTexParameterIivEXT;
extern	PFNGLTEXPARAMETERIUIVEXTPROC			glTexParameterIuivEXT;
extern	PFNGLGETTEXPARAMETERIIVEXTPROC			glGetTexParameterIivEXT;
extern	PFNGLGETTEXPARAMETERIUIVEXTPROC			glGetTexParameterIuivEXT;
extern	PFNGLCLEARCOLORIIEXTPROC				glClearColorIiEXT;
extern	PFNGLCLEARCOLORIUIEXTPROC				glClearColorIuiEXT;

															// EXT_draw_instanced
extern	PFNGLDRAWARRAYSINSTANCEDEXTPROC			glDrawArraysInstancedEXT;
extern	PFNGLDRAWELEMENTSINSTANCEDEXTPROC		glDrawElementsInstancedEXT;


															// EXT_texture_buffer_object
extern	PFNGLTEXBUFFEREXTPROC 					glTexBufferEXT;
															
															// EXT_gpu_shader4
extern	PFNGLGETUNIFORMUIVEXTPROC				glGetUniformuivEXT;
extern	PFNGLBINDFRAGDATALOCATIONEXTPROC		glBindFragDataLocationEXT;
extern	PFNGLGETFRAGDATALOCATIONEXTPROC			glGetFragDataLocationEXT;
extern	PFNGLUNIFORM1UIEXTPROC					glUniform1uiEXT;
extern	PFNGLUNIFORM2UIEXTPROC					glUniform2uiEXT;
extern	PFNGLUNIFORM3UIEXTPROC					glUniform3uiEXT;
extern	PFNGLUNIFORM4UIEXTPROC					glUniform4uiEXT;
extern	PFNGLUNIFORM1UIVEXTPROC					glUniform1uivEXT;
extern	PFNGLUNIFORM2UIVEXTPROC					glUniform2uivEXT;
extern	PFNGLUNIFORM3UIVEXTPROC					glUniform3uivEXT;
extern	PFNGLUNIFORM4UIVEXTPROC					glUniform4uivEXT;
extern	PFNGLVERTEXATTRIBI1IEXTPROC				glVertexAttribI1iEXT;
extern	PFNGLVERTEXATTRIBI2IEXTPROC				glVertexAttribI2iEXT;
extern	PFNGLVERTEXATTRIBI3IEXTPROC				glVertexAttribI3iEXT;
extern	PFNGLVERTEXATTRIBI4IEXTPROC				glVertexAttribI4iEXT;
extern	PFNGLVERTEXATTRIBI1UIEXTPROC			glVertexAttribI1uiEXT;
extern	PFNGLVERTEXATTRIBI2UIEXTPROC			glVertexAttribI2uiEXT;
extern	PFNGLVERTEXATTRIBI3UIEXTPROC			glVertexAttribI3uiEXT;
extern	PFNGLVERTEXATTRIBI4UIEXTPROC			glVertexAttribI4uiEXT;
extern	PFNGLVERTEXATTRIBI1IVEXTPROC			glVertexAttribI1ivEXT;
extern	PFNGLVERTEXATTRIBI2IVEXTPROC			glVertexAttribI2ivEXT;
extern	PFNGLVERTEXATTRIBI3IVEXTPROC			glVertexAttribI3ivEXT;
extern	PFNGLVERTEXATTRIBI4IVEXTPROC			glVertexAttribI4ivEXT;
extern	PFNGLVERTEXATTRIBI1UIVEXTPROC			glVertexAttribI1uivEXT;
extern	PFNGLVERTEXATTRIBI2UIVEXTPROC			glVertexAttribI2uivEXT;
extern	PFNGLVERTEXATTRIBI3UIVEXTPROC			glVertexAttribI3uivEXT;
extern	PFNGLVERTEXATTRIBI4UIVEXTPROC			glVertexAttribI4uivEXT;
extern	PFNGLVERTEXATTRIBI4BVEXTPROC			glVertexAttribI4bvEXT;
extern	PFNGLVERTEXATTRIBI4SVEXTPROC			glVertexAttribI4svEXT;
extern	PFNGLVERTEXATTRIBI4UBVEXTPROC			glVertexAttribI4ubvEXT;
extern	PFNGLVERTEXATTRIBI4USVEXTPROC			glVertexAttribI4usvEXT;
extern	PFNGLVERTEXATTRIBIPOINTEREXTPROC		glVertexAttribIPointerEXT;
extern	PFNGLGETVERTEXATTRIBIIVEXTPROC			glGetVertexAttribIivEXT;
extern	PFNGLGETVERTEXATTRIBIUIVEXTPROC			glGetVertexAttribIuivEXT;
															
														// EXT_geometry_shader4
extern	PFNGLPROGRAMPARAMETERIEXTPROC			glProgramParameteriEXT;
extern	PFNGLFRAMEBUFFERTEXTUREEXTPROC			glFramebufferTextureEXT;
extern	PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC		glFramebufferTextureLayerEXT;
extern	PFNGLFRAMEBUFFERTEXTUREFACEEXTPROC		glFramebufferTextureFaceEXT;

														// EXT_bindable_uniform
extern	PFNGLUNIFORMBUFFEREXTPROC				glUniformBufferEXT;
extern	PFNGLGETUNIFORMBUFFERSIZEEXTPROC		glGetUniformBufferSizeEXT;
extern	PFNGLGETUNIFORMOFFSETEXTPROC			glGetUniformOffsetEXT;

														// OpenGL 2.0 shader 														
extern	PFNGLCOMPILESHADERPROC					glCompileShader;
extern	PFNGLCREATEPROGRAMPROC					glCreateProgram;
extern	PFNGLCREATESHADERPROC					glCreateShader;
extern	PFNGLDELETEPROGRAMPROC					glDeleteProgram;
extern	PFNGLDELETESHADERPROC					glDeleteShader;
extern	PFNGLDETACHSHADERPROC					glDetachShader;
extern	PFNGLGETPROGRAMINFOLOGPROC				glGetProgramInfoLog;
extern	PFNGLGETSHADERINFOLOGPROC				glGetShaderInfoLog;
extern	PFNGLGETSHADERSOURCEPROC				glGetShaderSource;
extern	PFNGLLINKPROGRAMPROC					glLinkProgram;
extern	PFNGLSHADERSOURCEPROC					glShaderSource;
extern	PFNGLUSEPROGRAMPROC						glUseProgram;
extern	PFNGLVALIDATEPROGRAMPROC				glValidateProgram;
extern	PFNGLGETPROGRAMIVPROC					glGetProgramiv;
extern	PFNGLGETSHADERIVPROC					glGetShaderiv;
extern	PFNGLATTACHSHADERPROC					glAttachShader;
extern	PFNGLGETUNIFORMLOCATIONPROC				glGetUniformLocation;
extern	PFNGLGETATTRIBLOCATIONPROC				glGetAttribLocation;

extern	PFNGLUNIFORM4FVPROC						glUniform4fv;
extern	PFNGLUNIFORM3FVPROC						glUniform3fv;
extern	PFNGLUNIFORM2FVPROC						glUniform2fv;
extern	PFNGLUNIFORM1FPROC						glUniform1f;
extern	PFNGLUNIFORM1IPROC						glUniform1i;
extern	PFNGLUNIFORMMATRIX2FVPROC				glUniformMatrix2fv;
extern	PFNGLUNIFORMMATRIX3FVPROC				glUniformMatrix3fv;
extern	PFNGLUNIFORMMATRIX4FVPROC				glUniformMatrix4fv;
extern	PFNGLGETUNIFORMFVPROC					glGetUniformfv;
extern	PFNGLVERTEXATTRIB4FVPROC				glVertexAttrib4fv;
extern	PFNGLGETVERTEXATTRIBFVPROC				glGetVertexAttribfv;

                                    // OOpenGL 2 query support
extern  PFNGLGENQUERIESPROC                  glGenQueries;
extern  PFNGLDELETEQUERIESPROC               glDeleteQueries;
extern  PFNGLISQUERYPROC                     glIsQuery;
extern  PFNGLBEGINQUERYPROC                  glBeginQuery;
extern  PFNGLENDQUERYPROC                    glEndQuery;
extern  PFNGLGETQUERYIVPROC                  glGetQueryiv;
extern  PFNGLGETQUERYOBJECTIVPROC            glGetQueryObjectiv;
extern  PFNGLGETQUERYOBJECTUIVPROC           glGetQueryObjectuiv;


														// NV_transform_feedback
extern	PFNGLBEGINTRANSFORMFEEDBACKNVPROC		glBeginTransformFeedbackNV;
extern	PFNGLENDTRANSFORMFEEDBACKNVPROC			glEndTransformFeedbackNV;
extern	PFNGLTRANSFORMFEEDBACKATTRIBSNVPROC		glTransformFeedbackAttribsNV;
extern	PFNGLBINDBUFFERRANGENVPROC				glBindBufferRangeNV;
extern	PFNGLBINDBUFFEROFFSETNVPROC				glBindBufferOffsetNV;
extern	PFNGLBINDBUFFERBASENVPROC				glBindBufferBaseNV;
extern	PFNGLTRANSFORMFEEDBACKVARYINGSNVPROC	glTransformFeedbackVaryingsNV;
extern	PFNGLACTIVEVARYINGNVPROC				glActiveVaryingNV;
extern	PFNGLGETVARYINGLOCATIONNVPROC			glGetVaryingLocationNV;
extern	PFNGLGETACTIVEVARYINGNVPROC				glGetActiveVaryingNV;
extern	PFNGLGETTRANSFORMFEEDBACKVARYINGNVPROC	glGetTransformFeedbackVaryingNV;

														// NV_fence
extern	PFNGLDELETEFENCESNVPROC					glDeleteFencesNV;
extern	PFNGLGENFENCESNVPROC					glGenFencesNV;
extern	PFNGLISFENCENVPROC						glIsFenceNV;
extern	PFNGLTESTFENCENVPROC					glTestFenceNV;
extern	PFNGLGETFENCEIVNVPROC					glGetFenceivNV;
extern	PFNGLFINISHFENCENVPROC					glFinishFenceNV;
extern	PFNGLSETFENCENVPROC						glSetFenceNV;

														// NV__conditional_render
extern	PFNGLBEGINCONDITIONALRENDERNVPROC		glBeginConditionalRenderNV;
extern	PFNGLENDCONDITIONALRENDERNVPROC			glEndConditionalRenderNV;

														// ARB_sampler_objects
#ifndef	PFNGLGENSAMPLERSPROC
	typedef void (APIENTRYP PFNGLGENSAMPLERSPROC) (GLsizei count, GLuint *samplers);
	typedef void (APIENTRYP PFNGLDELETESAMPLERSPROC) (GLsizei count, const GLuint *samplers);
	typedef GLboolean (APIENTRYP PFNGLISSAMPLERPROC) (GLuint sampler);
	typedef void (APIENTRYP PFNGLBINDSAMPLERPROC) (GLuint unit, GLuint sampler);
	typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIPROC) (GLuint sampler, GLenum pname, GLint param);
	typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIVPROC) (GLuint sampler, GLenum pname, const GLint *param);
	typedef void (APIENTRYP PFNGLSAMPLERPARAMETERFPROC) (GLuint sampler, GLenum pname, GLfloat param);
	typedef void (APIENTRYP PFNGLSAMPLERPARAMETERFVPROC) (GLuint sampler, GLenum pname, const GLfloat *param);
	typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIIVPROC) (GLuint sampler, GLenum pname, const GLint *param);
	typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIUIVPROC) (GLuint sampler, GLenum pname, const GLuint *param);
	typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERIVPROC) (GLuint sampler, GLenum pname, GLint *params);
	typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERIIVPROC) (GLuint sampler, GLenum pname, GLint *params);
	typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERFVPROC) (GLuint sampler, GLenum pname, GLfloat *params);
	typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERIFVPROC) (GLuint sampler, GLenum pname, GLfloat *params);
#endif

extern	PFNGLGENSAMPLERSPROC			glGenSamplers;
extern	PFNGLDELETESAMPLERSPROC			glDeleteSamplers;
extern	PFNGLISSAMPLERPROC				glIsSampler;
extern	PFNGLBINDSAMPLERPROC			glBindSampler;
extern	PFNGLSAMPLERPARAMETERIPROC		glSamplerParameteri;
extern	PFNGLSAMPLERPARAMETERIVPROC		glSamplerParameteriv;
extern	PFNGLSAMPLERPARAMETERFPROC		glSamplerParameterf;
extern	PFNGLSAMPLERPARAMETERFVPROC		glSamplerParameterfv;
extern	PFNGLSAMPLERPARAMETERIIVPROC	glSamplerParameterIiv;
extern	PFNGLSAMPLERPARAMETERIUIVPROC	glSamplerParameterIuiv;
extern	PFNGLGETSAMPLERPARAMETERIVPROC	glGetSamplerParameteriv;
extern	PFNGLGETSAMPLERPARAMETERIIVPROC	glGetSamplerParameterIiv;
extern	PFNGLGETSAMPLERPARAMETERFVPROC	glGetSamplerParameterfv;
extern	PFNGLGETSAMPLERPARAMETERIFVPROC	glGetSamplerParameterIfv;
														
														// ARB_vertex_array_object
extern	PFNGLBINDVERTEXARRAYPROC		glBindVertexArray;
extern	PFNGLDELETEVERTEXARRAYSPROC		glDeleteVertexArrays;
extern	PFNGLGENVERTEXARRAYSPROC		glGenVertexArrays;
extern	PFNGLISVERTEXARRAYPROC			glIsVertexArray;

														// ARB_get_program_binary
#define	GL_PROGRAM_BINARY_RETRIEVABLE_HINT             0x8257
#define	GL_PROGRAM_BINARY_LENGTH                       0x8741
#define	GL_NUM_PROGRAM_BINARY_FORMATS                  0x87FE
#define	GL_PROGRAM_BINARY_FORMATS                      0x87FF

typedef void (APIENTRYP PFNGLGETPROGRAMBINARYPROC)  ( GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary );
typedef void (APIENTRYP PFNGLPROGRAMBINARYPROC)     ( GLuint program, GLenum binaryFormat, const GLvoid *binary, GLsizei length );
typedef void (APIENTRYP PFNGLPROGRAMPARAMETERIPROC) ( GLuint program, GLenum pname, GLint value );

extern PFNGLGETPROGRAMBINARYPROC  glGetProgramBinary;
extern PFNGLPROGRAMBINARYPROC     glProgramBinary;
extern PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;

														// ARB_instanced_arrays
extern	PFNGLVERTEXATTRIBDIVISORARBPROC	glVertexAttribDivisor;

#endif
