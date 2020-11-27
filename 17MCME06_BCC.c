//K Pranav Bharadwaj (17MCME06)
//GT Programming assignment
#include <stdio.h>
#include <stdlib.h>

//stack structure
struct stk
{
  int vert1[80];
  int vert2[80];
  int top;
};

typedef struct stk stack;

stack S; //global stack
int counter=1; //counter for biconnected components

void push(int u, int v)
{
  S.top++;
  S.vert1[S.top] = u;
  S.vert2[S.top] = v;
}

void pop(int* u,int* v)
{
  *u = S.vert1[S.top];
  *v = S.vert2[S.top];
  S.top--;
}
//fill edges where u < v
int Forwardfill(int u[],int v[], int index, int arr2[], int last, int E)
{
  int i;
  for(i=0;i<E;i++)
  {
    if(u[i] == index)
    {
      arr2[last] = v[i];
      last++;
    }
    if(u[i] > index)
    {
      break;
    }
  }
  return last;
}

//fill edges where u > v
int Backwardfill(int u[],int v[], int index, int arr2[], int last, int E)
{
  int i,j;
  for(i=0;i<E;i++)
  {
    if(u[i] == index)
    {
      break;
    }
  }
  for(j=0;j<i;j++)
  {
    if(v[j] == index)
    {
      arr2[last] = u[j];
      last++;
    }
  }
  return last;
}
//Forward star representation
void FS(int u[], int v[], int arr1[], int arr2[], int V, int E)
{
  int last = 0,i;
  arr1[0] = last;
  last = Forwardfill(u,v,0,arr2,last,E);
  for(i=1;i<V;i++)
  {
    arr1[i] = last;
    last = Backwardfill(u,v,i,arr2,last,E);
    last = Forwardfill(u,v,i,arr2,last,E);
  }
  arr1[V] = last;
}

//get Biconnected components function
void getBCC(int u,int visited[],int low[],int start[],int index,int parent[],int arr1[],int arr2[])
{
  int i,v,v1=-1,v2=-1;
  visited[u] = 1;
  start[u] = index+1;
  index++;
  low[u] = start[u];
  for(i=arr1[u];i<arr1[u+1];i++)
  {
    v = arr2[i];
    if (v == parent[u])
    {
      continue;
    }
    else if (visited[v] == 1 && start[v] < start[u])
    {
      push(u,v);
      if (low[u] > start[v])
      {
        low[u] = start[v];
      }
    }
    else if (visited[v] == 1 && start[v] > start[u])
    {
      continue;
    }
    else
    {
      push(u,v);
      parent[v] = u;
      getBCC(v,visited,low,start,index,parent,arr1,arr2);
      if (low[u] > low[v])
      {
        low[u] = low[v];
      }
      if (low[v] >= start[u])
      {
        printf("Biconnected component %d:\n",counter);
	counter++;
        while(v1!=u || v2!=v)
        {
	  pop(&v1,&v2);
	  printf("(%d,%d) ",v1+1,v2+1);
        }
	printf("\n");
      }
    }
  }      
}

int main(int argc, char **argv)
{
  int V,E,i;
  FILE *fptr;
  if (argc != 2)
  {
    printf("Usage:./a.out filename\n");
    exit(1);
  }
  if ((fptr=fopen(argv[1],"r")) == NULL)
  {
    printf("Error opening file\n");
    exit(1);
  }
  fscanf(fptr,"%d %d",&V,&E);
  int u[E],v[E];
  int arr1[V+1],arr2[2*E];
  int visited[V],low[V],start[V],parent[V],index=0;
  int v1,v2;
  for(i=0;i<E;i++)
  {
    fscanf(fptr,"%d %d",&u[i],&v[i]);
    u[i] = u[i] - 1;
    v[i] = v[i] - 1;
  }
  FS(u,v,arr1,arr2,V,E);
  printf("Forward star representation:\n");
  for(i=0;i<V+1;i++)
  {
    printf("%d, ",arr1[i]);
  }
  printf("\n");
  for(i=0;i<arr1[V];i++)
  {
    printf("%d, ",arr2[i]+1);
  }
  printf("\n");
  S.top = -1;
  for(i=0;i<V;i++)
  {
    visited[i] = 0;
  }
  getBCC(0,visited,low,start,index,parent,arr1,arr2);
  if(S.top != -1)
  {
    printf("Biconnected component %d:\n",counter);
    counter++;
    while(S.top != -1)
    {
      pop(&v1,&v2);
      printf("(%d,%d) ",v1+1,v2+1);
    }
    printf("\n");
  }
}
