#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
 
struct node
{
  size_t id;
  size_t degree;
  struct node *next_node;
  struct neighbor *next_neighbor; //head listy sąsiadów
};
typedef struct node node;
 
struct graph
{
  size_t n_nodes; // counter
  size_t n_links;
  struct node* next_node; // head listy węzłów
}; 
typedef struct graph graph;

struct neighbor
{
  size_t id;
  struct neighbor *next_neighbor;
};
typedef struct neighbor neighbor;
 
node *new_node (size_t id)
{
  node *new_node = (node*)malloc(sizeof(node));
    
  new_node->id = id;
  new_node->degree = 0;
  new_node->next_node = NULL;
    
  return new_node;
}
 
 
graph *new_graph (size_t n_nodes)
{
  graph *new_graph = (graph*)malloc(sizeof(graph));
     
  new_graph->n_links = 0;
  new_graph->n_nodes = n_nodes; //counter
  new_graph->next_node = NULL; //head

  node *curr = NULL; //bieżący

  size_t id;
  for(int i=0; i<n_nodes; i++)
  {
    node *nowy = new_node(id); //tworzymy węzeł
    if (curr) //jeśli bieżący na coś wskazuje
      curr->next_node = nowy; //wskaźnik bieżącego ustawiamy na ten nowy stworzony
    else //jeśli bieżący na nic nie wskazuje (lista pusta)
      new_graph->next_node = nowy; //na początku listy w grafie wstawiamy nowy węzeł
    curr = nowy; //ustawiamy żeby bieżący był na nowym węźle 
  }
     
  return new_graph;
}

neighbor *new_neighbor (size_t id)
{
  neighbor *new_neighbor = (neighbor*)malloc(sizeof(neighbor));

  new_neighbor->id = id;
  new_neighbor->next_neighbor = NULL;

  return new_neighbor;
}

void delete_node (node *n)
{
  while (n)
  {
    node *nastepny = n->next_node; //tworzymy węzeł krok za tym n
    while (n->next_neighbor)
    {
    neighbor *nastepny_n = n->next_neighbor;
    free (n->next_neighbor);
    n->next_neighbor = nastepny_n;
    }
    free (n); //usuwamy n który jest przed następnym
    n = nastepny; //ustawiamy się na tym za usuniętym
  }
}

void delete_graph (graph *g)
{
  while (g)
  {
    for(int i=0; i<g->n_nodes; i++)
    {
      delete_node (g->next_node);
    }
    free (g);
  }
}

neighbor *get_neighbor (node *n, size_t index)
{
  neighbor *curr = n->next_neighbor; //bieżący na headzie
  int i =0;
    
  for(i =0; curr != NULL && i < index; i++)
  {
    curr = curr->next_neighbor;
  }

  if(curr==NULL)
    return NULL;

  if(i < index || i> n->degree-1)
  {
    fprintf(stderr, "Nie istnieje sąsiad na danej pozycji\n");
    return NULL;
  }
  
  return curr;
}

neighbor *get_neighbor_by_id (node *n, size_t id)
{
  neighbor *curr = n->next_neighbor;
    
  while(curr != NULL && id != curr->id)
  {
    curr = curr->next_neighbor;
  }
  
  if(curr==NULL)
    return NULL;

  return curr;
}

node *get_node(graph *g, size_t index)
{
  node *curr = g->next_node; //bieżący na headzie
  int i =0;
    
  for(i =0; curr != NULL && i < index; i++)
  {
    curr = curr->next_node;
  }

  if(curr==NULL)
    return NULL;

  if(i < index || i> index)
  {
    fprintf(stderr, "Nie istnieje węzeł na danej pozycji\n");
    return NULL;
  }
  
  return curr;
}
 
node* get_node_by_id(graph *g, size_t id)
{
  node *curr = g->next_node;
    
  while(curr != NULL && id != curr->id)
  {
    curr = curr->next_node;
  }
  
  if(curr==NULL)
    return NULL;

  return curr;
}

bool add_neighbor (node *n, size_t id)
{
  neighbor *my_neighbor = new_neighbor(id);

  if (n->next_neighbor == NULL)
  {
    n->next_neighbor = my_neighbor;
    return true;
  }
    
  neighbor *curr = n->next_neighbor;
  if(curr->id == id)
  {
    fprintf(stderr, "Węzeł już posiada sąsiada o podanym id\n");
    return false;
  }
    
    
  while(curr->next_neighbor != NULL)
  {
    curr = curr->next_neighbor; //przechodzę przez całą listę sąsiadów aż do końca
    if(curr->id == id)
    {
      fprintf(stderr, "Węzeł już posiada sąsiada o podanym id\n");
      return false;
    }
  }
    
  curr->next_neighbor = my_neighbor; //wstawiam sąsiada na koniec
  n->degree ++; //aktualizuje stopień węzła
  return true;
}

bool remove_neighbor (node *n, size_t id)
{
  if (n->next_neighbor == NULL) //jeśli lista sąsiadów jest pusta
  {
    return false;
  }
    
  neighbor *curr = n->next_neighbor;
  neighbor *temp = NULL; 
    
  while(curr != NULL && curr->id != id)
  {
    temp = curr;
    curr = curr->next_neighbor; //bieżący jest za tymczasowym
  }

  if(curr == NULL)
    return false;
    
  temp->next_neighbor = curr->next_neighbor;
  free (curr);
  n->degree --;
  return true;
}

node *add_node(graph *g, size_t id)
{
  node *my_node = new_node(id);
    
  if (g->next_node == NULL)
  {
    g->next_node = my_node;
    return my_node;
  }
    
  node *curr = g->next_node;
  if(curr->id == id)
  {
    return NULL;
  }
    
    
  while(curr->next_node != NULL)
  {
    curr = curr->next_node; //przechodzę przez całą listę aż do końca
    if(curr->id == id)
    {
      fprintf(stderr, "W grafie już znajduje się węzeł o podanym id\n");
      return NULL;
    }
  }
    
  curr->next_node = my_node; //wstawiam węzeł na koniec
  g->n_nodes ++; //aktualizuje ilość węzłów w grafie
  return my_node;
}
 
node *remove_node (graph *g, size_t id)
{
  if (g->next_node == NULL) //jeśli lista jest pusta
  {
    return NULL;
  }
    
  node *curr = g->next_node;
  node *temp = NULL; 
    
  while(curr != NULL && curr->id != id)
  {
    temp = curr;
    curr = curr->next_node; //bieżący jest za tymczasowym
  }

  if(curr == NULL)
    return NULL;
    
  temp->next_node = curr->next_node;
  free (curr);
  g->n_nodes --; //zmniejsza liczbę węzłów w grafie
  return temp;

}

void print_neighbors (FILE *stream, node *n, char *sep)
{
  neighbor *curr = n->next_neighbor;
  while (curr)
  {
    fprintf (stream, "%lu%s", curr->id, sep);
    curr = curr->next_neighbor;
  }
  fprintf(stream, "\n");
}
 
void print_nodes(FILE *stream, graph *g)
{
  if(g->next_node == NULL)
    fprintf( stderr, "Lista jest pusta \n");
    
  node *curr = g->next_node;
    
  while(curr)
  {
    fprintf(stream, "\b%lu\t%lu\n", curr->id, curr->degree); 
    curr = curr->next_node; //przechodzę do kolejnego elementu listy
  }
}
 
bool add_link (graph *g, size_t id1, size_t id2)
{
  node *node1 = new_node (id1);
  node *node2 = new_node(id2);

  node1->next_node = node2;
  return true;
}


int main()
{
  graph* my_graph = new_graph(0);
  
  add_neighbor(add_node(my_graph, 3),234);
  add_node(my_graph, 35);
  add_node(my_graph, 31);
  add_node(my_graph, 13);
  add_node(my_graph, 5);
  add_node(my_graph, 132);
  add_node(my_graph, 51);
  add_node(my_graph, 2);
  
  print_nodes(stdout, my_graph);
  
  remove_node(my_graph, 31);
  add_node(my_graph, 1);
  printf("\n"); 
  print_nodes(stdout, my_graph);
  printf("\n"); 

  node* node_to_get = get_node(my_graph, 2);
  printf(" test - node to get value: %lu\n", node_to_get->id);

  node* node_to_get_by_id = get_node_by_id(my_graph, 3);
  printf(" test - node_to_get_by_id: %lu\n", node_to_get_by_id->id);
  printf("\n");

  puts("Ćw kontrolne 4");

  node *my_node = new_node(33);
  add_node (my_graph, my_node->id);
  print_nodes(stdout, my_graph);
  add_neighbor(my_node, 1234567);
  add_neighbor(my_node, 7);
  add_neighbor(my_node, 17800753);
  print_neighbors(stdout, my_node, " ");

  remove_neighbor(my_node, 7);
  print_neighbors(stdout, my_node, " ");
 
  return 0;
}