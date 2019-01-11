/* hxl.c - Copyright (c) 2019, Sijmen J. Mulder (see LICENSE.md) */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <err.h>

#ifdef __OpenBSD__
#include <unistd.h>
#endif

enum {
	C_NULL,
	C_SPACE,
	C_PRINT,
	C_CONTROL,
	C_HIGH
};

#define ESC_NULL 	"\33[0;2;37m"
#define ESC_SPACE	"\33[0;32m"
#define ESC_PRINT	"\33[0;36m"
#define ESC_CONTROL	"\33[0;35m"
#define ESC_HIGH	"\33[0;33m"

static int lastclass = -1;
static char line[1024];
static char *cursor;

#define LINE_APPEND(a) \
	do { \
		memcpy(cursor, a, sizeof(a)-1); \
		cursor += sizeof(a)-1; \
	} while (0)

static int
classify(uint8_t b)
{
	if (b == 0)	return C_NULL;
	if (b < 10)	return C_CONTROL;
	if (b < 14 || b == ' ') return C_SPACE;
	if (b < 32)	return C_CONTROL;
	if (b < 127)	return C_PRINT;
	if (b == 127)	return C_CONTROL;
	return C_HIGH;
}

static void
set_color(int class)
{
	if (class == lastclass)
		return;

	lastclass = class;

	switch (class) {
	case C_NULL:	LINE_APPEND(ESC_NULL); break;
	case C_SPACE:	LINE_APPEND(ESC_SPACE); break;
	case C_PRINT:	LINE_APPEND(ESC_PRINT); break;
	case C_CONTROL:	LINE_APPEND(ESC_CONTROL); break;
	case C_HIGH:	LINE_APPEND(ESC_HIGH); break;
	}

}

static void
print_hex(uint8_t b)
{
	set_color(classify(b));

	cursor[0] = "0123456789abcdef"[b >> 4];
	cursor[1] = "0123456789abcdef"[b & 15];
	cursor[2] = ' ';

	cursor += 3;
}

static void
print_char(uint8_t b)
{
	int class = classify(b);

	set_color(class);

	*cursor++ =
	    b == ' ' ? ' ' :
	    class == C_NULL ? '0' :
	    class == C_SPACE ? '_' :
	    class == C_PRINT ? b :
	    '.';
}

int
main(int argc, char **argv)
{
	FILE *file = stdin;
	uint8_t buf[16];
	size_t nread, max_8, i;

	if (argc > 2)
		errx(1, "usage: hxl [file]");
	else if (argc == 2 && !(file = fopen(argv[1], "r")))
		err(1, "%s", argv[1]);

#ifdef __OpenBSD__
	pledge("stdio", NULL);
#endif

	while ((nread = fread(buf, 1, 16, file)) == 16) {
		cursor = line;

		i = 0;
		while (i < 8)  print_hex(buf[i++]);
		*cursor++ = ' ';
		while (i < 16) print_hex(buf[i++]);

		*cursor++ = ' ';

		i = 0;
		while (i < 8)  print_char(buf[i++]);
		*cursor++ = ' ';
		while (i < 16) print_char(buf[i++]);

		*cursor++ = '\n';
		fwrite(line, cursor - line, 1, stdout);
	}

	if (nread) {
		max_8 = nread > 8 ? 8 : nread;
		cursor = line;

		i = 0;
		while (i < max_8) print_hex(buf[i++]);
		while (i < 8)     {LINE_APPEND("   "); i++;}
		*cursor++ = ' ';
		while (i < nread) print_hex(buf[i++]);
		while (i < 16)    {LINE_APPEND("   "); i++;}

		*cursor++ = ' ';

		i = 0;
		while (i < max_8) print_char(buf[i++]);
		if (nread > 8) {
			*cursor++ = ' ';
			while (i < nread) print_char(buf[i++]);
		}

		*cursor++ = '\n';
		fwrite(line, cursor - line, 1, stdout);
	}

	fputs("\33[0m", stdout);

	if (ferror(file))
		err(1, NULL);

	return 0;
}

