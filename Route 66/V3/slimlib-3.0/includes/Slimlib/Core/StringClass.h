/*
  Name: slimcore/String
  Copyright: CLIPS-IMAG
  Author: L. BALME
  Date: 24/05/06 10:05
  Description: String management class
*/

#ifndef __Class_String_
#define __Class_String_

#include <Slimlib/Core/Object.h>
#include <Slimlib/Core/Buffer.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief String is a class to deal with ASCII strings
 */
/**
 * String offers operators upon ASCII string to easily concat, replacing parts, trim, etc.
 */

class String: public Object
{
	private:
		char *string;
		size_t lg;

	public:
		/**
		 * Construct an empty String object that contains a NULL string
		 */
		String(void);


		/**
		 * Destroy a String object
		 */
		virtual ~String(void);


		/**
		 * Construct a String object from the string pointed by <i>string</i>.
		 * This constructor make an internal copy of the string pointed by <i>string</i>.
		 * if <i>offset</i> and <i>lenght</i> parameters are specified and <i>lenght</i> is different
		 * from 0, this constructor copies at most <i>length</i> characters from character number <i>offset</i>
		 * of <i>string</i>
		 * @param string a pointer upon the source null-terminated string
		 * @param offset the position of the charater where to begin to copy
		 * @param lenght the number of characters to copy
		 */
		String(const char *string, size_t offset = 0, size_t lenght = 0);

		/**
		 * Construct a String object from the string pointed by <i>string</i>.
		 * This string object will now manage the buffer pointed by <i>string</i> and will be in charge to free
		 * it at object destruction.
		 * @param string a pointer upon the source null-terminated string
		 * @param offset the position of the charater where to begin to copy
		 * @param lenght the number of characters to copy
		 */
		String(char *string);

		/**
		 * Construct a String object from a <b>long</b> <i>l</i>.
		 * This constructor make an string representation of <i>l</i> according to the specified format <i>format</i>.
		 * @param l the long integer value to represent as a string
		 * @param format an optional string that contains the c-standard conversion specification to use. Default value is "%d".
		 */
		String(long l, const char *format = NULL);

		/**
		 * Construct a String object from a <b>double</b> <i>d</i>.
		 * This constructor make an string representation of <i>d</i> according to the specified format <i>format</i>.
		 * @param d the double value to represent as a string
		 * @param format an optional string that contains the c-standard conversion specification to use. Default value is "%f".
		 */
		String(double d, const char *format = NULL);

		/**
		 * Construct a String object from a <b>bool</b> <i>b</i>.
		 * This constructor make an string representation of <i>b</i>. if b is true, string representation is "true", else string representation is "false"
		 * @param b a bool value to represent as a string
		 */
		String(bool b);

		/** Construct a String object from a buffer. This constructor make a copy of data
		 * contained within a given buffer and append a terminal null character.
		 */
		String(Buffer *buffer);

		/**
		 * Return a pointer upon the encapsulated string.
		 */
		const char *get(void);

		/**
		 *
		 */
		long set(const char *string, size_t offset = 0, size_t lenght = 0);
		long set(char *string);
		long set(String *string);
		long set(String &string);
		long set(long l, const char *format = NULL);
		long set(double d, const char *format = NULL);
		long set(bool b);
		long set(Buffer *buffer);

		/**
		 * Return the length of the encapsulated string, in terms of characters.
		 * In ASCII strings, one byte = one character.
		 * length of the encapsulated string do not include the NULL terminal character.
		 */
		size_t length(void);

		/**
		 * Replace any occurences of <i>target</i> with <i>replace</i> in the encapsulated string
		 * @param target a pointer onto the string to replace
		 * @param replace a pointer onto the string that is going to replace occurences of <i>target</i>
		 */
		const char *replace(const char *target, const char *replace);

		/**
		 * Replace any occurences of <i>target</i> with <i>replace</i> in the encapsulated string
		 * @param target a pointer onto the string to replace
		 * @param replace a pointer onto a String object that is going to replace occurences of <i>target</i>
		 */
		const char *replace(const char *target, String *replace);

		/**
		 * Replace any occurences of <i>target</i> with <i>replace</i> in the encapsulated string
		 * @param target a pointer onto the string to replace
		 * @param replace a String object that is going to replace occurences of <i>target</i>
		 */
		const char *replace(const char *target, String &replace);

		/**
		 * Replace any occurences of <i>target</i> character with <i>replace</i> character in the encapsulated string
		 * @param target the character to find and replace
		 * @param replace the character that is going to replace occurences of <i>target</i>
		 */
		const char *replace(char target, char replace);

		/**
		 * Add <i>string</i> at the end of the encapsulated string. This operation will perform an internal copy of <i>string</i>
		 * Then, <i>string</i> could be freed without any effect on the String object.
		 * @param string a pointer onto the string to concat
		 */
		const char *concat(const char *string);
		const char *concat(String *string);
		const char *concat(String &string);

		/**
		 * Worth true if <i>string</i> appears in the encapsulated string, false otherwise.
		 */
		bool contains(const char *string);

		/**
		 * Worth true if <i>string</i> appears in the encapsulated string, false otherwise.
		 */
		bool contains(String *string);

		/**
		 * Worth true if <i>string</i> appears in the encapsulated string, false otherwise.
		 */
		bool contains(String &string);

		/**
		 * Suppress whitespaces on the right and on the left of the encapsulated string.
		 */
		void trim(void);

		/**
		 * Suppress whitespaces on the right of the encapsulated string.
		 */
		void trimright(void);

		/**
		 * Suppress whitespaces on the left of the encapsulated string.
		 */
		void trimleft(void);

		/**
		 *  Capitalize the first character of the encapsulated string if it is a letter. Do nothing otherwise.
		 */
		const char *capitalize(void);

		/**
		 * Worth true if <i>string</i> represents the same sequence of characters as the encapsulated string
		 */
		int operator==(const char *string);

		/**
		 * Worth true if <i>string</i> represents the same sequence of characters as the encapsulated string
		 */
		int operator==(String &string);

		/**
		 * Worth true if <i>string</i> does not represent the same sequence of characters as the encapsulated string
		 */
		int operator!=(const char *string);

		/**
		 * Worth true if <i>string</i> does not represent the same sequence of characters as the encapsulated string
		 */
		int operator!=(String &string);

		/**
		 * @todo To be tested: done, its full of bugs here
		 * @todo Fix bugs
		 * Bugged: DO NOT USE IT !
		 */
		String *operator=(const char *string);

		static char *duplicate(const char *string);
};

#endif
