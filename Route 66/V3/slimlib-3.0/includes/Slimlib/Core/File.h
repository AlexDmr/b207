#ifndef __Class_File_
#define __Class_File_

#include <Slimlib/Core/Object.h>
#include <stdio.h>

/**
 * @brief File is a class to read/write files
 */
/**
 * Filename that include some path description could use either "/" or "\" caracters
 */
class File: public Object
{
    private:
        FILE *handler;
        char *filename;
        int lastError;

    public:

        /**
		 * Construct a File object linked with the file pointed by <i>filename</i>
		 * @param filename is a string containing a filename
		 */
        File(const char *filename);
        virtual ~File() { };



		/**
		 * Allocate a buffer, read the content of the linked file,  store it in previously allocated buffer and return a pointer on this buffer
		 * @param nbBytesRead is a pointer on a long, where number of bytes read by readContent() will be stored
		 * @return a pointer on buffer where was load the content of the linked file
		 */
        char *readContent(long *nbBytesRead);
		
		
		
		/**
		 * Open the linked file and set the reading cursor at the begining of the file. File's content could be read step by step by using readContentSnippet().
		 */
        void beginSequentialReading();
		
		
		
		/**
		 * Open the linked file for writing and set the writing cursor at the begining of the file. File's content could be write step by step by using writeContentSnippet().
		 */
        void beginSequentialWriting();
		
		
		
		/**
		 * Try to read <i>bufferSize</i> bytes from the linked file to <i>buffer</i> and increment the reading cursor by <i>bufferSize</i>. <i>buffer</i> must be big enough to host theses <i>bufferSize</i> bytes.
		 * @param buffer is a pointer the buffer that will host data from file
		 * @param bufferSize specified the size in bytes of <i>buffer</i> 
		 * @return number of bytes read or SLIMFILE_END_OF_FILE if end of file is reach.
		 */
        long readContentSnippet(char *buffer, long bufferSize);



		/**
		 * Try to write <i>bufferSize</i> bytes from the linked file to <i>buffer</i> and increment the writing cursor by <i>bufferSize</i>.
		 * @param buffer is a pointer the buffer that contains data to write
		 * @param bufferSize specified the size in bytes of <i>buffer</i> 
		 * @return number of bytes write or something else if a write error occurs.
		 */
        long writeContentSnippet(const char *buffer, long bufferSize);

		
		
		/**
		 * Close the linked file after a sequential reading.
		 */
        void close();


		
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

