#pragma once

#include <stdio.h>
#include <stdlib.h>

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
