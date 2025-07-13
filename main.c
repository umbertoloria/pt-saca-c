#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "int_list.h"
#include "icfl.h"
#include "prefix_tree.h"

int main() {
	// GENOME
	char w[] = "AAABCAABCADCAABCA";
	int n = strlen(w);

	// ICFL
	printf("Genome: %s \n", w);

	IntList* factors = int_list_create(MAX_ICFL_FACTORS);
	IntList* ff_list = int_list_create(n);
	icfl(w, n, ff_list, factors);
	int_list_free(ff_list);

	int_list_println(factors);

	// PREFIX TREE BUILD
	PTNode *pt_root = pt_create_root();

	return 0;
}
