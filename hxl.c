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

static const char hex[] = "0123456789abcdef";

static const unsigned char class_table[] = {
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

static const char char_table[] =
	"0.........____.................."
	" !\"#$%&'()*+,-./0123456789:l<=>?"
	"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
	"`abcdefghijklmnopqrstuvwxyz{|}~."
	"................................"
	"................................"
	"................................"
	"................................";

static char *cursor;

static void
set_color(int class)
{
	static int last_class = -1;

	if (class == last_class)
		return;

	cursor[0] = '\33';
	cursor[1] = '[';
	cursor[2] = hex[class >> 4];
	cursor[3] = hex[class & 15];
	cursor[4] = 'm';

	cursor += 5;
}

static void
print_offset(size_t off)
{
	int shift;

	cursor[0] = '\33';
	cursor[1] = '[';
	cursor[2] = '0';
	cursor[3] = 'm';
	cursor += 4;

	for (shift = 28; shift >= 0; shift -= 4)
		*cursor++ = hex[(off >> shift) & 15];

	*cursor++ = ' ';
	*cursor++ = ' ';
}

static void
print_hex(uint8_t b)
{
	set_color(class_table[b]);

	cursor[0] = hex[b >> 4];
	cursor[1] = hex[b & 15];
	cursor[2] = ' ';

	cursor += 3;
}

static void
print_char(uint8_t b)
{
	int class = class_table[b];

	set_color(class);
	*cursor++ = char_table[b];
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
	while (i < 8) {
		cursor[0] = ' ';
		cursor[1] = ' ';
		cursor[2] = ' ';
		cursor += 3;
		i++;
	}
	*cursor++ = ' ';
	while (i < count) print_hex(data[i++]);
	while (i < 16) {
		cursor[0] = ' ';
		cursor[1] = ' ';
		cursor[2] = ' ';
		cursor += 3;
		i++;
	}

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
	size_t offset = 0, nread;
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
		print_offset(offset);
		process_line(buf);
		fwrite(line, cursor - line, 1, stdout);
		offset += 16;
	}

	if (nread) {
		cursor = line;
		print_offset(offset);
		process_linepart(buf, nread);
		fwrite(line, cursor - line, 1, stdout);
	}

	fputs("\33[0m", stdout);

	if (ferror(file))
		err(1, NULL);

	return 0;
}

