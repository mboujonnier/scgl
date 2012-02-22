#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pair_t*
scgl_pair_create(const char *key, void *value) {
	pair_t *p;

	p = (pair_t*) malloc(sizeof(pair_t));
	p->key = (char*) malloc(strlen(key)+1);
	strcpy(p->key, key);
	p->value = value;

	return p;
}

void
pair_destroy(pair_t *pair) {
	if (pair != NULL) {
		free(pair->key);
		free(pair->value);
		free(pair)
		pair = NULL;
	}
}

int
pair_seeker(const void *elem, const void *key) {
	const pair_t *p;
	if (elem == NULL || key == NULL)
		return 0;

	p = (pair_t*) elem;
	return !(strcmp(p->key, (char*)key));
}

int
pair_comparator(const void *a, const void *b) {
	const pair_t *p1, *p2;

	if (a == NULL && b == NULL)
		return 0;
	else if (a == NULL)
		return -1;
	else if (b == NULL)
		return 1;

	p1 = (const pair_t*) a;
	p2 = (const pair_t*) b;

	if (p1->key == NULL && p2->key == NULL)
		return 0;
	else if (p1->key == NULL)
		return -1;
	else if (p2->key == NULL)
		return 1;

	return strcmp(p1->key, p2->key)*(-1);
}