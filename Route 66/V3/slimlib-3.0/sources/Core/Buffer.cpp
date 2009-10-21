#include <Slimlib/Core/Buffer.h>
#include <stdlib.h>
#include <iostream>

/** Return the index of a base64 char within the Base64 alphabet.
 * This function si used by Buffer::Base64Decode() only.
 */
char base64IndexOf(char c)
	{
		if((c > 64) && (c < 91))
		{
			return c - 65;
		}
		else if((c > 96) && (c < 123))
		{
			return c - 71;
		}
		else if((c > 47) && (c < 58))
		{
			return c + 4;
		}
		else if(c == 43) return 62;
		else if(c == 47) return 63;

		return 64;
	}


Buffer::Buffer()
{
	buffer = NULL;
	size = 0;
}



Buffer::~Buffer(void)
{
	if(buffer) free(buffer);
}



bool Buffer::allocateBuffer(size_t bufferSize)
{
	if(bufferSize != 0)
	{
		if((buffer = (char *) malloc(bufferSize)) != NULL)
		{
			size = bufferSize;
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}



bool Buffer::resize(size_t expectedBufferSize)
{
	if(expectedBufferSize == 0)
	{
		if(buffer) free(buffer);
		buffer = NULL;
		size = 0;
	}
	else
	{
		char *newBuffer;
		if((newBuffer = (char *) malloc(expectedBufferSize)) != NULL)
		{
			memcpy(newBuffer, buffer, expectedBufferSize);
			free(buffer);
			buffer = newBuffer;
			size = expectedBufferSize;
		}
		else
		{
			return false;
		}
	}

	return true;
}



size_t Buffer::getSize(void)
{
	return size;
}



char *Buffer::getBuffer(void)
{
	return buffer;
}



Buffer *Buffer::createBuffer(size_t size)
{
	Buffer *buffer = new Buffer();
	if(!buffer->allocateBuffer(size))
	{
		delete(buffer);
		return NULL;
	}

	return buffer;
}



Buffer *Buffer::createBuffer(const char *data, size_t size)
{
	Buffer *buffer = Buffer::createBuffer(size);
	if(buffer)
	{
		memcpy(buffer->getBuffer(), data, size);
	}

	return buffer;
}



Buffer *Buffer::Base64Encode(Buffer *buffer)
{
	char keyStr[66] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

	size_t size = buffer->getSize();
	char *input = buffer->getBuffer();

	Buffer *outputBuffer = Buffer::createBuffer(size + ((size * 3) / 4) + 4);
	char *output = outputBuffer->getBuffer();

	unsigned char chr1, chr2, chr3, enc1, enc2, enc3, enc4;
	size_t i = 0;
	size_t j = 0;

	while(i < size)
	{
		enc3 = 64;
		enc4 = 64;

		chr1 = (unsigned char) input[i++];
		enc1 = chr1 >> 2;

		chr2 = 0;
		if(i < size)
		{
			chr2 = (unsigned char) input[i++];

			chr3 = 0;
			if(i < size)
			{
				chr3 = (unsigned char) input[i++];
				enc4 = chr3 & 63;
			}

			enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);
		}

		enc2 = ((chr1 & 3) << 4) | (chr2 >> 4);

		output[j++] = keyStr[(int)enc1];
		output[j++] = keyStr[(int)enc2];
		output[j++] = keyStr[(int)enc3];
		output[j++] = keyStr[(int)enc4];
	}

	outputBuffer->resize(j);

	return outputBuffer;
}




Buffer *Buffer::Base64Decode(Buffer *buffer)
{
	unsigned char chr1, chr2, chr3, enc1, enc2, enc3, enc4;
	size_t i = 0;
	size_t j = 0;

	char *data = buffer->getBuffer();
	size_t dataSize = buffer->getSize();
	Buffer *inputBuffer = Buffer::createBuffer(dataSize);
	char *input = inputBuffer->getBuffer();



	while(i < (unsigned long) dataSize)
	{
		char c = data[i];
		if(((c > 64) && (c < 91)) || ((c > 96) && (c < 123)) || ((c > 47) && (c < 58))
			|| (c == 43) || (c == 47) || (c == 61))
		{
			input[j] = c;
			j++;
		}

		i++;
	}

	Buffer *outputBuffer = Buffer::createBuffer(dataSize);
	char *output = outputBuffer->getBuffer();

	i = 0;
	j = 0;

	while(i < dataSize)
	{
		enc1 = base64IndexOf(input[i++]);
		enc2 = base64IndexOf(input[i++]);
		enc3 = base64IndexOf(input[i++]);
		enc4 = base64IndexOf(input[i++]);

		chr1 = (enc1 << 2) | (enc2 >> 4);
		chr2 = ((enc2 & 15) << 4) | (enc3 >> 2);
		chr3 = ((enc3 & 3) << 6) | enc4;

		output[j++] = chr1;
		if(enc3 != 64) output[j++] = chr2;
		if(enc4 != 64) output[j++] = chr3;
	}

	outputBuffer->resize(j);

	return outputBuffer;
}
