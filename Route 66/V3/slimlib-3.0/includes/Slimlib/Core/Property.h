#ifndef __Class_Property_
#define __Class_Property_

#include <Slimlib/Core/Object.h>

class Property: public Object
{
	private:
		char *key;
		char *value;

	protected:

	public:
		Property(void);
		Property(const char *key, const char *value);
		virtual ~Property(void);

		const char *getKey(void);
		const char *getValue(void);

		void setKey(const char *key);
		void setValue(const char *value);

};

#endif

