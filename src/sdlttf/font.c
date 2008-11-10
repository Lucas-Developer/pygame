/*
  pygame - Python Game Library
  Copyright (C) 2000-2001 Pete Shinners, 2008 Marcus von Appen

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/
#define PYGAME_SDLTTFFONT_INTERNAL

#include "ttfmod.h"
#include "pgsdl.h"
#include "pgttf.h"

static PyObject *_font_new (PyTypeObject *type, PyObject *args, PyObject *kwds);
static int _font_init (PyObject *chunk, PyObject *args, PyObject *kwds);
static void _font_dealloc (PyFont *self);

static PyObject* _font_glyphmetrics (PyObject *self, PyObject* args);
static PyObject* _font_getsize (PyObject *self, PyObject* args);
static PyObject* _font_render (PyObject *self, PyObject* args);

static PyObject* _font_getstyle (PyObject *self, void *closure);
static int _font_setstyle (PyObject *self, PyObject *value, void *closure);
static PyObject* _font_getheight (PyObject *self, void *closure);
static PyObject* _font_getascent (PyObject *self, void *closure);
static PyObject* _font_getdescent (PyObject *self, void *closure);
static PyObject* _font_getlineskip (PyObject *self, void *closure);
static PyObject* _font_getfaces (PyObject *self, void *closure);
static PyObject* _font_getisfixedwidth (PyObject *self, void *closure);
static PyObject* _font_getfamilyname (PyObject *self, void *closure);
static PyObject* _font_getstylename (PyObject *self, void *closure);

/**
 */
static PyMethodDef _font_methods[] = {
    { "get_glyph_metrics", _font_glyphmetrics, METH_VARARGS, "" },
    { "get_size", _font_getsize, METH_VARARGS, "" },
    { "render", _font_render, METH_VARARGS, "" },
    { NULL, NULL, 0, NULL }
};

/**
 */
static PyGetSetDef _font_getsets[] = {
    { "style", _font_getstyle, _font_setstyle, "", NULL },
    { "height", _font_getheight, NULL, "", NULL },
    { "ascent", _font_getascent, NULL, "", NULL },
    { "descent", _font_getdescent, NULL, "", NULL },
    { "line_skip", _font_getlineskip, NULL, "", NULL },
    { "faces", _font_getfaces, NULL, "", NULL },
    { "is_fixed_width", _font_getisfixedwidth, NULL, "", NULL },
    { "family_name", _font_getfamilyname, NULL, "", NULL },
    { "style_name", _font_getstylename, NULL, "", NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

/**
 */
PyTypeObject PyFont_Type =
{
    TYPE_HEAD(NULL,0)
    "sdlttf.Font",              /* tp_name */
    sizeof (PyFont),            /* tp_basicsize */
    0,                          /* tp_itemsize */
    (destructor) _font_dealloc, /* tp_dealloc */
    0,                          /* tp_print */
    0,                          /* tp_getattr */
    0,                          /* tp_setattr */
    0,                          /* tp_compare */
    0,                          /* tp_repr */
    0,                          /* tp_as_number */
    0,                          /* tp_as_sequence */
    0,                          /* tp_as_mapping */
    0,                          /* tp_hash */
    0,                          /* tp_call */
    0,                          /* tp_str */
    0,                          /* tp_getattro */
    0,                          /* tp_setattro */
    0,                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    "",
    0,                          /* tp_traverse */
    0,                          /* tp_clear */
    0,                          /* tp_richcompare */
    0,                          /* tp_weaklistoffset */
    0,                          /* tp_iter */
    0,                          /* tp_iternext */
    _font_methods,              /* tp_methods */
    0,                          /* tp_members */
    _font_getsets,              /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc) _font_init,      /* tp_init */
    0,                          /* tp_alloc */
    _font_new,                  /* tp_new */
    0,                          /* tp_free */
    0,                          /* tp_is_gc */
    0,                          /* tp_bases */
    0,                          /* tp_mro */
    0,                          /* tp_cache */
    0,                          /* tp_subclasses */
    0,                          /* tp_weaklist */
    0,                          /* tp_del */
#if PY_VERSION_HEX >= 0x02060000
    0                           /* tp_version_tag */
#endif
};

static void
_font_dealloc (PyFont *self)
{
    TTF_CloseFont (self->font);
    ((PyObject*)self)->ob_type->tp_free ((PyObject *) self);
}

static PyObject*
_font_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyFont *font = (PyFont*) type->tp_alloc (type, 0);
    if (!font)
        return NULL;
    font->font = NULL;
    return (PyObject*) font;
}

static int
_font_init (PyObject *self, PyObject *args, PyObject *kwds)
{
    PyObject *file;
    int ptsize, _index = 0;
    TTF_Font *font;

    ASSERT_TTF_INIT (-1);
    if (!PyArg_ParseTuple (args, "Oi|i", &file, &ptsize, &_index))
        return -1;

    if (_index < 0)
    {
        PyErr_SetString (PyExc_ValueError, "index must not be negative");
        return -1;
    }

    if (PyString_Check (file) || PyUnicode_Check (file))
    {
        char *filename = PyString_AsString (file);
        Py_BEGIN_ALLOW_THREADS;
        if (_index == 0)
            font = TTF_OpenFont (filename, ptsize);
        else
            font = TTF_OpenFontIndex (filename, ptsize, _index);
        Py_END_ALLOW_THREADS;
    }
    else if (PyFile_Check (file))
    {
        SDL_RWops *rw = RWopsFromPython (file);
        if (!rw)
            return -1;

        Py_BEGIN_ALLOW_THREADS;
        if (_index == 0)
            font = TTF_OpenFontRW (rw, 1, ptsize);
        else
            font = TTF_OpenFontIndexRW (rw, 1, ptsize, _index);
        Py_END_ALLOW_THREADS;
    }
    else
    {
        PyErr_SetString (PyExc_TypeError, "file must be a string or file");
        return -1;
    }

    if (!font)
    {
        PyErr_SetString (PyExc_PyGameError, TTF_GetError ());
        return -1;
    }
    ((PyFont*)self)->font = font;
    return 0;
}

/* Getters/Setters */
static PyObject*
_font_getstyle (PyObject *self, void *closure)
{
    ASSERT_TTF_INIT (NULL);
    return PyInt_FromLong (TTF_GetFontStyle (((PyFont*)self)->font));
}

static int
_font_setstyle (PyObject *self, PyObject *value, void *closure)
{
    int style;
    ASSERT_TTF_INIT (-1);

    if (!IntFromObj (value, &style))
        return -1;
    
    TTF_SetFontStyle (((PyFont*)self)->font, style);
    return 0;
}

static PyObject*
_font_getheight (PyObject *self, void *closure)
{
    ASSERT_TTF_INIT (NULL);
    return PyInt_FromLong (TTF_FontHeight (((PyFont*)self)->font));
}

static PyObject*
_font_getascent (PyObject *self, void *closure)
{
    ASSERT_TTF_INIT (NULL);
    return PyInt_FromLong (TTF_FontAscent (((PyFont*)self)->font));
}

static PyObject*
_font_getdescent (PyObject *self, void *closure)
{
    ASSERT_TTF_INIT (NULL);
    return PyInt_FromLong (TTF_FontDescent (((PyFont*)self)->font));
}

static PyObject*
_font_getlineskip (PyObject *self, void *closure)
{
    ASSERT_TTF_INIT (NULL);
    return PyInt_FromLong (TTF_FontLineSkip (((PyFont*)self)->font));
}

static PyObject*
_font_getfaces (PyObject *self, void *closure)
{
    ASSERT_TTF_INIT (NULL);
    return PyLong_FromLong (TTF_FontFaces (((PyFont*)self)->font));
}

static PyObject*
_font_getisfixedwidth (PyObject *self, void *closure)
{
    ASSERT_TTF_INIT (NULL);
    return PyBool_FromLong (TTF_FontFaceIsFixedWidth (((PyFont*)self)->font));
}

static PyObject*
_font_getfamilyname (PyObject *self, void *closure)
{
    ASSERT_TTF_INIT (NULL);
    return PyString_FromString (TTF_FontFaceFamilyName (((PyFont*)self)->font));
}

static PyObject*
_font_getstylename (PyObject *self, void *closure)
{
    ASSERT_TTF_INIT (NULL);
    return PyString_FromString (TTF_FontFaceStyleName (((PyFont*)self)->font));
}

/* Methods */
static PyObject*
_font_glyphmetrics (PyObject *self, PyObject* args)
{
    PyFont *font = (PyFont*) self;
    int minx, miny, maxx, maxy, advance, isunicode = 0, i;
    PyObject *textobj, *list;
    Py_ssize_t length;
    void *buf = NULL;

    ASSERT_TTF_INIT (NULL);
    
    if (!PyArg_ParseTuple (args, "O:glyph_metrics", &textobj))
        return NULL;

    if (PyUnicode_Check (textobj))
    {
        buf = PyUnicode_AsUnicode (textobj);
        isunicode = 1;
    }
    else if (PyString_Check (textobj))
        buf = PyString_AsString (textobj);
    else
    {
        PyErr_SetString (PyExc_TypeError,
            "argument must be a string or unicode");
    }

    if (!buf)
        return NULL;

    if (isunicode)
        length = PyUnicode_GetSize (textobj);
    else
        length = PyString_Size (textobj);
    if (length == 0)
        Py_RETURN_NONE;

    list = PyList_New (length);
    if (!list)
        return NULL;

    if (isunicode)
    {
        for (i = 0; i < length; i++)
        {
            /* 
             * TTF_GlyphMetrics() seems to return a value for any character,
             * using the default invalid character, if the char is not found.
             */
            if (TTF_GlyphMetrics (font->font, (Uint16) ((Py_UNICODE*) buf)[i],
                    &minx, &maxx, &miny, &maxy, &advance) == -1)
            {
                Py_INCREF (Py_None);
                PyList_SetItem (list, i, Py_None); /* No matching metrics. */
            }
            else
            {
                PyList_SetItem (list, i, Py_BuildValue
                    ("(iiiii)", minx, maxx, miny, maxy, advance));
            }
        }
    }
    else
    {
        for (i = 0; i < length; i++)
        {
            /* 
             * TTF_GlyphMetrics() seems to returns a value for any character,
             * using the default invalid character, if the char is not found.
             */
            if (TTF_GlyphMetrics (font->font, (Uint16) ((char*) buf)[i], &minx,
                    &maxx, &miny, &maxy, &advance) == -1)
            {
                Py_INCREF (Py_None);
                PyList_SetItem (list, i, Py_None); /* No matching metrics. */
            }
            else
            {
                PyList_SetItem (list, i, Py_BuildValue
                    ("(iiiii)", minx, maxx, miny, maxy, advance));
            }
        }
    }
    return list;
}

static PyObject*
_font_getsize (PyObject *self, PyObject* args)
{
    PyFont *font = (PyFont*) self;
    PyObject *text;
    int w, h;
    
    ASSERT_TTF_INIT (NULL);

    if (!PyArg_ParseTuple (args, "O:size", &text))
        return NULL;

    if (PyUnicode_Check (text))
    {
        PyObject* strob = PyUnicode_AsEncodedString (text, "utf-8", "replace");
        char *string = PyString_AsString (strob);
        if (TTF_SizeUTF8 (font->font, string, &w, &h) == -1)
        {
            PyErr_SetString (PyExc_PyGameError, TTF_GetError ());
            Py_DECREF (strob);
            return NULL;
        }
        Py_DECREF (strob);
        return Py_BuildValue ("(ii)", w, h);
    }
    
    if (PyString_Check (text))
    {
        char* string = PyString_AsString (text);
        if (TTF_SizeText (font->font, string, &w, &h) == -1)
        {
            PyErr_SetString (PyExc_PyGameError, TTF_GetError ());
            return NULL;
        }
        return Py_BuildValue ("(ii)", w, h);
    }

    PyErr_SetString (PyExc_TypeError, "text must be a string");
    return NULL;
}

static PyObject*
_font_render (PyObject *self, PyObject* args)
{
    PyFont *font = (PyFont*) self;
    PyObject *text, *colorfg, *pysurf, *colorbg = NULL;
    SDL_Surface *surface = NULL;
    SDL_Color fgcolor, bgcolor;
    int render = -1;

    ASSERT_TTF_INIT (NULL);

    if (!PyArg_ParseTuple (args, "OO|Oi:render", &text, &colorfg, &colorbg,
            &render))
        return NULL;

    if (!PyColor_Check (colorfg))
    {
        PyErr_SetString (PyExc_TypeError, "fgcolor must be a Color");
        return NULL;
    }
    if (colorbg && !PyColor_Check (colorbg))
    {
        PyErr_SetString (PyExc_TypeError, "bgcolor must be a Color");
        return NULL;
    }

    fgcolor.r = ((PyColor*)colorfg)->r;
    fgcolor.g = ((PyColor*)colorfg)->g;
    fgcolor.b = ((PyColor*)colorfg)->b;

    if (colorbg)
    {
        bgcolor.r = ((PyColor*)colorbg)->r;
        bgcolor.g = ((PyColor*)colorbg)->g;
        bgcolor.b = ((PyColor*)colorbg)->b;
        /* If the user set a bgcolor and left the render type aside, use
         * a shaded mode to activate the bgcolor. Otherwise ignore it.
         */
        if (render == -1)
            render = RENDER_SHADED;
    }

    if (render < RENDER_SOLID || render > RENDER_BLENDED)
        render = RENDER_SOLID;

    if (PyUnicode_Check (text))
    {
        PyObject* strob = PyUnicode_AsEncodedString (text, "utf-8", "replace");
        char *string = PyString_AsString (strob);

        switch (render)
        {
        case RENDER_SOLID:
            surface = TTF_RenderUTF8_Solid (font->font, string, fgcolor);
            break;
        case RENDER_SHADED:
            surface = TTF_RenderUTF8_Shaded (font->font, string, fgcolor,
                bgcolor);
            break;
        case RENDER_BLENDED:
            surface = TTF_RenderUTF8_Blended (font->font, string, fgcolor);
            break;
        }
        Py_DECREF (strob);
    }
    else if (PyString_Check (text))
    {
        char* string = PyString_AsString (text);

        switch (render)
        {
        case RENDER_SOLID:
            surface = TTF_RenderText_Solid (font->font, string, fgcolor);
            break;
        case RENDER_SHADED:
            surface = TTF_RenderText_Shaded (font->font, string, fgcolor,
                bgcolor);
            break;
        case RENDER_BLENDED:
            surface = TTF_RenderText_Blended (font->font, string, fgcolor);
            break;
        }
        if (!surface)
        {
            PyErr_SetString (PyExc_PyGameError, TTF_GetError ());
            return NULL;
        }
        pysurf = PySurface_NewFromSDLSurface (surface);
        if (!pysurf)
        {
            PyErr_SetString (PyExc_PyGameError, SDL_GetError ());
            return NULL;
        }
    }
    else
    {
        PyErr_SetString (PyExc_TypeError, "text must be a string");
        return NULL;
    }

    if (!surface)
    {
        PyErr_SetString (PyExc_PyGameError, TTF_GetError ());
        return NULL;
    }
    pysurf = PySurface_NewFromSDLSurface (surface);
    if (!pysurf)
    {
        SDL_FreeSurface (surface);
        PyErr_SetString (PyExc_PyGameError, SDL_GetError ());
        return NULL;
    }
    return pysurf;
}

/* C API */
PyObject*
PyFont_New (char *file, int ptsize)
{
    PyFont *font;
    TTF_Font *ttf;

    ASSERT_TTF_INIT (NULL);
    
    font = (PyFont*) PyFont_Type.tp_new (&PyFont_Type, NULL, NULL);
    if (!font)
        return NULL;

    ttf = TTF_OpenFont (file, ptsize);
    if (!ttf)
    {
        PyErr_SetString (PyExc_PyGameError, TTF_GetError ());
        return NULL;
    }

    font->font = ttf;
    return (PyObject*) font;
}

void
font_export_capi (void **capi)
{
    capi[PYGAME_SDLTTFFONT_FIRSTSLOT] = &PyFont_Type;
    capi[PYGAME_SDLTTFFONT_FIRSTSLOT+1] = &PyFont_New;
}