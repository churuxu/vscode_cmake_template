
#define PLATFORM "platform: generic"
#define DATE "built on: Wed Jun 12 20:54:05 2019 UTC"

/*
 * Generate compiler_flags as an array of individual characters. This is a
 * workaround for the situation where CFLAGS gets too long for a C90 string
 * literal
 */
static const char compiler_flags[] = {'\0'};