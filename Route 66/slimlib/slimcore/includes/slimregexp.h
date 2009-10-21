/*
 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.
 */
#ifndef RegularexpH
#define RegularexpH

#define NSUBEXP  20
typedef struct Regexp {
	char *startp[NSUBEXP];
	char *endp[NSUBEXP];
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	int regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} Regexp;


/*
 * Compile a literal regular expression ("exp") into exploitable Regexp structure. 
 * @param exp a regular expression in text format 
 * @return a compiled regular expression
 */
 Regexp *regcomp(const char *exp);
 
 
 
 /*
  * Test the string "string" against the compiled regular expression "prog".
  * @param prog a regular expression
  * @param string the string to compare with
  * @return if "string" do not match the regular expression "prog" return 0, else something else.
  */
 int regexec(register const Regexp *prog, register const char *string);
 
 
 
 /*
  * Undocumented function. 
  */
 void regsub(const Regexp *prog, const char *source, char *dest);
 
 
 /*
  * Undocumented function.
  */
 void regerror(const char *s);
 
 
 
 /*
  * Get value of arguments defined in the regular expression, and the number of arguments.
  * @param regexp a regular expression
  * @param nb_args a pointer on a unsigned int where the number og arguments will be stored
  * @return a char * array. At [0] will be the first argument, [1] the second, etc. or NULL if regular expression defined not argument.
  */
 char **reggetsub(Regexp *regexp, unsigned int *nb_args);
#endif
