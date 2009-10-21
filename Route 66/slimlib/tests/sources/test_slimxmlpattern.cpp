#include <slimcore.h>
#include <slimxml.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    SlimXmlParser xmlParser;
    SlimXmlPattern sxp;
    SlimFile *pattern_file, *source_file;
    
    printf("# Start XML Pattern matching demo...\n");
    if(argv[1] == NULL)
    {
        printf("# ERROR:> filename parameter is missing on command line.\n");
        return 0;
    }
    else
    {
        long size;
        int err;
        
        printf("# Reading %s...", argv[1]);
        pattern_file = new SlimFile(argv[1]);
        err = pattern_file->getLastError();
        if(err != 0)
        {
            printf("\n# ERROR:> can't open %s (error #%d).\n", argv[1], err);
            return 0;
        }
        
        char *pattern = pattern_file->readContent(&size);
        printf("Ok.\n");
        
        //printf("# pattern read:\n%s\n",pattern);
        
        /*
        XmlNode *temp = xmlParser.parse(pattern);
        printf("# end of parsing.\n");
        printf("# pattern reserialize:\n %s\n", temp->serialize());
        
        return 0; */
        
        Pattern *p = sxp.compile_pattern(pattern);
        
        if(sxp.getLastError() != SXP_NO_ERROR)
        {
        	printf("# ERROR:> error code #%d\n", sxp.getLastError());
        }
        
        printf("# Reading %s...", argv[2]);
        source_file = new SlimFile(argv[2]);
        err = source_file->getLastError();
        if(err != 0)
        {
            printf("\n# ERROR:> can't open %s (error #%d).\n", argv[2], err);
            return 0;
        }
        
        char *source = source_file->readContent(&size);
        printf("Ok.\n");
        
        XmlNode *result = sxp.getMatch(source, p);
        
     	if(result != NULL)
     	{
     		printf("# Result:\n%s\n", result->serialize());
     	}
     	else
     	{
     		printf("# No Result.\n");
     		p->print();
     	}
        
        printf("\nEnd of Job.\n");
       
    }
    
    return 0; 
}
