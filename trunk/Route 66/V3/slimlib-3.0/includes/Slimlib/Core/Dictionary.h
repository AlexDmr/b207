#ifndef __Class_Dictionary_
#define __Class_Dictionary_

#include <Slimlib/Core/VectorClass.h>
#include <Slimlib/Core/StringClass.h>

/**
 * DictionaryItem encapsulate a pair <key, value> where the key is a string and value on object of class T.
 * T must be a pointer onto a class
 */
template <class T> class DictionaryItem: public Object
{
    private:
		/**
		 * The key of the item
		 */
		char *key;

		/**
		 * The item associated with the key. T must be a pointer onto a class
		 */
		T value;

	public:
		/**
		 * Construct a DictionaryItem object
		 * @param key a string to use as a key for value <i>value</i>
		 * @param value an object of class T. <b>T must be a pointer onto a class</b>
		 */
		DictionaryItem(const char *key, T value)
		{
			this->key = NULL;
			this->value = NULL;
			setKey(key);
			setValue(value);
		}

		/**
		 * Destruct a DictionaryItem object
		 */
		virtual ~DictionaryItem()
		{
			if(this->key != NULL) free(this->key);
			delete(this->value);
		}

		/**
		 * Set the key of the item
		 * @param key a string to use as a key associated with the item
		 */
		void setKey(const char *key)
		{
			if(this->key != NULL) free(this->key);
			this->key = NULL;
			if(key != NULL) this->key = String::duplicate(key);
		}

		/**
		 * Set the item value
		 * @param value an object of class T. <b>T must be a pointer onto a class</b>
		 */
		void setValue(T value)
		{
			delete(this->value);
			this->value = value;
		}

		/**
		 * Compare <i>anotherKey</i> with object key
		 * @return Return true if keys are the same, false otherwise
		 */
		bool compareKey(const char *anotherKey)
		{
			return strcmp(this->key, anotherKey) == 0;
		}

		/**
		 * Return the item key
		 * @return return a pointer onto string that contains item's key
		 */
		const char *getKey(void)
		{
			return this->key;
		}

		/**
		 * Return the item value
		 * @return return a pointer onto an object of class T
		 */
		T getValue(void)
		{
			return this->value;
		}
};


/**
 * Dictionary is a kind Vector where every items are associated to a unique key.
 * Keys are user-defined string. Dictionary ensure keys uniqueness by rejecting a new entry
 * if the key provided is already in use in this Dictionary object
 */
template <class T> class Dictionary: public Object
{
	private:
		/**
		 * items and their associated keys
		 */
		Vector<DictionaryItem<T> *> items;

	public:
		/**
		 * Construct a dictionary object
		 */
		Dictionary(void)
		{
		}


		/**
		 * Destruct a dictionary object, and all hosted items
		 */
		virtual ~Dictionary(void)
		{
			DictionaryItem<T> *item;
			while((item = items.removeFirstItem()) != NULL)
			{
				delete(item);
			}
		}


		/**
		 * Returns the value to which the key is mapped in this dictionary.
		 * @param key a string that is the key to associated with the value to get
		 * @return a pointer ont an object of class T if successfull, NULL otherwise
		 */
		T get(const char *key)
		{
			if(key != NULL)
			{
				DictionaryItem<T> *item = items.getFirstItem();
				while(item != NULL)
				{
					if(item->compareKey(key))
					{
						return item->getValue();
					}

					item = items.getNextItem();
				}
			}

			return NULL;
		}

		/**
		 * Tests if this dictionary is empty contains some values.
		 * @return true if Dictionary object is empty, false otherwise
		 */
  		bool isEmpty()
  		{
				return items.size() == 0;
			}

  		/**
		 * Add the value <i>value</i> in Dictionary objet, associated with the key <i>key</i>.
		 * @param key a string that is the key to associated with <i>value</i>
		 * @param value an object of class T. <b>T must be a pointer onto a class</b>
		 * @return true if successfull, false is key already exists in Dictionary object.
		 */
  		bool put(const char *key, T value)
  		{
				DictionaryItem<T> *item = items.getFirstItem();
				while(item != NULL)
				{
					if(item->compareKey(key))
					{
						return false;
					}

					item = items.getNextItem();
				}

				item = new DictionaryItem<T>(key, value);
				items.push(item);

				return true;
			}

		/**
		 * Removes the key (and its associated value) from this dictionary.
		 * @param key a string that identify the item to remove
		 * @return the removed item
		 */
  		void remove(const char *key)
  		{
				DictionaryItem<T> *item = items.getFirstItem();
				while(item != NULL)
				{
					if(item->compareKey(key))
					{
						items.remove();
						delete(item);
					}

					item = items.getNextItem();
				}
			}

		/**
		 * Return the first key that exist in this Dictionary object
		 * @see getNextKey()
		 * @return return a pointer onto a string that contains a key, or NULL if dictionary is empty
		 */
		const char *getFirstKey()
		{
			DictionaryItem<T> *item = items.getFirstItem();
			if(item != NULL)
			{
				return item->getKey();
			}

			return NULL;
		}


		/**
		 * Return the next key that exist in this Dictionary object
		 * @see getFirstKey()
		 * @return return a pointer onto a string that contains a key, or NULL if end of dictionary is reached
		 */
		const char *getNextKey()
		{
			DictionaryItem<T> *item = items.getNextItem();
			if(item != NULL)
			{
				return item->getKey();
			}

			return NULL;
		}


		/**
		 * Return the first value that exist in this Dictionary object
		 * @see getNextValue()
		 * @return return a pointer onto T object, or NULL if dictionary is empty
		 */
		T getFirstValue()
		{
			DictionaryItem<T> *item = items.getFirstItem();
			if(item != NULL)
			{
				return item->getValue();
			}

			return NULL;
		}


		/**
		 * Return the next key that exist in this Dictionary object
		 * @see getFirstValue()
		 * @return return a pointer onto a string that contains a key, or NULL if end of dictionary is reached
		 */
		T getNextValue()
		{
			DictionaryItem<T> *item = items.getNextItem();
			if(item != NULL)
			{
				return item->getValue();
			}

			return NULL;
		}


		/**
		 * Returns the number of entries in this dictionary.
		 * @return return the number of value stored in Dictionary object
		 */
		long size()
		{
			return items.size();
		}
};

#endif
