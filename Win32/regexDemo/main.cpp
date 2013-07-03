#include <stdio.h>
#include <string>
#include "regex.h"

#pragma comment(lib,"regex.lib")
//#pragma comment(lib,"regex-bcc.lib")

#define SUBSLEN 10             
#define EBUFLEN 128         
#define BUFLEN 1024         

using namespace std;
int main()
{
	size_t       len;
	regex_t       re;            
	regmatch_t    subs [SUBSLEN];
	char          matched [BUFLEN];    
	char          errbuf [EBUFLEN];
	int           err, i;
	char          src    [] = "111 <title>Hello World</title> 222";
	char          pattern [] = "<title>(.*)</title>";

	printf("String : %s\n", src);
	printf("Pattern: \"%s\"\n", pattern);

	err = regcomp(&re, pattern, REG_EXTENDED);
	if (err)
	{
		len = regerror(err, &re, errbuf, sizeof(errbuf));
		printf("error: regcomp: %s\n", errbuf);
		return 1;
	}

	printf("Total has subexpression_r_r: %d\n", re.re_nsub);

	err = regexec(&re, src, (size_t) SUBSLEN, subs, 0);
	if (err == REG_NOMATCH)
	{
		printf("Sorry, no match ...\n");
		regfree(&re);
		return 0;
	}
	else if (err)
	{  
		len = regerror(err, &re, errbuf, sizeof(errbuf));
		printf("error: regexec: %s\n", errbuf);
		return 1;
	}


	printf("\nOK, has matched ...\n\n");
	for (i = 0; i <= re.re_nsub; i++)
	{
		len = subs[i].rm_eo - subs[i].rm_so;
		if (i == 0)
		{
			printf ("begin: %d, len = %d   ", subs[i].rm_so, len);
		}
		else
		{
			printf("subexpression_r_r %d begin: %d, len = %d   ", i, subs[i].rm_so, len);
		}

		memcpy (matched, src + subs[i].rm_so, len);
		matched[len] = '\0';
		printf("match: %s\n", matched);
	}


	regfree(&re);

	system("pause");
	return (0);
}