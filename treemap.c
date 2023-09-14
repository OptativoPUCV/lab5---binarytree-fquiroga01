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
  TreeNode *currentNode = tree->root;
  TreeNode *parent = NULL;
  while(currentNode != NULL)
    {
      if(is_equal(tree, key, currentNode->pair->key))
      {
        return;
      }
      parent = currentNode;
      if (tree->lower_than(key, currentNode->pair->key))
      {
        currentNode = currentNode->left;
      }
      else
      {
        currentNode = currentNode->right;
      }
    }
  TreeNode *nuevoNode = createTreeNode(key, value);
  if (parent == NULL)
  {
    tree->root = nuevoNode;
  }
  else if (tree->lower_than(key, parent->pair->key))
  {
    parent->left = nuevoNode;
    nuevoNode->parent = parent;
  }
  tree->current = nuevoNode;
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
  if (node == NULL || tree == NULL) 
  {
    return;
  }

  if (node->left == NULL && node->right == NULL) 
  {
    if (node == tree->root) 
    {
      free(node->pair);
      free(node);
      tree->root = NULL;
    } 
    else 
    {
      TreeNode * parent = node->parent;
      if (parent->left == node) 
      {
        parent->left = NULL;
      } 
      else
      {
        parent->right = NULL;
      }
      free(node->pair);
      free(node);
    }
  }
  else if (node->left != NULL && node->right == NULL)
  {
    TreeNode *hijo = (node->left != NULL) ? node->left : node->right;
    if (node == tree->root) 
    {
      tree->root = hijo;
      hijo->parent = NULL;
    } 
    else 
    {
      TreeNode *parent = node->parent;
      if (parent->left == node) 
      {
        parent->left = hijo;
      } 
      else 
      {
        parent->right = hijo;      
      }
      hijo->parent = parent;    
    }
    free(node->pair);
    free(node);
  } 
  else
  {
    TreeNode *sucesor = minimum(node->right);
    node->pair->key = sucesor->pair->key;
    node->pair->value = sucesor->pair->value;
    removeNode(tree, sucesor);
  }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair * searchTreeMap(TreeMap * tree, void* key)
{
  TreeNode *current = tree->root;
  while (current != NULL)
    {
      if (tree->lower_than(key, current->pair->key))
      {
        current = current->left;
      }
      else if (tree->lower_than(current->pair->key, key))
      {
        current = current->right;
      }
      else
      {
        tree->current = current;
        return current->pair;
      }
    }
  tree->current = NULL;
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) 
{
  TreeNode *current = tree->root;
  TreeNode *ub_node = NULL;
  while(current != NULL)
  {
    if (tree->lower_than(key, current->pair->key))
    {
      ub_node = current;
      current = current->left;
    }
    else if (tree->lower_than(current->pair->key, key))
    {
      current = current->right;
    }
    else
    {
      return current->pair;
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
