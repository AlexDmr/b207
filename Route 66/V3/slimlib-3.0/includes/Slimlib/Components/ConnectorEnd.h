/* ---------------------------------------------------------------------------------------
 * ConnectorEnd.h
 * ---------------------------------------------------------------------------------------
 * This file contains the header of ConnectorEnd abstract class.
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
 * Slimlib/Components/Marshallable.h
 * Slimlib/Components/Marshaller.h
 * Slimlib/Components/Transmitter.h
 * Slimlib/Components/Receiver.h
 * ---------------------------------------------------------------------------------------
 */

#ifndef __Class_ConnectorEnd_
#define __Class_ConnectorEnd_

#include <Slimlib/Core/Object.h>
#include <Slimlib/Components/Marshallable.h>
#include <Slimlib/Components/Marshaller.h>
#include <Slimlib/Components/Transmitter.h>
#include <Slimlib/Components/Receiver.h>

/** ConnectorEnd is an abstract class to construct any kind of connector's end.
 * Two ConnectorEnd connected together build up a Connector
 */
class ConnectorEnd: public Object, public Transmitter
{
	protected:
		/** Worth true if the connector is connected to something.
		 * Derived class must set this attribute.
		 * Connector constructor initialy set it to false
		 */
		bool connected;

		/** The technology whitch is implemented by this connector.
		 * Basically, this is the name of the derived class
		 */
		char *technology;

		/** A pointer onto a Receiver object.
		 */
		Receiver *receiver;

	public:
		/** Construct a ConnectorEnd object
		 * @param technology a string that contains the connector's technology name. This name
		 *	is only descriptive but should be formatted as an XML namespace.
		 */
		ConnectorEnd(const char *technology);

		/** Destruct a ConnectorEnd object
		 * Derived classes do not need to override this destructor
		 */
		virtual ~ConnectorEnd();

		/** Set the receiver binded with this ConnectorEnd object
		 */
		void setReceiver(Receiver *receiver);

		/** Return true if the connector'end is connected to something, false otherwise
		 * @return a boolean the value of the connected protected attribute (@see connected)
		 */
		bool isConnected();

		/** Setup the connection between this connector and something
		 * MUST be implemented by derived classes
		 * In charge to derived classes to implement some compliance test between connector's
		 * end Compliance is ensured if both connectors support the same set of interfaces,
		 * AND if for each interface kind, one is INTERFACE_USED and the other is
		 * INTERFACE_OFFERED
		 * @return return true if connection succeed, false otherwise
		 */
		virtual bool connect() = 0;

		/** Unconnect this connector
		 * MUST be implemented by derived classes
		 * @return return true if deconnection succeed, false otherwise
		 */
		virtual void disconnect() = 0;

		/**
		 * Return the technology used by this conncetor
		 * @see ConnectorEnd::technology
		 * @return return a pointer onto a string that contains the technology's name
		 */
		 const char *getTechnology();

		/**
		 * return a pointer onto the Receiver object associated with this ConnectorEnd object.
		 * @return return a pointer onto a Receiver object or return NULL, if receiver was not
		 *	setted.
		 */
		Receiver *getReceiver();

		/** Herited from Transmitter abstract class. See Transmitter class definition for
		 * more details.
		 */
		virtual void asyncTransmission(const char *messageLabel, Marshalled *data) = 0;

		/** Herited from Transmitter abstract class. See Transmitter class definition for
		 * more details.
		 */
		virtual Unmarshaller *syncTransmission(const char *messageTitle, Marshalled *data) =0;

		/** Herited from Transmitter abstract class. See Transmitter class definition for
		 * more details.
		 */
		virtual Marshaller *getMarshaller() = 0;
		
		/** A static method to get the id of technology enabled by this connector end class.
		 * This method MUST be override in ALL subclasses.
		 * @return a pointer onto a const char that contains a technology id.	
		 */
		static const char *getStaticTechnologyId();
};

#endif
