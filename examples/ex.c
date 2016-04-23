#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scgl.h"

void print_graph(scgl_graph_t * graph, scgl_vertex_t * vertex);

void edge_attr_dump(char *key, void *value, void **fp) {
	if (fp != NULL && *fp != NULL)
		fprintf((FILE*)*fp, "%s %s ", key, (char*)value);
}

static void
print_edge(scgl_edge_t *edge, void **data) 
{
	scgl_graph_t * graph = (scgl_graph_t *) *data;

	// find name edge attribute
	char * name;
	list_head_t attributes = edge->attributes;
	scgl_attr_t * attr = (scgl_attr_t *) attributes.next;
	while(attr != NULL)
	{
			if(strcmp(attr->key, "name") == 0)
			{
				name = 	attr->value;
				break;
			}
			else
				attr = (scgl_attr_t *) attributes.next;
	}
	
	if(name == NULL)
	{
		fprintf(stderr, "No name found for this edge !\n");
		return;
	}
	
	printf("e: %s\n", name);
	
	if(edge->to != NULL)
	{
		scgl_vertex_t * vertex = edge->to;
		//printf("Found vertex: %s\n", vertex->id);
		print_graph(graph, vertex);		
	}
	else
		printf("no vertex at the end of this edge !\n");
	
}

void print_graph(scgl_graph_t * graph, scgl_vertex_t * vertex)
{
	int nb_edges = scgl_vertex_get_edges_out_count(vertex);
	
	printf("v: %s with %d edges\n", vertex->id, nb_edges);
	
	if(nb_edges > 0)
	{
		scgl_vertex_foreach_edge(vertex, 1, print_edge, &graph);
	}	
}

void edge_attr_free(char *key, void *value, void **data) 
{
	printf("deleting attribute with key %s\n", key);
	if(value != NULL) free(value);
}

int main(int argc, char **argv) 
{
	printf("building graph\n");
	
	scgl_vertex_t *v_site, *v_site_name, *v_site_type, *v_state, *v_building1;
	scgl_edge_t *e_site_name, *e_site_type, *e_site_state, *e_site_building1;
	scgl_attr_t *a_qt_hasName, *a_type, *a_isLocatedIn_State, *a_hasLocation_Building;
	
	scgl_graph_t *graph;
	
	printf("creating graph\n");
	graph 			= scgl_graph_create("linkingData", NULL, 0, NULL, 0);
	
	printf("creating vertices\n");
	v_site 			= scgl_vertex_create("boc:BostonOneCampus", NULL, 0, NULL, 0);
	v_site_name  		= scgl_vertex_create("BOC", NULL, 0, NULL, 0);
	v_site_type		= scgl_vertex_create("loc:Site", NULL, 0, NULL, 0);
	v_state			= scgl_vertex_create("loc:Massachusetts", NULL, 0, NULL, 0);
	v_building1		= scgl_vertex_create("boc:BOC_Builidng_800", NULL, 0, NULL, 0);
	
	printf("adding vertices to graph\n");
	scgl_graph_add_vertex(graph, v_site);
	scgl_graph_add_vertex(graph, v_site_name);
	scgl_graph_add_vertex(graph, v_site_type);
	scgl_graph_add_vertex(graph, v_state);
	scgl_graph_add_vertex(graph, v_building1);
	
	printf("creating attributes\n");
	a_qt_hasName 		= scgl_attr_create("name", "qt:hasName");
	a_type 			= scgl_attr_create("name", "@type");
	a_isLocatedIn_State 	= scgl_attr_create("name", "loc:isLocatedIn_State");
	a_hasLocation_Building 	= scgl_attr_create("name", "loc:hasLocation_Building");
	
	printf("creating edges\n");
	e_site_name 		= scgl_edge_create(v_site, v_site_name, 0, 0, &a_qt_hasName, 1);
	e_site_state 		= scgl_edge_create(v_site, v_state, 0, 0, &a_isLocatedIn_State, 1);
	e_site_building1 	= scgl_edge_create(v_site, v_building1, 0, 0, &a_hasLocation_Building, 1);
	e_site_type 		= scgl_edge_create(v_site, v_site_type, 0, 0, &a_type, 1);
	
	printf("printing stats\n");
	printf("nb of vertices %d\n", list_count(&graph->vertexes));
	printf("get id of vertex BOC: %s\n", scgl_graph_get_vertex(graph, "BOC")->id);
	printf("get id of first vertex: %s\n", scgl_graph_get_vertex_at(graph, 0)->id);	

	printf("getting root\n");
	scgl_vertex_t * root_vertex = scgl_graph_get_vertex_at(graph, 0);
	
	FILE *fp = fopen("scores.dat", "w");
	scgl_graph_dump(graph, fp, edge_attr_dump);
	fclose(fp);
	
	printf("calling print graph\n");
	print_graph(graph, root_vertex);

	printf("destroy graph with/out fun\n");
	scgl_graph_destroy(&graph, edge_attr_free);
	
	return 0;
}