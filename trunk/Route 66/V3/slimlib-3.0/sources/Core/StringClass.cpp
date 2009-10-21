#include <Slimlib/Core/StringClass.h>
#include <Slimlib/Core/Buffer.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


String::String(void)
{
	this->string = NULL;
	this->lg = 0;
}



String::~String(void)
{
	if(this->string != NULL) free(this->string);
}



String::String(const char *string, size_t offset, size_t lenght)
{
	this->string = NULL;
	set(string, offset, lenght);
}



String::String(char *string)
{
	this->string = NULL;
	set(string);
}



String::String(long l, const char *format)
{
	this->string = NULL;
	set(l, format);
}



String::String(double d, const char *format)
{
	this->string = NULL;
	set(d, format);
}



String::String(bool b)
{
	this->string = NULL;
	set(b);
}


String::String(Buffer *buffer)
{
	this->string = NULL;
	set(buffer);
}



const char *String::get(void)
{
	return this->string;
}



long String::set(String &string)
{
	return this->set(string.get());
}



long String::set(String *string)
{
	return this->set(string->get());
}



long String::set(const char *string, size_t offset, size_t lenght)
{
	if(string != NULL)
	{
		if(this->string != NULL)
		{
			free(this->string);
		}

		if(offset != 0)
		{
			string+= offset;
		}

		if(lenght != 0)
		{
			Buffer buffer;
			buffer.allocateBuffer(lenght + 1);
			char *substring = buffer.getBuffer();
			strncpy(substring, string, lenght);
			this->string = String::duplicate(substring);
		}
		else
		{
			this->string = String::duplicate(string);
		}

		if(this->string != NULL)
		{
			this->lg = strlen(this->string);
			return this->lg;
		}
	}

	if(this->string != NULL)
	{
		free(this->string);
	}

	this->string = NULL;

	return 0;
}


long String::set(char *string)
{
  if(this->string != NULL)
	{
		free(this->string);
	}

	if(string != NULL)
	{
		this->string = string;

		if(this->string != NULL)
		{
			this->lg = strlen(this->string);
			return this->lg;
		}
	}

	this->string = NULL;

	return 0;
}



long String::set(long l, const char *format)
{
	char tmp[100];
	if(format == NULL)
	{
		sprintf(tmp, "%ld", l);
	}
	else
	{
		sprintf(tmp, format, l);
	}

	return this->set((const char *) tmp);
}




long String::set(double d, const char *format)
{
	char tmp[100];
	if(format == NULL)
	{
		sprintf(tmp, "%f", d);
	}
	else
	{
		sprintf(tmp, format, d);
	}

	return this->set((const char *)tmp);
}



long String::set(bool b)
{
	if(b)
	{
		return this->set("true");
	}
	else
	{
		return this->set("false");
	}
}


long String::set(Buffer *buffer)
{
	if(string != NULL)
	{
		free(string);
		string = NULL;
		lg = 0;
	}

	if(buffer->getSize() > 0)
	{
		lg = strlen(buffer->getBuffer());
		if(lg > buffer->getSize())
		{
			lg = buffer->getSize();
		}

		string = (char *) malloc(lg + 1);
		if(string)
		{
			memcpy(string, buffer->getBuffer(), lg);
			string[lg] = 0;
			lg = strlen(string);
		}
	}

	return lg;
}



size_t String::length(void)
{
	return this->lg;
}



const char *String::concat(String &string)
{
	if(string.length() == 0)
	{
		return NULL;
	}

	return this->concat(string.get());
}




const char *String::concat(String *string)
{
	if(string == NULL)
	{
		return NULL;
	}

	return this->concat(string->get());
}



const char *String::concat(const char *string)
{
	if(string == NULL)
	{
		return NULL;
	}

	if(this->string == NULL)
	{
		set(string);
		return this->string;
	}

	long lg = strlen(string);
	char *s = (char *) malloc((this->lg + lg + 1) * sizeof(char));
	if(s != NULL)
	{
		s[0] = 0;
		strcat(s, this->string);
		strcat(s, string);

		free(this->string);
		this->string = s;
		this->lg += lg;

		return this->string;
	}

	return NULL;
}



const char *String::replace(const char *target, String *replace)
{
	if((replace == NULL) || (target == NULL))
	{
		return NULL;
	}
	return this->replace(target, replace->get());
}



const char *String::replace(const char *target, String &replace)
{
	if((replace.get() == NULL) || (target == NULL))
	{
		return NULL;
	}
	return this->replace(target, replace.get());
}



const char *String::replace(const char *target, const char *replace)
{
	if((replace == NULL) || (target == NULL))
	{
		return NULL;
	}

	long len_target = strlen(target);
	long i = 0;
	char *s = strstr(this->string, target);
	char *newstring;
	long len_newstring;

	while(s != NULL)
	{
		i++;
		s = strstr(s + (len_target * sizeof(char)), target);
	}

	len_newstring = this->lg + ((strlen(replace) - len_target) * i) + 1;

	if((i != 0) && ((newstring = (char *) malloc(len_newstring * sizeof(char))) != NULL))
	{
		newstring[0] = 0;

		char *part = this->string;
		s = strstr(this->string, target);

		while(s != NULL)
		{
			s[0] = 0;
			s += len_target * sizeof(char);

			strcat(newstring, part);
			strcat(newstring, replace);
			part = s;

			s = strstr(s, target);
		}

		strcat(newstring, part);

		free(this->string);
		this->string = newstring;
		this->lg = strlen(this->string);
		return this->string;
	}

	return NULL;
}



const char *String::replace(char target, char replace)
{
	long k = 0;

	while(this->string[k] != 0)
	{
		if(this->string[k] == target)
		{
			this->string[k] = replace;
		}
		k++;
	}

	return this->string;
}



bool String::contains(String &string)
{
	if(string.length() == 0)
	{
		return false;
	}

	return this->contains(string.get());
}




bool String::contains(String *string)
{
	if(string == NULL)
	{
		return false;
	}

	return this->contains(string->get());
}



bool String::contains(const char *string)
{
	if(string == NULL)
	{
		return false;
	}

	return strstr(this->string, string) != NULL;
}



void String::trim(void)
{
	if(this->string != NULL)
	{
		int k_e = this->lg - 1;
		char c = this->string[k_e];
		int k_b = 0;

		while(c != 0)
		{
			if((c == ' ') || (c == '\t') || (c == '\n'))
			{
				k_e --;
				c = this->string[k_e];
			}
			else
			{
				k_e++;
				break;
			}
		}

		c = this->string[0];

		while(c != 0)
		{
			if((c == ' ') || (c == '\t') || (c == '\n'))
			{
				k_b++;
				c = this->string[k_b];
			}
			else
			{
				break;
			}
		}

		char *s = (char *) malloc((k_e - k_b + 1) * sizeof(char));
		if(s != NULL)
		{
			strncpy(s, this->string + (k_b * sizeof(char)), k_e - k_b);
			free(this->string);
			this->string = s;
		}
	}
}



void String::trimright(void)
{
	if(this->string != NULL)
	{
		int k = this->lg - 1;
		char c = this->string[k];

		while(c != 0)
		{
			if((c == ' ') || (c == '\t') || (c == '\n'))
			{
				k--;
				c = this->string[k];
			}
			else
			{
				k++;
				break;
			}
		}

		char *s = (char *) malloc((k + 1) * sizeof(char));
		if(s != NULL)
		{
			strncpy(s, this->string, k);
			free(this->string);
			this->string = s;
		}
	}
}



void String::trimleft(void)
{
	if(this->string != NULL)
	{
		char c = this->string[0];
		int k = 0;

		while(c != 0)
		{
			if((c == ' ') || (c == '\t') || (c == '\n'))
			{
				k++;
				c = this->string[k];
			}
			else
			{
				break;
			}
		}

		char *s = (char *) malloc((this->lg - k) * sizeof(char));
		if(s != NULL)
		{
			strncpy(s, this->string + (k * sizeof(char)), this->lg - k);
			free(this->string);
			this->string = s;
		}
	}
}



const char *String::capitalize(void)
{
	if(this->string != NULL)
	{
		if((this->string[0] >= 97) && (this->string[0] <= 122))
		{
            this->string[0] -= 32;
		}

		return this->string;
	}

	return NULL;
}



int String::operator==(const char *string)
{
	if(!string && this->string) return 0;
	if(!string && !this->string) return 1;
	return strcmp(this->string, string) == 0;
}



int String::operator==(String &string)
{
	if(this->string && !string.get()) return 0;
	if(!this->string && !string.get()) return 1;
	return strcmp(this->string, string.get()) == 0;
}



int String::operator!=(const char *string)
{
	if(!string && this->string) return 1;
	if(!string && !this->string) return 0;
	return strcmp(this->string, string) != 0;
}



int String::operator!=(String &string)
{
	if(this->string && !string.get()) return 1;
	if(!this->string && !string.get()) return 0;
	return strcmp(this->string, string.get()) != 0;
}


/*
String *String::operator=(String &string)
{
	printf("##################### string=%s\n", string);
	if(this->set(string.get()) == NULL)
	{
		return NULL;
	}

	return this;
}
*/


String *String::operator=(const char *string)
{
	this->set(string);
	if(this->string == NULL)
	{
		return NULL;
	}

	return this;
}



char *String::duplicate(const char *string)
{
	if(string)
	{
		char *buffer;

		buffer = (char *) malloc((sizeof(char) * strlen(string)) + 1);
		if(buffer != NULL)
		{
			memcpy(buffer, string, strlen(string) + 1);
		}

		return buffer;
	}

	return NULL;
}


/*
String &String::operator+(String &string)
{
	if(this->concat(string) == NULL)
	{
		//return NULL;
	}

	//return this;
}



String &String::operator+(const char *string)
{
	if(this->concat(string) == NULL)
	{
		//return NULL;
	}

	//return this;
}



String &String::operator+(String *string)
{
	if(this->concat(string) == NULL)
	{
		//return NULL;
	}

	//return this;
}
*/
