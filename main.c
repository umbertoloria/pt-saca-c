#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ZERO_CHAR '0'

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

void icfl(char *w, int n, IntList *ff_list, char **fs, int fs_len) {
	// Find Prefix
	int prefix_x_length = icfl_find_prefix(w, n);
	if (prefix_x_length == n) {
		// x = w, y = *empty*
		// w is Inverse Lyndon word => w is the only ICFL factor here
		strcpy(fs[0], w);
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
	char str0[] = "                 ";
	char str1[] = "                 ";
	char str2[] = "                 ";
	char str3[] = "                 ";
	char str4[] = "                 ";
	char *l_fs[] = { str0, str1, str2, str3, str4};
	int l_fs_len = 5;
	icfl(bre_plus_y, bre_plus_y_len, ff_list, l_fs, l_fs_len);
	int l_fs_0_len = strlen(l_fs[0]);
	if (l_fs_0_len > last) {
		// FIXME: "l_fs[4]" sacrificed :(
		strcpy(fs[4], l_fs[3]);
		strcpy(fs[3], l_fs[2]);
		strcpy(fs[2], l_fs[1]);
		strcpy(fs[1], l_fs[0]);
		strcpy(fs[0], p);
	} else {
		int p_len = strlen(p);
		for (int i = l_fs_0_len - 1; i >= p_len; --i) {
			l_fs[0][i] = l_fs[0][i - p_len];
		}
		for (int i = 0; i < p_len; ++i) {
			l_fs[0][i] = p[i];
		}
		strcpy(fs[0], l_fs[0]);
		strcpy(fs[1], l_fs[1]);
		strcpy(fs[2], l_fs[2]);
		strcpy(fs[3], l_fs[3]);
		strcpy(fs[4], l_fs[4]);
	}
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
	char str0[] = "                        ";
	char str1[] = "                        ";
	char str2[] = "                        ";
	char str3[] = "                        ";
	char str4[] = "                        ";
	char *fs[] = { str0, str1, str2, str3, str4 };
	int fs_len = 5;

	printf("Genome: %s \n", w);

	IntList* ff_list = int_list_create(n);
	icfl(w, n, ff_list, fs, fs_len);
	int_list_free(ff_list);

	printf(" [0] %s\n", fs[0]);
	printf(" [1] %s\n", fs[1]);
	printf(" [2] %s\n", fs[2]);
	printf(" [3] %s\n", fs[3]);
	printf(" [4] %s\n", fs[4]);

	int icfl_idxs[] = { 0, 0, 0, 0 };
	icfl_idxs[0] = 0;
	int i = 0;
	while (i < fs_len && fs[i] != 0) {
		icfl_idxs[i + 1] = strlen(fs[i]) + icfl_idxs[i];
		++i;
	}

	for (int i = 0; i < 5; ++i) {
		printf("%d ", icfl_idxs[i]);
	}
	printf("\n");

	// PREFIX TREE BUILD
	PTNode *pt_root = pt_create_root();

	return 0;
}

