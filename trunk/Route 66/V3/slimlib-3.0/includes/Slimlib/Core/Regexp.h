/**
 * @brief set of routines to manipulate regular expressions.
 *
 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.
 */
#ifndef __Class_Regexp_
#define __Class_Regexp_

#include <Slimlib/Core/Object.h>
#include <Slimlib/Core/Array.h>
#include <Slimlib/Core/StringClass.h>

#define NSUBEXP  20
typedef struct RegexpStruct {
	char *startp[NSUBEXP];
	char *endp[NSUBEXP];
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	int regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} RegexpStruct;


/**
 * Compile a literal regular expression ("exp") into exploitable Regexp structure. 
 * @param exp a regular expression in text format 
 * @return a compiled regular expression
 */
 RegexpStruct *regcomp(const char *exp);
 
 
 
 /**
  * Test the string "string" against the compiled regular expression "prog".
  * @param prog a regular expression
  * @param string the string to compare with
  * @return if "string" do not match the regular expression "prog" return 0, else something else.
  */
 int regexec(register const RegexpStruct *prog, register const char *string);
 
 
 
 /**
  * Undocumented function. 
  */
 void regsub(const RegexpStruct *prog, const char *source, char *dest);
 
 
 /**
  * Undocumented function.
  */
 void regerror(const char *s);
 
 
 
 /**
  * Get value of arguments defined in the regular expression, and the number of arguments.
  * @param regexp a regular expression
  * @param nb_args a pointer on a unsigned int where the number og arguments will be stored
  * @return a char * array. At [0] will be the first argument, [1] the second, etc. or NULL if regular expression defined not argument.
  */
 char **reggetsub(RegexpStruct *regexp, unsigned int *nb_args);
 
 
/**
 * @brief Regexp is about creation and exploitation of regular expressions.
 * RegExp class is compliant with V8 Regexp(3)
 * <br>
 * REGULAR EXPRESSION SYNTAX
 *    A regular expression is zero or more branches, separated by `|'.  It
 *    matches anything that matches one of the branches.
 *
 *    A branch is zero or more pieces, concatenated.  It matches a match for
 *    the first, followed by a match for the second, etc.
 *
 *    A piece is an atom possibly followed by `*', `+', or `?'.  An atom fol-
 *    lowed by `*' matches a sequence of 0 or more matches of the atom.  An
 *    atom followed by `+' matches a sequence of 1 or more matches of the atom.
 *    An atom followed by `?' matches a match of the atom, or the null string.
 *
 *    An atom is a regular expression in parentheses (matching a match for the
 *    regular expression), a range (see below), `.'  (matching any single char-
 *    acter), `^' (matching the null string at the beginning of the input
 *    string), `$' (matching the null string at the end of the input string), a
 *    `\' followed by a single character (matching that character), or a single
 *    character with no other significance (matching that character).
 *
 *    A range is a sequence of characters enclosed in `[]'.  It normally
 *    matches any single character from the sequence.  If the sequence begins
 *    with `^', it matches any single character not from the rest of the
 *    sequence.  If two characters in the sequence are separated by `-', this
 *    is shorthand for the full list of ASCII characters between them (e.g.
 *    `[0-9]' matches any decimal digit).  To include a literal `]' in the
 *    sequence, make it the first character (following a possible `^').  To
 *    include a literal `-', make it the first or last character.
 *
 * AMBIGUITY
 *    If a regular expression could match two different parts of the input
 *    string, it will match the one which begins earliest.  If both begin in
 *    the same place but match different lengths, or match the same length in
 *    different ways, life gets messier, as follows.
 *
 *    In general, the possibilities in a list of branches are considered in
 *    left-to-right order, the possibilities for `*', `+', and `?' are consid-
 *    ered longest-first, nested constructs are considered from the outermost
 *    in, and concatenated constructs are considered leftmost-first.  The match
 *    that will be chosen is the one that uses the earliest possibility in the
 *    first choice that has to be made.  If there is more than one choice, the
 *    next will be made in the same manner (earliest possibility) subject to
 *    the decision on the first choice.  And so forth.
 *
 *    For example, `(ab|a)b*c' could match `abc' in one of two ways.  The first
 *    choice is between `ab' and `a'; since `ab' is earlier, and does lead to a
 *    successful overall match, it is chosen.  Since the `b' is already spoken
 *    for, the `b*' must match its last possibility--the empty string--since it
 *    must respect the earlier choice.
 *    In the particular case where no `|'s are present and there is only one
 *    `*', `+', or `?', the net effect is that the longest possible match will
 *    be chosen.  So `ab*', presented with `xabbbby', will match `abbbb'.  Note
 *    that if `ab*', is tried against `xabyabbbz', it will match `ab' just
 *    after `x', due to the begins-earliest rule.  (In effect, the decision on
 *    where to start the match is the first choice to be made, hence subsequent
 *    choices must respect it even if this leads them to less-preferred alter-
 *    natives.)
 */ 
class Regexp: public Object
{
	private:
		char *regexpString;
		RegexpStruct *regexp;
		
	public:
		
		/**
		 * Create a SlimRegexp object, and compile the specified literal regular expression <i>exp</i>
		 * @param exp a regular expression in text format 
		 */
		Regexp(const char *exp);
		virtual ~Regexp();
		
		/**
		 * Test the string "string" against the compiled regular expression "prog".
		 * @param string the string to compare with the regular expression
		 * @return if "string" do not match the regular expression return 0, else something else.
		 */
		int exec(const char *string);
		
		/**
		 * Get value of arguments defined in the regular expression, and the number of arguments.
		 * @param nb_args a pointer on a unsigned int where the number og arguments will be stored
		 * @return a char * array. At [0] will be the first argument, [1] the second, etc. or NULL if regular expression defined not argument.
		 */
		char **getsub(unsigned int *nb_args);
		
		/**
		 * Get value of arguments defined in the regular expression as an Array of objects of class String.
		 * @return return a pointer onto an object of class Array<String *> if successfull, or NULL if there is no sub expression to get.
		 */
		Array<String *> *getSubArray();
		
		/**
		 * Get a pointer onto a string that contains the textual version of this regular expression
		 */
		const char *getRegexpString();
};

#endif
