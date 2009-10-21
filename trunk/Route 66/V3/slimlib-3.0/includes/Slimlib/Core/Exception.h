#ifndef __Class_Exception_
#define __Class_Exception_

#include <Slimlib/Core/Object.h>

/**
 * @brief Exception is a base class to derivate classes that will represent exceptions.
 */
class Exception: public Object
{
	char *name;
	int code;
	
	public:
		/**
		 *  Construct a simple exception object
		 */
		Exception(void);
		
		/**
		 *  Construct a exception object. <i>name</i> specifies the exception's name and <i>code</i> a numerical code/
		 * @param name - a null-terminated string
		 * @param code - a integer
		 */
		Exception(const char *name, int code);
		
		/**
		 * Destroy a exception object
		 */
		virtual ~Exception(void);

		/**
		 * return a pointer on the exception's name
		 * @return a pointer on a string that contains the exception's name
		 */
		const char *getName(void); 
		
		/**
		 * return the exception's numerical code
		 * @return an integer that is the exception's numerical code
		 */
		int getCode(void); 
};

/*
 *  General exceptions
 */


//class OutOfMemoryException: public Exception { public: OutOfMemoryException(void); };
//class OutOfMemoryException: public Exception { public: OutOfMemoryException(void): Exception("Out Of Memory", 1) {}};

#endif
