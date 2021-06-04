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

#include <Zeni/Net_Primitives.h>

using namespace std;

/*** IPaddress functions ***/

bool operator==(const IPaddress &lhs, const IPaddress &rhs) {
  const IPaddress lhs_l = {SDLNet_Read32(&lhs.host), SDLNet_Read16(&lhs.port)};
  const IPaddress rhs_l = {SDLNet_Read32(&rhs.host), SDLNet_Read16(&rhs.port)};
  return lhs_l.host == rhs_l.host && lhs_l.port == rhs_l.port;
}

bool operator!=(const IPaddress &lhs, const IPaddress &rhs) {
  const IPaddress lhs_l = {SDLNet_Read32(&lhs.host), SDLNet_Read16(&lhs.port)};
  const IPaddress rhs_l = {SDLNet_Read32(&rhs.host), SDLNet_Read16(&rhs.port)};
  return lhs_l.host != rhs_l.host || lhs_l.port != rhs_l.port;
}

bool operator<(const IPaddress &lhs, const IPaddress &rhs) {
  const IPaddress lhs_l = {SDLNet_Read32(&lhs.host), SDLNet_Read16(&lhs.port)};
  const IPaddress rhs_l = {SDLNet_Read32(&rhs.host), SDLNet_Read16(&rhs.port)};
  return lhs_l.host < rhs_l.host || lhs_l.host == rhs_l.host && lhs_l.port <  rhs_l.port;
}

bool operator<=(const IPaddress &lhs, const IPaddress &rhs) {
  const IPaddress lhs_l = {SDLNet_Read32(&lhs.host), SDLNet_Read16(&lhs.port)};
  const IPaddress rhs_l = {SDLNet_Read32(&rhs.host), SDLNet_Read16(&rhs.port)};
  return lhs_l.host < rhs_l.host || lhs_l.host == rhs_l.host && lhs_l.port <= rhs_l.port;
}

bool operator>(const IPaddress &lhs, const IPaddress &rhs) {
  const IPaddress lhs_l = {SDLNet_Read32(&lhs.host), SDLNet_Read16(&lhs.port)};
  const IPaddress rhs_l = {SDLNet_Read32(&rhs.host), SDLNet_Read16(&rhs.port)};
  return lhs_l.host > rhs_l.host || lhs_l.host == rhs_l.host && lhs_l.port >  rhs_l.port;
}

bool operator>=(const IPaddress &lhs, const IPaddress &rhs) {
  const IPaddress lhs_l = {SDLNet_Read32(&lhs.host), SDLNet_Read16(&lhs.port)};
  const IPaddress rhs_l = {SDLNet_Read32(&rhs.host), SDLNet_Read16(&rhs.port)};
  return lhs_l.host > rhs_l.host || lhs_l.host == rhs_l.host && lhs_l.port >= rhs_l.port;
}

namespace Zeni {
  /*** Simple Helper Functions ***/

  string uitoa(const unsigned int &number) {
    char buf[64];
  #ifdef _WINDOWS
    sprintf_s
  #else
    sprintf
  #endif
      (buf, "%u", number);
    return buf;
  }

  string itoa(const int &number) {
    char buf[64];
  #ifdef _WINDOWS
    sprintf_s
  #else
    sprintf
  #endif
      (buf, "%d", number);
    return buf;
  }

  string ultoa(const unsigned long &number) {
    char buf[64];
  #ifdef _WINDOWS
    sprintf_s
  #else
    sprintf
  #endif
      (buf, "%lu", number);
    return buf;
  }

  string ltoa(const long &number) {
    char buf[64];
  #ifdef _WINDOWS
    sprintf_s
  #else
    sprintf
  #endif
      (buf, "%ld", number);
    return buf;
  }

  string ftoa(const float &number) {
    char buf[64];
  #ifdef _WINDOWS
    sprintf_s
  #else
    sprintf
  #endif
      (buf, "%f", number);
    return buf;
  }

  string dtoa(const double &number) {
    char buf[64];
  #ifdef _WINDOWS
    sprintf_s
  #else
    sprintf
  #endif
      (buf, "%f", number);
    return buf;
  }

  string ldtoa(const long double &number) {
    char buf[64];
  #ifdef _WINDOWS
    sprintf_s
  #else
    sprintf
  #endif
      (buf, "%Lf", number);
    return buf;
  }

  string iptoa(const IPaddress &address) {
    Uint32 host = SDLNet_Read32(&address.host);
    Uint16 port = SDLNet_Read16(&address.port);

    return uitoa(host >> 24 & 0xFF) + '.' +
           uitoa(host >> 16 & 0xFF) + '.' +
           uitoa(host >> 8 & 0xFF) + '.' +
           uitoa(host & 0xFF) + ':' + uitoa(port);
  }

  int grab_bytes(std::istream &is, char * const &store, const int &num_bytes) {
    char c;
    int i = 0;
    for(char * store_ptr = store; i < num_bytes; ++i, ++store_ptr)
      if(is.get(c))
        *store_ptr = c;
      else
        break;
    return i;
  }

  int grab_bytes(std::istream &is, std::string &store, const int &num_bytes) {
    store.resize(num_bytes);
    const int rv = grab_bytes(is, const_cast<char * const>(store.c_str()), num_bytes);
    store.resize(rv);
    return rv;
  }
  
  std::ostream & Serializable::serialize(std::ostream &os) const {
    return Zeni::serialize(os, m_size);
  }
  
  std::istream & Serializable::unserialize(std::istream &is) {
    if(!Zeni::unserialize(is, m_size))
      m_size = 0;
    
    return is;
  }
  
  /*** VLUID Class ***/

  char VLUID::compare(const VLUID &rhs) const {
    if(m_size > rhs.m_size)
      return 1;
    else if(rhs.m_size > m_size)
      return -1;
    
    Sint32 index = m_size - 1;
    for(; index > -1; --index) {
      const char &c = m_uid[index];
      const char &rhs_c = rhs.m_uid[index];
      
      if(c > rhs_c)
        return 1;
      else if(rhs_c > c)
        return -1;
    }
    
    return 0;
  }

  VLUID & VLUID::operator++() { //prefix
    Uint16 index = 0;
    for(; index < m_size; ++index)
      if(++m_uid[index])
        break;
    
    if(index == m_size)
      if(++m_size)
        m_uid += char(1);
      else
        m_uid.clear();

    return *this;
  }

  VLUID VLUID::operator++(int) { //postfix
    VLUID prev = *this;
    ++*this;
    return prev;
  }
  
  std::ostream & VLUID::serialize(std::ostream &os) const {
    Serializable::serialize(os);
    
    return os.write(m_uid.c_str(), m_size);
  }
  
  std::istream & VLUID::unserialize(std::istream &is) {
    Serializable::unserialize(is);
    
    if(is) {
      m_uid.resize(m_size);
      
      if(!is.read(const_cast<char * const>(m_uid.c_str()), m_size))
        m_uid.clear();
    }
    else
      m_uid.clear();
    
    return is;
  }

  /*** Stand-Alone serialization/unserialization functions ***/
  
  std::ostream & serialize(std::ostream &os, const Sint32 &value) {
    char buf[sizeof(Sint32)];
    
    SDLNet_Write32(value, buf);
    
    return os.write(buf, sizeof(Sint32));
  }
  
  std::ostream & serialize(std::ostream &os, const Uint32 &value) {
    char buf[sizeof(Uint32)];
    
    SDLNet_Write32(reinterpret_cast<const Sint32 &>(value), buf);
    
    return os.write(buf, sizeof(Uint32));
  }
  
  std::ostream & serialize(std::ostream &os, const Sint16 &value) {
    char buf[sizeof(Sint16)];
    
    SDLNet_Write16(value, buf);
    
    return os.write(buf, sizeof(Sint16));
  }
  
  std::ostream & serialize(std::ostream &os, const Uint16 &value) {
    char buf[sizeof(Uint16)];
    
    SDLNet_Write16(reinterpret_cast<const Sint16 &>(value), buf);
    
    return os.write(buf, sizeof(Uint16));
  }
  
  std::ostream & serialize(std::ostream &os, const Sint8 &value) {
    return os.write(reinterpret_cast<const char *>(&value), 1);
  }
  
  std::ostream & serialize(std::ostream &os, const char &value) {
    return os.write(&value, 1);
  }
  
  std::ostream & serialize(std::ostream &os, const unsigned char &value) {
    return os.write(reinterpret_cast<const char *>(&value), 1);
  }
  
  std::ostream & serialize(std::ostream &os, const float &value) {
    return os.write(reinterpret_cast<const char * const>(&value), sizeof(float));
  }
  
  std::ostream & serialize(std::ostream &os, const double &value) {
    return os.write(reinterpret_cast<const char * const>(&value), sizeof(double));
  }
  
  std::ostream & serialize(std::ostream &os, const bool &value) {
    const char c(value ? 1 : 0);
    return os.put(c);
  }
  
  std::ostream & serialize(std::ostream &os, const Point2i &value) {
    return serialize(serialize(os, value.x), value.y);
  }
  
  std::ostream & serialize(std::ostream &os, const Point2f &value) {
    return serialize(serialize(os, value.x), value.y);
  }
  
  std::ostream & serialize(std::ostream &os, const Point3i &value) {
    return serialize(serialize(serialize(os, value.x), value.y), value.z);
  }
  
  std::ostream & serialize(std::ostream &os, const Point3f &value) {
    return serialize(serialize(serialize(os, value.x), value.y), value.z);
  }
  
  std::ostream & serialize(std::ostream &os, const Vector3f &value) {
    return serialize(serialize(serialize(os, value.i), value.j), value.k);
  }
  
  std::ostream & serialize(std::ostream &os, const Quaternion &value) {
    return serialize(serialize(os, value.time), value.space);
  }

  std::ostream & serialize(std::ostream &os, const IPaddress &address) {
    const char *buf = reinterpret_cast<const char *>(&address);
    
    return os.write(buf, sizeof(IPaddress));
  }
  
  std::istream & unserialize(std::istream &is, Sint32 &value) {
    char buf[sizeof(Sint32)];
    
    if(is.read(buf, sizeof(Sint32)))
      value = SDLNet_Read32(buf);
    
    return is;
  }
  
  std::istream & unserialize(std::istream &is, Uint32 &value) {
    char buf[sizeof(Uint32)];
    
    if(is.read(buf, sizeof(Uint32))) {
      const Sint32 s_value = SDLNet_Read32(buf);
      value = reinterpret_cast<const Uint32 &>(s_value);
    }
    
    return is;
  }
  
  std::istream & unserialize(std::istream &is, Sint16 &value) {
    char buf[sizeof(Sint16)];
    
    if(is.read(buf, sizeof(Sint16)))
      value = SDLNet_Read16(buf);
    
    return is;
  }
  
  std::istream & unserialize(std::istream &is, Uint16 &value) {
    char buf[sizeof(Uint16)];
    
    if(is.read(buf, sizeof(Uint16))) {
      const Sint16 s_value = SDLNet_Read16(buf);
      value = reinterpret_cast<const Uint16 &>(s_value);
    }
    
    return is;
  }
  
  std::istream & unserialize(std::istream &is, Sint8 &value) {
    is.read(reinterpret_cast<char *>(&value), 1);
    return is;
  }
  
  std::istream & unserialize(std::istream &is, char &value) {
    is.read(&value, 1);
    return is;
  }
  
  std::istream & unserialize(std::istream &is, unsigned char &value) {
    is.read(reinterpret_cast<char *>(&value), 1);
    return is;
  }
  
  std::istream & unserialize(std::istream &is, float &value) {
    return is.read(reinterpret_cast<char * const>(&value), sizeof(float));
  }
  
  std::istream & unserialize(std::istream &is, double &value) {
    return is.read(reinterpret_cast<char * const>(&value), sizeof(double));
  }
  
  std::istream & unserialize(std::istream &is, bool &value) {
    char c;
    
    if(is.get(c))
      value = c ? true : false;
    
    return is;
  }
  
  std::istream & unserialize(std::istream &is, Point2i &value) {
    return unserialize(unserialize(is, value.x), value.y);
  }
  
  std::istream & unserialize(std::istream &is, Point2f &value) {
    return unserialize(unserialize(is, value.x), value.y);
  }
  
  std::istream & unserialize(std::istream &is, Point3i &value) {
    return unserialize(unserialize(unserialize(is, value.x), value.y), value.z);
  }
  
  std::istream & unserialize(std::istream &is, Point3f &value) {
    return unserialize(unserialize(unserialize(is, value.x), value.y), value.z);
  }
  
  std::istream & unserialize(std::istream &is, Vector3f &value) {
    return unserialize(unserialize(unserialize(is, value.i), value.j), value.k);
  }
  
  std::istream & unserialize(std::istream &is, Quaternion &value) {
    return unserialize(unserialize(is, value.time), value.space);
  }

  std::istream & unserialize(std::istream &is, IPaddress &address) {
    char buf[sizeof(IPaddress)];
    
    if(is.read(buf, sizeof(IPaddress)))
      memcpy(&address, buf, sizeof(IPaddress));
    
    return is;
  }
}
