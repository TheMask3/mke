/*
 ** window-binding.cpp
 **
 ** This file is part of mke.
 **
 ** Copyright (C) 2013 Jonas Kulla <Nyocurio@gmail.com>
 **
 ** mke is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 2 of the License, or
 ** (at your option) any later version.
 **
 ** mke is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with mke.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "binding-util.h"
#include "disposable-binding.h"
#include "viewportelement-binding.h"
#include "window.h"

#if RAPI_FULL > 187
DEF_TYPE(Window);
#else
DEF_ALLOCFUNC(Window);
#endif

RB_METHOD(windowInitialize) {
    GFX_LOCK;
    Window *w = viewportElementInitialize<Window>(argc, argv, self);
    
    setPrivateData(self, w);
    
    w->initDynAttribs();
    
    wrapProperty(self, &w->getCursorRect(), "cursor_rect", RectType);
    
    GFX_UNLOCK;
    return self;
}

RB_METHOD(windowUpdate) {
    RB_UNUSED_PARAM;
    
    Window *w = getPrivateData<Window>(self);
    
    GFX_GUARD_EXC(w->update(););
    
    return Qnil;
}

DEF_GFX_PROP_OBJ_REF(Window, Bitmap, Windowskin, "windowskin")
DEF_GFX_PROP_OBJ_REF(Window, Bitmap, Contents, "contents")
DEF_GFX_PROP_OBJ_VAL(Window, Rect, CursorRect, "cursor_rect")

DEF_GFX_PROP_B(Window, Stretch)
DEF_GFX_PROP_B(Window, Active)
DEF_GFX_PROP_B(Window, Pause)

DEF_GFX_PROP_I(Window, X)
DEF_GFX_PROP_I(Window, Y)
DEF_GFX_PROP_I(Window, Width)
DEF_GFX_PROP_I(Window, Height)
DEF_GFX_PROP_I(Window, OX)
DEF_GFX_PROP_I(Window, OY)
DEF_GFX_PROP_I(Window, Opacity)
DEF_GFX_PROP_I(Window, BackOpacity)
DEF_GFX_PROP_I(Window, ContentsOpacity)

void windowBindingInit() {
    VALUE klass = rb_define_class("Window", rb_cObject);
#if RAPI_FULL > 187
    rb_define_alloc_func(klass, classAllocate<&WindowType>);
#else
    rb_define_alloc_func(klass, WindowAllocate);
#endif
    
    disposableBindingInit<Window>(klass);
    viewportElementBindingInit<Window>(klass);
    
    _rb_define_method(klass, "initialize", windowInitialize);
    _rb_define_method(klass, "update", windowUpdate);
    
    INIT_PROP_BIND(Window, Windowskin, "windowskin");
    INIT_PROP_BIND(Window, Contents, "contents");
    INIT_PROP_BIND(Window, Stretch, "stretch");
    INIT_PROP_BIND(Window, CursorRect, "cursor_rect");
    INIT_PROP_BIND(Window, Active, "active");
    INIT_PROP_BIND(Window, Pause, "pause");
    INIT_PROP_BIND(Window, X, "x");
    INIT_PROP_BIND(Window, Y, "y");
    INIT_PROP_BIND(Window, Width, "width");
    INIT_PROP_BIND(Window, Height, "height");
    INIT_PROP_BIND(Window, OX, "ox");
    INIT_PROP_BIND(Window, OY, "oy");
    INIT_PROP_BIND(Window, Opacity, "opacity");
    INIT_PROP_BIND(Window, BackOpacity, "back_opacity");
    INIT_PROP_BIND(Window, ContentsOpacity, "contents_opacity");
}
