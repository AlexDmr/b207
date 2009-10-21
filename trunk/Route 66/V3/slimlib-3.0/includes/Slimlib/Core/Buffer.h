/* ---------------------------------------------------------------------------------------
 * Buffer.h
 * ---------------------------------------------------------------------------------------
 * This file contains the header of Buffer class.
 * ---------------------------------------------------------------------------------------
 * Copyright (C) 2007 Lionel Balme
 * http://iihm.imag.fr/balme/projects/slimlib/
 * ---------------------------------------------------------------------------------------
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * ---------------------------------------------------------------------------------------
 * The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT", "SHOULD", "SHOULD
 * NOT", "RECOMMENDED",  "MAY", and "OPTIONAL" in this document are to be interpreted as
 * described in RFC 2119 (http://www.ietf.org/rfc/rfc2119.txt).
 * ---------------------------------------------------------------------------------------
 * Version: 1.0
 * ---------------------------------------------------------------------------------------
 * Dependency list
 *
 * Slimlib/Core/Object.h
 * ---------------------------------------------------------------------------------------
 */

#ifndef __Class_Buffer_
#define __Class_Buffer_

#include <sys/types.h>
#include <Slimlib/Core/Object.h>

/** @brief Buffer class is a  very basic class to allocate and manipulate a buffer.
 */
/** To allocate a buffer with Buffer, you have to create it by calling the constructor,
 * then call allocateBuffer method with the desired buffer size in bytes as parameters.
 * You can do that two operation in one single shot by calling static function
 * Buffer::createBuffer function. It will return a pointer onto a Buffer object if
 * allocation succeed or NULL otherwise.
 */
class Buffer: public Object
{
	private:
		char *buffer;
		size_t size;

	public:
		/** Construct a Buffer object.
		 * No memory allocation is perfom by Buffer(). You need to call allocateBuffer() to do
		 * it. Calling getBuffer() right after calling the constructor will return NULL.
		 * To obtain a valid Buffer object directly, use one of createBuffer() functions.
		 * @see allocateBuffer()
		 * @see createBuffer()
		 */
		Buffer();

		/** Destroy a Buffer object, and free memory previously allocated for that buffer.
		 */
		virtual ~Buffer(void);

		/** Allocating memory for that buffer. Return 1 if succeed, 0 otherwise.
		 * @param bufferSize (size_t) size in bytes of the amount of memory to allocate
		 * @return (boolean) true if success, false otherwise.
		 */
		bool allocateBuffer(size_t bufferSize);

		/** Resize that buffer to <i>expectedBufferSize</i> bytes.
		 * @param expectedBufferSize the expected new buffer size expressed in bytes.
		 * @return true if success, false otherwise.
		 */
		bool resize(size_t expectedBufferSize);

		/** Get size in bytes of the memory allocated for the buffer
		 * @return (size_t) a size in bytes
		 */
		size_t getSize(void);

		/** Get a pointer on the allocated buffer.
		 * @return a pointer on the allocated buffer, NULL if no memory allocation was made
		 */
		char *getBuffer(void);

		/** Return a Buffer object that handle a <i>size</i> bytes large allocated buffer.
		 * @param size (long) a number of bytes.
		 * @return a pointer of a Buffer object if successful, or return NULL otherwise.
		 */
		static Buffer *createBuffer(size_t size);

		/**Return a Buffer object that handle a <i>size</i> bytes large allocated buffer, and
		 * copy <i>size</i> bytes from pointer <i>data</i>.
		 * @param data (const char *) a pointer onto data to copy.
		 * @param size (long) a number of bytes.
		 * @return a pointer of a Buffer object if successful, or return NULL otherwise.
		 */
		static Buffer *createBuffer(const char *data, size_t size);

		/** Encode a given buffer using Base64 encoding. This function does not modified the
		 * buffer to encode but return the result into a new buffer.
		 * @param buffer (Buffer *) a pointer onto a buffer to encode.
		 * @return (Buffer *) a pointer onto a buffer that contains encoded data.
		 * @see RFC 3548 - The base16, Base32, and Base64 Data Encodings
		 */
		static Buffer *Base64Encode(Buffer *buffer);

		/** Decode a given buffer containing Base64 encoded data. This function does not
		 * modified the buffer to decode but return the result into a new buffer.
		 * @param buffer (Buffer *) a pointer onto a buffer to decode.
		 * @return (Buffer *) a pointer onto a buffer that contains decoded data.
		 * @see RFC 3548 - The base16, Base32, and Base64 Data Encodings
		 */
		static Buffer *Base64Decode(Buffer *buffer);
};

#endif
