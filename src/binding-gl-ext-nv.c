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

/* OpenGL NVIDIA extensions */

/* #222 GL_NV_fence */
GL_FUNC_GENOBJECTS_LOAD(GenFencesNV,"GL_NV_fence")
GL_FUNC_DELETEOBJECTS_LOAD(DeleteFencesNV,"GL_NV_fence")
GL_FUNC_LOAD_2(SetFenceNV,GLvoid, GLuint,GLenum, "GL_NV_fence")
GL_FUNC_LOAD_1(TestFenceNV,GLboolean, GLuint,"GL_NV_fence")
GL_FUNC_LOAD_1(IsFenceNV,GLboolean, GLuint,"GL_NV_fence")
GL_FUNC_LOAD_1(FinishFenceNV,GLvoid, GLuint,"GL_NV_fence")

static VALUE gl_GetFenceivNV(VALUE obj,VALUE arg1,VALUE arg2)
{
	GLint ret = 0;
    DECL_GL_FUNC_PTR(GLvoid,glGetFenceivNV,(GLuint,GLenum,GLint *));
	LOAD_GL_FUNC(glGetFenceivNV, "GL_NV_fence");
	fptr_glGetFenceivNV(NUM2INT(arg1),NUM2INT(arg2),&ret);
	CHECK_GLERROR_FROM("glGetFenceivNV");
	return cond_GLBOOL2RUBY(NUM2INT(arg2),ret);
}

/* #233 GL_NV_vertex_program */
static VALUE gl_LoadProgramNV(VALUE obj,VALUE arg1,VALUE arg2,VALUE arg3)
{
    DECL_GL_FUNC_PTR(GLvoid,glLoadProgramNV,(GLenum,GLuint,GLsizei,const GLubyte *));
	LOAD_GL_FUNC(glLoadProgramNV, "GL_NV_vertex_program");
	Check_Type(arg3,T_STRING);
	fptr_glLoadProgramNV((GLenum)NUM2INT(arg1),(GLuint)NUM2UINT(arg2),(GLsizei)RSTRING_LENINT(arg3),(GLubyte *)RSTRING_PTR(arg3));
	CHECK_GLERROR_FROM("glLoadProgramNV");
	return Qnil;
}

static VALUE gl_GetProgramivNV(VALUE obj,VALUE arg1,VALUE arg2)
{
	GLint ret = 0;
    DECL_GL_FUNC_PTR(GLvoid,glGetProgramivNV,(GLuint,GLenum,GLint *));
	LOAD_GL_FUNC(glGetProgramivNV, "GL_NV_vertex_program");
	fptr_glGetProgramivNV(NUM2INT(arg1),NUM2INT(arg2),&ret);
	CHECK_GLERROR_FROM("glGetProgramivNV");
	return cond_GLBOOL2RUBY(NUM2INT(arg2),ret);
}

static VALUE gl_GetProgramStringNV(VALUE obj,VALUE arg1,VALUE arg2)
{
	GLsizei len = 0;
	char *buffer;
	VALUE ret_buffer;

    DECL_GL_FUNC_PTR(GLvoid,glGetProgramStringNV,(GLuint,GLenum,void *string));
    DECL_GL_FUNC_PTR(GLvoid,glGetProgramivNV,(GLuint,GLenum,GLint *));
	LOAD_GL_FUNC(glGetProgramStringNV, "GL_NV_vertex_program");
	LOAD_GL_FUNC(glGetProgramivNV, "GL_NV_vertex_program");

	fptr_glGetProgramivNV(NUM2INT(arg1),GL_PROGRAM_LENGTH_NV,&len);
	CHECK_GLERROR_FROM("glGetProgramivNV");
	if (len<=0)
		return rb_str_new2("");

	buffer = ALLOC_N(GLchar,len+1);
	memset(buffer,0,sizeof(GLchar) * (len+1));
		fptr_glGetProgramStringNV(NUM2INT(arg1),NUM2INT(arg2),buffer);
	ret_buffer = rb_str_new2(buffer);
	xfree(buffer);

	CHECK_GLERROR_FROM("glGetProgramStringNV");
	return ret_buffer;
}

GL_FUNC_LOAD_2(BindProgramNV,GLvoid, GLenum,GLuint,"GL_NV_vertex_program")
GL_FUNC_LOAD_1(IsProgramNV,GLboolean, GLuint,"GL_NV_vertex_program")
GL_FUNC_GENOBJECTS_LOAD(GenProgramsNV,"GL_NV_vertex_program")
GL_FUNC_DELETEOBJECTS_LOAD(DeleteProgramsNV,"GL_NV_vertex_program")

static VALUE gl_ExecuteProgramNV(VALUE obj,VALUE arg1,VALUE arg2,VALUE arg3)
{
	GLfloat params[4] = {0,0,0,0};
    DECL_GL_FUNC_PTR(GLvoid,glExecuteProgramNV,(GLenum,GLuint,const GLfloat *));
	LOAD_GL_FUNC(glExecuteProgramNV, "GL_NV_vertex_program");
	ary2cflt(arg3,params,4);
	fptr_glExecuteProgramNV(NUM2UINT(arg1),NUM2UINT(arg2),params);
	CHECK_GLERROR_FROM("glExecuteProgramNV");
	return Qnil;
}

static VALUE gl_VertexAttribPointerNV(VALUE obj,VALUE arg1,VALUE arg2,VALUE arg3,VALUE arg4,VALUE arg5)
{
	GLuint index;
	GLuint size;
	GLenum type;
	GLsizei stride;

    DECL_GL_FUNC_PTR(GLvoid,glVertexAttribPointerNV,(GLuint,GLint,GLenum,GLsizei,const GLvoid *));
	LOAD_GL_FUNC(glVertexAttribPointerNV, "GL_NV_vertex_program");

	index = (GLuint)NUM2UINT(arg1);
	size = (GLuint)NUM2UINT(arg2);
	type = (GLenum)NUM2INT(arg3);
	stride = (GLsizei)NUM2UINT(arg4);
	if (index>_MAX_VERTEX_ATTRIBS)
		rb_raise(rb_eArgError, "Index too large, maximum allowed value '%i'",_MAX_VERTEX_ATTRIBS);

	if (CHECK_BUFFER_BINDING(GL_ARRAY_BUFFER_BINDING)) {
		GET_GLIMPL_VARIABLE(VertexAttrib_ptr)[index] = arg5;
		fptr_glVertexAttribPointerNV(index,size,type,stride,(GLvoid *)NUM2SIZET(arg5));
	} else {
		VALUE data;
		data = pack_array_or_pass_string(type,arg5);
		rb_str_freeze(data);
		GET_GLIMPL_VARIABLE(VertexAttrib_ptr)[index] = data;
		fptr_glVertexAttribPointerNV(index,size,type,stride,(GLvoid *)RSTRING_PTR(data));
	}

	CHECK_GLERROR_FROM("glVertexAttribPointerNV");
	return Qnil;
}


static VALUE
gl_GetVertexAttribPointervNV(obj,arg1)
VALUE obj,arg1;
{
	GLuint index;
    DECL_GL_FUNC_PTR(GLvoid,glGetVertexAttribPointervNV,(GLuint,GLenum,GLvoid **));
	LOAD_GL_FUNC(glGetVertexAttribPointervNV, "GL_NV_vertex_program");
	index =(GLuint) NUM2INT(arg1);
	if (index>_MAX_VERTEX_ATTRIBS)
		rb_raise(rb_eArgError, "Index too large, maximum allowed value '%i'",_MAX_VERTEX_ATTRIBS);

	return GET_GLIMPL_VARIABLE(VertexAttrib_ptr)[index];
}

GL_FUNC_LOAD_6(ProgramParameter4dNV,GLvoid, GLenum,GLuint,GLdouble,GLdouble,GLdouble,GLdouble, "GL_NV_vertex_program")
GL_FUNC_LOAD_6(ProgramParameter4fNV,GLvoid, GLenum,GLuint,GLfloat,GLfloat,GLfloat,GLfloat, "GL_NV_vertex_program")

#define PROGRAMPARAM_FUNC_V(_name_,_type_,_conv_,_extension_) \
static VALUE \
gl_##_name_(obj,arg1,arg2,arg3) \
VALUE obj,arg1,arg2,arg3; \
{ \
	_type_ cary[4]; \
    DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLenum,GLuint,const _type_ *)); \
	LOAD_GL_FUNC(gl##_name_, _extension_); \
	_conv_(arg3,cary,4); \
	fptr_gl##_name_(NUM2UINT(arg1),NUM2UINT(arg2),cary); \
	CHECK_GLERROR_FROM("gl" #_name_); \
	return Qnil; \
}

PROGRAMPARAM_FUNC_V(ProgramParameter4dvNV,GLdouble,ary2cdbl,"GL_NV_vertex_program")
PROGRAMPARAM_FUNC_V(ProgramParameter4fvNV,GLfloat,ary2cflt,"GL_NV_vertex_program")

#define GETPROGRAMPARAM_FUNC(_name_,_type_,_extension_) \
static VALUE \
gl_##_name_(obj,arg1,arg2,arg3) \
VALUE obj,arg1,arg2,arg3; \
{ \
	_type_ cary[4] = {(_type_)0.0, (_type_)0.0, (_type_)0.0, (_type_)0.0}; \
    DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLenum,GLuint,GLenum,_type_ *)); \
	LOAD_GL_FUNC(gl##_name_, _extension_); \
	fptr_gl##_name_(NUM2UINT(arg1),NUM2UINT(arg2),NUM2UINT(arg3),cary); \
	RET_ARRAY_OR_SINGLE("gl" #_name_, 4, RETCONV_##_type_, cary); \
}

GETPROGRAMPARAM_FUNC(GetProgramParameterdvNV,GLdouble,"GL_NV_vertex_program")
GETPROGRAMPARAM_FUNC(GetProgramParameterfvNV,GLfloat,"GL_NV_vertex_program")
#undef GETPROGRAMPARAM_FUNC

#define PROGRAMPARAM_MULTI_FUNC_V(_name_,_type_,_conv_,_extension_) \
static VALUE \
gl_##_name_(obj,arg1,arg2,arg3) \
VALUE obj,arg1,arg2,arg3; \
{ \
	_type_ *cary; \
	GLuint len; \
    DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLenum,GLuint,GLuint,const _type_ *)); \
	LOAD_GL_FUNC(gl##_name_, _extension_); \
	len = (GLuint)RARRAY_LENINT(rb_Array(arg3)); \
	if (len<=0 || (len % 4) != 0) \
		rb_raise(rb_eArgError, "Parameter array size must be multiplication of 4"); \
	cary = ALLOC_N(_type_,len); \
	_conv_(arg3,cary,len); \
	fptr_gl##_name_((GLenum)NUM2UINT(arg1),(GLuint)NUM2UINT(arg2),len / 4, cary); \
	xfree(cary); \
	CHECK_GLERROR_FROM("gl" #_name_); \
	return Qnil; \
}

PROGRAMPARAM_MULTI_FUNC_V(ProgramParameters4dvNV,GLdouble,ary2cdbl,"GL_NV_vertex_program")
PROGRAMPARAM_MULTI_FUNC_V(ProgramParameters4fvNV,GLfloat,ary2cflt,"GL_NV_vertex_program")

GL_FUNC_LOAD_2(VertexAttrib1dNV,GLvoid, GLuint,GLdouble, "GL_NV_vertex_program")
GL_FUNC_LOAD_2(VertexAttrib1fNV,GLvoid, GLuint,GLfloat, "GL_NV_vertex_program")
GL_FUNC_LOAD_2(VertexAttrib1sNV,GLvoid, GLuint,GLshort, "GL_NV_vertex_program")
GL_FUNC_LOAD_3(VertexAttrib2dNV,GLvoid, GLuint,GLdouble,GLdouble, "GL_NV_vertex_program")
GL_FUNC_LOAD_3(VertexAttrib2fNV,GLvoid, GLuint,GLfloat,GLfloat, "GL_NV_vertex_program")
GL_FUNC_LOAD_3(VertexAttrib2sNV,GLvoid, GLuint,GLshort,GLshort, "GL_NV_vertex_program")
GL_FUNC_LOAD_4(VertexAttrib3dNV,GLvoid, GLuint,GLdouble,GLdouble,GLdouble, "GL_NV_vertex_program")
GL_FUNC_LOAD_4(VertexAttrib3fNV,GLvoid, GLuint,GLfloat,GLfloat,GLfloat, "GL_NV_vertex_program")
GL_FUNC_LOAD_4(VertexAttrib3sNV,GLvoid, GLuint,GLshort,GLshort,GLshort, "GL_NV_vertex_program")
GL_FUNC_LOAD_5(VertexAttrib4dNV,GLvoid, GLuint,GLdouble,GLdouble,GLdouble,GLdouble, "GL_NV_vertex_program")
GL_FUNC_LOAD_5(VertexAttrib4fNV,GLvoid, GLuint,GLfloat,GLfloat,GLfloat,GLfloat, "GL_NV_vertex_program")
GL_FUNC_LOAD_5(VertexAttrib4sNV,GLvoid, GLuint,GLshort,GLshort,GLshort,GLshort, "GL_NV_vertex_program")
GL_FUNC_LOAD_5(VertexAttrib4ubNV,GLvoid, GLuint,GLubyte,GLubyte,GLubyte,GLubyte, "GL_NV_vertex_program")

#define VERTEXATTRIB_FUNC_V(_name_,_type_,_conv_,_size_,_extension_) \
static VALUE \
gl_##_name_(obj,arg1,arg2) \
VALUE obj,arg1,arg2; \
{ \
	GLuint index; \
	_type_ v[_size_]; \
    DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLuint,_type_ *)); \
	LOAD_GL_FUNC(gl##_name_, _extension_); \
	index = (GLuint)NUM2UINT(arg1); \
	_conv_(arg2,v,_size_); \
	fptr_gl##_name_(index,v); \
	CHECK_GLERROR_FROM("gl" #_name_); \
	return Qnil; \
}

VERTEXATTRIB_FUNC_V(VertexAttrib4ubvNV,GLubyte,ary2cubyte,4,"GL_NV_vertex_program")
VERTEXATTRIB_FUNC_V(VertexAttrib4dvNV,GLdouble,ary2cdbl,4,"GL_NV_vertex_program")
VERTEXATTRIB_FUNC_V(VertexAttrib4fvNV,GLfloat,ary2cflt,4,"GL_NV_vertex_program")
VERTEXATTRIB_FUNC_V(VertexAttrib4svNV,GLshort,ary2cshort,4,"GL_NV_vertex_program")
VERTEXATTRIB_FUNC_V(VertexAttrib3dvNV,GLdouble,ary2cdbl,3,"GL_NV_vertex_program")
VERTEXATTRIB_FUNC_V(VertexAttrib3fvNV,GLfloat,ary2cflt,3,"GL_NV_vertex_program")
VERTEXATTRIB_FUNC_V(VertexAttrib3svNV,GLshort,ary2cshort,3,"GL_NV_vertex_program")
VERTEXATTRIB_FUNC_V(VertexAttrib2dvNV,GLdouble,ary2cdbl,2,"GL_NV_vertex_program")
VERTEXATTRIB_FUNC_V(VertexAttrib2fvNV,GLfloat,ary2cflt,2,"GL_NV_vertex_program")
VERTEXATTRIB_FUNC_V(VertexAttrib2svNV,GLshort,ary2cshort,2,"GL_NV_vertex_program")
VERTEXATTRIB_FUNC_V(VertexAttrib1dvNV,GLdouble,ary2cdbl,1,"GL_NV_vertex_program")
VERTEXATTRIB_FUNC_V(VertexAttrib1fvNV,GLfloat,ary2cflt,1,"GL_NV_vertex_program")
VERTEXATTRIB_FUNC_V(VertexAttrib1svNV,GLshort,ary2cshort,1,"GL_NV_vertex_program")
#undef VERTEXATTRIB_FUNC_V

#define VERTEXATTRIB_MULTI_FUNC_V(_name_,_type_,_conv_,_size_,_extension_) \
static VALUE \
gl_##_name_(obj,arg1,arg2) \
VALUE obj,arg1,arg2; \
{ \
	GLuint index; \
	_type_ *cary; \
	GLsizei len; \
    DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLuint,GLsizei,_type_ *)); \
	LOAD_GL_FUNC(gl##_name_, _extension_); \
	len = (GLsizei)RARRAY_LENINT(rb_Array(arg2)); \
	if (len<=0 || (len % _size_) != 0) \
		rb_raise(rb_eArgError, "Parameter array size must be multiplication of %i",_size_); \
	cary = ALLOC_N(_type_,len); \
	index = (GLuint)NUM2UINT(arg1); \
	_conv_(arg2,cary,len); \
	fptr_gl##_name_(index,len / _size_,cary); \
	xfree(cary); \
	CHECK_GLERROR_FROM("gl" #_name_); \
	return Qnil; \
}

VERTEXATTRIB_MULTI_FUNC_V(VertexAttribs4ubvNV,GLubyte,ary2cubyte,4,"GL_NV_vertex_program")
VERTEXATTRIB_MULTI_FUNC_V(VertexAttribs4dvNV,GLdouble,ary2cdbl,4,"GL_NV_vertex_program")
VERTEXATTRIB_MULTI_FUNC_V(VertexAttribs4fvNV,GLfloat,ary2cflt,4,"GL_NV_vertex_program")
VERTEXATTRIB_MULTI_FUNC_V(VertexAttribs4svNV,GLshort,ary2cshort,4,"GL_NV_vertex_program")
VERTEXATTRIB_MULTI_FUNC_V(VertexAttribs3dvNV,GLdouble,ary2cdbl,3,"GL_NV_vertex_program")
VERTEXATTRIB_MULTI_FUNC_V(VertexAttribs3fvNV,GLfloat,ary2cflt,3,"GL_NV_vertex_program")
VERTEXATTRIB_MULTI_FUNC_V(VertexAttribs3svNV,GLshort,ary2cshort,3,"GL_NV_vertex_program")
VERTEXATTRIB_MULTI_FUNC_V(VertexAttribs2dvNV,GLdouble,ary2cdbl,2,"GL_NV_vertex_program")
VERTEXATTRIB_MULTI_FUNC_V(VertexAttribs2fvNV,GLfloat,ary2cflt,2,"GL_NV_vertex_program")
VERTEXATTRIB_MULTI_FUNC_V(VertexAttribs2svNV,GLshort,ary2cshort,2,"GL_NV_vertex_program")
VERTEXATTRIB_MULTI_FUNC_V(VertexAttribs1dvNV,GLdouble,ary2cdbl,1,"GL_NV_vertex_program")
VERTEXATTRIB_MULTI_FUNC_V(VertexAttribs1fvNV,GLfloat,ary2cflt,1,"GL_NV_vertex_program")
VERTEXATTRIB_MULTI_FUNC_V(VertexAttribs1svNV,GLshort,ary2cshort,1,"GL_NV_vertex_program")
#undef VERTEXATTRIB_MULTI_FUNC_V

#define GETVERTEXATTRIB_FUNC(_name_,_type_,_extension_) \
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
	if (pname==GL_CURRENT_ATTRIB_NV) \
		size = 4; \
	else \
		size = 1; \
	fptr_gl##_name_(index,pname,params); \
	RET_ARRAY_OR_SINGLE("gl" #_name_, size, RETCONV_##_type_, params); \
}

GETVERTEXATTRIB_FUNC(GetVertexAttribdvNV,GLdouble,"GL_NV_vertex_program")
GETVERTEXATTRIB_FUNC(GetVertexAttribfvNV,GLfloat,"GL_NV_vertex_program")
#undef GETVERTEXATTRIB_FUNC

static VALUE
gl_GetVertexAttribivNV(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLuint index;
	GLenum pname;
	GLint params[4] = {0,0,0,0};
	GLint size;
    DECL_GL_FUNC_PTR(GLvoid,glGetVertexAttribivNV,(GLuint,GLenum,GLint *));
	LOAD_GL_FUNC(glGetVertexAttribivNV, "GL_NV_vertex_program");
	index = (GLuint)NUM2UINT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	if (pname==GL_CURRENT_VERTEX_ATTRIB)
		size = 4;
	else
		size = 1;
	fptr_glGetVertexAttribivNV(index,pname,params);
  RET_ARRAY_OR_SINGLE_BOOL("glGetVertexAttribivNV", size, cond_GLBOOL2RUBY,
      pname, params); \
}


GL_FUNC_LOAD_4(TrackMatrixNV,GLvoid, GLenum,GLuint,GLenum,GLenum, "GL_NV_vertex_program")

static VALUE gl_GetTrackMatrixivNV(VALUE obj,VALUE arg1,VALUE arg2,VALUE arg3)
{
	GLint ret = 0;
    DECL_GL_FUNC_PTR(GLvoid,glGetTrackMatrixivNV,(GLenum,GLuint,GLenum,GLint *));
	LOAD_GL_FUNC(glGetTrackMatrixivNV, "GL_NV_vertex_program");
	fptr_glGetTrackMatrixivNV(NUM2UINT(arg1),NUM2UINT(arg2),NUM2UINT(arg3),&ret);
	CHECK_GLERROR_FROM("glGetTrackMatrixivNV");
	return INT2NUM(ret);
}

static VALUE gl_RequestResidentProgramsNV(VALUE obj,VALUE arg1)
{
  DECL_GL_FUNC_PTR(GLvoid,glRequestResidentProgramsNV,(GLsizei, GLuint *));
	LOAD_GL_FUNC(glRequestResidentProgramsNV, "GL_NV_vertex_program");
	if (TYPE(arg1)==T_ARRAY) {
		GLsizei n;
		GLuint *programs;
		n = (GLsizei)RARRAY_LENINT(arg1);
		programs = ALLOC_N(GLuint,n);
		ary2cuint(arg1,programs,n);
		fptr_glRequestResidentProgramsNV( n, programs);
		xfree(programs);
	} else {
		GLuint program;
		program = NUM2INT(arg1);
		fptr_glRequestResidentProgramsNV( 1, &program);
	}
	CHECK_GLERROR_FROM("glRequestResidentProgramsNV");
	return Qnil;
}

static VALUE gl_AreProgramsResidentNV(VALUE obj,VALUE arg1)
{
	GLuint *programs;
	GLboolean *residences;
	GLsizei size;
	GLboolean r;
	VALUE retary;
	VALUE ary;
	int i;
    DECL_GL_FUNC_PTR(GLboolean,glAreProgramsResidentNV,(GLsizei, const GLuint *, GLboolean *));
	LOAD_GL_FUNC(glAreProgramsResidentNV, "GL_NV_vertex_program");
	ary = rb_Array(arg1);
	size = (GLsizei)RARRAY_LENINT(ary);
	programs = ALLOC_N(GLuint,size);
	residences = ALLOC_N(GLboolean,size);
	ary2cuint(ary,programs,size);
	r = fptr_glAreProgramsResidentNV(size,programs,residences);
	retary = rb_ary_new2(size);
	if (r==GL_TRUE) { /* all are resident */
		for(i=0;i<size;i++)
			rb_ary_push(retary, GLBOOL2RUBY(GL_TRUE));
	} else {
		for(i=0;i<size;i++)
			rb_ary_push(retary, GLBOOL2RUBY(residences[i]));
	}
	xfree(programs);
	xfree(residences);
	CHECK_GLERROR_FROM("glAreProgramsResidentNV");
	return retary;
}

/* #261 GL_NV_occlusion_query */
GL_FUNC_GENOBJECTS_LOAD(GenOcclusionQueriesNV,"GL_NV_occlusion_query")
GL_FUNC_DELETEOBJECTS_LOAD(DeleteOcclusionQueriesNV,"GL_NV_occlusion_query")
GL_FUNC_LOAD_1(IsOcclusionQueryNV,GLboolean, GLuint, "GL_NV_occlusion_query")
GL_FUNC_LOAD_1(BeginOcclusionQueryNV,GLvoid, GLuint, "GL_NV_occlusion_query")
GL_FUNC_LOAD_0(EndOcclusionQueryNV,GLvoid, "GL_NV_occlusion_query")

#define GETOCCLUSIONQUERY_FUNC(_name_,_type_,_conv_) \
static VALUE gl_##_name_(VALUE obj,VALUE arg1,VALUE arg2) \
{ \
	_type_ ret = 0; \
    DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLuint,GLenum,_type_ *)); \
	LOAD_GL_FUNC(gl##_name_, "GL_NV_occlusion_query"); \
	fptr_gl##_name_(NUM2INT(arg1),NUM2INT(arg2),&ret); \
	CHECK_GLERROR_FROM("gl" #_name_); \
	return _conv_(NUM2INT(arg2),ret); \
}

GETOCCLUSIONQUERY_FUNC(GetOcclusionQueryivNV,GLint,cond_GLBOOL2RUBY)
GETOCCLUSIONQUERY_FUNC(GetOcclusionQueryuivNV,GLuint,cond_GLBOOL2RUBY_U)
#undef GETOCCLUSIONQUERY_FUNC

/* #262 GL_NV_point_sprite */
GL_FUNC_LOAD_2(PointParameteriNV,GLvoid, GLenum,GLint, "GL_NV_point_sprite")

static VALUE gl_PointParameterivNV(VALUE obj,VALUE arg1,VALUE arg2)
{
	GLint param = 0;
    DECL_GL_FUNC_PTR(GLvoid,glPointParameterivNV,(GLenum,const GLint *));
	LOAD_GL_FUNC(glPointParameterivNV, "GL_NV_point_sprite");
	ary2cint(arg2,&param,1);
	fptr_glPointParameterivNV(NUM2UINT(arg1),&param);
	CHECK_GLERROR_FROM("glPointParameterivNV");
	return Qnil;
}

/* #282 GL_NV_fragment_program */
#define PROGRAMNAMEDPARAM_FUNC(_name_,_type_,_conv_,_extension_) \
static VALUE \
gl_##_name_(obj,arg1,arg2,arg3,arg4,arg5,arg6) \
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6; \
{ \
    DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLuint,GLsizei,const GLubyte *,_type_,_type_,_type_,_type_)); \
	LOAD_GL_FUNC(gl##_name_, _extension_); \
	Check_Type(arg2,T_STRING); \
	fptr_gl##_name_((GLuint)NUM2UINT(arg1),(GLsizei)RSTRING_LENINT(arg2),(GLubyte *)RSTRING_PTR(arg2),(_type_)_conv_(arg3),(_type_)_conv_(arg4),(_type_)_conv_(arg5),(_type_)_conv_(arg6)); \
	CHECK_GLERROR_FROM("gl" #_name_); \
	return Qnil; \
}

PROGRAMNAMEDPARAM_FUNC(ProgramNamedParameter4dNV,GLdouble,NUM2DBL,"GL_NV_fragment_program")
PROGRAMNAMEDPARAM_FUNC(ProgramNamedParameter4fNV,GLfloat,NUM2DBL,"GL_NV_fragment_program")
#undef PROGRAMNAMEDPARAM_FUNC

#define PROGRAMNAMEDPARAM_FUNC_V(_name_,_type_,_conv_,_extension_) \
static VALUE \
gl_##_name_(obj,arg1,arg2,arg3) \
VALUE obj,arg1,arg2,arg3; \
{ \
	_type_ cary[4]; \
    DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLuint,GLsizei,const GLubyte *,const _type_ *)); \
	LOAD_GL_FUNC(gl##_name_, _extension_); \
	Check_Type(arg2,T_STRING); \
	_conv_(arg3,cary,4); \
	fptr_gl##_name_(NUM2UINT(arg1),(GLsizei)RSTRING_LENINT(arg2),(GLubyte *)RSTRING_PTR(arg2),cary); \
	CHECK_GLERROR_FROM("gl" #_name_); \
	return Qnil; \
}

PROGRAMNAMEDPARAM_FUNC_V(ProgramNamedParameter4dvNV,GLdouble,ary2cdbl,"GL_NV_vertex_program")
PROGRAMNAMEDPARAM_FUNC_V(ProgramNamedParameter4fvNV,GLfloat,ary2cflt,"GL_NV_vertex_program")
#undef PROGRAMNAMEDPARAM_FUNC_V

#define GETPROGRAMNAMEDPARAM_FUNC(_name_,_type_,_extension_) \
static VALUE \
gl_##_name_(obj,arg1,arg2) \
VALUE obj,arg1,arg2; \
{ \
	_type_ cary[4] = {(_type_)0.0, (_type_)0.0, (_type_)0.0, (_type_)0.0}; \
    DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLuint,GLsizei,const GLubyte *,_type_ *)); \
	LOAD_GL_FUNC(gl##_name_, _extension_); \
	Check_Type(arg2,T_STRING); \
	fptr_gl##_name_((GLuint)NUM2UINT(arg1),(GLsizei)RSTRING_LENINT(arg2),(GLubyte *)RSTRING_PTR(arg2),cary); \
	RET_ARRAY_OR_SINGLE("gl" #_name_, 4, RETCONV_##_type_, cary); \
}

GETPROGRAMNAMEDPARAM_FUNC(GetProgramNamedParameterdvNV,GLdouble,"GL_NV_vertex_program")
GETPROGRAMNAMEDPARAM_FUNC(GetProgramNamedParameterfvNV,GLfloat,"GL_NV_vertex_program")
#undef GETPROGRAMNAMEDPARAM_FUNC

/* #285 GL_NV_primitive_restart */
GL_FUNC_LOAD_0(PrimitiveRestartNV,GLvoid, "GL_NV_primitive_restart")
GL_FUNC_LOAD_1(PrimitiveRestartIndexNV,GLvoid, GLuint, "GL_NV_primitive_restart")

/* #322 GL_NV_gpu_program4 */
GL_FUNC_LOAD_6(ProgramLocalParameterI4iNV,GLvoid, GLenum,GLuint, GLint,GLint,GLint,GLint, "GL_NV_gpu_program4")
GL_FUNC_LOAD_6(ProgramLocalParameterI4uiNV,GLvoid, GLenum,GLuint, GLuint,GLuint,GLuint,GLuint, "GL_NV_gpu_program4")
GL_FUNC_LOAD_6(ProgramEnvParameterI4iNV,GLvoid, GLenum,GLuint, GLint,GLint,GLint,GLint, "GL_NV_gpu_program4")
GL_FUNC_LOAD_6(ProgramEnvParameterI4uiNV,GLvoid, GLenum,GLuint, GLuint,GLuint,GLuint,GLuint, "GL_NV_gpu_program4")

PROGRAMPARAM_FUNC_V(ProgramLocalParameterI4ivNV,GLint,ary2cint,"GL_NV_gpu_program4")
PROGRAMPARAM_FUNC_V(ProgramLocalParameterI4uivNV,GLuint,ary2cuint,"GL_NV_gpu_program4")
PROGRAMPARAM_FUNC_V(ProgramEnvParameterI4ivNV,GLint,ary2cint,"GL_NV_gpu_program4")
PROGRAMPARAM_FUNC_V(ProgramEnvParameterI4uivNV,GLuint,ary2cuint,"GL_NV_gpu_program4")

PROGRAMPARAM_MULTI_FUNC_V(ProgramLocalParametersI4ivNV,GLint,ary2cint,"GL_NV_gpu_program4")
PROGRAMPARAM_MULTI_FUNC_V(ProgramLocalParametersI4uivNV,GLuint,ary2cuint,"GL_NV_gpu_program4")
PROGRAMPARAM_MULTI_FUNC_V(ProgramEnvParametersI4ivNV,GLint,ary2cint,"GL_NV_gpu_program4")
PROGRAMPARAM_MULTI_FUNC_V(ProgramEnvParametersI4uivNV,GLuint,ary2cuint,"GL_NV_gpu_program4")

#undef PROGRAMPARAM_MULTI_FUNC_V
#undef PROGRAMPARAM_FUNC_V

#define GETPROGRAMPARAM_FUNC_2(_name_,_type_,_extension_) \
static VALUE \
gl_##_name_(obj,arg1,arg2) \
VALUE obj,arg1,arg2; \
{ \
	_type_ cary[4] = {(_type_)0.0, (_type_)0.0, (_type_)0.0, (_type_)0.0}; \
    DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(GLenum,GLuint,_type_ *)); \
	LOAD_GL_FUNC(gl##_name_, _extension_); \
	fptr_gl##_name_(NUM2UINT(arg1),NUM2UINT(arg2),cary); \
	RET_ARRAY_OR_SINGLE("gl" #_name_, 4, RETCONV_##_type_, cary); \
}

GETPROGRAMPARAM_FUNC_2(GetProgramLocalParameterIivNV,GLint,"GL_NV_gpu_program4")
GETPROGRAMPARAM_FUNC_2(GetProgramLocalParameterIuivNV,GLuint,"GL_NV_gpu_program4")
GETPROGRAMPARAM_FUNC_2(GetProgramEnvParameterIivNV,GLint,"GL_NV_gpu_program4")
GETPROGRAMPARAM_FUNC_2(GetProgramEnvParameterIuivNV,GLuint,"GL_NV_gpu_program4")
#undef GETPROGRAMPARAM_FUNC_2

/* #323 GL_NV_geometry_program4 */
GL_FUNC_LOAD_2(ProgramVertexLimitNV,GLvoid, GLenum,GLint, "GL_EXT_geometry_shader4")
GL_FUNC_LOAD_4(FramebufferTextureEXT,GLvoid, GLenum,GLenum,GLuint,GLuint, "GL_EXT_geometry_shader4")
GL_FUNC_LOAD_5(FramebufferTextureLayerEXT,GLvoid, GLenum,GLenum,GLuint,GLint,GLint, "GL_EXT_geometry_shader4")
GL_FUNC_LOAD_5(FramebufferTextureFaceEXT,GLvoid, GLenum,GLenum,GLuint,GLint,GLenum, "GL_EXT_geometry_shader4")

/* #334 GL_NV_depth_buffer_float */
GL_FUNC_LOAD_2(DepthRangedNV,GLvoid, GLdouble,GLdouble, "GL_NV_depth_buffer_float")
GL_FUNC_LOAD_1(ClearDepthdNV,GLvoid, GLdouble, "GL_NV_depth_buffer_float")
GL_FUNC_LOAD_2(DepthBoundsdNV,GLvoid, GLdouble,GLdouble, "GL_NV_depth_buffer_float")

/* #336 GL_NV_framebuffer_multisample_coverage */
GL_FUNC_LOAD_6(RenderbufferStorageMultisampleCoverageNV,GLvoid, GLenum,GLsizei,GLsizei,GLenum,GLsizei,GLsizei, "GL_NV_framebuffer_multisample_coverage")

void gl_init_functions_ext_nv(VALUE klass)
{
	/* #222 GL_NV_fence */
	rb_define_method(klass, "glGenFencesNV", gl_GenFencesNV, 1);
	rb_define_method(klass, "glDeleteFencesNV", gl_DeleteFencesNV, 1);
	rb_define_method(klass, "glSetFenceNV", gl_SetFenceNV, 2);
	rb_define_method(klass, "glTestFenceNV", gl_TestFenceNV, 1);
	rb_define_method(klass, "glFinishFenceNV", gl_FinishFenceNV, 1);
	rb_define_method(klass, "glIsFenceNV", gl_IsFenceNV, 1);
	rb_define_method(klass, "glGetFenceivNV", gl_GetFenceivNV, 2);

	/* #233 GL_NV_vertex_program */
	rb_define_method(klass, "glLoadProgramNV", gl_LoadProgramNV, 3);
	rb_define_method(klass, "glGetProgramStringNV", gl_GetProgramStringNV, 2);
	rb_define_method(klass, "glGetProgramivNV", gl_GetProgramivNV, 2);
	rb_define_method(klass, "glBindProgramNV", gl_BindProgramNV, 2);
	rb_define_method(klass, "glGenProgramsNV", gl_GenProgramsNV, 1);
	rb_define_method(klass, "glDeleteProgramsNV", gl_DeleteProgramsNV, 1);
	rb_define_method(klass, "glIsProgramNV", gl_IsProgramNV, 1);
	rb_define_method(klass, "glExecuteProgramNV", gl_ExecuteProgramNV, 3);
	rb_define_method(klass, "glVertexAttribPointerNV", gl_VertexAttribPointerNV, 5);
	rb_define_method(klass, "glGetVertexAttribPointervNV", gl_GetVertexAttribPointervNV, 1);
	rb_define_method(klass, "glProgramParameter4dNV", gl_ProgramParameter4dNV, 6);
	rb_define_method(klass, "glProgramParameter4fNV", gl_ProgramParameter4fNV, 6);
	rb_define_method(klass, "glProgramParameter4dvNV", gl_ProgramParameter4dvNV, 3);
	rb_define_method(klass, "glProgramParameter4fvNV", gl_ProgramParameter4fvNV, 3);
	rb_define_method(klass, "glProgramParameters4dvNV", gl_ProgramParameters4dvNV, 3);
	rb_define_method(klass, "glProgramParameters4fvNV", gl_ProgramParameters4fvNV, 3);
	rb_define_method(klass, "glGetProgramParameterdvNV", gl_GetProgramParameterdvNV, 3);
	rb_define_method(klass, "glGetProgramParameterfvNV", gl_GetProgramParameterfvNV, 3);
	rb_define_method(klass, "glVertexAttrib1dNV", gl_VertexAttrib1dNV, 2);
	rb_define_method(klass, "glVertexAttrib1fNV", gl_VertexAttrib1fNV, 2);
	rb_define_method(klass, "glVertexAttrib1sNV", gl_VertexAttrib1sNV, 2);
	rb_define_method(klass, "glVertexAttrib2dNV", gl_VertexAttrib2dNV, 3);
	rb_define_method(klass, "glVertexAttrib2fNV", gl_VertexAttrib2fNV, 3);
	rb_define_method(klass, "glVertexAttrib2sNV", gl_VertexAttrib2sNV, 3);
	rb_define_method(klass, "glVertexAttrib3dNV", gl_VertexAttrib3dNV, 4);
	rb_define_method(klass, "glVertexAttrib3fNV", gl_VertexAttrib3fNV, 4);
	rb_define_method(klass, "glVertexAttrib3sNV", gl_VertexAttrib3sNV, 4);
	rb_define_method(klass, "glVertexAttrib4dNV", gl_VertexAttrib4dNV, 5);
	rb_define_method(klass, "glVertexAttrib4fNV", gl_VertexAttrib4fNV, 5);
	rb_define_method(klass, "glVertexAttrib4sNV", gl_VertexAttrib4sNV, 5);
	rb_define_method(klass, "glVertexAttrib4ubNV", gl_VertexAttrib4ubNV, 5);
	rb_define_method(klass, "glVertexAttrib1dvNV", gl_VertexAttrib1dvNV, 2);
	rb_define_method(klass, "glVertexAttrib1fvNV", gl_VertexAttrib1fvNV, 2);
	rb_define_method(klass, "glVertexAttrib1svNV", gl_VertexAttrib1svNV, 2);
	rb_define_method(klass, "glVertexAttrib2dvNV", gl_VertexAttrib2dvNV, 2);
	rb_define_method(klass, "glVertexAttrib2fvNV", gl_VertexAttrib2fvNV, 2);
	rb_define_method(klass, "glVertexAttrib2svNV", gl_VertexAttrib2svNV, 2);
	rb_define_method(klass, "glVertexAttrib3dvNV", gl_VertexAttrib3dvNV, 2);
	rb_define_method(klass, "glVertexAttrib3fvNV", gl_VertexAttrib3fvNV, 2);
	rb_define_method(klass, "glVertexAttrib3svNV", gl_VertexAttrib3svNV, 2);
	rb_define_method(klass, "glVertexAttrib4dvNV", gl_VertexAttrib4dvNV, 2);
	rb_define_method(klass, "glVertexAttrib4fvNV", gl_VertexAttrib4fvNV, 2);
	rb_define_method(klass, "glVertexAttrib4svNV", gl_VertexAttrib4svNV, 2);
	rb_define_method(klass, "glVertexAttrib4ubvNV", gl_VertexAttrib4ubvNV, 2);
	rb_define_method(klass, "glVertexAttribs1dvNV", gl_VertexAttribs1dvNV, 2);
	rb_define_method(klass, "glVertexAttribs1fvNV", gl_VertexAttribs1fvNV, 2);
	rb_define_method(klass, "glVertexAttribs1svNV", gl_VertexAttribs1svNV, 2);
	rb_define_method(klass, "glVertexAttribs2dvNV", gl_VertexAttribs2dvNV, 2);
	rb_define_method(klass, "glVertexAttribs2fvNV", gl_VertexAttribs2fvNV, 2);
	rb_define_method(klass, "glVertexAttribs2svNV", gl_VertexAttribs2svNV, 2);
	rb_define_method(klass, "glVertexAttribs3dvNV", gl_VertexAttribs3dvNV, 2);
	rb_define_method(klass, "glVertexAttribs3fvNV", gl_VertexAttribs3fvNV, 2);
	rb_define_method(klass, "glVertexAttribs3svNV", gl_VertexAttribs3svNV, 2);
	rb_define_method(klass, "glVertexAttribs4dvNV", gl_VertexAttribs4dvNV, 2);
	rb_define_method(klass, "glVertexAttribs4fvNV", gl_VertexAttribs4fvNV, 2);
	rb_define_method(klass, "glVertexAttribs4svNV", gl_VertexAttribs4svNV, 2);
	rb_define_method(klass, "glVertexAttribs4ubvNV", gl_VertexAttribs4ubvNV, 2);
	rb_define_method(klass, "glGetVertexAttribdvNV", gl_GetVertexAttribdvNV, 2);
	rb_define_method(klass, "glGetVertexAttribfvNV", gl_GetVertexAttribfvNV, 2);
	rb_define_method(klass, "glGetVertexAttribivNV", gl_GetVertexAttribivNV, 2);
	rb_define_method(klass, "glTrackMatrixNV", gl_TrackMatrixNV, 4);
	rb_define_method(klass, "glGetTrackMatrixivNV", gl_GetTrackMatrixivNV, 3);
	rb_define_method(klass, "glRequestResidentProgramsNV", gl_RequestResidentProgramsNV, 1);
	rb_define_method(klass, "glAreProgramsResidentNV", gl_AreProgramsResidentNV, 1);

	/* #261 GL_NV_occlusion_query */
	rb_define_method(klass, "glGenOcclusionQueriesNV", gl_GenOcclusionQueriesNV, 1);
	rb_define_method(klass, "glDeleteOcclusionQueriesNV", gl_DeleteOcclusionQueriesNV, 1);
	rb_define_method(klass, "glIsOcclusionQueryNV", gl_IsOcclusionQueryNV, 1);
	rb_define_method(klass, "glBeginOcclusionQueryNV", gl_BeginOcclusionQueryNV, 1);
	rb_define_method(klass, "glEndOcclusionQueryNV", gl_EndOcclusionQueryNV, 0);
	rb_define_method(klass, "glGetOcclusionQueryivNV", gl_GetOcclusionQueryivNV, 2);
	rb_define_method(klass, "glGetOcclusionQueryuivNV", gl_GetOcclusionQueryuivNV, 2);

	/* #262 GL_NV_point_sprite */
	rb_define_method(klass, "glPointParameteriNV", gl_PointParameteriNV, 2);
	rb_define_method(klass, "glPointParameterivNV", gl_PointParameterivNV, 2);

	/* #282 GL_NV_fragment_program */
	rb_define_method(klass, "glProgramNamedParameter4fNV", gl_ProgramNamedParameter4fNV, 6);
	rb_define_method(klass, "glProgramNamedParameter4dNV", gl_ProgramNamedParameter4dNV, 6);
	rb_define_method(klass, "glProgramNamedParameter4fvNV", gl_ProgramNamedParameter4fvNV, 3);
	rb_define_method(klass, "glProgramNamedParameter4dvNV", gl_ProgramNamedParameter4dvNV, 3);
	rb_define_method(klass, "glGetProgramNamedParameterdvNV", gl_GetProgramNamedParameterdvNV, 2);
	rb_define_method(klass, "glGetProgramNamedParameterfvNV", gl_GetProgramNamedParameterfvNV, 2);

	/* #285 GL_NV_primitive_restart */
	rb_define_method(klass, "glPrimitiveRestartNV", gl_PrimitiveRestartNV, 0);
	rb_define_method(klass, "glPrimitiveRestartIndexNV", gl_PrimitiveRestartIndexNV, 1);

	/* #322 GL_NV_gpu_program4 */
	rb_define_method(klass, "glProgramLocalParameterI4iNV", gl_ProgramLocalParameterI4iNV, 6);
	rb_define_method(klass, "glProgramLocalParameterI4uiNV", gl_ProgramLocalParameterI4uiNV, 6);
	rb_define_method(klass, "glProgramLocalParameterI4ivNV", gl_ProgramLocalParameterI4ivNV, 3);
	rb_define_method(klass, "glProgramLocalParameterI4uivNV", gl_ProgramLocalParameterI4uivNV, 3);
	rb_define_method(klass, "glProgramLocalParametersI4ivNV", gl_ProgramLocalParametersI4ivNV, 3);
	rb_define_method(klass, "glProgramLocalParametersI4uivNV", gl_ProgramLocalParametersI4uivNV, 3);
	rb_define_method(klass, "glGetProgramLocalParameterIivNV", gl_GetProgramLocalParameterIivNV, 2);
	rb_define_method(klass, "glGetProgramLocalParameterIuivNV", gl_GetProgramLocalParameterIuivNV, 2);
	rb_define_method(klass, "glProgramEnvParameterI4iNV", gl_ProgramEnvParameterI4iNV, 6);
	rb_define_method(klass, "glProgramEnvParameterI4uiNV", gl_ProgramEnvParameterI4uiNV, 6);
	rb_define_method(klass, "glProgramEnvParameterI4ivNV", gl_ProgramEnvParameterI4ivNV, 3);
	rb_define_method(klass, "glProgramEnvParameterI4uivNV", gl_ProgramEnvParameterI4uivNV, 3);
	rb_define_method(klass, "glProgramEnvParametersI4ivNV", gl_ProgramEnvParametersI4ivNV, 3);
	rb_define_method(klass, "glProgramEnvParametersI4uivNV", gl_ProgramEnvParametersI4uivNV, 3);
	rb_define_method(klass, "glGetProgramEnvParameterIivNV", gl_GetProgramEnvParameterIivNV, 2);
	rb_define_method(klass, "glGetProgramEnvParameterIuivNV", gl_GetProgramEnvParameterIuivNV, 2);

	/* #323 GL_NV_geometry_program4 */
	rb_define_method(klass, "glProgramVertexLimitNV", gl_ProgramVertexLimitNV, 2);
	rb_define_method(klass, "glFramebufferTextureEXT", gl_FramebufferTextureEXT, 4);
	rb_define_method(klass, "glFramebufferTextureLayerEXT", gl_FramebufferTextureLayerEXT, 5);
	rb_define_method(klass, "glFramebufferTextureFaceEXT", gl_FramebufferTextureFaceEXT, 5);

	/* #334 GL_NV_depth_buffer_float */
	rb_define_method(klass, "glDepthRangedNV", gl_DepthRangedNV, 2);
	rb_define_method(klass, "glClearDepthdNV", gl_ClearDepthdNV, 1);
	rb_define_method(klass, "glDepthBoundsdNV", gl_DepthBoundsdNV, 2);

	/* #336 GL_NV_framebuffer_multisample_coverage */
	rb_define_method(klass, "glRenderbufferStorageMultisampleCoverageNV", gl_RenderbufferStorageMultisampleCoverageNV, 6);

}
