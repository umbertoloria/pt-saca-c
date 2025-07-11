#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ZERO_CHAR '0'
#define MAX_ICFL_FACTORS 256

// INT LIST
typedef struct IntList {
	int *list;
	int length;
} IntList;
IntList *int_list_create(int capacity) {
	IntList *int_list = (IntList *) malloc(sizeof(struct IntList));
	int_list->list = (int *) malloc(capacity);
	int_list->length = 0;
	return int_list;
}
void int_list_free(IntList *int_list) {
	free(int_list->list);
	free(int_list);
}
/*void int_list_expand(IntList *dst, IntList *src) {
	for (size_t i = 0; i < src->length; ++i) {
		dst->list[dst->length] = src->list[i];
		dst->length++;
	}
}*/
void int_list_append(IntList* dst, int new_value) {
	dst->list[dst->length] = new_value;
	dst->length++;
}
void int_list_println(IntList* dst) {
	printf("(%d", dst->list[0]);
	for (size_t i = 1; i < dst->length; ++i) {
		printf(", %d", dst->list[i]);
	}
	printf(")\n");
}

// ICFL
int icfl_find_prefix(char *w, int n) {
	// Parameter "n": length of "w"
	// Result: if w = xy, this function returns |x| (and so |y|=|w|-|x|)

	if (n == 1) {
		// Here w is an Inverse Lyndon word with only one letter:
		//   x = w
		//   y = *empty*
		return n;
	}

	int i = 0;
	int j = 1;
	while (j < n - 1 && w[j] <= w[i]) {
		if (w[j] < w[i]) {
			i = 0;
		} else {
			++i;
		}
		++j;
	}

	if (j == n - 1 && w[j] <= w[j]) {
		// Here w is an Inverse Lyndon word:
		//   x = w
		//   y = *empty*
		return n;
	}

	// x = w[:j + 1]
	// y = w[j + 1:]

	/*
	char *x = (char *) malloc(j + 1);
	strncpy(x, w, j + 1);
	x[j + 1] = 0;
	char *y = w + j + 1;
	printf("ICFL_FIND_PREFIX: from w=\"%s\" with result x=\"%s\", y=\"%s\" \n", w, x, y);
	*/
	return j + 1;
}

void icfl_get_failure_function(char *s, int s_inner_length, IntList *ff_list) {
	int m = s_inner_length;
	for (size_t i = 0; i < m; ++i) {
		ff_list->list[i] = 0;
	}
	ff_list->length = m;
	int i = 1;
	int j = 0;
	while (i < m) {
		if (s[j] == s[i]) {
			ff_list->list[i] = j + 1;
			++i;
			++j;
		} else if (j > 0) {
			j = ff_list->list[j - 1];
		} else {
			ff_list->list[i] = 0;
			++i;
		}
	}
	/*
	printf("ICFL_GET_FAILURE_FUNCTION: got s=\"%s\" and s_inner_size=%d, results f with %d items\n", s, s_inner_size, ff_list->length);
	for (int i = 0; i < ff_list->length; ++i) {
		printf("%d ", ff_list->list[i]);
	}
	printf("\n");
	*/
}

void icfl_find_bre(char *w, int w_len, char *x, char *y, IntList *ff_list, char *p, char *bre, int *res_last) {
	/*
	printf("ICFL_FIND_BRE: from x=\"%s\" and y=\"%s\", made w=\"%s\"\n", x, y, w);
	*/
	int x_len = strlen(x);
	char x_last_char = x[x_len - 1];
	int n = x_len - 1;

	// Get Failure Function
	icfl_get_failure_function(x, n, ff_list);
	int i = n - 1;
	int last = n;
	while (i >= 0) {
		if (w[ff_list->list[i]] < x_last_char) {
			last = ff_list->list[i] - 1;
		}
		i = ff_list->list[i] - 1;
	}

	int sep1 = n - last - 1;
	if (sep1 < 0) sep1 += w_len;
	int sep2 = n + 1;

	// p = w[:sep1]
	strncpy(p, w, sep1);
	p[sep1] = 0;
	// bre = *empty*
	bre[0] = 0;
	if (sep2 > sep1) {
		// bre = w[sep1:sep2]
		i = 0;
		for (int h = sep1; h < sep2; ++h) {
			bre[i++] = w[h];
		}
		bre[i] = 0;
	}
	*res_last = last + 1;
}

void icfl(char *w, int n, IntList *ff_list, IntList* factors) {
	// Find Prefix
	int prefix_x_length = icfl_find_prefix(w, n);
	if (prefix_x_length == n) {
		// x = w, y = *empty*
		// w is Inverse Lyndon word => w is the only ICFL factor here
		int_list_append(factors, n);
		return;
	}

	// Find Bre
	char *prefix_x = (char *) malloc(prefix_x_length);
	strncpy(prefix_x, w, prefix_x_length);
	prefix_x[prefix_x_length] = 0;
	char *prefix_y = w + prefix_x_length;
	char p[256];
	char bre[256];
	int last;
	icfl_find_bre(w, n, prefix_x, prefix_y, ff_list, p, bre, &last);
	free(prefix_x);

	// Recursive ICFL
	char *bre_plus_y = (char*) malloc(512);
	strcpy(bre_plus_y, bre);
	strcat(bre_plus_y, prefix_y);
	int bre_plus_y_len = strlen(bre_plus_y);
	IntList* l_factors = int_list_create(MAX_ICFL_FACTORS);
	icfl(bre_plus_y, bre_plus_y_len, ff_list, l_factors);
	int p_len = strlen(p);
	int l_fs_0_len = l_factors->list[0];
	if (l_fs_0_len > last) {
		int_list_append(factors, p_len);
	}
	for (size_t i = 0; i < l_factors->length; ++i) {
		int_list_append(factors, l_factors->list[i]);
	}
	if (l_fs_0_len <= last) {
		factors->list[0] += p_len;
	}
	int_list_free(l_factors);
	/*
	printf("ICFL on w=%s, result: ", w);
	int_list_println(factors);
	*/
}

// PREFIX TREE
typedef struct PTNodeChild PTNodeChild;
typedef struct PTNode {
	int *rks;
	int rks_size;
	PTNodeChild *children;
} PTNode;
typedef struct PTNodeChild {
	// Both included.
	char *prefix_p;
	char *prefix_q;
	PTNode* node;
} PTNodeChild;

PTNode* pt_create_root() {
	PTNode* result = (PTNode*) malloc(sizeof(PTNode));
	result->rks = 0;
	result->rks_size = 0;
	result->children = 0;
	return result;
}

// MAIN
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

