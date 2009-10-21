#ifndef __Class_Tokenizer_
#define __Class_Tokenizer_

#include <Slimlib/Core.h>

class Tokenizer
{
	public:
		Tokenizer(const char* string = NULL, const char *sep = NULL);
		~Tokenizer();

		void reset();
		const char *getNextToken(const char *sep = NULL);
		void addToken(const char *token);
		long getNbReadToken();
		const char *getTokens();

	private:
		bool firstToken;
		char *string;
		char *last;
		String sep;
		String tokens;
		long nbReadToken;
};

#endif
