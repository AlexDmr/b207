#ifndef __Class_Component_
#define __Class_Component_

#include <Slimlib/Core/Object.h>
#include <Slimlib/Core/VectorClass.h>
#include <Slimlib/Components/Port.h>

/** Component is a base class to encapsulate component-oriented logic.
 * A component is defined by its name. Component object contains a list of all ports
 * provided by this component.
 */
class Component: public Object
{
	private:
		/**
		 * Attribute used while going throught ports by getFirstPortWith() and
getNextPortWith().
		 */
		Port *port;

	protected:

		/** The component name
		 */
		 char *name;

		/** Dictionary of ports of this Component.
		 */
		Dictionary<Port *> ports;

	public:
		/** Construct a Component object named <i>name</i>
		 * @param name a string that contains the component name
		 */
		Component(const char *name);

		/** Destroy a Component object. All attached Port objects are also destroyed.
		 */
		virtual ~Component();

		/** Return the component name
		 */
		const char *getName();

		/** Add an Port object into this Component object
		 * @param port a pointer onto the Port object to add.
		 */
		void addPort(Port *port);

		/** Return a pointer onto a Port object that match <i>portName</i>
		 * @param portName a string that contains the searched Port object
		 * @return a pointer onto a Port object if succeed, NULL if no interface matched
		 */
		Port *getPort(const char *portName);

		/** Plug the given ConnectorEnd object into the component's port identified by
		 * <i>portName</i>
		 * @param portName a string that contains a port's name
		 * @param connectorEnd a pointer onto the ConnectorEnd object to plug in
		 * @return return <b>true</b> if the ConnectorEnd object was succefully plugged in, or
		 *	<b>false</b> otherwise.
		 */
		bool plugConnectorEndInPort(const char *portName, ConnectorEnd *connectorEnd);
		
		/** A static method to get the name of component instances from this class.
		 * This method MUST be override in ALL subclasses.
		 * @return a pointer onto a const char that contains the component class name.	
		 */
		static const char *getStaticName();
};

#endif
