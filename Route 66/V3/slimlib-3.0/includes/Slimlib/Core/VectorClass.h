#ifndef __Class_Vector_
#define __Class_Vector_

#include <Slimlib/Errors.h>
#include <Slimlib/Core/Object.h>
#include <stdlib.h>

/*
class OutOfMemoryException: public Exception { public: OutOfMemoryException(void): Exception("Out Of Memory", 1) {}};
class EndOfVectorException: public Exception { public: EndOfVectorException(void): Exception("End Of Vector", 2) {}};
class VectorEmptyException: public Exception { public: VectorEmptyException(void): Exception("Vector Empty", 3) {}};
class OutOfVectorException: public Exception { public: OutOfVectorException(void): Exception("Out Of Vector", 4) {}};
*/


template <class T> class VectorItem: public Object
{
    private:
        T content;
        VectorItem<T> *prev;
        VectorItem<T> *next;

    public:

        VectorItem(VectorItem<T> *prev, VectorItem<T> *next, T content);

		virtual ~VectorItem(void);

		T getContent(void) const;

		void setContent(T content);

		void setNext(VectorItem<T> *next);

        void setPrev(VectorItem<T> *prev);

        VectorItem<T> *getNext(void) const;

        VectorItem<T> *getPrev(void) const;
};



/**
* @brief Vector is a template class to manage a vector of pointer onto any kind of elements. */

template <class T> class Vector: public Object
{
	private:
		int	err;
		//bool isPointer;
        long nbItems;
    	VectorItem<T> *firstItem;
    	VectorItem<T> *lastItem;
    	VectorItem<T> *readingCursor;
    	
    	void g_push(VectorItem<T> *item);

		void g_insert(VectorItem<T> *item);

		VectorItem<T> *g_pop(void);

		T g_remove(VectorItem<T> *item);
        
	public:

		/**
		 * Construct a vector with no item in it. Vector is pre-configured to host a collection of pointers onto any kind of elements
		 */
		Vector(void);
		
		
		/**
		 *	Configure vector to host item that are NOT pointers.
		 */
		//void hostingNonPointerItems(void);
		
		
		/**
		 * Destroy the vector and all the items stored in it.
		 */
		virtual ~Vector(void);
		

		/**
		 * Get the number of items stored in vector
		 * @return a number of items
		 */
        const long size(void) const;

		
    	/**
		 * Add the object referenced by o at the end of vector.
	 	 * @param o object (if o is a simple type) or pointer (if o is a instance of a class or malloced structure) to store in vector
	 	 * @throw new OutOfMemory exception
		 */
        void push(T o);
		
		
		/**
		 * Insert the object o before the item at reading cursor position.
	 	 * @param o object (if o is a simple type) or pointer (if o is a instance of a class or malloced structure) to store in vector
	 	 * @throw new OutOfMemory exception
		 */
        void insert(T o);


        /**
		 * Get a reference on the last object previously added in vector
		 * @return last object added in vector
		 * @throw new VectorEmpty exception
		 */
        T pop(void);
		

		/**
		 * Remove from vector the object at position x and return it. The object destruction is in charge of the caller.
		 * @param x the position of the object to remove from vector
		 */
        T remove(unsigned int x);


        /**
		 * Retrieve a reference onto item stored at position x. vector[0] retrieve a reference onto first item.
		 * @param x position of the item to get.
		 * @return an pointer onto an instance of class T (if T is a class), a pointer onto a structure (if T is a structure), or an object (if T is a simple type)
		 * @throw new OutOfVector exception
		 */
		T operator[](unsigned int x);


		/**
		* Retrieve a reference onto last item in the vector. This function must not be call if vector is empty.
		* @return an instance of class T
		* @throw new VectorEmpty exception
		*/
		T getLastItem(void);


		/**
		* Getting a reference onto first item in vector, and set a reading cursor on it to begin a sequential reading of the vector.
		* You can get next items step by step by calling getNextItem() or remove and destroy current item by calling removeItemAtReadingCursor().
		* @return the first item of vector
		* @throw new EndOfVector exception
		* @see getNextItem()
		* @see removeItemAtReadingCursor()
		*/
		T getFirstItem(void);


        /**
		 * Make internal reading cursor going one item forward, and newly current item.
		 * @return next item in vector.
		 * @throw new EndOfVector exception
		 */
        T getNextItem(void);
        

		/**
		 * Replace the item at reading position by <i>item</i>
		 * @throw new OutOfVector exception
		 */
        void replace(T item);
        

        /**
		 * Remove the item at reading position. This reading position is set when vector is traversed by mean of getFirstItem() and getNextItem() functions.
		 * If internal reading cursor is NULL, removeItemAtReadingCursor() will remove and destroy the last item. After removing, internal reading cursor is set on the previous item.
		 * As a consequence, the next call to getNextItem() will get a pointer on the "ex-"following item of the removed one. But the next call to removeItemAtReadingCursor() will remove
		 * the "ex-"previous item of the removed one. If vector is empty, removeItemAtReadingCursor() will return without doing anything.
		 * @see getFirstItem()
		 * @see getNextItem()
		 */
        T remove(void);
        
        
        /**
		 * Remove the item at first position in this vector.
		 * @return return a pointer onto the removed object of class T, or NULL if vector was empty when calling removeFirstItem()
		 */
	    T removeFirstItem(void);

		/**
		 * Test if the reading cursor is after the last item of the vector
		 * @return true if the reading cursor is after the last item, false else.
		 */
        bool isEndOfVector(void);
		
		
		/**
		 * Add item's of vector <i>vector</i> at the end of this vector
		 */
		void addVector(Vector<T> *vector);

		/**
		 *	Return the error code of the last occured error
		 */
		int getLastError(void);
};


/*
 *  Vectors.cpp
 *
 *
 *  Created by Lionel Balme on 8/02/06.
 *  Copyright 2006 CLIPS-IMAG. All rights reserved.
 *
 */

template <class T>
VectorItem<T>::VectorItem(VectorItem<T> *prev, VectorItem<T> *next, T content)
{
	this->content = content;
	this->prev = prev;
	this->next = next;
}

template <class T>
VectorItem<T>::~VectorItem(void)
{
}

template <class T>
T VectorItem<T>::getContent(void) const
{
   return content;
}

template <class T>
void VectorItem<T>::setContent(T content)
{
	this->content = content;
}

template <class T>
void VectorItem<T>::setNext(VectorItem<T> *next)
{
   this->next = next;
}

template <class T>
void VectorItem<T>::setPrev(VectorItem<T> *prev)
{
   this->prev = prev;
}

template <class T>
VectorItem<T> *VectorItem<T>::getNext(void) const
{
	return next;
}

template <class T>
VectorItem<T> *VectorItem<T>::getPrev(void) const
{
   return prev;
}





template <class T>
void Vector<T>::g_push(VectorItem<T> *item)
{
	if(nbItems == 0)
	{
		firstItem = item;
		readingCursor = firstItem;
	}
	else
	{
		lastItem->setNext(item);
	}

	lastItem = item;
	nbItems++;
}


template <class T>
void Vector<T>::g_insert(VectorItem<T> *item)
{
	if(readingCursor != NULL)
	{
		item->setNext(readingCursor);
		item->setPrev(readingCursor->getPrev());
		if(readingCursor == firstItem)
		{
			firstItem = item;
		}
		readingCursor = item;
		nbItems++;
	}
}


template <class T>
VectorItem<T> *Vector<T>::g_pop(void)
{
	VectorItem<T> *item;

	if(lastItem == NULL)
	{
		return NULL;
	}

	item = lastItem->getPrev();

	if(item == NULL)
	{
		firstItem = NULL;
		readingCursor = NULL;
	}
	else
	{
		item->setNext(NULL);

		if(readingCursor == lastItem)
		{
			readingCursor = item;
		}
	}

	nbItems--;
	item = lastItem;
	lastItem = item->getPrev();

	return item;
}


template <class T>
T Vector<T>::g_remove(VectorItem<T> *item)
{
	VectorItem<T> *prev;
	VectorItem<T> *next;

	prev = item->getPrev();
	next = item->getNext();

	if(readingCursor == item)
	{
		readingCursor = prev;
	}

	if(prev != NULL)
	{
		prev->setNext(next);
	}
	else
	{
		firstItem = next;
	}

	if(next != NULL)
	{
		next->setPrev(prev);
	}
	else
	{
		lastItem = prev;
	}

	T t = item->getContent();
	
	delete(item);
	nbItems--;
	
	return t;
}


template <class T>
Vector<T>::Vector()
{
	//this->isPointer = true;
	firstItem = NULL;
	lastItem = NULL;
	readingCursor = NULL;
	nbItems = 0;
	err = NO_ERROR;
}

/*
template <class T>
void Vector<T>::hostingNonPointerItems(void)
{
	//this->isPointer = false;
}
*/

template <class T>
Vector<T>::~Vector()
{
}


template <class T>
const long Vector<T>::size(void) const
{
	return nbItems;
}


template <class T>
void Vector<T>::push(T o)
{
	err = 0;
	VectorItem<T> *item = new VectorItem<T>(lastItem, NULL, o);
	if(item == NULL)// throw new Exception("Out of Memory",1); //OutOfMemoryException();
	{
		err = OUT_OF_MEMORY;
	}
	else
	{
		g_push(item);
	}
}


template <class T>
void Vector<T>::insert(T o)
{
	err = 0;
	VectorItem<T> *item = new VectorItem<T>(NULL, NULL, o);
	if(item == NULL) //throw new OutOfMemoryException();
	{
		err = OUT_OF_MEMORY;
	}
	else
	{
		g_insert(item);
	}
}


template <class T>
T Vector<T>::pop(void)
{
	err = 0;
	if(firstItem == NULL) //throw new VectorEmptyException();
	{
		err = VECTOR_IS_EMPTY;
		return NULL;
	}

	VectorItem<T> *item = g_pop();
	return item->getContent();
}


template <class T>
T Vector<T>::remove(unsigned int x)
{
	err = 0;
	unsigned int i = 0;
	VectorItem<T> *item = firstItem;

	while(item != NULL)
	{
		if(x == i)
		{
			T t = g_remove(item);
			return t;
		}

		i++;
		item = item->getNext();
	}
	
	//if(!isPointer) throw new OutOfVectorException();
	err = OUT_OF_VECTOR;
	return NULL;
}


template <class T>
T Vector<T>::removeFirstItem()
{
	if(firstItem != NULL)
	{
		T t = g_remove(firstItem);
		return t;
	}
	
	return NULL;
}



template <class T>
T Vector<T>::operator[](unsigned int x)
{
	err = 0;
	unsigned int i = 0;
	VectorItem<T> *item = firstItem;

	while(item != NULL)
	{
		if(x == i)
		{
		   return item->getContent();
		}

		i++;
		item = item->getNext();
	}

	//throw new OutOfVectorException();
	err = OUT_OF_VECTOR;
	return NULL;
}


template <class T>
T Vector<T>::getLastItem(void)
{
	err = 0;
	if(lastItem == NULL)
	{
		//throw new VectorEmptyException();
		err = VECTOR_IS_EMPTY;
		return NULL;
	}
	return lastItem->getContent();
}


template <class T>
T Vector<T>::getFirstItem(void)
{
	err = 0;
	if(firstItem == NULL)
	{
		readingCursor = NULL;
		//if(!isPointer) throw new EndOfVectorException();
		err = VECTOR_IS_EMPTY;
		return NULL;
	}

	readingCursor = firstItem;
	return firstItem->getContent();
}


template <class T>
T Vector<T>::getNextItem(void)
{
	err = 0;
	if(readingCursor != NULL)
	{
		readingCursor = readingCursor->getNext();
		if(readingCursor != NULL)
		{
			return readingCursor->getContent();
		}
	}

	//if(!isPointer) throw new EndOfVectorException();
	err = END_OF_VECTOR;
	return NULL;
}


template <class T>
void Vector<T>::replace(T item)
{
	err = 0;
	VectorItem<T> *curr;

	if(readingCursor != NULL)
	{
		readingCursor->setContent(item);
	}

	//throw new OutOfVectorException();
	err = OUT_OF_VECTOR;
}


template <class T>
T Vector<T>::remove(void)
{
	err = 0;
	VectorItem<T> *curr;

	if(readingCursor != NULL)
	{
		curr = readingCursor->getPrev();
		T t = g_remove(readingCursor);
		readingCursor = curr;
		return t;
	}
	
	//if(!isPointer) throw new EndOfVectorException();
	err = END_OF_VECTOR;
	return NULL;
}


template <class T>
bool Vector<T>::isEndOfVector(void)
{
	return readingCursor == NULL;
}


template <class T>
void Vector<T>::addVector(Vector<T> *vector)
{
	err = 0;
	//try
	//{
		T item = vector->getFirstItem();
		while(vector->getLastError() != 0)
		{
			this->push(item);
			item = vector->getNextItem();
		}

	//}
	//catch(EndOfVectorException e) {};
}


template <class T>
int Vector<T>::getLastError(void)
{
	return err;
}

#endif
