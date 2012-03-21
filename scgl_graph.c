#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simclist.h"
#include "scgl_edge.h"
#include "scgl_pair.h"
#include "scgl_vertex.h"
#include "scgl_graph.h"

scgl_graph_t*
scgl_graph_create(char *id, scgl_vertex_t **vertexes, unsigned int vertexes_n, scgl_edge_t **edges, unsigned int edges_n) {
	scgl_graph_t *g;
	unsigned int i;

	g = (scgl_graph_t*) malloc(sizeof(scgl_graph_t));
	g->id = (char*) malloc(strlen(id)+1);
	strcpy(g->id, id);

	g->vertexes = (list_t*) malloc(sizeof(list_t));
	g->edges = (list_t*) malloc(sizeof(list_t));

	list_init(g->vertexes);
	list_init(g->edges);

	list_attributes_seeker(g->vertexes, scgl_vertex_seeker);
	list_attributes_seeker(g->edges, scgl_edge_seeker);

	list_attributes_comparator(g->vertexes, scgl_vertex_comparator);
	list_attributes_comparator(g->edges, scgl_edge_comparator);

	for (i=0; i<vertexes_n; ++i)
		list_append(g->vertexes,(void*)vertexes[i]);

	for (i=0; i<edges_n; ++i)
		list_append(g->edges, (void*)edges[i]);

	return g;
}

void
scgl_graph_destroy(scgl_graph_t *graph) {
	if (graph != NULL) {
		list_destroy(graph->vertexes);
		list_destroy(graph->edges);
		free(graph->vertexes);
		free(graph->edges);
		free(graph->id);
		free(graph);
		graph = NULL;
	}
}

int
scgl_graph_add_vertex(scgl_graph_t *graph, const scgl_vertex_t *vertex) {
	if (graph == NULL || vertex == NULL)
		return -1;

	list_append(graph->vertexes, vertex);

	return 0;
}

scgl_vertex_t*
scgl_graph_del_veretex(scgl_graph_t *graph, char *vertex_id) {
	if (graph == NULL)
		return NULL;

	scgl_vertex_t *v;
	v = list_seek(graph->vertexes, vertex_id);
	if (v != NULL)
		list_delete(graph->vertexes, v);

	//problem jest taki ze, usuwając node z grafu nie musimy usuwać krawędzi,
	//ale powinniśmy dereference, ale to zaś "psuje" edge bo co to za edge bez reference,
	//moze zamiast dereference usuwac edge z graph - ale to bez sensu
	return v;
}

scgl_vertex_t*
scgl_graph_get_vertex(scgl_graph_t *graph, char *vertex_id) {
	if (graph == NULL || vertex_id == NULL)
		return NULL;
	return list_seek(graph->vertexes, vertex_id);
}

int
scgl_graph_get_vertex_count(const scgl_graph_t *graph) {
	if (graph == NULL)
		return -1;
	return list_size(graph->vertexes);
}

int
scgl_graph_add_edge(scgl_graph_t *graph, const scgl_edge_t *edge) {
	if (graph == NULL || edge == NULL)
		return -1;

	list_append(graph->edges, edge);
	return 0;
}

scgl_edge_t*
scgl_graph_del_edge(scgl_graph_t *graph, const char *edge_id) {
	scgl_edge_t *e;
	e = list_seek(graph->edges, edge_id);
	if (e != NULL) {
		list_delete(graph->edges, e);
		list_delete(e->from->in, e);
		list_delete(e->from->out, e);
	}
	return e;
}

scgl_edge_t*
scgl_graph_get_edge(const scgl_graph_t *graph, const char *edge_id) {
	if (graph == NULL || edge_id == NULL)
		return NULL;
	return list_seek(graph->edges, edge_id);
}

int
scgl_graph_get_edge_count(const scgl_graph_t *graph) {
	if (graph == NULL)
		return -1;
	return list_size(graph->edges);
}
