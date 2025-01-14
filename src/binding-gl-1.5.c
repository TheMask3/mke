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

/* OpenGL 1.5 functions */

GL_FUNC_LOAD_1(EndQuery,GLvoid, GLenum, "1.5");
GL_FUNC_LOAD_1(IsQuery,GLboolean, GLuint, "1.5");
GL_FUNC_LOAD_2(BeginQuery,GLvoid, GLenum,GLuint, "1.5");
GL_FUNC_LOAD_2(BindBuffer,GLvoid, GLenum,GLuint, "1.5");
GL_FUNC_LOAD_1(IsBuffer,GLboolean, GLuint, "1.5");
GL_FUNC_GENOBJECTS_LOAD(GenQueries, "1.5");
GL_FUNC_DELETEOBJECTS_LOAD(DeleteQueries, "1.5");
GL_FUNC_GENOBJECTS_LOAD(GenBuffers, "1.5");
GL_FUNC_DELETEOBJECTS_LOAD(DeleteBuffers, "1.5");

static VALUE
gl_GetQueryiv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum pname;
	GLint params = 0;
  DECL_GL_FUNC_PTR(GLvoid,glGetQueryiv,(GLenum,GLenum,GLint *));
	LOAD_GL_FUNC(glGetQueryiv, "1.5");
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	fptr_glGetQueryiv(target,pname,&params);
	CHECK_GLERROR_FROM("glGetQueryiv");
	return RETCONV_GLint(params);
}

static VALUE
gl_GetQueryObjectiv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLuint id;
	GLenum pname;
	GLint params = 0;
  DECL_GL_FUNC_PTR(GLvoid,glGetQueryObjectiv,(GLuint,GLenum,GLint *));
	LOAD_GL_FUNC(glGetQueryObjectiv, "1.5");
	id = (GLuint)NUM2UINT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	fptr_glGetQueryObjectiv(id,pname,&params);
	CHECK_GLERROR_FROM("glGetQueryObjectiv");
	return cond_GLBOOL2RUBY(pname,params);
}

static VALUE
gl_GetQueryObjectuiv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLuint id;
	GLenum pname;
	GLuint params = 0;
  DECL_GL_FUNC_PTR(GLvoid,glGetQueryObjectuiv,(GLuint,GLenum,GLuint *));
	LOAD_GL_FUNC(glGetQueryObjectuiv, "1.5");
	id = (GLuint)NUM2UINT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	fptr_glGetQueryObjectuiv(id,pname,&params);
	CHECK_GLERROR_FROM("glGetQueryObjectuiv");
	return cond_GLBOOL2RUBY_U(pname,params);
}

static VALUE
gl_BufferData(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLenum target;
	GLsizeiptr size;
	GLenum usage;
  DECL_GL_FUNC_PTR(GLvoid,glBufferData,(GLenum,GLsizeiptr,GLvoid *,GLenum));
	LOAD_GL_FUNC(glBufferData, "1.5");
	target = (GLenum)NUM2INT(arg1);
	size = (GLsizeiptr)NUM2INT(arg2);
	usage = (GLenum)NUM2INT(arg4);
	if (TYPE(arg3) == T_STRING) {
		fptr_glBufferData(target,size,(GLvoid *)RSTRING_PTR(arg3),usage);
	} else if (NIL_P(arg3)) {
		fptr_glBufferData(target,size,NULL,usage);
	} else {
		Check_Type(arg3,T_STRING); /* force exception - HACK do it right */
	}
	CHECK_GLERROR_FROM("glBufferData");
	return Qnil;
}

static VALUE
gl_BufferSubData(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLenum target;
	GLintptr offset;
	GLsizeiptr size;
  DECL_GL_FUNC_PTR(GLvoid,glBufferSubData,(GLenum,GLintptr,GLsizeiptr,GLvoid *));
	LOAD_GL_FUNC(glBufferSubData, "1.5");
	target = (GLenum)NUM2INT(arg1);
	offset = (GLintptr)NUM2INT(arg2);
	size = (GLsizeiptr)NUM2INT(arg3);
	Check_Type(arg4,T_STRING);
	fptr_glBufferSubData(target,offset,size,(GLvoid *)RSTRING_PTR(arg4));
	CHECK_GLERROR_FROM("glBufferSubData");
	return Qnil;
}

static VALUE
gl_GetBufferSubData(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLintptr offset;
	GLsizeiptr size;
	VALUE data;
  DECL_GL_FUNC_PTR(GLvoid,glGetBufferSubData,(GLenum,GLintptr,GLsizeiptr,GLvoid *));
	LOAD_GL_FUNC(glGetBufferSubData, "1.5");
	target = (GLenum)NUM2INT(arg1);
	offset = (GLintptr)NUM2INT(arg2);
	size = (GLsizeiptr)NUM2INT(arg3);
	data = allocate_buffer_with_string((long)size);
	fptr_glGetBufferSubData(target,offset,size,(GLvoid *)RSTRING_PTR(data));
	CHECK_GLERROR_FROM("glGetBufferSubData");
	return data;
}

static VALUE
gl_GetBufferParameteriv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum value;
	GLint data = 0;
  DECL_GL_FUNC_PTR(GLvoid,glGetBufferParameteriv,(GLenum,GLenum,GLint *));
	LOAD_GL_FUNC(glGetBufferParameteriv, "1.5");
	target = (GLenum)NUM2INT(arg1);
	value = (GLenum)NUM2INT(arg2);
	fptr_glGetBufferParameteriv(target,value,&data);
	CHECK_GLERROR_FROM("glGetBufferParameteriv");
	return cond_GLBOOL2RUBY(value,data);
}

static VALUE
gl_MapBuffer(VALUE obj, VALUE _target, VALUE _access) {
	GLenum target = CONV_GLenum(_target);
	GLenum access = CONV_GLenum(_access);
	GLint size = 0;
	GLvoid *buffer_ptr = NULL;

  DECL_GL_FUNC_PTR(GLvoid *,glMapBuffer,(GLenum,GLenum));
  DECL_GL_FUNC_PTR(GLvoid,glGetBufferParameteriv,(GLenum,GLenum,GLint *));
	LOAD_GL_FUNC(glMapBuffer, "1.5");
	LOAD_GL_FUNC(glGetBufferParameteriv, "1.5");

	fptr_glGetBufferParameteriv(target, GL_BUFFER_SIZE, &size);
	CHECK_GLERROR_FROM("glGetBufferParameteriv");

	buffer_ptr = fptr_glMapBuffer(target, access);
	CHECK_GLERROR_FROM("glMapBuffer");

	/* fail late to avoid GL_INVALID_OPERATION from glUnampBuffer */
	if (buffer_ptr == NULL || size <= 0)
		return Qnil;

	return rb_str_new(buffer_ptr, size);
}

static VALUE
gl_UnmapBuffer(obj,arg1)
VALUE obj,arg1;
{
	GLenum target;
	GLboolean ret;
  DECL_GL_FUNC_PTR(GLboolean,glUnmapBuffer,(GLenum));
	LOAD_GL_FUNC(glUnmapBuffer, "1.5");
	target = (GLenum)NUM2INT(arg1);
	ret = fptr_glUnmapBuffer(target);
	CHECK_GLERROR_FROM("glUnmapBuffer");
	return GLBOOL2RUBY(ret);
}

static VALUE
gl_GetBufferPointerv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
  DECL_GL_FUNC_PTR(GLvoid,glGetBufferPointerv,(GLenum,GLenum,GLvoid **));
	LOAD_GL_FUNC(glGetBufferPointerv, "1.5");
	rb_raise(rb_eArgError, "glGetBufferPointerv not implemented");
	CHECK_GLERROR_FROM("glGetBufferPointerv");
	return Qnil;
}

void gl_init_functions_1_5(VALUE klass)
{
	rb_define_method(klass, "glGenQueries", gl_GenQueries, 1);
	rb_define_method(klass, "glDeleteQueries", gl_DeleteQueries, 1);
	rb_define_method(klass, "glIsQuery", gl_IsQuery, 1);
	rb_define_method(klass, "glBeginQuery", gl_BeginQuery, 2);
	rb_define_method(klass, "glEndQuery", gl_EndQuery, 1);
	rb_define_method(klass, "glGetQueryiv", gl_GetQueryiv, 2);
	rb_define_method(klass, "glGetQueryObjectiv", gl_GetQueryObjectiv, 2);
	rb_define_method(klass, "glGetQueryObjectuiv", gl_GetQueryObjectuiv, 2);
	rb_define_method(klass, "glBindBuffer", gl_BindBuffer, 2);
	rb_define_method(klass, "glDeleteBuffers", gl_DeleteBuffers, 1);
	rb_define_method(klass, "glGenBuffers", gl_GenBuffers, 1);
	rb_define_method(klass, "glIsBuffer", gl_IsBuffer, 1);
	rb_define_method(klass, "glBufferData", gl_BufferData, 4);
	rb_define_method(klass, "glBufferSubData", gl_BufferSubData, 4);
	rb_define_method(klass, "glGetBufferSubData", gl_GetBufferSubData, 3);
	rb_define_method(klass, "glMapBuffer", gl_MapBuffer, 2);
	rb_define_method(klass, "glUnmapBuffer", gl_UnmapBuffer, 1);
	rb_define_method(klass, "glGetBufferParameteriv", gl_GetBufferParameteriv, 2);
	rb_define_method(klass, "glGetBufferPointerv", gl_GetBufferPointerv, 3);
}
