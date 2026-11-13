/* test_tdu: unit tests for tdu's pure headers */
#include <assert.h>
#include <stdio.h>
#include <string.h>

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

int
main(void)
{
	test_fmt_size();
	printf("%d test groups passed\n", ngroups);
	return 0;
}
