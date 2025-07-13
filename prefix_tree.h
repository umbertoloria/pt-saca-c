#pragma once

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
