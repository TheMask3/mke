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

/* Graphic Remedy extensions */

/* #311 GL_GREMEDY_string_marker */
static VALUE gl_StringMarkerGREMEDY(VALUE obj,VALUE arg1)
{
  DECL_GL_FUNC_PTR(GLvoid,glStringMarkerGREMEDY,(GLsizei,const void *));
	LOAD_GL_FUNC(glStringMarkerGREMEDY, "GL_GREMEDY_string_marker");
	Check_Type(arg1,T_STRING);
	fptr_glStringMarkerGREMEDY((GLsizei)RSTRING_LENINT(arg1),RSTRING_PTR(arg1));
	CHECK_GLERROR_FROM("glStringMarkerGREMEDY");
	return Qnil;
}

/* #345 GL_GREMEDY_frame_terminator */
GL_FUNC_LOAD_0(FrameTerminatorGREMEDY,GLvoid, "GL_GREMEDY_frame_terminator")

void gl_init_functions_ext_gremedy(VALUE klass)
{
/* #311 GL_GREMEDY_string_marker */
	rb_define_method(klass, "glStringMarkerGREMEDY", gl_StringMarkerGREMEDY, 1);

/* #345 GL_GREMEDY_frame_terminator */
	rb_define_method(klass, "glFrameTerminatorGREMEDY", gl_FrameTerminatorGREMEDY, 0);
}
