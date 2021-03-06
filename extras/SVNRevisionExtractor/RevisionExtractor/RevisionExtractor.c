#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

char build_host[100];
char build_user[100];
int g_revision = 0;
int g_oldRevision = -1;

int main(int argc, char* argv[])
{
	FILE * pFile = fopen("../.svn/entries", "r");
	printf("SVN Revision Extractor\n");
	if( pFile == NULL )
	{
		printf("Could not open entries file. This may not be a SVN checkout? Assuming revision 0.\n");
		return 1;
	}
	else
	{
		char * p;
		char * q;
		char * buffer;
		long sz;

		fseek( pFile, 0, SEEK_END );
		sz = ftell( pFile );
		fseek( pFile, 0, SEEK_SET );

		buffer = (char*)malloc( sz );
		if( buffer != NULL )
		{
			fread( buffer, sz, 1, pFile );

			p = strstr( buffer, "dir" );
			if( p != NULL )
			{
				while( !isdigit( *p ) )
					++p;

				q = p;
				while( isdigit( *q ) )
					++q;

				*q = 0;

				g_revision = atoi( p );
				printf("Revision found: %u\n", g_revision);
			}

			free( buffer );
		}

		fclose ( pFile );
		printf("Opening existing file...\n");
		
		pFile = fopen("svn_revision.h", "r");
		if( pFile != NULL )
		{
			fseek( pFile, 0, SEEK_END );
			sz = ftell( pFile );
			fseek( pFile, 0, SEEK_SET );

			buffer = (char*)malloc( sz );
			if( buffer != NULL )
			{
				fread( buffer, sz, 1, pFile );
				p = strstr( buffer, "BUILD_REVISION ");
				if( p != NULL )
				{
					while( !isdigit( *p ) )
						++p;

					q = p;
					while( isdigit( *q ) )
						++q;

					*q = 0;

					g_oldRevision = atoi( p );
				}

				free( buffer );
			}

			fclose( pFile );
		}
	}

	if( g_oldRevision == g_revision )
	{
		printf("Revisions match exiting file. Skipping.\n");
		return 0;
	}

	printf("Writing output file...\n");
	
	pFile = fopen( "svn_revision.h", "w" );
	if( pFile == NULL )
	{
		printf("Output file could not be opened.\n");
		return 2;
	}
	else
	{
		DWORD len = 100;
		GetComputerName(build_host, &len);
		len = 100;
		GetUserName(build_user, &len);

		fprintf(pFile, "// This file was automatically generated by the SVN revision extractor.\n");
		fprintf(pFile, "// There is no need to modify it.\n");
		fprintf(pFile, "\n");
		fprintf(pFile, "#ifndef SVN_REVISION_H\n");
		fprintf(pFile, "#define SVN_REVISION_H\n\n");
		fprintf(pFile, "#define BUILD_REVISION %u\n", g_revision);
		fprintf(pFile, "#define BUILD_HOST \"%s\"\n", build_host);
		fprintf(pFile, "#define BUILD_USER \"%s\"\n", build_user);
		fprintf(pFile, "#define BUILD_TIME __TIME__\n");
		fprintf(pFile, "#define BUILD_DATE __DATE__\n\n");
		fprintf(pFile, "#endif\t\t // SVN_REVISION_H\n\n");
		fflush( pFile );
		fclose( pFile );

		printf("Output file written. Exiting cleanly.\n");
	}
	return 0;
}