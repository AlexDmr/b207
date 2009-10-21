#ifndef __SLIMVECTOR__
#define __SLIMVECTOR__                                                                                                       

#include <stdlib.h>
#include <string.h>

#define SLIMVECTOR_NOERROR      0
#define SLIMVECTOR_OUTOFMEMORY  1
#define SLIMVECTOR_ENDOFVECTOR  2



template <class T> class VectorItem
{
    private:
        VectorItem *prev;
        VectorItem *next;
        T *content;
        int lastError;
      
    public:
        VectorItem(VectorItem *prev, VectorItem *next, T content)
        {
           lastError = SLIMVECTOR_NOERROR;
           this->prev = prev;
           this->next = next;
           if((this->content = (T*) malloc(sizeof(T))) == NULL)
           {
                lastError = SLIMVECTOR_OUTOFMEMORY;
           }
           else
           {
                memcpy(this->content, &content, sizeof(T));
           }
        }   
        
        
        
        VectorItem(VectorItem *prev, VectorItem *next, T *content)
        {
            this->prev = prev;
            this->next = next;
            this->content = content;
        } 
        
        
        
        void setNext(VectorItem *next)
        {
           this->next = next;
        }      
          
          
        
        void setPrev(VectorItem *prev)
        {
           this->prev = prev;
        }  
            
             
        
        VectorItem *getNext(void) const
        {
            return next;
        }
        
        
        
        VectorItem *getPrev(void) const
        {
           return prev;
        }
        
        
        
        T getContent(void) const
        {
           return *content;
        }        
        
        
        
        T* getPointerOnContent(void)
        {
           return content;
        }        
        
        
        
        ~VectorItem(void)
        {
            
        }
        
        
        
        int getLastError(void) const
        {
            return lastError;
        }        
};



/**
* @brief SlimVector is a template class to manage a vector of something. */
/** Something could be a class, a struct or a basic type as int, long, char *, etc.
*/
template <class T> class SlimVector
{
    protected:
	
       unsigned int nbItems;
       VectorItem<T> *firstItem;
       VectorItem<T> *lastItem;
       VectorItem<T> *readingCursor;
       int lastError;
       
       
       
       void g_push(VectorItem<T> *item)
       {
            if(item != NULL)
        	{
        		if(nbItems == 0) firstItem = item;
        		else lastItem->setNext(item);
        		lastItem = item;
        		nbItems++;
        	}
       }
       
       
       
       VectorItem<T> *g_pop(void)
       {
            VectorItem<T> *item;
            
            if(lastItem == NULL) 
            {
                return NULL;
            }
            
            item = lastItem->getPrev();
            if(item == NULL) firstItem = NULL; 
            else item->setNext(NULL);
            nbItems--;
            item = lastItem;
            lastItem = item->getPrev();
            
            return item;
       }
       
       
       
       void g_erase(VectorItem<T> *item)
       {
            VectorItem<T> *prev;
            VectorItem<T> *next;
            
            prev = item->getPrev();
            next = item->getNext();
            
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
            
            delete item;
            nbItems--;
       }
       
       
       
    public:
		SlimVector(void)
        {
            clear();
            lastError = SLIMVECTOR_NOERROR;
        }	
        
        
        
	    int getLastError(void) const
        {
            return lastError;
        }	 
        
        
          
		/**
		* Remove and destroy all the items stored in vector
        * @todo the real implementation of that method
		*/		
		void clear(void)
        {
        	nbItems = 0;
        	firstItem = NULL;
        	lastItem = NULL;
        }



		/**
		* Get the number of items stored in vector
		* @return a number of items
		*/
        const unsigned int size(void) const
        {
            return nbItems;
        }



       /**
		* Add a copy of an object at the end of vector.
		* @param o the object to store a copy in vector
		* @return 0 if success, or SLIMVECTOR_OUTOFMEMORY error code else
		*/
        int push(T o)
        {
            VectorItem<T> *item = new VectorItem<T>(lastItem, NULL, o);
            g_push(item);
            
            return item->getLastError();        	
        }        
          
        
        
        /**
		* Add the object referenced by o at the end of vector.
		* @param o a reference on the object to store in vector
		* @return 0 if success, or SLIMVECTOR_OUTOFMEMORY error code else
		*/
        int push(T *o)
        {
            VectorItem<T> *item = new VectorItem<T>(lastItem, NULL, o);
        	g_push(item);
            
            return item->getLastError();
        }      
      
      
      
      	/**
		* Get a copy of the last object previously added in vector.
		* If vector is empty, a SLIMVECTOR_ENDOFVECTOR error code will be returned in lastError.
		* @return a copy of the last object added in vector.
		* @todo TEST
		*/  
        T pop(void)
        {
            VectorItem<T> *item = g_pop();
            
            if(item == NULL)
            {
                T o;
                lastError = SLIMVECTOR_ENDOFVECTOR;
                return o;
            }
            
            return item->getContent();
        }
        
        
                
        /**
		* Get a reference on the last object previously added in vector
		* @return a reference on the last object added in vector, or NULL if vector is empty
		* @todo TEST
		*/  
        T *pop_reference(void)
        {
            VectorItem<T> *item = g_pop();
            
            if(item == NULL)
            {
                return NULL;
            }
            
            return item->getPointerOnContent();
        }
        
        
                
        /**
		* Remove from vector the object at position x and destroy it.
		* @param x the position of the object to remove and destroy
		*/
        void erase(unsigned int x)
        {
            unsigned int i = 0;
            VectorItem<T> *item = firstItem;
            
            while(item != NULL)
            {
                if(x == i) 
                {
            		g_erase(item);
                    return;
                }
                
                i++;
                item = item->getNext();
            }
        }
        
        
        
        /**
		* Erase the item pointed by internal reading cursor. This cursor is set when vector is traversed by mean of beginSequentialReading() and readItem() functions.
		* If internal reading cursor is NULL, eraseItemAtReadingCursor() will remove and destroy the last item. After removing, internal reading cursor is set on the previous item.
		* As a consequence, the next call to readItem() will get a pointer on the "ex-"following item of the removed one. But the next call to eraseItemAtReadingCursor() will remove
		* the "ex-"previous item of the removed one. If vector is empty, eraseItemAtReadingCursor() will return without doing anything.
		* @see beginSequentialReading()
		* @see readItem()
		*/
        void eraseItemAtReadingCursor(void)
        {
            VectorItem<T> *curr;
            
        	if(readingCursor == NULL)
        	{
        		curr = lastItem;
        	}
        	else
        	{
        		curr = readingCursor->getPrev();
        	}
        	
        	if(curr == NULL)
        	{
        		curr = readingCursor;
        	}
        	
        	g_erase(curr);
        }        
        
        
        
        /**
		* Retrieve a copy of the item stored at position x. vector[0] retrieve a copy of first item, vector[1] retrieve a copy of second item, etc.
		* @param x position of the item to get a copy of. x must be lower than the number of items in vector.
		* @return an instance of class T
		*/
		T operator[](unsigned int x) const
        {
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
        }



		/**
		* Retrieve a copy of the last item in the vector. This function must not be call if vector is empty.
		* @return an instance of class T
		*/		
		T getLastItem(void)    
        {
        	return lastItem->getContent();
        }
        
        
        
        /**
		* Retrieve a reference on the last item in the vector.
		* @return a reference instance of class T or NULL if vector is empty.
		*/		
		T *getPointerOnLastItem(void)    
        {
        	if(lastItem == NULL)
        	{
                return NULL;
            }
            
            return lastItem->getPointerOnContent();
        }                   
       
       
       
       /**
		* Retrieve a reference on the item stored at position x
		* @param x position of the item to get a reference on.
		* @return a pointer on a instance of class T or NULL if x is greater than the number of items in vector
		*/            
		T* getPointerOnItem(unsigned int x)
        {
        	unsigned int i = 0;
        	VectorItem<T> *item = firstItem;
        
        	while(item != NULL)
        	{
        		if(x == i) 
        		{
        			return item->getPointerOnContent();
        		}
        		
        		i++;
        		item = item->getNext();
        	}
        }		
		
		
		
		/**
		* Begin a sequential reading of the vector. This function only set an internal reading cursor just before the first item stored in the vector.
		* Calling next readItem() or eraseItemAtReadingCursor() will, respectively, get a reference on the first item in the vector or remove and destroy it.
		* @see readItem()
		* @see eraseItemAtReadingCursor()
		*/
		void beginSequentialReading(void)
        {
            readingCursor = firstItem;
        }
        
        
        
        /**
		* Make internal reading cursor going one item forward, and retrieve a reference on the newly current item.
		* @return a reference on an instance of class T, or NULL if the end of the vector is reached.
		*/
        T* readItemPointer(void)
        {
            T* o;
            if(readingCursor == NULL) return NULL;
            o = readingCursor->getPointerOnContent();
            readingCursor = readingCursor->getNext();
            return o;
        }        
        
        
        
        /**
		* Make internal reading cursor going one item forward, and retrieve the newly current item.
		* @return an instance of class T, or NULL if the end of the vector is reached.
		*/
        T readItem(void)
        {
            T o;
            if(readingCursor == NULL) 
            {
                lastError = SLIMVECTOR_ENDOFVECTOR;
                return o;
            }
            o = readingCursor->getContent();
            readingCursor = readingCursor->getNext();
            return o;
        }
        
        
        
        /**
		* Test if the reading cursor is after the last item of the vector
		* @return true if the reading cursor is after the last item, false else.
		*/
        bool isEndOfVector(void)
        {
        	if(readingCursor == NULL) return true;
        	return false;
        }        
        
        
        
        /**
		* Destroy the vector and all the items stored in it.
		*/		
		~SlimVector ()
        {
        	VectorItem<T> *temp;
        	VectorItem<T> *item = firstItem;
         
        	 while(item != NULL)
        	 {
            	 temp = item->getNext();
            	 delete item;
            	 item = temp;
        	 }
        }
};
#endif
