/*
 * Copyright (C) 2007 Jan Dvorak <jan.dvorak@kraxnet.cz>
 *
 * This program is distributed under the terms of the MIT license.
 * See the included MIT-LICENSE file for the terms of this license.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "sdl_opengl/common.h"

/* OpenGL EXT extensions */
/* Those are extensions that are not blessed by ARB committee but were
   created or agreed upon by multiple vendors */

/* #2 - GL_EXT_blend_color */
GL_FUNC_LOAD_4(BlendColorEXT,GLvoid, GLclampf,GLclampf,GLclampf,GLclampf, "GL_EXT_blend_color")

/* #3 - GL_EXT_polygon_offset */
GL_FUNC_LOAD_2(PolygonOffsetEXT,GLvoid, GLfloat,GLfloat, "GL_EXT_polygon_offset")

/* #20 - GL_EXT_texture_object */
GL_FUNC_GENOBJECTS_LOAD(GenTexturesEXT,"GL_EXT_texture_object")
GL_FUNC_DELETEOBJECTS_LOAD(DeleteTexturesEXT,"GL_EXT_texture_object")
GL_FUNC_LOAD_2(BindTextureEXT,GLvoid, GLenum,GLint, "GL_EXT_texture_object")

static VALUE
gl_PrioritizeTexturesEXT(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLuint *textures;
	GLclampf *priorities;
	GLsizei size;
  DECL_GL_FUNC_PTR(GLvoid,glPrioritizeTexturesEXT,(GLsizei,const GLuint *,const GLclampf *));
	LOAD_GL_FUNC(glPrioritizeTexturesEXT, "GL_EXT_texture_object");
	Check_Type(arg1,T_ARRAY);
	Check_Type(arg2,T_ARRAY);
	if ((size = (GLsizei)RARRAY_LENINT(arg1)) != (GLsizei)RARRAY_LENINT(arg2))
		rb_raise(rb_eArgError, "passed arrays must have the same length");
	textures = ALLOC_N(GLuint,size);
	priorities = ALLOC_N(GLclampf,size);
	ary2cuint(arg1,textures,size);
	ary2cflt(arg2,priorities,size);
	fptr_glPrioritizeTexturesEXT(size,textures,priorities);
	xfree(textures);
	xfree(priorities);
	CHECK_GLERROR_FROM("glPrioritizeTexturesEXT");
	return Qnil;
}

static VALUE
gl_AreTexturesResidentEXT(obj,arg1)
VALUE obj,arg1;
{
	GLuint *textures;
	GLboolean *residences;
	GLsizei size;
	GLboolean r;
	VALUE retary;
	VALUE ary;
	int i;
  DECL_GL_FUNC_PTR(GLboolean,glAreTexturesResidentEXT,(GLsizei,const GLuint *,GLboolean *));
	LOAD_GL_FUNC(glAreTexturesResidentEXT, "GL_EXT_texture_object");
	ary = rb_Array(arg1);
	size = (GLsizei)RARRAY_LENINT(ary);
	textures = ALLOC_N(GLuint,size);
	residences = ALLOC_N(GLboolean,size);
	ary2cuint(ary,textures,size);
	r = fptr_glAreTexturesResidentEXT(size,textures,residences);
	retary = rb_ary_new2(size);
	if (r==GL_TRUE) { /* all are resident */
		for(i=0;i<size;i++)
			rb_ary_push(retary, GLBOOL2RUBY(GL_TRUE));
	} else {
		for(i=0;i<size;i++)
			rb_ary_push(retary, GLBOOL2RUBY(residences[i]));
	}
	xfree(textures);
	xfree(residences);
	CHECK_GLERROR_FROM("glAreTexturesResidentEXT");
	return retary;
}

GL_FUNC_LOAD_1(IsTextureEXT,GLboolean, GLuint, "GL_EXT_texture_object")

/* #37 - GL_EXT_blend_minmax */
GL_FUNC_LOAD_1(BlendEquationEXT,GLvoid, GLenum, "GL_EXT_blend_minmax")

/* #54 - GL_EXT_point_parameters */
GL_FUNC_LOAD_2(PointParameterfEXT,GLvoid, GLenum,GLfloat, "GL_EXT_point_parameters")

static VALUE gl_PointParameterfvEXT(VALUE obj,VALUE arg1,VALUE arg2)
{
	GLfloat params[3] = {(GLfloat)0.0,(GLfloat)0.0,(GLfloat)0.0};
	GLenum pname;
	GLint size;
  DECL_GL_FUNC_PTR(GLvoid,glPointParameterfvEXT,(GLenum,GLfloat *));
	LOAD_GL_FUNC(glPointParameterfvEXT, "GL_EXT_point_parameters");
	pname = NUM2UINT(arg1);
	Check_Type(arg2,T_ARRAY);
	if (pname==GL_POINT_DISTANCE_ATTENUATION)
		size = 3;
	else
		size = 1;
	ary2cflt(arg2,params,size);
	fptr_glPointParameterfvEXT(pname,params);
	CHECK_GLERROR_FROM("glPointParameterfvEXT");
	return Qnil;
}

/* #145 - GL_EXT_secondary_color */
GL_FUNC_LOAD_3(SecondaryColor3bEXT,GLvoid, GLbyte,GLbyte,GLbyte, "GL_EXT_secondary_color")
GL_FUNC_LOAD_3(SecondaryColor3sEXT,GLvoid, GLshort,GLshort,GLshort, "GL_EXT_secondary_color")
GL_FUNC_LOAD_3(SecondaryColor3iEXT,GLvoid, GLint,GLint,GLint, "GL_EXT_secondary_color")
GL_FUNC_LOAD_3(SecondaryColor3fEXT,GLvoid, GLfloat,GLfloat,GLfloat, "GL_EXT_secondary_color")
GL_FUNC_LOAD_3(SecondaryColor3dEXT,GLvoid, GLdouble,GLdouble,GLdouble, "GL_EXT_secondary_color")
GL_FUNC_LOAD_3(SecondaryColor3ubEXT,GLvoid, GLubyte,GLubyte,GLubyte, "GL_EXT_secondary_color")
GL_FUNC_LOAD_3(SecondaryColor3usEXT,GLvoid, GLushort,GLushort,GLushort, "GL_EXT_secondary_color")
GL_FUNC_LOAD_3(SecondaryColor3uiEXT,GLvoid, GLuint,GLuint,GLuint, "GL_EXT_secondary_color")

#define GLSECONDARYCOLOREXT_VFUNC(_name_,_type_,_conv_) \
VALUE gl_##_name_(VALUE obj,VALUE arg1) \
{ \
	_type_ cary[3] = {0,0,0}; \
  DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(_type_ *)); \
	LOAD_GL_FUNC(gl##_name_, "GL_EXT_secondary_color"); \
	Check_Type(arg1,T_ARRAY); \
	_conv_(arg1,cary,3); \
	fptr_gl##_name_(cary); \
	CHECK_GLERROR_FROM("gl" #_name_); \
	return Qnil; \
}

GLSECONDARYCOLOREXT_VFUNC(SecondaryColor3bvEXT,GLbyte,ary2cbyte)
GLSECONDARYCOLOREXT_VFUNC(SecondaryColor3dvEXT,GLdouble,ary2cdbl)
GLSECONDARYCOLOREXT_VFUNC(SecondaryColor3fvEXT,GLfloat,ary2cflt)
GLSECONDARYCOLOREXT_VFUNC(SecondaryColor3ivEXT,GLint,ary2cint)
GLSECONDARYCOLOREXT_VFUNC(SecondaryColor3svEXT,GLshort,ary2cshort)
GLSECONDARYCOLOREXT_VFUNC(SecondaryColor3uivEXT,GLuint,ary2cuint)
GLSECONDARYCOLOREXT_VFUNC(SecondaryColor3ubvEXT,GLubyte,ary2cubyte)
GLSECONDARYCOLOREXT_VFUNC(SecondaryColor3usvEXT,GLushort,ary2cushort)
#undef GLSECONDARYCOLOREXT_VFUNC

static VALUE
gl_SecondaryColorPointerEXT(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLint size;
	GLenum type;
	GLsizei stride;
  DECL_GL_FUNC_PTR(GLvoid,glSecondaryColorPointerEXT,(GLint,GLenum,GLsizei,const GLvoid *));
	LOAD_GL_FUNC(glSecondaryColorPointerEXT, "GL_EXT_secondary_color");
	size = (GLint)NUM2INT(arg1);
	type = (GLenum)NUM2INT(arg2);
	stride = (GLsizei)NUM2UINT(arg3);
	if (CHECK_BUFFER_BINDING(GL_ARRAY_BUFFER_BINDING)) {
		SET_GLIMPL_VARIABLE(SecondaryColor_ptr, arg4);
		fptr_glSecondaryColorPointerEXT(size,type, stride, (const GLvoid*)NUM2SIZET(arg4));
	} else {
		VALUE data;
		data = pack_array_or_pass_string(type,arg4);
		rb_str_freeze(data);
		SET_GLIMPL_VARIABLE(SecondaryColor_ptr, data);
		fptr_glSecondaryColorPointerEXT(size,type, stride, (const GLvoid*)RSTRING_PTR(data));
	}
	CHECK_GLERROR_FROM("glSecondaryColorPointerEXT");
	return Qnil;
}

/* #97 - GL_EXT_compiled_vertex_array */
GL_FUNC_LOAD_2(LockArraysEXT,GLvoid, GLint,GLint, "GL_EXT_compiled_vertex_array")
GL_FUNC_LOAD_0(UnlockArraysEXT,GLvoid, "GL_EXT_compiled_vertex_array")

/* #112 - GL_EXT_draw_range_elements */
static VALUE
gl_DrawRangeElementsEXT(obj,arg1,arg2,arg3,arg4,arg5,arg6)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6;
{
	GLenum mode;
	GLuint start;
	GLuint end;
	GLsizei count;
	GLenum type;
  DECL_GL_FUNC_PTR(GLvoid,glDrawRangeElementsEXT,(GLenum,GLuint,GLuint,GLsizei,GLenum,GLvoid*));
	LOAD_GL_FUNC(glDrawRangeElementsEXT, "GL_EXT_draw_range_elements");
	mode = (GLenum)NUM2INT(arg1);
	start = (GLuint)NUM2UINT(arg2);
	end = (GLuint)NUM2UINT(arg3);
	count = (GLsizei)NUM2UINT(arg4);
	type = (GLenum)NUM2INT(arg5);
	if (CHECK_BUFFER_BINDING(GL_ELEMENT_ARRAY_BUFFER_BINDING)) {
		fptr_glDrawRangeElementsEXT(mode, start, end, count, type, (GLvoid *)NUM2SIZET(arg6));
	} else {
		VALUE data;
		data = pack_array_or_pass_string(type,arg6);
		fptr_glDrawRangeElementsEXT(mode, start, end, count, type, RSTRING_PTR(data));
	}
	CHECK_GLERROR_FROM("glDrawRangeElementsEXT");
	return Qnil;
}

/* #148 - GL_EXT_multi_draw_arrays */
static VALUE
gl_MultiDrawArraysEXT(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum mode;
	GLint *ary1;
	GLsizei *ary2;
  int len1,len2;
  DECL_GL_FUNC_PTR(GLvoid,glMultiDrawArraysEXT,(GLenum,GLint*,GLsizei*,GLsizei));
	LOAD_GL_FUNC(glMultiDrawArraysEXT, "GL_EXT_multi_draw_arrays");
  len1 = (int)RARRAY_LENINT(arg2);
  len2 = (int)RARRAY_LENINT(arg3);
	if (len1!=len2)
			rb_raise(rb_eArgError, "Passed arrays must have same length");
	mode = (GLenum)NUM2INT(arg1);
	ary1 = ALLOC_N(GLint,len1);
	ary2 = ALLOC_N(GLsizei,len2);
	ary2cint(arg2,ary1,len1);
	ary2cint(arg3,ary2,len2);
	fptr_glMultiDrawArraysEXT(mode,ary1,ary2,len1);
	xfree(ary1);
	xfree(ary2);
	CHECK_GLERROR_FROM("glMultiDrawArraysEXT");
	return Qnil;
}

static VALUE
gl_MultiDrawElementsEXT(argc,argv,obj)
int argc;
VALUE *argv;
VALUE obj;
{
	GLenum mode;
	GLenum type;
	GLsizei *counts;
	GLvoid **indices;
	GLint size;
	int i;
	VALUE ary, args[4];
  DECL_GL_FUNC_PTR(GLvoid,glMultiDrawElementsEXT,(GLenum,const GLsizei *,GLenum,GLvoid **,GLsizei));
	LOAD_GL_FUNC(glMultiDrawElementsEXT, "GL_EXT_multi_draw_arrays");
	switch (rb_scan_args(argc, argv, "31", &args[0], &args[1], &args[2],&args[3])) {
		default:
		case 3:
			if (CHECK_BUFFER_BINDING(GL_ELEMENT_ARRAY_BUFFER_BINDING))
				rb_raise(rb_eArgError, "Element array buffer bound, but offsets array missing");
			mode = (GLenum)NUM2INT(args[0]);
			type = (GLenum)NUM2INT(args[1]);
			Check_Type(args[2],T_ARRAY);
			ary = args[2];
			size = (GLsizei)RARRAY_LENINT(ary);
			counts = ALLOC_N(GLsizei,size);
			indices = ALLOC_N(GLvoid*,size);
			for (i=0;i<size;i++) {
				VALUE data;
				data = pack_array_or_pass_string(type,RARRAY_PTR(ary)[i]);

				indices[i] = RSTRING_PTR(data);
				counts[i] = (GLsizei)RSTRING_LENINT(data);
			}
			fptr_glMultiDrawElementsEXT(mode,counts,type,indices,size);
			xfree(counts);
			xfree(indices);
			break;
		case 4:
			if (!CHECK_BUFFER_BINDING(GL_ELEMENT_ARRAY_BUFFER_BINDING))
				rb_raise(rb_eArgError, "Element array buffer not bound");
			mode = (GLenum)NUM2INT(args[0]);
			type = (GLenum)NUM2INT(args[1]);
			Check_Type(args[2],T_ARRAY);
			Check_Type(args[3],T_ARRAY);
			if (RARRAY_LEN(args[2]) != RARRAY_LEN(args[3]))
				rb_raise(rb_eArgError, "Count and indices offset array must have same length");

			size = (GLsizei)RARRAY_LENINT(args[2]);

			counts = ALLOC_N(GLsizei,size);
			indices = ALLOC_N(GLvoid*,size);
			for (i=0;i<size;i++) {
				counts[i] = NUM2INT(rb_ary_entry(args[2],i));
				indices[i] = (GLvoid *)NUM2SIZET(rb_ary_entry(args[3],i));
			}
			fptr_glMultiDrawElementsEXT(mode,counts,type,indices,size);
			xfree(counts);
			xfree(indices);
			break;
	}
	CHECK_GLERROR_FROM("glMultiDrawElementsEXT");
	return Qnil;
}

/* #149 - GL_EXT_fog_coord */
GL_FUNC_LOAD_1(FogCoordfEXT,GLvoid, GLfloat, "GL_EXT_fog_coord")
GL_FUNC_LOAD_1(FogCoorddEXT,GLvoid, GLdouble, "GL_EXT_fog_coord")

#define GLFOGCOORD_VFUNC(_name_,_type_,_conv_) \
VALUE gl_##_name_(VALUE obj,VALUE arg1) \
{ \
	_type_ cary = 0; \
  DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(_type_ *)); \
	LOAD_GL_FUNC(gl##_name_, "GL_EXT_secondary_color"); \
	Check_Type(arg1,T_ARRAY); \
	_conv_(arg1,&cary,1); \
	fptr_gl##_name_(&cary); \
	CHECK_GLERROR_FROM("gl" #_name_); \
	return Qnil; \
}
GLFOGCOORD_VFUNC(FogCoordfvEXT,GLfloat,ary2cflt)
GLFOGCOORD_VFUNC(FogCoorddvEXT,GLdouble,ary2cdbl)
#undef GLFOGCOORD_VFUNC

static VALUE
gl_FogCoordPointerEXT(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum type;
	GLsizei stride;
  DECL_GL_FUNC_PTR(GLvoid,glFogCoordPointerEXT,(GLenum,GLsizei,const GLvoid *));
	LOAD_GL_FUNC(glFogCoordPointerEXT, "GL_EXT_secondary_color");
	type = (GLenum)NUM2INT(arg1);
	stride = (GLsizei)NUM2UINT(arg2);
	if (CHECK_BUFFER_BINDING(GL_ARRAY_BUFFER_BINDING)) {
		SET_GLIMPL_VARIABLE(FogCoord_ptr, arg3);
		fptr_glFogCoordPointerEXT(type, stride, (const GLvoid*)NUM2SIZET(arg3));
	} else {
		VALUE data;
		data = pack_array_or_pass_string(type,arg3);
		rb_str_freeze(data);
		SET_GLIMPL_VARIABLE(FogCoord_ptr, data);
		fptr_glFogCoordPointerEXT(type, stride, (const GLvoid*)RSTRING_PTR(data));
	}
	CHECK_GLERROR_FROM("glFogCoordPointerEXT");
	return Qnil;
}

/* #173 - GL_EXT_blend_func_separate */
GL_FUNC_LOAD_4(BlendFuncSeparateEXT,GLvoid, GLenum,GLenum,GLenum,GLenum, "GL_EXT_blend_func_separate")

/* #268 - GL_EXT_stencil_two_side */
GL_FUNC_LOAD_1(ActiveStencilFaceEXT,GLvoid, GLenum, "GL_EXT_stencil_two_side")

/* #297 - GL_EXT_depth_bounds_test */
GL_FUNC_LOAD_2(DepthBoundsEXT,GLvoid, GLclampd,GLclampd, "GL_EXT_depth_bounds_test")

/* #299 - GL_EXT_blend_equation_separate */
GL_FUNC_LOAD_2(BlendEquationSeparateEXT,GLvoid, GLenum,GLenum, "GL_EXT_blend_equation_separate")

/* #310 - GL_EXT_framebuffer_object */
GL_FUNC_LOAD_1(IsRenderbufferEXT,GLboolean, GLuint, "GL_EXT_framebuffer_object")
GL_FUNC_LOAD_2(BindRenderbufferEXT,GLvoid, GLenum,GLuint, "GL_EXT_framebuffer_object")
GL_FUNC_LOAD_4(RenderbufferStorageEXT,GLvoid, GLenum,GLenum,GLsizei,GLsizei, "GL_EXT_framebuffer_object")
GL_FUNC_GENOBJECTS_LOAD(GenRenderbuffersEXT,"GL_EXT_framebuffer_object")
GL_FUNC_DELETEOBJECTS_LOAD(DeleteRenderbuffersEXT,"GL_EXT_framebuffer_object")

static VALUE gl_GetRenderbufferParameterivEXT(VALUE obj,VALUE arg1,VALUE arg2)
{
	GLint param = 0;
  DECL_GL_FUNC_PTR(GLvoid,glGetRenderbufferParameterivEXT,(GLenum,GLenum,GLint *));
	LOAD_GL_FUNC(glGetRenderbufferParameterivEXT, "GL_EXT_framebuffer_object");
	fptr_glGetRenderbufferParameterivEXT(NUM2UINT(arg1),NUM2UINT(arg2),&param);
	CHECK_GLERROR_FROM("glGetRenderbufferParameterivEXT");
	return INT2NUM(param);
}

GL_FUNC_GENOBJECTS_LOAD(GenFramebuffersEXT,"GL_EXT_framebuffer_object")
GL_FUNC_DELETEOBJECTS_LOAD(DeleteFramebuffersEXT,"GL_EXT_framebuffer_object")
GL_FUNC_LOAD_1(IsFramebufferEXT,GLboolean, GLuint, "GL_EXT_framebuffer_object")
GL_FUNC_LOAD_2(BindFramebufferEXT,GLvoid, GLenum,GLuint, "GL_EXT_framebuffer_object")
GL_FUNC_LOAD_1(CheckFramebufferStatusEXT,GLenum, GLenum, "GL_EXT_framebuffer_object")
GL_FUNC_LOAD_5(FramebufferTexture1DEXT,GLvoid, GLenum,GLenum,GLenum,GLuint,GLint, "GL_EXT_framebuffer_object")
GL_FUNC_LOAD_5(FramebufferTexture2DEXT,GLvoid, GLenum,GLenum,GLenum,GLuint,GLint, "GL_EXT_framebuffer_object")
GL_FUNC_LOAD_6(FramebufferTexture3DEXT,GLvoid, GLenum,GLenum,GLenum,GLuint,GLint,GLint, "GL_EXT_framebuffer_object")

GL_FUNC_LOAD_4(FramebufferRenderbufferEXT,GLvoid, GLuint,GLuint,GLuint,GLuint, "GL_EXT_framebuffer_object")

static VALUE gl_GetFramebufferAttachmentParameterivEXT(VALUE obj,VALUE arg1, VALUE arg2, VALUE arg3)
{
	GLint ret = 0;
  DECL_GL_FUNC_PTR(GLvoid,glGetFramebufferAttachmentParameterivEXT,(GLenum,GLenum,GLenum,GLint *));
	LOAD_GL_FUNC(glGetFramebufferAttachmentParameterivEXT, "GL_EXT_framebuffer_object");
	fptr_glGetFramebufferAttachmentParameterivEXT(NUM2UINT(arg1),NUM2UINT(arg2),NUM2UINT(arg3),&ret);
	CHECK_GLERROR_FROM("glGetFramebufferAttachmentParameterivEXT");
	return cond_GLBOOL2RUBY(NUM2UINT(arg3),ret);
}

GL_FUNC_LOAD_1(GenerateMipmapEXT,GLvoid, GLenum, "GL_EXT_framebuffer_object")

/* #314 - GL_EXT_stencil_clear_tag */
GL_FUNC_LOAD_2(StencilClearTagEXT,GLvoid, GLsizei,GLuint, "GL_EXT_stencil_clear_tag")

/* #316 - GL_EXT_framebuffer_blit */
GL_FUNC_LOAD_10(BlitFramebufferEXT,GLvoid, GLint,GLint,GLint,GLint,GLint,GLint,GLint,GLint,GLbitfield,GLenum, "GL_EXT_framebuffer_blit")

/* #317 - GL_EXT_framebuffer_multisample */
GL_FUNC_LOAD_5(RenderbufferStorageMultisampleEXT,GLvoid, GLenum,GLsizei,GLenum,GLsizei,GLsizei, "GL_EXT_framebuffer_multisample")

/* #319 - GL_EXT_timer_query */

#define GETQUERY_FUNC(_name_,_type_,_conv_) \
static VALUE gl_##_name_(VALUE obj,VALUE arg1,VALUE arg2) \
{ \
	_type_ ret = 0; \
  DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLuint,GLenum,_type_ *)); \
	LOAD_GL_FUNC(gl##_name_, "GL_EXT_timer_query"); \
	fptr_gl##_name_(NUM2INT(arg1),NUM2INT(arg2),&ret); \
	CHECK_GLERROR_FROM("gl" #_name_); \
	return _conv_(NUM2INT(arg2),ret); \
}

GETQUERY_FUNC(GetQueryObjecti64vEXT,GLint64,cond_GLBOOL2RUBY_LL)
GETQUERY_FUNC(GetQueryObjectui64vEXT,GLuint64,cond_GLBOOL2RUBY_ULL)
#undef GETQUERY_FUNC


/* #320 - GL_EXT_gpu_program_parameters */
#define PROGRAMPARAM_MULTI_FUNC_V(_name_,_type_,_conv_,_extension_) \
static VALUE \
gl_##_name_(obj,arg1,arg2,arg3) \
VALUE obj,arg1,arg2,arg3; \
{ \
	_type_ *cary; \
	GLsizei len; \
  DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLenum,GLuint,GLsizei,const _type_ *)); \
	LOAD_GL_FUNC(gl##_name_, _extension_); \
	len = (GLsizei)RARRAY_LENINT(rb_Array(arg3)); \
	if (len<=0 || (len % 4) != 0) \
		rb_raise(rb_eArgError, "Parameter array size must be multiplication of 4"); \
	cary = ALLOC_N(_type_,len); \
	_conv_(arg3,cary,len); \
	fptr_gl##_name_((GLenum)NUM2UINT(arg1),(GLuint)NUM2UINT(arg2),len / 4, cary); \
	xfree(cary); \
	CHECK_GLERROR_FROM("gl" #_name_); \
	return Qnil; \
}

PROGRAMPARAM_MULTI_FUNC_V(ProgramEnvParameters4fvEXT,GLfloat,ary2cflt,"GL_EXT_gpu_program_parameters")
PROGRAMPARAM_MULTI_FUNC_V(ProgramLocalParameters4fvEXT,GLfloat,ary2cflt,"GL_EXT_gpu_program_parameters")
#undef PROGRAMPARAM_MULTI_FUNC_V

/* #324 - GL_EXT_geometry_shader4 */
GL_FUNC_LOAD_3(ProgramParameteriEXT,GLvoid, GLuint,GLenum,GLint, "GL_EXT_geometry_shader4")

/* #326 - GL_EXT_gpu_shader4 */
GL_FUNC_LOAD_2(VertexAttribI1iEXT,GLvoid, GLuint,GLint, "GL_EXT_gpu_shader4")
GL_FUNC_LOAD_3(VertexAttribI2iEXT,GLvoid, GLuint,GLint,GLint, "GL_EXT_gpu_shader4")
GL_FUNC_LOAD_4(VertexAttribI3iEXT,GLvoid, GLuint,GLint,GLint,GLint, "GL_EXT_gpu_shader4")
GL_FUNC_LOAD_5(VertexAttribI4iEXT,GLvoid, GLuint,GLint,GLint,GLint,GLint, "GL_EXT_gpu_shader4")
GL_FUNC_LOAD_2(VertexAttribI1uiEXT,GLvoid, GLuint,GLuint, "GL_EXT_gpu_shader4")
GL_FUNC_LOAD_3(VertexAttribI2uiEXT,GLvoid, GLuint,GLuint,GLuint, "GL_EXT_gpu_shader4")
GL_FUNC_LOAD_4(VertexAttribI3uiEXT,GLvoid, GLuint,GLuint,GLuint,GLuint, "GL_EXT_gpu_shader4")
GL_FUNC_LOAD_5(VertexAttribI4uiEXT,GLvoid, GLuint,GLuint,GLuint,GLuint,GLuint, "GL_EXT_gpu_shader4")

#define GLVERTEXATTRIB_VFUNC(_name_,_type_,_conv_,_size_) \
static VALUE \
gl_##_name_(obj,arg1,arg2) \
VALUE obj,arg1,arg2; \
{ \
	_type_ value[_size_]; \
  DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLuint,const _type_ *)); \
	LOAD_GL_FUNC(gl##_name_, "GL_EXT_gpu_shader4"); \
	_conv_(arg2,value,_size_); \
	fptr_gl##_name_(NUM2UINT(arg1),value); \
	CHECK_GLERROR_FROM("gl" #_name_); \
	return Qnil; \
}

GLVERTEXATTRIB_VFUNC(VertexAttribI1ivEXT,GLint,ary2cint,1)
GLVERTEXATTRIB_VFUNC(VertexAttribI2ivEXT,GLint,ary2cint,2)
GLVERTEXATTRIB_VFUNC(VertexAttribI3ivEXT,GLint,ary2cint,3)
GLVERTEXATTRIB_VFUNC(VertexAttribI4ivEXT,GLint,ary2cint,4)
GLVERTEXATTRIB_VFUNC(VertexAttribI1uivEXT,GLuint,ary2cuint,1)
GLVERTEXATTRIB_VFUNC(VertexAttribI2uivEXT,GLuint,ary2cuint,2)
GLVERTEXATTRIB_VFUNC(VertexAttribI3uivEXT,GLuint,ary2cuint,3)
GLVERTEXATTRIB_VFUNC(VertexAttribI4uivEXT,GLuint,ary2cuint,4)
GLVERTEXATTRIB_VFUNC(VertexAttribI4bvEXT,GLbyte,ary2cbyte,4)
GLVERTEXATTRIB_VFUNC(VertexAttribI4svEXT,GLshort,ary2cshort,4)
GLVERTEXATTRIB_VFUNC(VertexAttribI4ubvEXT,GLubyte,ary2cubyte,4)
GLVERTEXATTRIB_VFUNC(VertexAttribI4usvEXT,GLushort,ary2cushort,4)
#undef GLVERTEXATTRIB_VFUNC

#define GETVERTEXATTRIB_FUNC(_name_,_type_,_conv_,_extension_) \
static VALUE \
gl_##_name_(obj,arg1,arg2) \
VALUE obj,arg1,arg2; \
{ \
	GLuint index; \
	GLenum pname; \
	_type_ params[4] = {0,0,0,0}; \
	GLint size; \
  DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLuint,GLenum,_type_ *)); \
	LOAD_GL_FUNC(gl##_name_, _extension_); \
	index = (GLuint)NUM2UINT(arg1); \
	pname = (GLenum)NUM2INT(arg2); \
	if (pname==GL_CURRENT_VERTEX_ATTRIB_ARB) \
		size = 4; \
	else \
		size = 1; \
	fptr_gl##_name_(index,pname,params); \
	RET_ARRAY_OR_SINGLE_BOOL("gl" #_name_, size, _conv_, pname, params); \
}

GETVERTEXATTRIB_FUNC(GetVertexAttribIivEXT,GLint,cond_GLBOOL2RUBY,"GL_EXT_gpu_shader4")
GETVERTEXATTRIB_FUNC(GetVertexAttribIuivEXT,GLuint,cond_GLBOOL2RUBY_U,"GL_EXT_gpu_shader4")
#undef GETVERTEXATTRIB_FUNC

static VALUE gl_VertexAttribIPointerEXT(VALUE obj,VALUE arg1,VALUE arg2,VALUE arg3,VALUE arg4,VALUE arg5)
{
	GLuint index;
	GLuint size;
	GLenum type;
	GLsizei stride;

  DECL_GL_FUNC_PTR(GLvoid,glVertexAttribIPointerEXT,(GLuint,GLint,GLenum,GLsizei,const GLvoid *));
	LOAD_GL_FUNC(glVertexAttribIPointerEXT, "GL_EXT_gpu_shader4");

	index = (GLuint)NUM2UINT(arg1);
	size = (GLuint)NUM2UINT(arg2);
	type = (GLenum)NUM2INT(arg3);
	stride = (GLsizei)NUM2UINT(arg4);
	if (index>_MAX_VERTEX_ATTRIBS)
		rb_raise(rb_eArgError, "Index too large, maximum allowed value '%i'",_MAX_VERTEX_ATTRIBS);

	if (CHECK_BUFFER_BINDING(GL_ARRAY_BUFFER_BINDING)) {
		SET_GLIMPL_VARIABLE(VertexAttrib_ptr[index], arg5);
		fptr_glVertexAttribIPointerEXT(index,size,type,stride,(GLvoid *)NUM2SIZET(arg5));
	} else {
		VALUE data;
		data = pack_array_or_pass_string(type,arg5);
		rb_str_freeze(data);
		SET_GLIMPL_VARIABLE(VertexAttrib_ptr[index], data);
		fptr_glVertexAttribIPointerEXT(index,size,type,stride,(GLvoid *)RSTRING_PTR(data));
	}

	CHECK_GLERROR_FROM("glVertexAttribIPointerEXT");
	return Qnil;
}

GL_FUNC_LOAD_2(Uniform1uiEXT,GLvoid, GLint,GLuint, "GL_EXT_gpu_shader4")
GL_FUNC_LOAD_3(Uniform2uiEXT,GLvoid, GLint,GLuint,GLuint, "GL_EXT_gpu_shader4")
GL_FUNC_LOAD_4(Uniform3uiEXT,GLvoid, GLint,GLuint,GLuint,GLuint, "GL_EXT_gpu_shader4")
GL_FUNC_LOAD_5(Uniform4uiEXT,GLvoid, GLint,GLuint,GLuint,GLuint,GLuint, "GL_EXT_gpu_shader4")

#define GLUNIFORM_VFUNC(_name_,_type_,_conv_,_size_) \
static VALUE \
gl_##_name_(obj,arg1,arg2) \
VALUE obj,arg1,arg2; \
{ \
	GLint location; \
	GLsizei count; \
	_type_ *value; \
  DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLint,GLsizei,const _type_ *)); \
	LOAD_GL_FUNC(gl##_name_, "GL_EXT_gpu_shader4"); \
	Check_Type(arg2,T_ARRAY); \
	count = (GLsizei)RARRAY_LENINT(arg2); \
	if (count<=0 || (count % _size_) != 0) \
		rb_raise(rb_eArgError, "Parameter array size must be multiplication of %i",_size_); \
	location = (GLint)NUM2INT(arg1); \
	value = ALLOC_N(_type_,count); \
	_conv_(arg2,value,count); \
	fptr_gl##_name_(location,count / _size_,value); \
	xfree(value); \
	CHECK_GLERROR_FROM("gl" #_name_); \
	return Qnil; \
}

GLUNIFORM_VFUNC(Uniform1uivEXT,GLuint,ary2cuint,1)
GLUNIFORM_VFUNC(Uniform2uivEXT,GLuint,ary2cuint,2)
GLUNIFORM_VFUNC(Uniform3uivEXT,GLuint,ary2cuint,3)
GLUNIFORM_VFUNC(Uniform4uivEXT,GLuint,ary2cuint,4)
#undef GLUNIFORM_VFUNC

#define GETUNIFORM_FUNC(_name_,_type_) \
static VALUE \
gl_##_name_(obj,arg1,arg2) \
VALUE obj,arg1,arg2; \
{ \
	GLuint program; \
	GLint location; \
	_type_ params[16]; \
	GLint unused = 0; \
	GLenum uniform_type = 0; \
	GLint uniform_size = 0; \
\
  DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLuint,GLint,_type_ *)); \
  DECL_GL_FUNC_PTR(GLvoid,glGetActiveUniformARB,(GLuint,GLuint,GLsizei,GLsizei*,GLint*,GLenum*,GLchar*)); \
	LOAD_GL_FUNC(gl##_name_, "GL_EXT_gpu_shader4"); \
	LOAD_GL_FUNC(glGetActiveUniformARB, "GL_ARB_shader_objects"); \
	program = (GLuint)NUM2UINT(arg1); \
	location = (GLint)NUM2INT(arg2); \
\
	fptr_glGetActiveUniformARB(program,location,0,NULL,&unused,&uniform_type,NULL); \
	CHECK_GLERROR_FROM("glGetActiveUniformARB"); \
	if (uniform_type==0) \
		rb_raise(rb_eTypeError, "Can't determine the uniform's type"); \
\
	uniform_size = get_uniform_size(uniform_type); \
\
	memset(params,0,16*sizeof(_type_)); \
	fptr_gl##_name_(program,location,params); \
	RET_ARRAY_OR_SINGLE("gl" #_name_, uniform_size, RETCONV_##_type_, params); \
}

GETUNIFORM_FUNC(GetUniformuivEXT,GLuint)
#undef GETUNIFORM_FUNC

static VALUE gl_BindFragDataLocationEXT(VALUE obj,VALUE arg1,VALUE arg2,VALUE arg3)
{
  DECL_GL_FUNC_PTR(GLvoid,glBindFragDataLocationEXT,(GLuint,GLuint,const GLchar *));
	LOAD_GL_FUNC(glBindFragDataLocationEXT, "GL_EXT_gpu_shader4");
	Check_Type(arg3,T_STRING);
	fptr_glBindFragDataLocationEXT(NUM2UINT(arg1),NUM2UINT(arg2),RSTRING_PTR(arg3));
	CHECK_GLERROR_FROM("glBindFragDataLocationEXT");
	return Qnil;
}

static VALUE gl_GetFragDataLocationEXT(VALUE obj,VALUE arg1,VALUE arg2)
{
	GLint ret;
  DECL_GL_FUNC_PTR(GLint,glGetFragDataLocationEXT,(GLuint,const GLchar *));
	LOAD_GL_FUNC(glGetFragDataLocationEXT, "GL_EXT_gpu_shader4");
	Check_Type(arg2,T_STRING);
	ret = fptr_glGetFragDataLocationEXT(NUM2UINT(arg1),RSTRING_PTR(arg2));
	CHECK_GLERROR_FROM("glGetFragDataLocationEXT");
	return INT2NUM(ret);
}

/* #327 - GL_EXT_draw_instanced */
GL_FUNC_LOAD_4(DrawArraysInstancedEXT,GLvoid, GLenum,GLint,GLsizei,GLsizei, "GL_EXT_draw_instanced")

static VALUE gl_DrawElementsInstancedEXT(obj,arg1,arg2,arg3,arg4,arg5)
VALUE obj,arg1,arg2,arg3,arg4,arg5;
{
	GLenum mode;
	GLsizei count;
	GLenum type;
	GLsizei primcount;
  DECL_GL_FUNC_PTR(GLvoid,glDrawElementsInstancedEXT,(GLenum,GLsizei,GLenum,const GLvoid *,GLsizei));
	LOAD_GL_FUNC(glDrawElementsInstancedEXT, "GL_EXT_draw_instanced");
	mode = (GLenum)NUM2INT(arg1);
	count = (GLsizei)NUM2UINT(arg2);
	type = (GLenum)NUM2INT(arg3);
	primcount = (GLsizei)NUM2INT(arg5);
	if (CHECK_BUFFER_BINDING(GL_ELEMENT_ARRAY_BUFFER_BINDING)) {
		fptr_glDrawElementsInstancedEXT(mode, count, type, (const GLvoid*)NUM2SIZET(arg4), primcount);
	} else {
		VALUE data;
		data = pack_array_or_pass_string(type,arg4);
		fptr_glDrawElementsInstancedEXT(mode, count, type, (const GLvoid*)RSTRING_PTR(data), primcount);
	}
	CHECK_GLERROR_FROM("glDrawArraysInstancedEXT");
	return Qnil;
}

/* #330 - GL_EXT_texture_buffer_object */
GL_FUNC_LOAD_3(TexBufferEXT,GLvoid, GLenum,GLenum,GLenum, "GL_EXT_texture_buffer_object")

/* #343 - GL_EXT_texture_integer */
GL_FUNC_LOAD_4(ClearColorIiEXT,GLvoid, GLint,GLint,GLint,GLint, "GL_EXT_texture_integer")
GL_FUNC_LOAD_4(ClearColorIuiEXT,GLvoid, GLuint,GLuint,GLuint,GLuint, "GL_EXT_texture_integer")

#define TEXPARAMETER_VFUNC(_name_,_type_,_conv_) \
static VALUE \
gl_##_name_(obj,arg1,arg2,arg3) \
VALUE obj,arg1,arg2,arg3; \
{ \
	GLenum target; \
	GLenum pname; \
	_type_ params[4] = {0,0,0,0}; \
  DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLenum,GLenum,_type_ *)); \
	LOAD_GL_FUNC(gl##_name_, "GL_EXT_texture_integer"); \
	target = (GLenum)NUM2UINT(arg1); \
	pname = (GLenum)NUM2UINT(arg2); \
	Check_Type(arg3,T_ARRAY); \
	_conv_(arg3,params,4); \
	fptr_gl##_name_(target,pname,params); \
	CHECK_GLERROR_FROM("gl" #_name_); \
	return Qnil; \
}

TEXPARAMETER_VFUNC(TexParameterIivEXT,GLint,ary2cint)
TEXPARAMETER_VFUNC(TexParameterIuivEXT,GLuint,ary2cuint)
#undef TEXPARAMETER_VFUNC

#define GETTEXPARAMETER_VFUNC(_name_,_type_,_conv_) \
static VALUE \
gl_##_name_(obj,arg1,arg2) \
VALUE obj,arg1,arg2; \
{ \
	GLenum target; \
	GLenum pname; \
	_type_ params[4] = {0,0,0,0}; \
	int size; \
  DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLenum,GLenum,_type_ *)); \
	LOAD_GL_FUNC(gl##_name_, "GL_EXT_texture_integer"); \
	target = (GLenum)NUM2INT(arg1); \
	pname = (GLenum)NUM2INT(arg2); \
	switch(pname) { \
		case GL_TEXTURE_BORDER_COLOR: \
		case GL_TEXTURE_BORDER_VALUES_NV: \
		case GL_POST_TEXTURE_FILTER_BIAS_SGIX: \
		case GL_POST_TEXTURE_FILTER_SCALE_SGIX: \
			size = 4; \
			break; \
		default: \
			size = 1; \
			break; \
	} \
	fptr_gl##_name_(target,pname,params); \
	RET_ARRAY_OR_SINGLE_BOOL("gl" #_name_, size, _conv_, pname, params); \
}

GETTEXPARAMETER_VFUNC(GetTexParameterIivEXT,GLint,cond_GLBOOL2RUBY)
GETTEXPARAMETER_VFUNC(GetTexParameterIuivEXT,GLuint,cond_GLBOOL2RUBY_U)
#undef GETTEXPARAMETER_VFUNC

void gl_init_functions_ext_ext(VALUE klass)
{
/* #2 - GL_EXT_blend_color */
	rb_define_method(klass, "glBlendColorEXT", gl_BlendColorEXT, 4);

/* #3 - GL_EXT_polygon_offset */
	rb_define_method(klass, "glPolygonOffsetEXT", gl_PolygonOffsetEXT, 2);

/* #20 - GL_EXT_texture_object */
	rb_define_method(klass, "glGenTexturesEXT", gl_GenTexturesEXT, 1);
	rb_define_method(klass, "glDeleteTexturesEXT", gl_DeleteTexturesEXT, 1);
	rb_define_method(klass, "glBindTextureEXT", gl_BindTextureEXT, 2);
	rb_define_method(klass, "glPrioritizeTexturesEXT", gl_PrioritizeTexturesEXT, 2);
	rb_define_method(klass, "glAreTexturesResidentEXT", gl_AreTexturesResidentEXT, 1);
	rb_define_method(klass, "glIsTextureEXT", gl_IsTextureEXT, 1);

/* #37 - GL_EXT_blend_minmax */
	rb_define_method(klass, "glBlendEquationEXT", gl_BlendEquationEXT, 1);

/* #54 - GL_EXT_point_parameters */
	rb_define_method(klass, "glPointParameterfEXT", gl_PointParameterfEXT, 2);
	rb_define_method(klass, "glPointParameterfvEXT", gl_PointParameterfvEXT, 2);

/* #97 - GL_EXT_compiled_vertex_array */
	rb_define_method(klass, "glLockArraysEXT", gl_LockArraysEXT, 2);
	rb_define_method(klass, "glUnlockArraysEXT", gl_UnlockArraysEXT, 0);

/* #112 - GL_EXT_draw_range_elements */
	rb_define_method(klass, "glDrawRangeElementsEXT", gl_DrawRangeElementsEXT, 6);

/* #145 - GL_EXT_secondary_color */
	rb_define_method(klass, "glSecondaryColor3bEXT", gl_SecondaryColor3bEXT, 3);
	rb_define_method(klass, "glSecondaryColor3dEXT", gl_SecondaryColor3dEXT, 3);
	rb_define_method(klass, "glSecondaryColor3fEXT", gl_SecondaryColor3fEXT, 3);
	rb_define_method(klass, "glSecondaryColor3iEXT", gl_SecondaryColor3iEXT, 3);
	rb_define_method(klass, "glSecondaryColor3sEXT", gl_SecondaryColor3sEXT, 3);
	rb_define_method(klass, "glSecondaryColor3ubEXT", gl_SecondaryColor3ubEXT, 3);
	rb_define_method(klass, "glSecondaryColor3uiEXT", gl_SecondaryColor3uiEXT, 3);
	rb_define_method(klass, "glSecondaryColor3usEXT", gl_SecondaryColor3usEXT, 3);
	rb_define_method(klass, "glSecondaryColor3bvEXT", gl_SecondaryColor3bvEXT, 1);
	rb_define_method(klass, "glSecondaryColor3dvEXT", gl_SecondaryColor3dvEXT, 1);
	rb_define_method(klass, "glSecondaryColor3fvEXT", gl_SecondaryColor3fvEXT, 1);
	rb_define_method(klass, "glSecondaryColor3ivEXT", gl_SecondaryColor3ivEXT, 1);
	rb_define_method(klass, "glSecondaryColor3svEXT", gl_SecondaryColor3svEXT, 1);
	rb_define_method(klass, "glSecondaryColor3ubvEXT", gl_SecondaryColor3ubvEXT, 1);
	rb_define_method(klass, "glSecondaryColor3uivEXT", gl_SecondaryColor3uivEXT, 1);
	rb_define_method(klass, "glSecondaryColor3usvEXT", gl_SecondaryColor3usvEXT, 1);
	rb_define_method(klass, "glSecondaryColorPointerEXT", gl_SecondaryColorPointerEXT, 4);

/* #148 - GL_EXT_multi_draw_arrays */
	rb_define_method(klass, "glMultiDrawArraysEXT", gl_MultiDrawArraysEXT, 3);
	rb_define_method(klass, "glMultiDrawElementsEXT", gl_MultiDrawElementsEXT, -1);

/* #149 - GL_EXT_fog_coord */
	rb_define_method(klass, "glFogCoordfEXT", gl_FogCoordfEXT, 1);
	rb_define_method(klass, "glFogCoorddEXT", gl_FogCoorddEXT, 1);
	rb_define_method(klass, "glFogCoordfvEXT", gl_FogCoordfvEXT, 1);
	rb_define_method(klass, "glFogCoorddvEXT", gl_FogCoorddvEXT, 1);
	rb_define_method(klass, "glFogCoordPointerEXT", gl_FogCoordPointerEXT, 3);

/* #173 - GL_EXT_blend_func_separate */
	rb_define_method(klass, "glBlendFuncSeparateEXT", gl_BlendFuncSeparateEXT, 4);

/* #268 - GL_EXT_stencil_two_side */
	rb_define_method(klass, "glActiveStencilFaceEXT", gl_ActiveStencilFaceEXT, 1);

/* #297 - GL_EXT_depth_bounds_test */
	rb_define_method(klass, "glDepthBoundsEXT", gl_DepthBoundsEXT, 2);

/* #299 - GL_EXT_blend_equation_separate */
	rb_define_method(klass, "glBlendEquationSeparateEXT", gl_BlendEquationSeparateEXT, 2);

/* #310 - GL_EXT_framebuffer_object */
	rb_define_method(klass, "glIsRenderbufferEXT", gl_IsRenderbufferEXT, 1);
	rb_define_method(klass, "glBindRenderbufferEXT", gl_BindRenderbufferEXT, 2);
	rb_define_method(klass, "glDeleteRenderbuffersEXT", gl_DeleteRenderbuffersEXT, 1);
	rb_define_method(klass, "glGenRenderbuffersEXT", gl_GenRenderbuffersEXT, 1);
	rb_define_method(klass, "glRenderbufferStorageEXT", gl_RenderbufferStorageEXT, 4);
	rb_define_method(klass, "glGetRenderbufferParameterivEXT", gl_GetRenderbufferParameterivEXT, 2);
	rb_define_method(klass, "glIsFramebufferEXT", gl_IsFramebufferEXT, 1);
	rb_define_method(klass, "glBindFramebufferEXT", gl_BindFramebufferEXT, 2);
	rb_define_method(klass, "glDeleteFramebuffersEXT", gl_DeleteFramebuffersEXT, 1);
	rb_define_method(klass, "glGenFramebuffersEXT", gl_GenFramebuffersEXT, 1);
	rb_define_method(klass, "glCheckFramebufferStatusEXT", gl_CheckFramebufferStatusEXT, 1);
	rb_define_method(klass, "glFramebufferTexture1DEXT", gl_FramebufferTexture1DEXT, 5);
	rb_define_method(klass, "glFramebufferTexture2DEXT", gl_FramebufferTexture2DEXT, 5);
	rb_define_method(klass, "glFramebufferTexture3DEXT", gl_FramebufferTexture3DEXT, 6);
	rb_define_method(klass, "glFramebufferRenderbufferEXT", gl_FramebufferRenderbufferEXT, 4);
	rb_define_method(klass, "glGetFramebufferAttachmentParameterivEXT", gl_GetFramebufferAttachmentParameterivEXT, 3);
	rb_define_method(klass, "glGenerateMipmapEXT", gl_GenerateMipmapEXT, 1);

/* #314 - GL_EXT_stencil_clear_tag */
	rb_define_method(klass, "glStencilClearTagEXT", gl_StencilClearTagEXT, 2);

/* #316 - GL_EXT_framebuffer_blit */
	rb_define_method(klass, "glBlitFramebufferEXT", gl_BlitFramebufferEXT, 10);

/* #317 - GL_EXT_framebuffer_multisample */
	rb_define_method(klass, "glRenderbufferStorageMultisampleEXT", gl_RenderbufferStorageMultisampleEXT, 5);

/* #319 - GL_EXT_timer_query */
	rb_define_method(klass, "glGetQueryObjecti64vEXT", gl_GetQueryObjecti64vEXT, 2);
	rb_define_method(klass, "glGetQueryObjectui64vEXT", gl_GetQueryObjectui64vEXT, 2);

/* #320 - GL_EXT_gpu_program_parameters */
	rb_define_method(klass, "glProgramEnvParameters4fvEXT", gl_ProgramEnvParameters4fvEXT, 3);
	rb_define_method(klass, "glProgramLocalParameters4fvEXT", gl_ProgramLocalParameters4fvEXT, 3);

/* #324 - GL_EXT_geometry_shader4 */
	rb_define_method(klass, "glProgramParameteriEXT", gl_ProgramParameteriEXT, 3);

/* #326 - GL_EXT_gpu_shader4 */
	rb_define_method(klass, "glVertexAttribI1iEXT", gl_VertexAttribI1iEXT, 2);
	rb_define_method(klass, "glVertexAttribI2iEXT", gl_VertexAttribI2iEXT, 3);
	rb_define_method(klass, "glVertexAttribI3iEXT", gl_VertexAttribI3iEXT, 4);
	rb_define_method(klass, "glVertexAttribI4iEXT", gl_VertexAttribI4iEXT, 5);
	rb_define_method(klass, "glVertexAttribI1uiEXT", gl_VertexAttribI1uiEXT, 2);
	rb_define_method(klass, "glVertexAttribI2uiEXT", gl_VertexAttribI2uiEXT, 3);
	rb_define_method(klass, "glVertexAttribI3uiEXT", gl_VertexAttribI3uiEXT, 4);
	rb_define_method(klass, "glVertexAttribI4uiEXT", gl_VertexAttribI4uiEXT, 5);
	rb_define_method(klass, "glVertexAttribI1ivEXT", gl_VertexAttribI1ivEXT, 2);
	rb_define_method(klass, "glVertexAttribI2ivEXT", gl_VertexAttribI2ivEXT, 2);
	rb_define_method(klass, "glVertexAttribI3ivEXT", gl_VertexAttribI3ivEXT, 2);
	rb_define_method(klass, "glVertexAttribI4ivEXT", gl_VertexAttribI4ivEXT, 2);
	rb_define_method(klass, "glVertexAttribI1uivEXT", gl_VertexAttribI1uivEXT, 2);
	rb_define_method(klass, "glVertexAttribI2uivEXT", gl_VertexAttribI2uivEXT, 2);
	rb_define_method(klass, "glVertexAttribI3uivEXT", gl_VertexAttribI3uivEXT, 2);
	rb_define_method(klass, "glVertexAttribI4uivEXT", gl_VertexAttribI4uivEXT, 2);
	rb_define_method(klass, "glVertexAttribI4bvEXT", gl_VertexAttribI4bvEXT, 2);
	rb_define_method(klass, "glVertexAttribI4svEXT", gl_VertexAttribI4svEXT, 2);
	rb_define_method(klass, "glVertexAttribI4ubvEXT", gl_VertexAttribI4ubvEXT, 2);
	rb_define_method(klass, "glVertexAttribI4usvEXT", gl_VertexAttribI4usvEXT, 2);
	rb_define_method(klass, "glVertexAttribIPointerEXT", gl_VertexAttribIPointerEXT, 5);
	rb_define_method(klass, "glGetVertexAttribIivEXT", gl_GetVertexAttribIivEXT, 2);
	rb_define_method(klass, "glGetVertexAttribIuivEXT", gl_GetVertexAttribIuivEXT, 2);
	rb_define_method(klass, "glUniform1uiEXT", gl_Uniform1uiEXT, 2);
	rb_define_method(klass, "glUniform2uiEXT", gl_Uniform2uiEXT, 3);
	rb_define_method(klass, "glUniform3uiEXT", gl_Uniform3uiEXT, 4);
	rb_define_method(klass, "glUniform4uiEXT", gl_Uniform4uiEXT, 5);
	rb_define_method(klass, "glUniform1uivEXT", gl_Uniform1uivEXT, 2);
	rb_define_method(klass, "glUniform2uivEXT", gl_Uniform2uivEXT, 2);
	rb_define_method(klass, "glUniform3uivEXT", gl_Uniform3uivEXT, 2);
	rb_define_method(klass, "glUniform4uivEXT", gl_Uniform4uivEXT, 2);
	rb_define_method(klass, "glGetUniformuivEXT", gl_GetUniformuivEXT, 2);
	rb_define_method(klass, "glBindFragDataLocationEXT", gl_BindFragDataLocationEXT, 3);
	rb_define_method(klass, "glGetFragDataLocationEXT", gl_GetFragDataLocationEXT, 2);

/* #327 - GL_EXT_draw_instanced */
	rb_define_method(klass, "glDrawArraysInstancedEXT", gl_DrawArraysInstancedEXT, 4);
	rb_define_method(klass, "glDrawElementsInstancedEXT", gl_DrawElementsInstancedEXT, 5);

/* #330 - GL_EXT_texture_buffer_object */
	rb_define_method(klass, "glTexBufferEXT", gl_TexBufferEXT, 3);

/* #343 - GL_EXT_texture_integer */
	rb_define_method(klass, "glClearColorIiEXT", gl_ClearColorIiEXT, 4);
	rb_define_method(klass, "glClearColorIuiEXT", gl_ClearColorIuiEXT, 4);
	rb_define_method(klass, "glTexParameterIivEXT", gl_TexParameterIivEXT, 3);
	rb_define_method(klass, "glTexParameterIuivEXT", gl_TexParameterIuivEXT, 3);
	rb_define_method(klass, "glGetTexParameterIivEXT", gl_GetTexParameterIivEXT, 2);
	rb_define_method(klass, "glGetTexParameterIuivEXT", gl_GetTexParameterIuivEXT, 2);
}
