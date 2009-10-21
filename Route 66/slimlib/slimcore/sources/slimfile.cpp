#include <slimcore.h>
#include <stdlib.h>
#include <string.h>

SlimFile::SlimFile(const char *filename)
{
    unsigned long i;

    handler = NULL;

    this->filename = new char[strlen(filename)+1]; strcpy(this->filename, filename);

    for(i = 0; i < strlen(this->filename); i++)
    {
        #ifdef WIN32
		if(this->filename[i] == 47)
        {
            this->filename[i] = 92;
        }
		#endif

		#if defined (unix) || defined (__APPLE__)
		if(this->filename[i] == 92)
        {
            this->filename[i] = 47;
        }
		#endif
    }
}



char *SlimFile::readContent(long *nbBytesRead)
{
        long file_size;
        char *buffer;

        lastError = SLIMFILE_NOERROR;

        if(handler == NULL)
        {
            if ((handler = fopen(filename, "r")) == NULL)
            {
                lastError = SLIMFILE_FILE_NOT_FOUND;
                return NULL;
            }
        }

        file_size = getSize();
	if(lastError != SLIMFILE_NOERROR) return NULL;

        buffer = new char[file_size];
        if(buffer == NULL)
        {
            lastError = SLIMFILE_OUT_OF_MEMORY;
            fclose(handler);
            return NULL;
        }

	fread(buffer, file_size, 1, handler);
        fclose(handler);
        handler = NULL;

        return buffer;
}



int SlimFile::writeContent(const char *buffer, long nbBytesToWrite)
{
    lastError = SLIMFILE_NOERROR;

    if(handler == NULL)
    {
        if ((handler = fopen(filename, "w+")) == NULL)
        {
                lastError = SLIMFILE_CANNOT_OPEN_FOR_WRITING;
                return lastError;
        }

        if(fwrite(buffer, nbBytesToWrite, 1, handler) < 1)
        {
                lastError = SLIMFILE_ERROR_WHILE_WRITING;
        }

        fclose(handler);
        handler = NULL;

        return lastError;
    }
}



long SlimFile::getSize(void)
{
    long curpos, length;

    lastError = SLIMFILE_NOERROR;

    if(handler == NULL)
    {
        if ((handler = fopen(filename, "r")) == NULL)
	{
		lastError = SLIMFILE_FILE_NOT_FOUND;
                return 0;
	}

        curpos = ftell(handler);
        fseek(handler, 0L, SEEK_END);
        length = ftell(handler);
        fseek(handler, curpos, SEEK_SET);
        fclose(handler);
        handler = NULL;
    }
    else
    {
        curpos = ftell(handler);
        fseek(handler, 0L, SEEK_END);
        length = ftell(handler);
        fseek(handler, curpos, SEEK_SET);
    }

    return length;
}



int SlimFile::getLastError(void) const
{
    return lastError;
}
