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

/* OpenGL 1.4 functions */

GL_FUNC_LOAD_4(BlendFuncSeparate,GLvoid, GLenum,GLenum,GLenum,GLenum, "1.4")
GL_FUNC_LOAD_1(FogCoordf,GLvoid, GLfloat, "1.4")
GL_FUNC_LOAD_1(FogCoordd,GLvoid, GLdouble, "1.4")
GL_FUNC_LOAD_2(PointParameteri,GLvoid, GLenum,GLint, "1.4")
GL_FUNC_LOAD_2(PointParameterf,GLvoid, GLenum,GLfloat, "1.4")
GL_FUNC_LOAD_3(SecondaryColor3b,GLvoid, GLbyte,GLbyte,GLbyte, "1.4")
GL_FUNC_LOAD_3(SecondaryColor3d,GLvoid, GLdouble,GLdouble,GLdouble, "1.4")
GL_FUNC_LOAD_3(SecondaryColor3f,GLvoid, GLfloat,GLfloat,GLfloat, "1.4")
GL_FUNC_LOAD_3(SecondaryColor3i,GLvoid, GLint,GLint,GLint, "1.4")
GL_FUNC_LOAD_3(SecondaryColor3s,GLvoid, GLshort,GLshort,GLshort, "1.4")
GL_FUNC_LOAD_3(SecondaryColor3ub,GLvoid, GLubyte,GLubyte,GLubyte, "1.4")
GL_FUNC_LOAD_3(SecondaryColor3ui,GLvoid, GLuint,GLuint,GLuint, "1.4")
GL_FUNC_LOAD_3(SecondaryColor3us,GLvoid, GLushort,GLushort,GLushort, "1.4")
GL_FUNC_LOAD_2(WindowPos2d,GLvoid, GLdouble,GLdouble, "1.4")
GL_FUNC_LOAD_2(WindowPos2f,GLvoid, GLfloat,GLfloat, "1.4")
GL_FUNC_LOAD_2(WindowPos2i,GLvoid, GLint,GLint, "1.4")
GL_FUNC_LOAD_2(WindowPos2s,GLvoid, GLshort,GLshort, "1.4")
GL_FUNC_LOAD_3(WindowPos3d,GLvoid, GLdouble,GLdouble,GLdouble, "1.4")
GL_FUNC_LOAD_3(WindowPos3f,GLvoid, GLfloat,GLfloat,GLfloat, "1.4")
GL_FUNC_LOAD_3(WindowPos3i,GLvoid, GLint,GLint,GLint, "1.4")
GL_FUNC_LOAD_3(WindowPos3s,GLvoid, GLshort,GLshort,GLshort, "1.4")

static VALUE
gl_FogCoordfv(obj,arg1)
VALUE obj,arg1;
{
	GLfloat coord;
  DECL_GL_FUNC_PTR(GLvoid,glFogCoordfv,(GLfloat *));
	LOAD_GL_FUNC(glFogCoordfv, "1.4");
	Check_Type(arg1,T_ARRAY);
	ary2cflt(arg1,&coord,1);
	fptr_glFogCoordfv(&coord);
	CHECK_GLERROR_FROM("glFogCoordfv");
	return Qnil;
}

static VALUE
gl_FogCoorddv(obj,arg1)
VALUE obj,arg1;
{
	GLdouble coord;
  DECL_GL_FUNC_PTR(GLvoid,glFogCoorddv,(GLdouble *));
	LOAD_GL_FUNC(glFogCoorddv, "1.4");
	Check_Type(arg1,T_ARRAY);
	ary2cdbl(arg1,&coord,1);
	fptr_glFogCoorddv(&coord);
	CHECK_GLERROR_FROM("glFogCoorddv");
	return Qnil;
}

static VALUE
gl_FogCoordPointer(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum type;
	GLsizei stride;
  DECL_GL_FUNC_PTR(GLvoid,glFogCoordPointer,(GLenum,GLsizei,const GLvoid *));
	LOAD_GL_FUNC(glFogCoordPointer, "1.4");
	type = (GLenum)NUM2INT(arg1);
	stride = (GLsizei)NUM2UINT(arg2);
	if (CHECK_BUFFER_BINDING(GL_ARRAY_BUFFER_BINDING)) {
		SET_GLIMPL_VARIABLE(FogCoord_ptr, arg3);
		fptr_glFogCoordPointer(type, stride, (const GLvoid*)NUM2SIZET(arg3));
	} else {
		VALUE data;
		data = pack_array_or_pass_string(GL_UNSIGNED_BYTE,arg3);
		rb_str_freeze(data);
		SET_GLIMPL_VARIABLE(FogCoord_ptr, data);
		fptr_glFogCoordPointer(type, stride, (const GLvoid*)RSTRING_PTR(data));
	}
	CHECK_GLERROR_FROM("glFogCoordPointer");
	return Qnil;
}

static VALUE
gl_MultiDrawArrays(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum mode;
	GLint *ary1;
	GLsizei *ary2;
	int len1,len2;
  DECL_GL_FUNC_PTR(GLvoid,glMultiDrawArrays,(GLenum,GLint*,GLsizei*,GLsizei));
	LOAD_GL_FUNC(glMultiDrawArrays, "1.4");
	len1 = (int)RARRAY_LENINT(arg2);
	len2 = (int)RARRAY_LENINT(arg3);
	if (len1!=len2)
			rb_raise(rb_eArgError, "Passed arrays must have same length");
	mode = (GLenum)NUM2INT(arg1);
	ary1 = ALLOC_N(GLint,len1);
	ary2 = ALLOC_N(GLsizei,len2);
	ary2cint(arg2,ary1,len1);
	ary2cint(arg3,ary2,len2);
	fptr_glMultiDrawArrays(mode,ary1,ary2,len1);
	xfree(ary1);
	xfree(ary2);
	CHECK_GLERROR_FROM("glMultiDrawArrays");
	return Qnil;
}

static VALUE
gl_MultiDrawElements(argc,argv,obj)
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
  DECL_GL_FUNC_PTR(GLvoid,glMultiDrawElements,(GLenum,const GLsizei *,GLenum,GLvoid **,GLsizei));
	LOAD_GL_FUNC(glMultiDrawElements, "1.4");
	switch (rb_scan_args(argc, argv, "31", &args[0], &args[1], &args[2],&args[3])) {
		default:
		case 3:
			if (CHECK_BUFFER_BINDING(GL_ELEMENT_ARRAY_BUFFER_BINDING))
				rb_raise(rb_eArgError, "Element array buffer bound, but offsets array missing");
			mode = (GLenum)NUM2INT(args[0]);
			type = (GLenum)NUM2INT(args[1]);
			Check_Type(args[2],T_ARRAY);
			ary = args[2];
			size = (GLint)RARRAY_LENINT(ary);
			counts = ALLOC_N(GLsizei,size);
			indices = ALLOC_N(GLvoid*,size);
			for (i=0;i<size;i++) {
				VALUE data;
				data = pack_array_or_pass_string(type,RARRAY_PTR(ary)[i]);
				indices[i] = RSTRING_PTR(data);
				counts[i] = (GLsizei)RSTRING_LENINT(data);
			}
			fptr_glMultiDrawElements(mode,counts,type,indices,size);
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

			size = (GLint)RARRAY_LENINT(args[2]);

			counts = ALLOC_N(GLsizei,size);
			indices = ALLOC_N(GLvoid*,size);
			for (i=0;i<size;i++) {
				counts[i] = (GLsizei)NUM2INT(rb_ary_entry(args[2],i));
				indices[i] = (GLvoid *)NUM2SIZET(rb_ary_entry(args[3],i));
			}
			fptr_glMultiDrawElements(mode,counts,type,indices,size);
			xfree(counts);
			xfree(indices);
			break;
	}
	CHECK_GLERROR_FROM("glMultiDrawElements");
	return Qnil;
}

static VALUE
gl_PointParameterfv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLfloat params[3] = {(GLfloat)0.0,(GLfloat)0.0,(GLfloat)0.0};
	GLint size;
  DECL_GL_FUNC_PTR(GLvoid,glPointParameterfv,(GLenum,GLfloat *));
	LOAD_GL_FUNC(glPointParameterfv, "1.4");
	pname = (GLenum)NUM2INT(arg1);
	Check_Type(arg2,T_ARRAY);
	if (pname==GL_POINT_DISTANCE_ATTENUATION)
		size = 3;
	else
		size = 1;
	ary2cflt(arg2,params,size);
	fptr_glPointParameterfv(pname,params);
	CHECK_GLERROR_FROM("glPointParameterfv");
	return Qnil;
}

static VALUE
gl_PointParameteriv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLint params[3] = {0,0,0};
	GLint size;
  DECL_GL_FUNC_PTR(GLvoid,glPointParameteriv,(GLenum,GLint *));
	LOAD_GL_FUNC(glPointParameteriv, "1.4");
	pname = (GLenum)NUM2INT(arg1);
	Check_Type(arg2,T_ARRAY);
	if (pname==GL_POINT_DISTANCE_ATTENUATION)
		size = 3;
	else
		size = 1;
	ary2cint(arg2,params,size);
	fptr_glPointParameteriv(pname,params);
	CHECK_GLERROR_FROM("glPointParameteriv");
	return Qnil;
}

#define GLSECONDARYCOLOR_VFUNC(_name_,_type_,_conv_) \
VALUE gl_##_name_(VALUE obj,VALUE arg1) \
{ \
	_type_ cary[3] = {0,0,0}; \
  DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(_type_ *)); \
	LOAD_GL_FUNC(gl##_name_, "1.4"); \
	Check_Type(arg1,T_ARRAY); \
	_conv_(arg1,cary,3); \
	fptr_gl##_name_(cary); \
	CHECK_GLERROR_FROM("gl" #_name_); \
	return Qnil; \
}

GLSECONDARYCOLOR_VFUNC(SecondaryColor3bv,GLbyte,ary2cbyte)
GLSECONDARYCOLOR_VFUNC(SecondaryColor3dv,GLdouble,ary2cdbl)
GLSECONDARYCOLOR_VFUNC(SecondaryColor3fv,GLfloat,ary2cflt)
GLSECONDARYCOLOR_VFUNC(SecondaryColor3iv,GLint,ary2cint)
GLSECONDARYCOLOR_VFUNC(SecondaryColor3sv,GLshort,ary2cshort)
GLSECONDARYCOLOR_VFUNC(SecondaryColor3uiv,GLuint,ary2cuint)
GLSECONDARYCOLOR_VFUNC(SecondaryColor3ubv,GLubyte,ary2cubyte)
GLSECONDARYCOLOR_VFUNC(SecondaryColor3usv,GLushort,ary2cushort)
#undef GLSECONDARYCOLOR_VFUNC

static VALUE
gl_SecondaryColorPointer(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLint size;
	GLenum type;
	GLsizei stride;
  DECL_GL_FUNC_PTR(GLvoid,glSecondaryColorPointer,(GLint,GLenum,GLsizei,const GLvoid *));
	LOAD_GL_FUNC(glSecondaryColorPointer, "1.4");
	size = (GLint)NUM2INT(arg1);
	type = (GLenum)NUM2INT(arg2);
	stride = (GLsizei)NUM2UINT(arg3);
	if (CHECK_BUFFER_BINDING(GL_ARRAY_BUFFER_BINDING)) {
		SET_GLIMPL_VARIABLE(SecondaryColor_ptr, arg4);
		fptr_glSecondaryColorPointer(size,type, stride, (const GLvoid*)NUM2SIZET(arg4));
	} else {
		VALUE data;
		data = pack_array_or_pass_string(type,arg4);
		rb_str_freeze(data);
		SET_GLIMPL_VARIABLE(SecondaryColor_ptr, data);
		fptr_glSecondaryColorPointer(size,type, stride, (const GLvoid*)RSTRING_PTR(data));
	}
	CHECK_GLERROR_FROM("glSecondaryColorPointer");
	return Qnil;
}

#define GLWINDOWPOS_VFUNC(_name_,_type_,_size_,_conv_) \
VALUE gl_##_name_(VALUE obj,VALUE arg1) \
{ \
	_type_ cary[3] = {0,0,0}; \
  DECL_GL_FUNC_PTR(GLvoid,gl##_name_,(_type_ *)); \
	LOAD_GL_FUNC(gl##_name_, "1.4"); \
	Check_Type(arg1,T_ARRAY); \
	_conv_(arg1,cary,_size_); \
	fptr_gl##_name_(cary); \
	CHECK_GLERROR_FROM("gl" #_name_); \
	return Qnil; \
}

GLWINDOWPOS_VFUNC(WindowPos2dv,GLdouble,2,ary2cdbl)
GLWINDOWPOS_VFUNC(WindowPos2fv,GLfloat,2,ary2cflt)
GLWINDOWPOS_VFUNC(WindowPos2iv,GLint,2,ary2cint)
GLWINDOWPOS_VFUNC(WindowPos2sv,GLshort,2,ary2cshort)
GLWINDOWPOS_VFUNC(WindowPos3dv,GLdouble,3,ary2cdbl)
GLWINDOWPOS_VFUNC(WindowPos3fv,GLfloat,3,ary2cflt)
GLWINDOWPOS_VFUNC(WindowPos3iv,GLint,3,ary2cint)
GLWINDOWPOS_VFUNC(WindowPos3sv,GLshort,3,ary2cshort)
#undef GLWINDOWPOS_VFUNC


void gl_init_functions_1_4(VALUE klass)
{
	rb_define_method(klass, "glBlendFuncSeparate", gl_BlendFuncSeparate, 4);
	rb_define_method(klass, "glFogCoordf", gl_FogCoordf, 1);
	rb_define_method(klass, "glFogCoordfv", gl_FogCoordfv, 1);
	rb_define_method(klass, "glFogCoordd", gl_FogCoordd, 1);
	rb_define_method(klass, "glFogCoorddv", gl_FogCoorddv, 1);
	rb_define_method(klass, "glFogCoordPointer", gl_FogCoordPointer, 3);
	rb_define_method(klass, "glMultiDrawArrays", gl_MultiDrawArrays, 3);
	rb_define_method(klass, "glMultiDrawElements", gl_MultiDrawElements, -1);
	rb_define_method(klass, "glPointParameterf", gl_PointParameterf, 2);
	rb_define_method(klass, "glPointParameterfv", gl_PointParameterfv, 2);
	rb_define_method(klass, "glPointParameteri", gl_PointParameteri, 2);
	rb_define_method(klass, "glPointParameteriv", gl_PointParameteriv, 2);
	rb_define_method(klass, "glSecondaryColor3b", gl_SecondaryColor3b, 3);
	rb_define_method(klass, "glSecondaryColor3d", gl_SecondaryColor3d, 3);
	rb_define_method(klass, "glSecondaryColor3f", gl_SecondaryColor3f, 3);
	rb_define_method(klass, "glSecondaryColor3i", gl_SecondaryColor3i, 3);
	rb_define_method(klass, "glSecondaryColor3s", gl_SecondaryColor3s, 3);
	rb_define_method(klass, "glSecondaryColor3ub", gl_SecondaryColor3ub, 3);
	rb_define_method(klass, "glSecondaryColor3ui", gl_SecondaryColor3ui, 3);
	rb_define_method(klass, "glSecondaryColor3us", gl_SecondaryColor3us, 3);
	rb_define_method(klass, "glSecondaryColor3bv", gl_SecondaryColor3bv, 1);
	rb_define_method(klass, "glSecondaryColor3dv", gl_SecondaryColor3dv, 1);
	rb_define_method(klass, "glSecondaryColor3fv", gl_SecondaryColor3fv, 1);
	rb_define_method(klass, "glSecondaryColor3iv", gl_SecondaryColor3iv, 1);
	rb_define_method(klass, "glSecondaryColor3sv", gl_SecondaryColor3sv, 1);
	rb_define_method(klass, "glSecondaryColor3ubv", gl_SecondaryColor3ubv, 1);
	rb_define_method(klass, "glSecondaryColor3uiv", gl_SecondaryColor3uiv, 1);
	rb_define_method(klass, "glSecondaryColor3usv", gl_SecondaryColor3usv, 1);
	rb_define_method(klass, "glSecondaryColorPointer", gl_SecondaryColorPointer, 4);
	rb_define_method(klass, "glWindowPos2d", gl_WindowPos2d, 2);
	rb_define_method(klass, "glWindowPos2f", gl_WindowPos2f, 2);
	rb_define_method(klass, "glWindowPos2i", gl_WindowPos2i, 2);
	rb_define_method(klass, "glWindowPos2s", gl_WindowPos2s, 2);
	rb_define_method(klass, "glWindowPos3d", gl_WindowPos3d, 3);
	rb_define_method(klass, "glWindowPos3f", gl_WindowPos3f, 3);
	rb_define_method(klass, "glWindowPos3i", gl_WindowPos3i, 3);
	rb_define_method(klass, "glWindowPos3s", gl_WindowPos3s, 3);
	rb_define_method(klass, "glWindowPos2dv", gl_WindowPos2dv, 1);
	rb_define_method(klass, "glWindowPos2fv", gl_WindowPos2fv, 1);
	rb_define_method(klass, "glWindowPos2iv", gl_WindowPos2iv, 1);
	rb_define_method(klass, "glWindowPos2sv", gl_WindowPos2sv, 1);
	rb_define_method(klass, "glWindowPos3dv", gl_WindowPos3dv, 1);
	rb_define_method(klass, "glWindowPos3fv", gl_WindowPos3fv, 1);
	rb_define_method(klass, "glWindowPos3iv", gl_WindowPos3iv, 1);
	rb_define_method(klass, "glWindowPos3sv", gl_WindowPos3sv, 1);
}
