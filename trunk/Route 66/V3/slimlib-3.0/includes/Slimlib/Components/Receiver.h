/* ---------------------------------------------------------------------------------------
 * Receiver.h
 * ---------------------------------------------------------------------------------------
 * This file contains the header of Receiver abstract class.
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
 * Slimlib/Components/Marshaller.h
 * Slimlib/Components/Marshalled.h
 * ---------------------------------------------------------------------------------------
 */

#ifndef __Class_Receiver_
#define __Class_Receiver_

#include <Slimlib/Components/Marshallable.h>
#include <Slimlib/Components/Marshaller.h>
#include <Slimlib/Components/Marshalled.h>


/** Receiver is an abstract class that describe generic methods to recieve messages
 * from any entities.
 */
class Receiver
{
	public:

		/** Perform an asynchronous reception (e.g. receive something WITHOUT any reply.
		 * @param messageLabel (const char*) a title that describe the received message.
		 * @param data (Unmarshaller *) received data and mechanism to unmarshal them.
		 */
		virtual void asyncReception(const char *messageLabel, Unmarshaller *data) = 0;

		/** Perform a synchronous reception (e.g. receive something AND replying something).
		 * @param messageLabel (const char*) a title that describe the received message.
		 * @param data (Unmarshaller *) received data and mechanism to unmarshal them.
		 * @param marshaller (Marshaller *) a pointer onto a Marshaller object to marshall
		 *	data to reply.
		 * @return (Marshalled *) a Marshalled object containing data to send as a reply.
		 */
		virtual Marshalled *syncReception(const char *messageLabel, Unmarshaller *data,
			Marshaller *marshaller) = 0;

		virtual ~Receiver() {}
};

#endif //__Class_Receiver_
