#include "SLogger.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE * __log_file = NULL;

void close_logger()
{
	if (__log_file != NULL)
	{
		fclose(__log_file);
		__log_file = NULL;
	}
}

void init_logger(const char *log_file_path)
{
	if (log_file_path == NULL)
	{
		__log_file = fopen("output.log", "a");
	}
	else
	{
		__log_file = fopen(log_file_path, "a");
	}
	atexit(close_logger);
}

void _slog(char *f, int l, char* channel, char *msg, ...)
{
	va_list ap;
	/*echo all logging to stdout*/
	va_start(ap, msg);


	fprintf(stdout, "%s:%i: ", f, l);
	vfprintf(stdout, msg, ap);
	fprintf(stdout, "\n");
	va_end(ap);
	fprintf(stdout, "\n");
	if (__log_file != NULL)
	{
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		char* time[20];
		sprintf(time, "%d-%d-%d %d:%d:%d: ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

		char* formattedChannel[3];
		sprintf(formattedChannel, "%c%s%c%s", '[', channel, ']', " ");

		va_start(ap, msg);
		fprintf(__log_file, "%s:%i: ", f, l);
		vfprintf(__log_file, time, ap);
		vfprintf(__log_file, formattedChannel, ap);
		vfprintf(__log_file, msg, ap);
		fprintf(__log_file, "\n");
		va_end(ap);
	}
}
