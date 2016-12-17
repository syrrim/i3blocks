/*
 * json.c - basic JSON parsing and printing
 * Copyright (C) 2014  Vivien Didelot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bar.h"
#include "block.h"

static inline bool
is_number(const char *str)
{
	char *end;

	strtoul(str, &end, 10);

	/* not a valid number if end is not a null character */
	return !(*str == 0 || *end != 0);
}

static inline void
empty_escape(const char *str)
{
	while (*str) {
		switch (*str) {
		case '"':
		case '\\':
			fprintf(stdout, "\\%c", *str);
			break;
		default:
			fprintf(stdout, "%c", *str);
		}

		str++;
	}
}

static inline void
escape(const char *str)
{
	fprintf(stdout, "\"");
	empty_escape(str);
	fprintf(stdout, "\"");
}

static void
print_prop(const char *key, const char *value, int flags)
{
	/* Only print i3bar-specific properties */
	if (!(flags & PROP_I3BAR))
		return;

	if (!*value)
		return;

	fprintf(stdout, ",\"%s\":", key);

	/* Print as-is (except strings which must be escaped) */
	if (flags & PROP_STRING && flags & PROP_NUMBER && is_number(value))
		fprintf(stdout, "%s", value);
	else if (flags & PROP_STRING)
		escape(value);
	else
		fprintf(stdout, "%s", value);
}

static void
print_block(struct block *block)
{
#define PRINT(_name, _size, _flags) \
	print_prop(#_name, block->updated_props._name, _flags); \

	fprintf(stdout, ",{\"\":\"\"");
	PROPERTIES(PRINT);
	fprintf(stdout, "}");

#undef PRINT
}

/*
 * Parse the <json> input for the key <name> and store the start of its value
 * into <start> and its size into <len>.
 *
 * <start> set to 0 means that the key was not present.
 */
void
json_parse(const char *json, const char *name, int *start, int *len)
{
	const size_t keylen = strlen(name) + 2;
	char key[keylen + 1];
	snprintf(key, sizeof(key), "\"%s\"", name);

	*start = *len = 0;

	char *here = strstr(json, key);
	if (here) {
		here += keylen + 1;

		/* skip pre-value whitespace */
		while (isspace(*here))
			here++;

		if (*here == '"') {
			/* string */
			here++;
			*start = here - json;
			while (*here && *here != '"')
				*len += 1, here++;

			/* invalidate on incomplete string */
			if (*here != '"')
				*start = 0;
		} else {
			/* number */
			*start = here - json;
			while (isdigit(*here++))
				*len += 1;
		}
	}
}

void
json_print_bar(struct bar *bar)
{
	#define OPEN "<span background=\\\"%s\\\" foreground=\\\"%s\\\">" 
	#define CLOSE "</span>"
	#define NUM_COLOS 5
	#define MAX(x, y) (x < y ? y : x)
	#define MIN(x, y) (x > y ? y : x)

	//char * back = "#1E272B";
	char * colos[NUM_COLOS] = {"#1E272B", "#3FD98B", "#0D3813", "#081F1F", "#040E12"};
	char * fgs[NUM_COLOS] = {"#EEEEEE", "#1E272B", "#EEEEEE", "#EEEEEE", "#EEEEEE"};
	int first = 0;
	int second = 0;
	fprintf(stdout, ",[{\"full_text\":\"");
	for(int i = 0; i < bar->num; i++){
		if(i < MIN(bar->num/2,NUM_COLOS)){
			second = i+1;
		}else if(i > MAX(bar->num/2,bar->num - NUM_COLOS)){
			second = bar->num - i - 1;
		}
		struct block *block = bar->blocks + i;
		int text_colo = first;
		//fprintf(stdout, "%d %d\n", first, second);
		if(i > bar->num/2){
			if(first == second){
				fprintf(stdout, OPEN "" CLOSE, colos[second], fgs[second]);
			}
			fprintf(stdout, OPEN "" CLOSE, colos[first], colos[second]);
			text_colo = second;
		}
		if (*FULL_TEXT(block)) {
			fprintf(stdout, OPEN " ", colos[text_colo], fgs[text_colo]);
			empty_escape(block->updated_props.full_text);
			fprintf(stdout, " " CLOSE);
		}
                if(i < bar->num/2){
			if(first == second){
				fprintf(stdout, OPEN "" CLOSE, colos[second], fgs[second]);
			}
			fprintf(stdout, OPEN "" CLOSE, colos[second], colos[first]);
		}
		first = second;
	}
	fprintf(stdout, "\", \"markup\":\"pango\",\"flavor\":\"lemon\"}]\n");
	#undef NUM_COLOS
	#undef OPEN
	#undef CLOSE
	#undef MAX
	#undef MIN
	/*
	fprintf(stdout, ",[{\"full_text\":\"\"}");

	for (int i = 0; i < bar->num; ++i) {
		struct block *block = bar->blocks + i;

		// full_text is the only mandatory key, skip if empty 
		if (!*FULL_TEXT(block)) {
			bdebug(block, "no text to display, skipping");
			continue;
		}

		print_block(block);
	}

	fprintf(stdout, "]\n");
	*/
	fflush(stdout);
}
