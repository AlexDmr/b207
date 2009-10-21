/* ---------------------------------------------------------------------------------------
 * Unmarshaller.h
 * ---------------------------------------------------------------------------------------
 * This file contains the abstract class Unmarshaller.
 * ---------------------------------------------------------------------------------------
 * Copyright (C) 2007 Lionel Balme
 * http://iihm.imag.fr/balme/projects/slimlib/
 * ---------------------------------------------------------------------------------------
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * ---------------------------------------------------------------------------------------
 * The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT", "SHOULD", "SHOULD
 * NOT", "RECOMMENDED",  "MAY", and "OPTIONAL" in this document are to be interpreted as
 * described in RFC 2119 (http://www.ietf.org/rfc/rfc2119.txt).
 * ---------------------------------------------------------------------------------------
 * Version: 1.0
 * ---------------------------------------------------------------------------------------
 * Dependency list
 *
 * Slimlib/Components/Marshalled.h
 * ---------------------------------------------------------------------------------------
 */

#ifndef __Class_Unmarshaller_
#define __Class_Unmarshaller_

#include <Slimlib/Components/Marshalled.h>

class Unmarshaller
{
	public:
		virtual bool unmarshalBoolean(const char *name) = 0;
		virtual long unmarshalInteger(const char *name) = 0;
		virtual double unmarshalDecimal(const char *name) = 0;
		virtual char *unmarshalString(const char *name) = 0;
		virtual void enterElement(const char *name) = 0;
		virtual ~Unmarshaller() {}
};

#endif // __Class_Unmarshaller_
