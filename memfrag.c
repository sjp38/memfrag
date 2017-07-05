#include <err.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

void **alloc_blocks(size_t sz_block, unsigned long nr_blocks)
{
	void **blocks;
	unsigned long i, j;

	blocks = malloc(nr_blocks * sizeof(void *));
	for (i = 0; i < nr_blocks; i++) {
		blocks[i] = malloc(sz_block);
		for (j = 0; j < sz_block / sizeof(unsigned long); j++)
			((unsigned long *)blocks[i])[j] = j;
	}

	return blocks;
}

void make_holes(void **blocks, size_t sz_block, unsigned long nr_blocks,
		unsigned long nr_holes)
{
	unsigned long nr_free = 0;
	unsigned long bidx;

	srand(42);

	while (nr_free < nr_holes) {
		bidx = rand() % nr_blocks;
		if (blocks[bidx] == NULL)
			continue;
		free(blocks[bidx]);
		blocks[bidx] = NULL;
		nr_free++;
	}
}

void free_blocks(void **blocks, unsigned long nr_blocks)
{
	unsigned long i;

	for (i = 0; i < nr_blocks; i++) {
		if (blocks[i] == NULL)
			continue;
		free(blocks[i]);
	}
	free(blocks);
}

int main(int argc, char *argv[])
{
	size_t sz_block;
	unsigned long nr_blocks;
	unsigned long nr_holes;
	void **blocks;
	char tmp;

	if (argc < 4) {
		fprintf(stderr, "Usage: %s <block size> <# of allocs> <# of holes>\n",
				argv[0]);
		exit(1);
	}
	setlocale(LC_NUMERIC, "");

	sz_block = atol(argv[1]);
	nr_blocks = atol(argv[2]);
	nr_holes = atol(argv[3]);

	if (nr_holes > nr_blocks)
		errx(1, "nr_holes %lu is larger than nr_blocks %lu",
				nr_holes, nr_blocks);

	printf("Populates %'lu blocks of %'zu bytes then free %'lu blocks\n",
			nr_blocks, sz_block, nr_holes);

	blocks = alloc_blocks(sz_block, nr_blocks);
	printf("allocation done...\n");

	make_holes(blocks, sz_block, nr_blocks, nr_holes);
	printf("holes made...\n");

	printf("Press any key to exit: ");
	if (scanf("%c", &tmp))
		exit(0);

	free_blocks(blocks, nr_blocks);

	return 0;
}
