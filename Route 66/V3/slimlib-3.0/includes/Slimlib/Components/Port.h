/* ---------------------------------------------------------------------------------------
 * Port.h
 * ---------------------------------------------------------------------------------------
 * This file contains the header of Port abstract class.
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
 * Slimlib/Core/Object.h
 * Slimlib/Core/Dictionary.h
 * Slimlib/Components/ConnectorEnd.h
 * Slimlib/Components/Receiver.h
 * ---------------------------------------------------------------------------------------
 */

#ifndef __Class_Port_
#define __Class_Port_

#include <Slimlib/Core/Object.h>
#include <Slimlib/Core/Dictionary.h>
#include <Slimlib/Components/ConnectorEnd.h>
#include <Slimlib/Components/Receiver.h>

#define PROVIDED_PORT "provided"
#define USED_PORT 		"used"

/**
 * Port is an abstract class that encapsulates a component port.
 * A component port is defined by its name. Port object contains a list of all interfaces
 * hosted by this port for self-description purpose.
 */
class Port: public Object, public Receiver
{
	protected:

		/** The port unique "class" name
		 */
		char *name;

		 /** A slot to plug a connector's end
		  */
		ConnectorEnd *connectorEnd;

		/** List of interface keys of interfaces managed by this port. This is for
		 * self-description purpose.
		 */
		Dictionary<Object *> interfaceKeys;

		/** The port direction.
		 * One of PROVIDED_PORT or USED_PORT
		 */
		char *direction;

		/** Required status of this port.
		 * if <i>required</i> worth true, port is required to be connected.
		 */
		bool required;

	public:
		/** Construct a Port object named <i>name</i>
		 * @param name a string that contains the port name
		 */
		Port(const char *name);

		/** Destruct a Port object
		 * if a connector's end object is attached to this Port object, connector's end
		 * object will also be destroyed.
		 */
		virtual ~Port();


		/** Return the Port name
		 */
		const char *getName();

		/** Register an interface key into this Port object
		 * @param key a string that contains the interface key to register.
		 *	see Interface::getKey() to get more details about interface keys.
		 * @return return true if adding was successfull, false if interface key has already
		 *	been registered in this port.
		 */
		 bool registerInterfaceKey(const char *key);

		/** Plug a connector's end into a Port object. If the connector's end slot is
		 * already taken up by a connector's end, a ConnectorEnd::deconnect() is sent
		 * to that connector's end, then it is destroyed and replaced by <i>connectorEnd</i>.
		 * @param connectorEnd a pointer onto a ConnectorEnd object
		 * @return return true if <i>connectorEnd</i> was sucessfully plugged, false
		 * otherwise.
		 */
		bool plugConnectorEnd(ConnectorEnd *connectorEnd);

		/** Return a pointer onto this port connector end
		 * @return a pointer onto a ConnectorEnd object, or NULL if no ConnectorEnd was
		 *	plugged into this port.
		 */
		ConnectorEnd *getConnectorEnd();

		/** Herited from Receiver abstract class. See Receiver class definition for
		 * more details.
		 */
		virtual void asyncReception(const char *messageTitle, Unmarshaller *data) = 0;

		/** Herited from Receiver abstract class. See Receiver class definition for
		 * more details.
		 */
		virtual Marshalled *syncReception(const char *messageTitle, Unmarshaller *data,
			Marshaller *marshaller) = 0;
};

#endif
