//
// "$Id: Fl_grab.cxx,v 1.5 2000/02/16 07:30:05 bill Exp $"
//
// Grab/release code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <config.h>
#include <FL/Fl.H>
#include <FL/x.H>

////////////////////////////////////////////////////////////////
// "Grab" is done while menu systems are up.  This has several effects:
// Events are all sent to the "grab window", which does not even
// have to be displayed (and in the case of Fl_Menu.C it isn't).
// The system is also told to "grab" events and send them to this app.
// This also modifies how Fl_Window::show() works, on X it turns on
// override_redirect, it does similar things on WIN32.

extern void fl_fix_focus(int); // in Fl.cxx

void Fl::grab(Fl_Window* w) {
  if (w) {
    if (!grab_) {
#ifdef WIN32
      HWND w = fl_xid(first_window());
      SetActiveWindow(w); // is this necessary?
      SetCapture(w);
#else
      XGrabPointer(fl_display,
		   fl_xid(first_window()),
		   1,
		   ButtonPressMask|ButtonReleaseMask|
		   ButtonMotionMask|PointerMotionMask,
		   GrabModeAsync,
		   GrabModeAsync, 
		   None,
		   0,
		   fl_event_time);
      XGrabKeyboard(fl_display,
		    fl_xid(first_window()),
		    1,
		    GrabModeAsync,
		    GrabModeAsync, 
		    fl_event_time);
#endif
    }
    if (pushed_) pushed_ = w;
    grab_ = w;
  } else {
    if (grab_) {
#ifdef WIN32
      ReleaseCapture();
#else
      XUngrabKeyboard(fl_display, fl_event_time);
      XUngrabPointer(fl_display, fl_event_time);
      // this flush is done in case the picked menu item goes into
      // an infinite loop, so we don't leave the X server locked up:
      XFlush(fl_display);
#endif
      grab_ = 0;
      pushed_ = 0;
      fl_fix_focus(1);
    }
  }
}

//
// End of "$Id: Fl_grab.cxx,v 1.5 2000/02/16 07:30:05 bill Exp $".
//
