/* test_tdu: unit tests for tdu's pure headers */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "bar.h"
#include "fmt.h"

static int ngroups = 0;

static void
ok(const char *name)
{
	ngroups++;
	printf("PASS %s\n", name);
}

static void
test_fmt_size(void)
{
	char b[8];

	fmt_size(0ULL, b, sizeof b);             assert(!strcmp(b, "0B"));
	fmt_size(512ULL, b, sizeof b);           assert(!strcmp(b, "512B"));
	fmt_size(986710016ULL, b, sizeof b);     assert(!strcmp(b, "941M"));
	fmt_size(1073741824ULL, b, sizeof b);    assert(!strcmp(b, "1.0G"));
	fmt_size(334958692991ULL, b, sizeof b);  assert(!strcmp(b, "311G"));
	fmt_size(1649267441664ULL, b, sizeof b); assert(!strcmp(b, "1.5T"));
	ok("fmt_size");
}

static void
test_bar(void)
{
	assert(bar_cells(0.0, 24) == 0);
	assert(bar_cells(1.0, 24) == 24);
	assert(bar_cells(0.67, 24) == 16);
	assert(bar_cells(1.7, 24) == 24);          /* clamp */
	assert(frac_used(0, 0) == 0.0);            /* zero-block fs */
	assert(frac_used(67, 33) == 0.67);
	assert(bar_color(0.69, 0.70, 0.90, 1, 2, 3) == 1);
	assert(bar_color(0.70, 0.70, 0.90, 1, 2, 3) == 2);
	assert(bar_color(0.95, 0.70, 0.90, 1, 2, 3) == 3);
	ok("bar");
}

int
main(void)
{
	test_fmt_size();
	test_bar();
	printf("%d test groups passed\n", ngroups);
	return 0;
}
