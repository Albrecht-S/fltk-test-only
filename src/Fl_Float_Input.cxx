//
// "$Id$"
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <fltk/events.h>
#include <fltk/Widget.h>
#include <fltk/FloatInput.h>
#include <string.h>
#include <stdlib.h>
using namespace fltk;

/*! \class fltk::FloatInput

  A subclass of NumericInput that only allows the user to type
  floating point numbers (sign, digits, decimal point, more digits,
  'E' or 'e', sign, digits), or hex constants that start with
  "0x". This is done by overriding the replace() method. Besides
  editing the text the user can use the up/down arrow keys to change
  the digits.

  You may want a ValueInput widget instead. It has value() methods
  that take and return double values rather than strings, and has
  up/down buttons on it.

  If you change when() to fltk::WHEN_ENTER_KEY the callback is only
  done when the user hits the up/down arrow keys or when the user
  types the Enter key. This may be more useful than the default
  setting of fltk::WHEN_CHANGED which can make the callback happen
  when partially-edited numbers are in the field.

  The type() can either be either FloatInput::FLOAT or
  FloatInput::INT. Setting it to INT makes this act like the IntInput
  subclass.

*/
bool FloatInput::replace(int b, int e, const char* text, int ilen) {
  for (int n = 0; n < ilen; n++) {
    char ascii = text[n];
    compose_reset(); // ignore any foreign letters...
    // This is complex to allow "0xff12" hex to be typed:
    if (b+n==0 && (ascii == '+' || ascii == '-') ||
	(ascii >= '0' && ascii <= '9') ||
	(b+n==1 && (index(0)=='0'||text[0]=='0') && (ascii=='x' || ascii == 'X')) ||
	(b+n>1 && (index(0)=='0'||text[0]=='0') && ((index(1)=='x'||text[1]=='x')||(index(1)=='X'||text[1]=='X'))
	 && (ascii>='A'&& ascii<='F' || ascii>='a'&& ascii<='f')) ||
	type()==FLOAT && ascii && strchr(".eE+-", ascii))
      continue; // it's ok;    
    return false;
  }
  return Input::replace(b,e,text,ilen);
}

/*! \class fltk::IntInput
  NumericInput modified to only allow integers (in decimal, or in hex
  if preceeded by "0x") to by typed. Currently this is implemented by
  FloatInput by checking type() but this may change in the future.  */

/*! \int FloatInput::ivalue() const
  Convert the string to an integer, using int(strtol()) */

/*! Convert the string to a long using strtol() */
long FloatInput::lvalue() const {
  return strtol(value(), 0, 0);
}

/*! Convert the string to a double using strtod() */
double FloatInput::fvalue() const {
  return strtod(value(), 0);
}

//
// End of "$Id$"
//
