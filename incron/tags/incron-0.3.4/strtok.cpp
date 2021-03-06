
/// string tokenizer implementation
/**
 * \file strtok.cpp
 * 
 * string tokenizer
 * 
 * Copyright (C) 2006 Lukas Jelinek, <lukas@aiken.cz>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of one of the following licenses:
 *
 * \li 1. X11-style license (see LICENSE-X11)
 * \li 2. GNU Lesser General Public License, version 2.1 (see LICENSE-LGPL)
 * \li 3. GNU General Public License, version 2  (see LICENSE-GPL)
 *
 * If you want to help with choosing the best license for you,
 * please visit http://www.gnu.org/licenses/license-list.html.
 * 
 */


#include <sstream>

#include "strtok.h"

StringTokenizer::StringTokenizer(const std::string& rStr, char cDelim, char cPrefix)
{
  m_str = rStr;
  m_cDelim = cDelim;
  m_cPrefix = cPrefix;
  m_pos = 0;
  m_len = rStr.length();
}
    
std::string StringTokenizer::GetNextToken(bool fSkipEmpty)
{
  std::string s;
  
  do {
    _GetNextToken(s, true);
  } while (fSkipEmpty && s.empty() && m_pos < m_len);
  
  return s;
}

std::string StringTokenizer::GetNextTokenRaw(bool fSkipEmpty)
{
  std::string s;
  
  do {
    _GetNextToken(s, false);
  } while (fSkipEmpty && s.empty() && m_pos < m_len);
  
  return s;
}

std::string StringTokenizer::GetRemainder()
{
  return  m_cPrefix == '\0'
      ?   m_str.substr(m_pos)
      :   StripPrefix(m_str.c_str() + m_pos, m_len - m_pos);
}

std::string StringTokenizer::StripPrefix(const char* s, SIZE cnt)
{
  std::ostringstream stream;
  SIZE pos = 0;
  while (pos < cnt) {
    if (s[pos] == m_cPrefix) {
      if ((pos < cnt - 1) && s[pos+1] == m_cPrefix) {
        stream << m_cPrefix;
        pos++;
      }
    }
    else {
      stream << s[pos];
    }
    
    pos++;
  }
  
  return stream.str();
}

void StringTokenizer::_GetNextToken(std::string& rToken, bool fStripPrefix)
{
  if (m_cPrefix == '\0') {
    _GetNextTokenNoPrefix(rToken);
  }
  else {
    _GetNextTokenWithPrefix(rToken);
    if (fStripPrefix)
      rToken = StripPrefix(rToken.c_str(), rToken.length());
  }
}

void StringTokenizer::_GetNextTokenNoPrefix(std::string& rToken)
{
  for (SIZE i=m_pos; i<m_len; i++) {
    if (m_str[i] == m_cDelim) {
      rToken = m_str.substr(m_pos, i - m_pos);
      m_pos = i + 1;
      return;
    }    
  }
  
  rToken = m_str.substr(m_pos);
  m_pos = m_len;
}
  
void StringTokenizer::_GetNextTokenWithPrefix(std::string& rToken)
{
  int pref = 0;
  for (SIZE i=m_pos; i<m_len; i++) {
    if (m_str[i] == m_cDelim) {
      if (pref == 0) {
        rToken = m_str.substr(m_pos, i - m_pos);
        m_pos = i + 1;
        return;
      }
      else {
        pref = 0;
      }
    }
    else if (m_str[i] == m_cPrefix) {
      if (pref == 1)
        pref = 0;
      else
        pref = 1;
    }
    else {
      pref = 0;
    }
  }
  
  rToken = m_str.substr(m_pos);
  m_pos = m_len;
}

