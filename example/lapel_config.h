#ifndef LAPEL_CONFIG_H
#define LAPEL_CONFIG_H

#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

extern FILE *logfile;

#define LAPEL_DEBUG(TAG, ...)    do{fprintf(logfile, "D: %s: ", TAG);fprintf(logfile, __VA_ARGS__);fprintf(logfile, "\n");fflush(logfile);}while(0);
#define LAPEL_ERROR(TAG, ...)    do{fprintf(logfile, "E: %s: ", TAG);fprintf(logfile, __VA_ARGS__);fprintf(logfile, "\n");fflush(logfile);}while(0);
#define LAPEL_ASSERT(XX)         assert(XX);

#endif
