/* test_tdu: unit tests for tdu's pure headers */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "bar.h"
#include "fmt.h"
#include "mounts.h"

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

static void
test_mounts(void)
{
	static const char mtext[] =
	    "/dev/mapper/aegixluks / btrfs rw 0 0\n"
	    "proc /proc proc rw 0 0\n"
	    "/dev/mapper/aegixluks /home btrfs rw 0 0\n"
	    "/dev/nvme0n1p1 /boot vfat rw 0 0\n"
	    "/dev/sdb1 /mnt/my\\040drive ext4 rw 0 0\n"
	    "tmpfs /tmp tmpfs rw 0 0\n";
	Device devs[MNT_MAX];
	char mnts[MNT_MAX][MNT_PATH];
	int n, i, seen = 0;

	n = mounts_parse(mtext, devs, MNT_MAX);
	assert(n == 3);
	assert(!strcmp(devs[0].src, "/dev/mapper/aegixluks"));
	assert(!strcmp(devs[0].name, "aegixluks"));
	assert(!strcmp(devs[0].mnt, "/"));
	assert(!strcmp(devs[0].extra, " +/home"));
	assert(!strcmp(devs[1].name, "nvme0n1p1"));
	assert(!strcmp(devs[1].extra, ""));
	assert(!strcmp(devs[2].mnt, "/mnt/my drive")); /* \040 unescaped */

	n = mounts_all(mtext, mnts, MNT_MAX);
	assert(n == 6);
	for (i = 0; i < n; i++)
		if (!strcmp(mnts[i], "/proc") || !strcmp(mnts[i], "/tmp") ||
		    !strcmp(mnts[i], "/mnt/my drive"))
			seen++;
	assert(seen == 3);
	ok("mounts");
}

int
main(void)
{
	test_fmt_size();
	test_bar();
	test_mounts();
	printf("%d test groups passed\n", ngroups);
	return 0;
}
