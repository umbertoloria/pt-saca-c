#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ZERO_CHAR '0'

// ICFL
void icfl_find_prefix(char *w, int n, char *x, char *y) {
	// Parameter "n": length of "w"

	if (n == 1) {
		// x = w + '0'
		x[0] = w[0];
		x[1] = ZERO_CHAR;
		x[2] = 0;
		// y = *empty*
		y[0] = 0;
		return;
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
		// x = w + '0'
		strcpy(x, w);
		x[n] = ZERO_CHAR;
		x[n + 1] = 0;
		// y = *empty*
		y[0] = 0;
		return;
	}

	// x = w[:j + 1]
	strncpy(x, w, j + 1);
	x[j + 1] = 0;
	// y = w[j + 1:]
	strcpy(y, w + j + 1);
	/*
	   printf("ICFL_FIND_PREFIX: from w=\"%s\" with result 0=\"%s\", 1=\"%s\" \n", w, x, y);
	   */
}

void icfl_get_failure_function(char *s, int s_inner_size, int *f, int *f_len) {
	int m = s_inner_size;
	for (size_t i = 0; i < m; ++i) {
		f[i] = 0;
	}
	*f_len = m;
	int i = 1;
	int j = 0;
	while (i < m) {
		if (s[j] == s[i]) {
			f[i] = j + 1;
			++i;
			++j;
		} else if (j > 0) {
			j = f[j - 1];
		} else {
			f[i] = 0;
			++i;
		}
	}
	/*
	   printf("ICFL_GET_FAILURE_FUNCTION: got s=\"%s\" and s_inner_size=%d, results f with %d items\n", s, s_inner_size, *f_len);
	   for (int i = 0; i < *f_len; ++i) {
	   printf("%d ", f[i]);
	   }
	   printf("\n");
	   */
}

void icfl_find_bre(char *x, char *y, char *p, char *bre, int *res_last) {
	char w[512]; // TODO: Capacity of "w" should be the sum of "x" and "y" capacities
	strcpy(w, x);
	strcat(w, y); // TODO: We already know the "x" size, don't we?
	/*
	   printf("ICFL_FIND_BRE: from x=\"%s\" and y=\"%s\", made w=\"%s\"\n", x, y, w);
	   */
	int w_len = strlen(w);
	int x_len = strlen(x);
	int n = x_len - 1;

	// Get Failure Function
	int f[512];
	int f_len = 0;
	icfl_get_failure_function(x, n, f, &f_len);

	int i = n - 1;
	int last = n;

	while (i >= 0) {
		if (w[f[i]] < x[x_len - 1]) {
			last = f[i] - 1;
		}
		i = f[i] - 1;
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

void icfl(char *w, int n, char **fs, int fs_len) {
	// Find Prefix
	char x[256] = "";
	char y[256] = "";
	icfl_find_prefix(w, n, x, y);

	if (
			x[n + 1] == 0
			&& x[n] == ZERO_CHAR
			&& strncmp(x, w, n - 1) == 0
	   ) {
		strcpy(fs[0], w);
		return;
	}

	// Find Bre
	char p[256];
	char bre[256];
	int last;
	icfl_find_bre(x, y, p, bre, &last);
	/*
	   printf("ICFL: icfl_find_bre with results p=\"%s\", bre=\"%s\", last=%d\n", p, bre, last);
	   */

	// Recursive ICFL
	char *bre_plus_y = (char*) malloc(512);
	strcpy(bre_plus_y, bre);
	strcat(bre_plus_y, y);
	int bre_plus_y_len = strlen(bre_plus_y);
	char str0[] = "                 ";
	char str1[] = "                 ";
	char str2[] = "                 ";
	char str3[] = "                 ";
	char str4[] = "                 ";
	char *l_fs[] = { str0, str1, str2, str3, str4};
	int l_fs_len = 5;
	icfl(bre_plus_y, bre_plus_y_len, l_fs, l_fs_len);
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

	icfl(w, n, fs, fs_len);

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

