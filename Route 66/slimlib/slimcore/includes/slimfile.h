
#ifndef _SLIMFILE_
#define _SLIMFILE_

#include <stdio.h>


#define SLIMFILE_NOERROR                    		0
#define SLIMFILE_FILE_NOT_FOUND             		1
#define SLIMFILE_OUT_OF_MEMORY              		2
#define SLIMFILE_CANNOT_OPEN_FOR_WRITING    		3
#define SLIMFILE_ERROR_WHILE_WRITING        		4


/**
 * @brief SlimFile is a class to read/write files
 */
/**
 * Filename that include some path description could use either "/" or "\" caracters
 */
class SlimFile
{
    private:
        FILE *handler;
        char *filename;
        int lastError;

    public:

        /**
		 * Construct a SlimFile object linked with the file pointed by <i>filename</i>
		 * @param filename is a string containing a filename
		 */
        SlimFile(const char *filename);
        ~SlimFile() { };



		/**
		 * Allocate a buffer, read the content of the linked file,  store it in previously allocated buffer and return a pointer on this buffer
		 * @param nbBytesRead is a pointer on a long, where number of bytes read by readContent() will be stored
		 * @return a pointer on buffer where was load the content of the linked file
		 */
        char *readContent(long *nbBytesRead);


		
		/**
		 * Write <i>nbBytesToWrite</i> bytes of <i>buffer</i> in the linked file
		 * @param buffer is a pointer on the buffer that contains data to write in file
		 * @param nbBytesToWrite contains the number of bytes of buffer to write in file
		 * @return 0 (SLIMFILE_NOERROR) if the operation went well, i dont remember why else
		 * @todo fix the previous line of comment
		 */
        int writeContent(const char *buffer, long nbBytesToWrite);



		/**
		 * Get the size of the linked file, in bytes
		 * @return size of the linked file
		 */
        long getSize(void);



		/**
		 * If a error is raised in one of the method of this class, getLastError() get the code of the last error encounted
		 * @return an error code
		 * @see #define in slimfile.h
		 */
        int getLastError(void) const;
};
#endif

