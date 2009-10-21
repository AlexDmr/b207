#include <slimxmlpattern.h>
#include <stdio.h>



SxpElement::SxpElement(const char *element_name, const char *element_id)
{
	this->element_name = new char[strlen(element_name)+1]; strcpy(this->element_name, element_name);
	if(element_id != NULL)
	{
		this->element_id = new char[strlen(element_id)+1]; strcpy(this->element_id, element_id);
	}

	element_pattern = NULL;
	match = SXP_FALSE;
}



void SxpElement::setElementPattern(PatternItem *element_pattern)
{
	this->element_pattern = element_pattern;
}



const char *SxpElement::getId() const
{
	if(element_id == NULL)
	{
		return NULL;
	}

	return element_id;
}



long SxpElement::getNbOfMatches(void) const
{
	return matches.size();
}



XmlNode *SxpElement::getMatchAt(int position)
{
	if(position >= (int)matches.size())
	{
		return NULL;
	}

	return matches.getPointerOnItem(position);
}



int SxpElement::isMatch(XmlNode *source, bool toplevel)
{
	int match;

	if(strcmp(element_name, source->getName()) == 0)
	{
		if(element_pattern != NULL)
		{
			match = element_pattern->isMatch(source);
		}
		else
		{
			match = SXP_TRUE;
		}
	}
	else
	{
		match = SXP_FALSE;
	}

	if((match == SXP_TRUE) && (toplevel == SXP_TOPLEVEL_ELEMENT))
	{
		matches.push(source);
	}

	if(this->match == SXP_FALSE)
	{
		this->match = match;
	}

	return match;
}



int SxpElement::evaluate(void) const
{
	return match;
}



void SxpElement::print(void) const
{
	printf(" - %s [%s]\n", element_name, element_id);
}







PatternItem::PatternItem(int op)
{
	this->op = op;
	element = NULL;
}



PatternItem::PatternItem(SxpElement *element)
{
	op = SXPPI_ELEMENT;
	this->element = element;
}



int PatternItem::addOperand(PatternItem *item)
{
	if(op != SXPPI_ELEMENT)
	{
		operands.push(item);
		return SXP_TRUE;
	}

	return SXP_FALSE;
}



void PatternItem::setData(const char *data)
{
	if(data != NULL)
	{
		this->data = new char[strlen(data)+1]; strcpy(this->data, data);
	}
}



int PatternItem::evaluate(void)
{
	switch(op)
	{
		case SXPPI_ELEMENT:
			return element->evaluate();


		case SXPPI_AND:
			{
				PatternItem *pi;
				operands.beginSequentialReading();
				while((pi = operands.readItemPointer()) != NULL)
				{
					if(pi->evaluate() == SXP_FALSE)
					{
						return SXP_FALSE;
					}
				}

				return SXP_TRUE;
			}

		case SXPPI_OR:
			{
				PatternItem *pi;
				operands.beginSequentialReading();
				while((pi = operands.readItemPointer()) != NULL)
				{
					if(pi->evaluate() == SXP_TRUE)
					{
						return SXP_TRUE;
					}
				}

				return SXP_FALSE;
			}
                default: return -1;
	}
}



int PatternItem::isMatch(XmlNode *source)
{
	XmlNode *n;
	const char *simpleContent = source->getSimpleContent();

	switch(op)
	{
		case SXPPI_ELEMENT:

			// printf("isMatch: ELEMENT\nsource: %s\nelement:\n", source->serialize());
			//element->print();
			return element->isMatch(source, SXP_SUB_ELEMENT);


		case SXPPI_COMPLEXCONTENT_EXCLUDE:

			n = source->getFirstChildNode();
			while(n != NULL)
			{
				PatternItem *pi;
				operands.beginSequentialReading();
				while((pi = operands.readItemPointer()) != NULL)
				{
					if(pi->isMatch(n) == SXP_TRUE)
					{
						return SXP_FALSE;
					}
				}

				n = source->getNextChildNode();
			}

			return SXP_TRUE;


		case SXPPI_COMPLEXCONTENT_CONTAIN:

			// printf("isMatch: COMPLEXCONTENT_CONTAIN: source\n");
			{
				PatternItem *pi;
				operands.beginSequentialReading();
				while((pi = operands.readItemPointer()) != NULL)
				{
					n = source->getFirstChildNode();
					while(n != NULL)
					{
						if(pi->isMatch(n) == SXP_TRUE)
						{
							break;
						}

						n = source->getNextChildNode();
					}

					if(n == NULL)
					{
						return SXP_FALSE;
					}
				}

				return SXP_TRUE;
			}

		case SXPPI_SIMPLECONTENT_CONTAIN:

			// printf("isMatch:contain: %s == %s\n", simpleContent, data);
			if((simpleContent != NULL) && (strstr(simpleContent, data) != NULL))
			{
				return SXP_TRUE;
			}

			return SXP_FALSE;


		case SXPPI_SIMPLECONTENT_EQUAL:

			// printf("isMatch:equal: %s == %s\n", simpleContent, data);
			if((simpleContent != NULL) && (strcmp(simpleContent, data) == 0))
			{
				return SXP_TRUE;
			}

			return SXP_FALSE;


		case SXPPI_SIMPLECONTENT_MATCH:

			/* Not Yet supported */
			return SXP_FALSE;


		case SXPPI_SIMPLECONTENT_EXCLUDE:

			if(simpleContent == NULL)
			{
				return SXP_TRUE;
			}
			else if(strstr(simpleContent, data) == NULL)
			{
				return SXP_TRUE;
			}

			return SXP_FALSE;


		case SXPPI_AND:

			{
				PatternItem *pi;
				operands.beginSequentialReading();
				while((pi = operands.readItemPointer()) != NULL)
				{

					// printf("k = %d\n", k);
					if(pi->isMatch(source) == SXP_FALSE)
					{
						// printf("AND return false.\n");
						return SXP_FALSE;
					}
				}

				return SXP_TRUE;
			}


		case SXPPI_OR:

			{
				PatternItem *pi;
				operands.beginSequentialReading();
				while((pi = operands.readItemPointer()) != NULL)
				{

					// printf("k = %d\n", k);
					if(pi->isMatch(source) == SXP_TRUE)
					{
						// printf("AND return false.\n");
						return SXP_TRUE;
					}
				}

				return SXP_FALSE;
			}
	}

	return SXP_FALSE;
}








Pattern::Pattern(const char *pattern_id)
{
	topLevelItem = NULL;
	id = new char[strlen(pattern_id)+1]; strcpy(id, pattern_id);
}


void Pattern::setTopLevelItem(PatternItem *patternItem)
{
	topLevelItem = patternItem;
}


PatternItem *Pattern::getTopLevelItem(void) const
{
	return topLevelItem;
}


const char *Pattern::getId() const
{
	if(id == NULL)
	{
		return NULL;
	}

	return id;
}


long Pattern::getNbOfTopLevelElement(void) const
{
	return topLevelElements.size();
}




SxpElement *Pattern::getTopLevelElementAt(int position)
{
	/*
	if(position >= topLevelElements.size())
	{
		return NULL;
	}
	*/
	return topLevelElements.getPointerOnItem(position);
}




void Pattern::addTopLevelElement(SxpElement *element)
{
	topLevelElements.push(element);
}



void Pattern::print(void) const
{
	printf("# Pattern::print\n");
	printf("----------------\n");
	printf("> TopLevelElements:\n");
	printf("  -----------------\n");
	for(unsigned int k = 0; k < topLevelElements.size(); k++)
	{
		topLevelElements[k].print();
	}

	printf("\n# Source Match ? ");
	if(topLevelItem->evaluate() == SXP_TRUE)
	{
		printf("Yes.\n");
	}
	else
	{
		printf("No.\n");
	}
}







void SlimXmlPattern::init(void)
{
}



SlimXmlPattern::SlimXmlPattern(void)
{
    xmlParser = new SlimXmlParser();
    init();
}



SlimXmlPattern::SlimXmlPattern(SlimXmlParser *xmlParser)
{
    this->xmlParser = xmlParser;
    init();
}



XmlNode *SlimXmlPattern::getMatch(const char *source, Pattern *pattern)
{
    XmlNode *s;

    if((s = xmlParser->parse(source)) == NULL)
    {
        return NULL;
    }

    return getMatch(s,pattern);
}



XmlNode *SlimXmlPattern::getMatch(XmlNode *source, Pattern *pattern)
{
    analyse_element(source, pattern);

    PatternItem *pi = pattern->getTopLevelItem();

    if(pi == NULL)
    {
    	return NULL;
    }

    if(pi->evaluate() == SXP_TRUE)
    {
    	XmlNode *result = new XmlNode(SXP_RESULT_ELEMENT_NAME);
    	result->addAttribute(new XmlAttribute(SXP_ATTRIBUTE_ID, pattern->getId()));

    	for(int k = 0; k < pattern->getNbOfTopLevelElement(); k++)
    	{
    		SxpElement *tle;
    		if((tle = pattern->getTopLevelElementAt(k)) == NULL)
    		{
    			return NULL;
    		}

    		if(tle->getId() != NULL)
    		{
    			XmlNode *element = new XmlNode(tle->getId());
    			for(int l = 0; l < tle->getNbOfMatches(); l++)
    			{
    				element->addChildNode(tle->getMatchAt(l));
    			}

    			result->addChildNode(element);
    		}
    	}

    	return result;
    }

    return NULL;
}



int SlimXmlPattern::getLastError(void) const
{
	return lastError;
}



void SlimXmlPattern::analyse_element(XmlNode *element, Pattern *pattern)
{
	for(int k = 0; k < pattern->getNbOfTopLevelElement(); k++)
	{
		SxpElement *tle = pattern->getTopLevelElementAt(k);
		if(tle != NULL)
		{
			tle->isMatch(element, SXP_TOPLEVEL_ELEMENT);
		}
	}

	XmlNode *child = element->getFirstChildNode();
	while(child != NULL)
	{
		analyse_element(child, pattern);

		child = element->getNextChildNode();
	}

	//printf("#>analyse_element: end of second loop\n");
}



Pattern *SlimXmlPattern::compile_pattern(const char *pattern)
{
	XmlNode *p;
	if((p = xmlParser->parse(pattern)) == NULL)
    {
        return NULL;
    }

	return compile_pattern(p);
}



Pattern *SlimXmlPattern::compile_pattern(XmlNode *pattern)
{
    lastError = SXP_NO_ERROR;

     //printf("Pattern:\n %s\n", pattern->serialize());

    // Test if root element is sxp:pattern. If not, this is not a sxp valid pattern
    if(strcmp(pattern->getName(), SXP_PATTERN) == 0)
    {
        // Yes,
        // Test if this pattern declare a namespace appartenance (required)

        const char *ns = pattern->getAttributeValueByName(SXP_XMLNS);
        if(ns != NULL)
        {
        	// Yes,
        	// Test if this pattern is made with a supported version of sxp schema (required)

			if(strcmp(ns, SXP_XMLNS_CONTENT) == 0)
			{
				//Yes,
				//Get the pattern id
				const char *pattern_id = pattern->getAttributeValueByName(SXP_ATTRIBUTE_ID);
				if(pattern_id == NULL)
				{
					// No id defined for this pattern. This is an error.
					lastError = SXP_ERROR_NO_PATTERN_ID;
					return NULL;
				}

				// Get pattern child element
				XmlNode *n;
				if((n = pattern->getFirstChildNode()) == NULL)
				{
					// No Child node for sxp:pattern: This is not valid
					lastError = SXP_ERROR_UNEXPECTED_END;
					return NULL;
				}


				this->pattern = new Pattern(pattern_id);

				PatternItem *pi;

				// Get child element nature
				const char *op = n->getName();

				if(strcmp(op, SXP_AND) == 0)
				{
					// Child element is an AND operator
					if((pi = compile_toplevel_operator(n, SXPPI_AND)) == NULL)
					{
						return NULL;
					}
				}
				else if(strcmp(op, SXP_OR) == 0)
				{
					// Child element is an OR operator
					if((pi = compile_toplevel_operator(n, SXPPI_OR)) == NULL)
					{
						return NULL;
					}
				}
				else if(strcmp(op, SXP_ELEMENT) == 0)
				{
					//Child element is an element to find
					if((pi = compile_element(n, SXP_TOPLEVEL_ELEMENT)) == NULL)
					{
						return NULL;
					}
				}
				else
				{
					lastError = SXP_UNKNOWN_OPERATOR;
					return NULL;
				}

				this->pattern->setTopLevelItem(pi);
				return this->pattern;
			}
			lastError = SXP_NAMESPACE_NOT_SUPPORTED;
			return NULL;
		}
		lastError = SXP_NO_SCHEMA_REFERENCE;
		return NULL;
    }
    lastError = SXP_NO_PATTERN_TAG;
    return NULL;
}



PatternItem *SlimXmlPattern::compile_toplevel_operator(XmlNode *node, int op)
{
	XmlNode *n;
	PatternItem *pi = new PatternItem(op);

	n = node->getFirstChildNode();
	while(n != NULL)
	{
		const char *op_ = n->getName();

		if(strcmp(op_, SXP_AND) == 0)
		{
			PatternItem *sub_pi;

			if((sub_pi = compile_toplevel_operator(n, SXPPI_AND)) == NULL)
			{
				// if compile_toplevel_operator return NULL, an error occured.
				return NULL;
			}

			pi->addOperand(sub_pi);
		}
		else if(strcmp(op_, SXP_OR) == 0)
		{
			PatternItem *sub_pi;

			if((sub_pi = compile_toplevel_operator(n, SXPPI_OR)) == NULL)
			{
				// if compile_toplevel_operator return NULL, an error occured.
				return NULL;
			}

			pi->addOperand(sub_pi);
		}
		else if(strcmp(op_, SXP_ELEMENT) == 0)
		{
			PatternItem *sub_pi;

			if((sub_pi = compile_element(n, SXP_TOPLEVEL_ELEMENT)) == NULL)
			{
				// if compile_toplevel_element return NULL, an error occured.
				return NULL;
			}

			pi->addOperand(sub_pi);
		}
		else
		{
			lastError = SXP_UNKNOWN_OPERATOR;
			return NULL;
		}

		n = node->getNextChildNode();
	}

	return pi;
}



PatternItem *SlimXmlPattern::compile_element(XmlNode *node, bool toplevel)
{
	const char *element_id = node->getAttributeValueByName(SXP_ATTRIBUTE_ID);
	const char *element_name = node->getAttributeValueByName(SXP_ATTRIBUTE_NAME);

	if(element_name == NULL)
	{
		lastError = SXP_NO_NAME_ATTRIBUTE;
		return NULL;
	}

	//printf("compile_element: %s (%d)\node:\n%s\n", element_name, element_id, node->serialize());

	SxpElement *e = new SxpElement(element_name, element_id);

	XmlNode *n;
	PatternItem *pi;

	if((n = node->getFirstChildNode()) != NULL)
	{
		const char *op = n->getName();

		if(strcmp(op, SXP_AND) == 0)
		{
			if((pi = compile_element_operator(n, SXPPI_AND)) == NULL)
			{
				// if compile_element_operator return NULL, an error occured.
				return NULL;
			}
		}
		else if(strcmp(op, SXP_OR) == 0)
		{
			if((pi = compile_element_operator(n, SXPPI_OR)) == NULL)
			{
				// if compile_element_operator return NULL, an error occured.
				return NULL;
			}
		}
		else if(strcmp(op, SXP_COMPLEXCONTENT_CONTAIN) == 0)
		{
			if((pi = compile_element_operator(n, SXPPI_COMPLEXCONTENT_CONTAIN)) == NULL)
			{
				// if compile_element_operator return NULL, an error occured.
				return NULL;
			}
		}
		else if(strcmp(op, SXP_COMPLEXCONTENT_EXCLUDE) == 0)
		{
			if((pi = compile_element_operator(n, SXPPI_COMPLEXCONTENT_EXCLUDE)) == NULL)
			{
				// if compile_element_operator return NULL, an error occured.
				return NULL;
			}
		}
		else if(strcmp(op, SXP_SIMPLECONTENT_CONTAIN) == 0)
		{
			if((pi = compile_element_operator(n, SXPPI_SIMPLECONTENT_CONTAIN)) == NULL)
			{
				// if compile_element_operator return NULL, an error occured.
				return NULL;
			}
		}
		else if(strcmp(op, SXP_SIMPLECONTENT_EQUAL) == 0)
		{
			if((pi = compile_element_operator(n, SXPPI_SIMPLECONTENT_EQUAL)) == NULL)
			{
				// if compile_element_operator return NULL, an error occured.
				return NULL;
			}
		}
		else if(strcmp(op, SXP_SIMPLECONTENT_MATCH) == 0)
		{
			if((pi = compile_element_operator(n, SXPPI_SIMPLECONTENT_MATCH)) == NULL)
			{
				// if compile_element_operator return NULL, an error occured.
				return NULL;
			}
		}
		else if(strcmp(op, SXP_SIMPLECONTENT_EXCLUDE) == 0)
		{
			if((pi = compile_element_operator(n, SXPPI_SIMPLECONTENT_EXCLUDE)) == NULL)
			{
				// if compile_element_operator return NULL, an error occured.
				return NULL;
			}
		}
		else
		{
			lastError = SXP_UNKNOWN_OPERATOR;
			return NULL;
		}

		e->setElementPattern(pi);
	}

	pi = new PatternItem(e);

	if(toplevel)
	{
		this->pattern->addTopLevelElement(e);
	}

	return pi;
}



PatternItem *SlimXmlPattern::compile_element_operator(XmlNode *node, int op)
{
	XmlNode *n;
	PatternItem *pi = new PatternItem(op);

	switch(op)
	{
		case SXPPI_SIMPLECONTENT_CONTAIN:
		case SXPPI_SIMPLECONTENT_EQUAL:
		case SXPPI_SIMPLECONTENT_MATCH:
		case SXPPI_SIMPLECONTENT_EXCLUDE:
			{
				const char *data = node->getSimpleContent();
				if(data == NULL)
				{
					lastError = SXP_NO_OPERATOR_DATA;
					return NULL;
				}

				pi->setData(data);

				return pi;
			}

		case SXPPI_AND:
		case SXPPI_OR:
		case SXPPI_COMPLEXCONTENT_CONTAIN:
		case SXPPI_COMPLEXCONTENT_EXCLUDE:
			{
				n = node->getFirstChildNode();
				while(n != NULL)
				{
					const char *op_ = n->getName();

					if(strcmp(op_, SXP_AND) == 0)
					{
						PatternItem *sub_pi;

						if((sub_pi = compile_element_operator(n, SXPPI_AND)) == NULL)
						{
							// if compile_toplevel_operator return NULL, an error occured.
							return NULL;
						}

						pi->addOperand(sub_pi);
					}
					else if(strcmp(op_, SXP_OR) == 0)
					{
						PatternItem *sub_pi;

						if((sub_pi = compile_element_operator(n, SXPPI_OR)) == NULL)
						{
							// if compile_toplevel_operator return NULL, an error occured.
							return NULL;
						}

						pi->addOperand(sub_pi);
					}
					else if(strcmp(op_, SXP_COMPLEXCONTENT_CONTAIN) == 0)
					{
						PatternItem *sub_pi;

						if((sub_pi = compile_element_operator(n, SXPPI_COMPLEXCONTENT_CONTAIN)) == NULL)
						{
							// if compile_toplevel_operator return NULL, an error occured.
							return NULL;
						}

						pi->addOperand(sub_pi);
					}
					else if(strcmp(op_, SXP_COMPLEXCONTENT_EXCLUDE) == 0)
					{
						PatternItem *sub_pi;

						if((sub_pi = compile_element_operator(n, SXPPI_COMPLEXCONTENT_EXCLUDE)) == NULL)
						{
							// if compile_toplevel_operator return NULL, an error occured.
							return NULL;
						}

						pi->addOperand(sub_pi);
					}
					else if(strcmp(op_, SXP_ELEMENT) == 0)
					{
						PatternItem *sub_pi;

						if((sub_pi = compile_element(n, SXP_SUB_ELEMENT)) == NULL)
						{
							// if compile_element return NULL, an error occured.
							return NULL;
						}

						pi->addOperand(sub_pi);
					}
					else
					{
						lastError = SXP_UNKNOWN_OPERATOR;
						return NULL;
					}

					n = node->getNextChildNode();
				}

				return pi;
			}
               default: return (PatternItem *)NULL;
	}
}
