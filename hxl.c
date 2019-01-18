/* hxl.c - Copyright (c) 2019, Sijmen J. Mulder (see LICENSE.md) */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <err.h>

#ifdef __OpenBSD__
#include <unistd.h>
#endif

/* character classes. coincidentally, values are colors. */
#define CN 0x37 /* NULL    */
#define CS 0x92 /* SPACE   */
#define CP 0x96 /* PRINT   */
#define CC 0x95 /* CONTROL */
#define CH 0x93 /* HIGH    */

static const unsigned char table[] = {
	CN, CC, CC, CC, CC, CC, CC, CC, CC, CC, CS, CS, CS, CS, CC, CC,
	CC, CC, CC, CC, CC, CC, CC, CC, CC, CC, CC, CC, CC, CC, CC, CC,
	CS, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP,
	CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP,
	CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP,
	CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP,
	CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP,
	CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CC,
	CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH,
	CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH,
	CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH,
	CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH,
	CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH,
	CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH,
	CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH,
	CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH
};

static char *cursor;

#define LINE_APPEND(a) \
	do { \
		memcpy(cursor, a, sizeof(a)-1); \
		cursor += sizeof(a)-1; \
	} while (0)

static void
set_color(int class)
{
	static int last_class = -1;

	if (class == last_class)
		return;

	cursor[0] = '\33';
	cursor[1] = '[';
	cursor[2] = "0123456789abcdef"[class >> 4];
	cursor[3] = "0123456789abcdef"[class & 15];
	cursor[4] = 'm';

	cursor += 5;
}

static void
print_hex(uint8_t b)
{
	set_color(table[b]);

	cursor[0] = "0123456789abcdef"[b >> 4];
	cursor[1] = "0123456789abcdef"[b & 15];
	cursor[2] = ' ';

	cursor += 3;
}

static void
print_char(uint8_t b)
{
	int class = table[b];

	set_color(class);

	*cursor++ =
	    b == ' ' ? ' ' :
	    class == CN ? '0' :
	    class == CS ? '_' :
	    class == CP ? b :
	    '.';
}

static void
process_line(uint8_t data[16])
{
	size_t i;

	i = 0;
	while (i < 8)  print_hex(data[i++]);
	*cursor++ = ' ';
	while (i < 16) print_hex(data[i++]);

	*cursor++ = ' ';

	i = 0;
	while (i < 8)  print_char(data[i++]);
	*cursor++ = ' ';
	while (i < 16) print_char(data[i++]);

	*cursor++ = '\n';
}

static void
process_linepart(uint8_t data[16], size_t count)
{
	size_t i, max_8;

	max_8 = count > 8 ? 8 : count;

	i = 0;
	while (i < max_8) print_hex(data[i++]);
	while (i < 8)     {LINE_APPEND("   "); i++;}
	*cursor++ = ' ';
	while (i < count) print_hex(data[i++]);
	while (i < 16)    {LINE_APPEND("   "); i++;}

	*cursor++ = ' ';

	i = 0;
	while (i < max_8) print_char(data[i++]);
	if (count > 8) {
		*cursor++ = ' ';
		while (i < count) print_char(data[i++]);
	}

	*cursor++ = '\n';
}

int
main(int argc, char **argv)
{
	FILE *file = stdin;
	uint8_t buf[16];
	size_t nread;
	char line[1024];

	if (argc > 2)
		errx(1, "usage: hxl [file]");
	else if (argc == 2 && !(file = fopen(argv[1], "r")))
		err(1, "%s", argv[1]);

#ifdef __OpenBSD__
	pledge("stdio", NULL);
#endif

	while ((nread = fread(buf, 1, 16, file)) == 16) {
		cursor = line;
		process_line(buf);
		fwrite(line, cursor - line, 1, stdout);
	}

	if (nread) {
		cursor = line;
		process_linepart(buf, nread);
		fwrite(line, cursor - line, 1, stdout);
	}

	fputs("\33[0m", stdout);

	if (ferror(file))
		err(1, NULL);

	return 0;
}

