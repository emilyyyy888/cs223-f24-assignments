/*----------------------------------------------
* Author: Emily Lu
* Date: 12/1/2024
* Description: Implemented a binary search tree.
---------------------------------------------*/

#include "tree.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct tree_node* find(const char* name, struct tree_node* root)
{
  if(root == NULL) return NULL;
  int cmp = strcmp(name, root-> data.name);
  if(cmp == 0) return root;
  if(cmp < 0) return find(name, root->left);
  return find(name, root->right);
}

struct tree_node* insert(const char* name, struct tree_node* root)
{
  if(root == NULL) {
    struct tree_node* node = (struct tree_node*)malloc(sizeof(struct tree_node));
    if(!node) {
          fprintf(stderr, "Memory allocation error\n");
          exit(1);
    }
    strcpy(node ->data.name, name);
    node->left = NULL;
    node->right = NULL;
    return node;
  }

  int cmp = strcmp(name, root-> data.name);
  if(cmp < 0) {
    root->left = insert(name, root->left);
  } else {
    root->right = insert(name, root->right);
  }
  return root;
}

void clear(struct tree_node* root)
{
  if(root == NULL) {
    return;
  }
  clear(root->left);
  clear(root->right);
  free(root);
}

void print(struct tree_node* root)
{   
  if (root == NULL) return;

  printf("%s\n", root->data.name);  
  if (root->left) {
      printf(" l:");
      print(root->left);  
  }
  if (root->right) {
      printf(" r:");
      print(root->right); 
  }

}

void printSorted(struct tree_node* root)
{    
  if (root == NULL) return;

  printSorted(root->left);   
  printf("%s\n", root->data.name);  
  printSorted(root->right); 
}

