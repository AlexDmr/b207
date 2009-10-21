#include <Slimlib/Errors.h>
#include <Slimlib/Core/File.h>
#include <Slimlib/Core/StringClass.h>
#include <stdlib.h>
#include <string.h>

File::File(const char *filename)
{
    unsigned long i;
    
    handler = NULL;
    lastError = NO_ERROR;
    
    this->filename = String::duplicate(filename);
    
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


void File::beginSequentialReading()
{
	lastError = NO_ERROR;
        
	if(handler == NULL)
	{
		if ((handler = fopen(filename, "r")) == NULL)
		{
			lastError = FILE_NOT_FOUND;
			return;
		}
	}
}

void File::beginSequentialWriting()
{
	lastError = NO_ERROR;
        
	if(handler == NULL)
	{
		if ((handler = fopen(filename, "w+")) == NULL)
		{
			lastError = CANNOT_OPEN_FOR_WRITING;
			return;
		}
	}
}

long File::readContentSnippet(char *buffer, long bufferSize)
{
	memset(buffer, 0, bufferSize);
	return (long) fread(buffer, 1, bufferSize, handler);
}


long File::writeContentSnippet(const char *buffer, long bufferSize)
{
	return (long) fwrite(buffer, 1, bufferSize, handler);
}

void File::close()
{
	fclose(handler);   
	handler = NULL;
}

char *File::readContent(long *nbBytesRead)
{
        long file_size;
        char *buffer;
        
        lastError = NO_ERROR;
        
        if(handler == NULL)
        {
            if ((handler = fopen(filename, "r")) == NULL)
            {
                lastError = FILE_NOT_FOUND;
                return NULL;
            }
        }
        
        file_size = getSize();
		
		if(lastError != NO_ERROR) return NULL;
        
        if((buffer = (char *) malloc(file_size * sizeof(char))) == NULL)
        {
            lastError = OUT_OF_MEMORY;
            fclose(handler);
            return NULL;
        }
        	
	fread(buffer, file_size, 1, handler);
        fclose(handler);   
        handler = NULL;
        
        return buffer;
}

int File::writeContent(const char *buffer, long nbBytesToWrite)
{
    lastError = NO_ERROR;
        
    if(handler == NULL)
    {
        if ((handler = fopen(filename, "w+")) == NULL)
        {
                lastError = CANNOT_OPEN_FOR_WRITING;
                return lastError;
        }
        
        if(fwrite(buffer, nbBytesToWrite, 1, handler) < 1)
        {
                lastError = ERROR_WHILE_WRITING;
        }
        
        fclose(handler);
        handler = NULL;
        
        return lastError;
    }
	lastError = FILE_ALREADY_OPEN;
	return lastError;
}

long File::getSize(void)
{
    long curpos, length;
    
    lastError = NO_ERROR;
    
    if(handler == NULL)
    {
        if ((handler = fopen(filename, "r")) == NULL)
	{
		lastError = FILE_NOT_FOUND;
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

int File::getLastError(void) const
{
    return lastError;
}
