#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
  //new->lower_than = lower_than;
  TreeMap * newTreeMap = (TreeMap *) malloc (sizeof(TreeMap));
  if (newTreeMap == NULL)
  {
    return NULL;
  }
  newTreeMap->lower_than = lower_than;
  newTreeMap->root = NULL;
  newTreeMap->current = NULL;
  return newTreeMap;
}



void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
  tree->current = tree->root;
  while (tree->current != NULL)
    {
      int comparacion = tree->lower_than(key, tree->current->pair->key);
      if (comparacion == 0)
      {
        return;
      }
      else if (comparacion < 0)
      {
        if(tree->current->left == NULL)
        {
          TreeNode * nuevoNode = createTreeNode(key, value);
          nuevoNode->parent = tree->current;
          tree->current->left = nuevoNode;
          tree->current = nuevoNode;
          return;
        }
        tree->current = tree->current->left;
      }
      else
      {
        if (tree->current->right == NULL)
        {
          TreeNode * nuevoNode = createTreeNode(key, value);
          nuevoNode->parent = tree->current;
          tree->current->right = nuevoNode;
          tree->current = nuevoNode;
          return;
        }
        tree->current = tree->current->right;
      }
    }
  tree->root = createTreeNode(key, value);
  tree->current = tree->root;
}

TreeNode * minimum(TreeNode * x)
{ 
  while (x->left != NULL)
    {
      x = x->left;
    }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) 
{
  TreeNode * parent = node->parent;
  if (node->left == NULL && node->right == NULL)
  {
    if (parent == NULL)
    {
      free(node->pair);
      free(node);
      tree->root = NULL;
    }
    else if (parent->left == node)
    {
      free(node->pair);
      free(node);
      tree->root = NULL;
    }
    else
    {
      free(node->parent);
      free(node);
      parent->right = NULL;
    }
  }
  else if (node->left != NULL || node->right != NULL)
  {
    TreeNode * hijo = (node->left != NULL) ? node->left : node->right;
    if (parent == NULL)
    {
      free(node->pair);
      free(node);
      tree->root = hijo;
      hijo->parent = NULL;
    }
    else if (parent->left == node)
    {
      free(node->pair);
      free(node);
      parent->left = hijo;
      hijo->parent = parent;
    }
    else
    {
      free(node->pair);
      free(node);
      parent->right = hijo;
      hijo->parent = parent;
    }
  }
  TreeNode * minDerecha = minimum(node->right);
  node->pair->key = minDerecha->pair->key;
  node->pair->value = minDerecha->pair->value;
  removeNode(tree, minDerecha);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair * searchTreeMap(TreeMap * tree, void* key)
{
  tree->current = tree->root;
  while (tree->current != NULL)
    {
      int comparacion;
      comparacion = tree->lower_than(key, tree->current->pair->key);
      if (comparacion == 0)
      {
        return tree->current->pair;
      }
      else if (comparacion < 0)
      {
        tree->current = tree->current->left;
      }
      else
      {
        tree->current = tree->current->right;
      }
    }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) 
{
  if (tree == NULL || tree->root == NULL)
  {
    return NULL;
  }
  TreeNode * current = tree->root;
  TreeNode * ub_node = NULL;
  while(current != NULL)
    {
      int comparacion = tree->lower_than(key, current->pair->key);
      {
        if (comparacion == 0)
        {
          return current->pair;
        }
        else if (comparacion < 0)
        {
          ub_node = current;
          current = current->left;
        }
        else
        {
          current = current->right;
        }
      }
    }
  if (ub_node != NULL)
  {
    return ub_node->pair;
  }
  else
  {
    return NULL;
  }
}

Pair * firstTreeMap(TreeMap * tree) 
{
  tree->current = minimum(tree->root);
  if (tree->current != NULL)
  {
    return tree->current->pair;
  }
  else
  {
    return NULL;
  }
}

Pair * nextTreeMap(TreeMap * tree) 
{
  TreeNode * sucesor = tree->current;
  if (sucesor->right != NULL)
  {
    sucesor = minimum(sucesor->right);
    tree->current = sucesor;
    if (sucesor != NULL)
    {
      return sucesor->pair;
    }
    else
    {
      return NULL;
    }
  }
  while (sucesor->parent != NULL && sucesor == sucesor->parent->right)
    {
      sucesor = sucesor->parent;
    }
  tree->current = sucesor->parent;
  if (sucesor->parent != NULL)
  {
    return sucesor->parent->pair;
  }
  else
  {
    return NULL;
  }
}
