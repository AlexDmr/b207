/* ---------------------------------------------------------------------------------------
 * Marshaller.h
 * ---------------------------------------------------------------------------------------
 * This file contains the abstract class Marshaller.
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

#ifndef __Class_Marshaller_
#define __Class_Marshaller_

#include <Slimlib/Components/Marshalled.h>

class Marshaller
{
	public:
		virtual void marshal(bool value, const char *name) = 0;
		virtual void marshal(long value, const char *name) = 0;
		virtual void marshal(double value, const char *name) = 0;
		virtual void marshal(const char *value, const char *name) = 0;
		virtual void beginElement(const char *elementType, const char *name) = 0;
		virtual void endElement() = 0;
		virtual Marshalled *getMarshalledData() = 0;
		virtual ~Marshaller() {}
};

#endif // __Class_Marshaller_
