/* GdkGLExt - OpenGL Extension to GDK
 * Copyright (C) 2002  Naofumi Yasufuku
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA.
 */

#include <pango/pangowin32.h>

#include "gdkglwin32.h"
#include "gdkglprivate-win32.h"
#include "gdkglfont.h"

#ifdef GDKGLEXT_MULTIHEAD_SUPPORT
#include <gdk/gdkdisplay.h>
#endif /* GDKGLEXT_MULTIHEAD_SUPPORT */

PangoFont *
gdk_gl_font_use_pango_font (const PangoFontDescription *font_desc,
                            int                         first,
                            int                         count,
                            int                         list_base)
{
  PangoFontMap *font_map;
  PangoWin32FontCache *font_cache;
  PangoFont *font = NULL;
  LOGFONT *logfont = NULL;
  HFONT hfont;
  HDC hdc;

  g_return_val_if_fail (font_desc != NULL, NULL);

  GDK_GL_NOTE (FUNC, g_message (" - gdk_gl_font_use_pango_font ()"));

  font_map = pango_win32_font_map_for_display ();
  font_cache = pango_win32_font_map_get_font_cache (font_map);

  font = pango_font_map_load_font (font_map, NULL, font_desc);
  if (font == NULL)
    {
      g_warning ("cannot load PangoFont");
      goto FAIL;
    }

  logfont = pango_win32_font_logfont (font);
  if (logfont == NULL)
    {
      g_warning ("cannot get LOGFONT struct");
      font = NULL;
      goto FAIL;
    }

  hfont = pango_win32_font_cache_load (font_cache, logfont);

  hdc = CreateCompatibleDC (NULL);
  if (hdc == NULL)
    {
      g_warning ("cannot create a memory DC");
      font = NULL;
      goto FAIL;
    }

  SelectObject (hdc, hfont);

  if (!wglUseFontBitmaps (hdc, first, count, list_base))
    {
      g_warning ("cannot create the font display lists");
      font = NULL;
      goto FAIL;
    }

  if (!DeleteDC (hdc))
    g_warning ("cannot delete the memory DC");

  pango_win32_font_cache_unload (font_cache, hfont);

 FAIL:

  if (logfont != NULL)
    g_free (logfont);

  return font;
}

#ifdef GDKGLEXT_MULTIHEAD_SUPPORT

PangoFont *
gdk_gl_font_use_pango_font_for_display (GdkDisplay                 *display,
                                        const PangoFontDescription *font_desc,
                                        int                         first,
                                        int                         count,
                                        int                         list_base)
{
  PangoFontMap *font_map;
  PangoWin32FontCache *font_cache;
  PangoFont *font = NULL;
  LOGFONT *logfont = NULL;
  HFONT hfont;
  HDC hdc;

  g_return_val_if_fail (display == gdk_display_get_default (), NULL);
  g_return_val_if_fail (font_desc != NULL, NULL);

  GDK_GL_NOTE (FUNC, g_message (" - gdk_gl_font_use_pango_font ()"));

  font_map = pango_win32_font_map_for_display ();
  font_cache = pango_win32_font_map_get_font_cache (font_map);

  font = pango_font_map_load_font (font_map, NULL, font_desc);
  if (font == NULL)
    {
      g_warning ("cannot load PangoFont");
      goto FAIL;
    }

  logfont = pango_win32_font_logfont (font);
  if (logfont == NULL)
    {
      g_warning ("cannot get LOGFONT struct");
      font = NULL;
      goto FAIL;
    }

  hfont = pango_win32_font_cache_load (font_cache, logfont);

  hdc = CreateCompatibleDC (NULL);
  if (hdc == NULL)
    {
      g_warning ("cannot create a memory DC");
      font = NULL;
      goto FAIL;
    }

  SelectObject (hdc, hfont);

  if (!wglUseFontBitmaps (hdc, first, count, list_base))
    {
      g_warning ("cannot create the font display lists");
      font = NULL;
      goto FAIL;
    }

  if (!DeleteDC (hdc))
    g_warning ("cannot delete the memory DC");

  pango_win32_font_cache_unload (font_cache, hfont);

 FAIL:

  if (logfont != NULL)
    g_free (logfont);

  return font;
}

#endif /* GDKGLEXT_MULTIHEAD_SUPPORT */
