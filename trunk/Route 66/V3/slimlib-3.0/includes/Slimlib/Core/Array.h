#ifndef __Class_Array_
#define __Class_Array_

#include <Slimlib/Core/Object.h>
#include <stdlib.h>

/**
 * Manage an Array of <b>pointers</b> onto instances of a <b>class</b> T. Array destructor also destruct handled items.
 * Once a size is specified, it cannot be modified.
 */
template <class T> class Array: public Object
{
    private:
        /**
         * the array size in term of numberb of items
         */
        unsigned long size;

        /**
         * a pointer onto the array
         */
        T* objects;

    public:
        /**
         * Construct an Array object without any memory allocation. setSize() must be called to make Array's object able
         * to store items
         */
        Array()
        {
            size = 0;
            objects = NULL;
        }


        /**
         * Construct an array object that could handle <i>size</i> items.
         * Due to a lack of memory, this constructor can failed. Test if getSize() != 0 before storing items in Array
         * object.
         * @param size the size of the array in terms of a number of items
         */
        Array(unsigned long size)
        {
            this->size = 0;
            setSize(size);
        }


        /**
         * Destruct an Array object and all handled items
         */
        ~Array()
        {
            for(unsigned long k = 0; k < size; k++)
            {
                delete objects[k];
            }

            free(objects);
        }


        /**
         * Set the array size. Could be call once if Array() constructor was used. If setSize() is called more than
         * once, false will be returned.
         * @param size the size of the array in terms of a number of items
         * @return return true if was successfull, false otherwise.
         */
        bool setSize(unsigned long size)
        {
            if(this->size == 0)
            {
                objects = (T*) malloc(size * sizeof(T*));
                if(objects == NULL)
                {
                    return false;
                }

                this->size = size;
                memset(objects, 0, size * sizeof(T*));
            }

            return false;
        }

        /**
         * Return the array size, in terms of number of items
         * @return return the number of items
         */
        unsigned long getSize()
        {
            return size;
        }


        /**
         * If object was statically instantiate, operator[] could be used. It allows to access to an handled items by
         * its position in array
         * @param index the position of the item in array
         * @return return a pointer onto a T object if successfull, or NULL if no object is stored at this position or
         * if this position is out of bounds.
         */
        T operator[](unsigned long index)
        {
            return get(index);
        }


        /**
         * If object was dynamically instanciate (via new operator), items in array must be accessed via set() and get()
         * methods. set() stores <i>object</i> at <i>index</i> position in array. If an object was already stored at this
         * position, it will be destroyed
         * @param index the position of the item in array
         * @param object a pointer onto an object of class T
         * @return return true if successfull, false if index is out of bounds
         */
        bool set(unsigned long index, T object)
        {
            if(index < size)
            {
                if(objects[index] != NULL) delete objects[index];
                objects[index]= object;
                return true;
            }

            return false;
        }


        /**
         * If object was dynamically instanciate (via new operator), items in array must be accessed via set() and get()
         * methods. get() retrieves a pointer onto the object of class T stored at position <i>index</i> in array.
         * @param index the position of the item in array
         * @return return a pointer onto an object of class T if successfull, NULL if no object is stored at this
         * position or if this position is out of bounds.
         */
        T get(unsigned long index)
        {
           if(objects != NULL)
            {
                if(index < size)
                {
                    return objects[index];
                }
            }

            return NULL;
        }
};

#endif
