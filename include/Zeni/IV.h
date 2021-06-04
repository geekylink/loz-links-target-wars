/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
*
* The Zenipex Library is free software; you can redistribute it and/or 
* modify it under the terms of the GNU General Public License as 
* published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
*
* The Zenipex Library is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with the Zenipex Library; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 
* 02110-1301 USA.
*
* As a special exception, you may use this file as part of a free software
* library without restriction.  Specifically, if other files instantiate
* templates or use macros or inline functions from this file, or you compile
* this file and link it with other files to produce an executable, this
* file does not by itself cause the resulting executable to be covered by
* the GNU General Public License.  This exception does not however
* invalidate any other reasons why the executable file might be covered by
* the GNU General Public License.
*/

#ifndef IV_H
#define IV_H

/**
* \class Zeni::Line<Base_Class, BASE_VTYPE>
*
* \brief Inline Virtual base class
*
* This template offers the storage for the virtual type specifier.
*
* Additionally, it offers a getter for the type.
*
* \note This is an advanced technique with only a few valid uses.
*
* \author bazald
*
* Contact: bazald@zenipex.com
*/

template <class Base_Class,
          typename BASE_VTYPE>
class IV {
  BASE_VTYPE m_vtype;

protected:
  typedef BASE_VTYPE VTYPE;

  IV(const VTYPE &vtype_) : m_vtype(vtype_) {}
  ~IV() {}

public:
  VTYPE vtype() const {return m_vtype;}
};

#endif
