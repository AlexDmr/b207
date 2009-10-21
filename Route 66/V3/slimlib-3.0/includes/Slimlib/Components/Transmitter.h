/* ---------------------------------------------------------------------------------------
 * Transmitter.h
 * ---------------------------------------------------------------------------------------
 * This file contains the header of Transmitter abstract class.
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
 * Slimlib/Components/Marshallable.h
 * Slimlib/Components/Unmarshaller.h
 * ---------------------------------------------------------------------------------------
 */

#ifndef __Class_Transmitter_
#define __Class_Transmitter_

#include <Slimlib/Components/Marshalled.h>
#include <Slimlib/Components/Unmarshaller.h>
#include <Slimlib/Components/Marshaller.h>


/** Transmitter is an abstract class that describe generic methods to transmit messages
 * between any entities.
 */
class Transmitter
{
	public:

		/** Perform an asynchronous transmission (e.g. send something WITHOUT waiting for any
		 * response).
		 * @param messageTitle (const char*) a title that describe the message to transmit.
		 * @param data (Marshalled *) data to transmit.
		 */
		virtual void asyncTransmission(const char *messageLabel, Marshalled *data) = 0;

		/** Perform a synchronous transmission (e.g. send something AND waiting for a
		 * response).
		 * @param messageTitle (const char*) a title that describe the message to transmit.
		 * @param data (Marshalled *) data to transmit.
		 * @return (Unmarshaller *) an Unmarshaller object that contains received data and
		 *	mechanism to unmarshall them.
		 */
		virtual Unmarshaller *syncTransmission(const char *messageLabel, Marshalled *data) =0;

		virtual Marshaller *getMarshaller() = 0;

		virtual ~Transmitter() {}
};

#endif //__Class_Transmitter_
