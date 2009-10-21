#include <Misc/Tokenizer.h>



//----------------------------------------------------------------------------------------
Tokenizer::Tokenizer(const char* string, const char *sep)
{
	last = NULL;
	firstToken = true;
	this->string = NULL;
	nbReadToken = 0;
	
	if(sep != NULL)
	{
		this->sep.set(sep);
	}
	else
	{
		this->sep.set(" ");
	}
	
	if(string != NULL)
	{
		this->string = String::duplicate(string);
		this->tokens.set(string);
	}
}



//----------------------------------------------------------------------------------------
Tokenizer::~Tokenizer()
{
	if(string != NULL) free(string);
}



//----------------------------------------------------------------------------------------
void Tokenizer::reset()
{
	if(string != NULL) free(string);
	nbReadToken = 0;
	last = NULL;
	firstToken = true;
	string = String::duplicate(tokens.get());
}



//----------------------------------------------------------------------------------------
const char *Tokenizer::getNextToken(const char *sep)
{
	if(sep != NULL) 
	{
		this->sep.set(sep);
	}

	char *token;
	if(firstToken)
	{
		firstToken = false;
		token = strtok_r(string, this->sep.get(), &last);
	}
	else
	{
		token = strtok_r(NULL, this->sep.get(), &last);
	}
	
	if(token != NULL) nbReadToken++;
	return token;
}



//----------------------------------------------------------------------------------------
void Tokenizer::addToken(const char *token)
{
	tokens.concat(sep);
	tokens.concat(token);
}



//----------------------------------------------------------------------------------------
long Tokenizer::getNbReadToken()
{
	return nbReadToken;
}



//----------------------------------------------------------------------------------------
const char *Tokenizer::getTokens()
{
	return tokens.get();
}
