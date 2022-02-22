/*
COP 3502C Assignment 4
This program is written by: Jennifer Nguyen
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

#define MAXLEN 30

typedef struct itemNode
{
    char name[MAXLEN];
    int count;
    struct itemNode *left, *right;
}itemNode;

typedef struct treeNameNode
{
    char treeName[MAXLEN];
    int catagory;
    struct treeNameNode *left, *right;
    itemNode *theTree;
}treeNameNode;

FILE *ofp;

void traverse_in_traverse(treeNameNode *root)
{
    if (root != NULL)
    {
        traverse_in_traverse(root->left);
        printf("%s ", root->treeName);
        fprintf(ofp, "%s ", root->treeName);
        traverse_in_traverse(root->right);
    }
}

void transverseItem(itemNode *root)
{
    if (root != NULL)
    {
        transverseItem(root->left);
        printf("%s ", root->name);
        fprintf(ofp, "%s ", root->name);
        transverseItem(root->right);
    }
}

void printItem(treeNameNode *root)
{
    if (root != NULL)
    {
        printItem(root->left);
        printf("\n===%s===\n", root->treeName);
        fprintf(ofp, "\n===%s===\n", root->treeName);
        transverseItem(root->theTree);
        printItem(root->right);
    }
}

treeNameNode *searchNameNode(treeNameNode *root, char treeName[MAXLEN])
{
    while (strcmp(root->treeName, treeName) != 0)
    {
        if (strcmp(root->treeName, treeName) > 0 && root->left != NULL)
            root = root->left;
        else if (root->right != NULL)
            root = root->right;
        else
            break;
    }
    return root;
}

// search for a particular item in a given tree and display the count of the item if it is found
// if not found prints item not found
//if the tree does not exist, then it prints tree does not exist
void search(treeNameNode *root, itemNode *itemRoot, char treeName[], char item[])
{
    if (strcmp(root->treeName, treeName) != 0)
    {
        printf("\n%s does not exist", treeName);
        fprintf(ofp, "\n%s does not exist", treeName);
        return;
    }
    // search for item
    if (itemRoot != NULL)
    {
        if (strcmp(itemRoot->name, item) > 0)
            search(root, itemRoot->left, treeName, item);
        else if (strcmp(itemRoot->name, item) < 0)
            search(root, itemRoot->right, treeName, item);
        else
        {
            printf("\n%d %s found in %s", itemRoot->count, itemRoot->name, root->treeName);
            fprintf(ofp, "\n%d %s found in %s", itemRoot->count, itemRoot->name, root->treeName);
            return;
        }
    }
    else
        printf("\n%s not found in %s", item, treeName);
        
    return;
}
// counts number of items  in  a  given  tree  coming  before  a  given item name
void item_before(treeNameNode *root, itemNode *itemRoot, char treeName[], char item[], int counter)
{
    // transverse to find the item while tracking iterations
    while (strcmp(root->treeName, treeName) != 0)
    {
        counter++;
        if (strcmp(root->treeName, treeName) > 0)
        {
            root = root->left;
            itemRoot = root->theTree;
        }
        else
        {
            root = root->right;
            itemRoot = root->theTree;
        }
    }
    counter++;
    if (itemRoot != NULL)
    {
        if (strcmp(itemRoot->name, item) > 0)
            item_before(root, itemRoot->left, treeName, item, counter);
        else if (strcmp(itemRoot->name, item) < 0)
            item_before(root, itemRoot->right, treeName, item, counter);
        else
        {
            printf("\nitem before %s: %d", itemRoot->name, counter);
            fprintf(ofp, "\nitem before %s: %d", itemRoot->name, counter);
            return;
        }
    }
    
}

// Tests if tree is height balanced or not
// absolute value of difference of the height of left vs right sub tree
// more  than  1  then  we  say  the  tree  is  imbalanced.
// tree with no node will be considered as height -1

// finds the height of tree
int height(itemNode *itemRoot)
{
    int height1, height2;
    if (itemRoot == NULL)
        return -1;
    else
    {
        height1 = height(itemRoot->left);
        height2 = height(itemRoot->right);
        
        if (height1 > height2)
            return (height1 + 1);
        else
            return (height2 + 1);
    }
}

void height_balance(treeNameNode *root, itemNode *itemRoot)
{
    int heightL, heightR, difference;
    
    // find the height of each subtree
    heightL = height(root->theTree->left);
    heightR = height(root->theTree->right);
    
    // compare the heights
    difference = abs(heightL - heightR);
    if (difference > 1)
    {
        printf("\n%s: left height %d, right height %d, difference %d, not balanced", root->treeName, heightL, heightR, difference);
        fprintf(ofp, "\n%s: left height %d, right height %d, difference %d, not balanced", root->treeName, heightL, heightR, difference);
    }
    else
    {
        printf("\n%s: left height %d, right height %d, difference %d, balanced", root->treeName, heightL, heightR, difference);
        fprintf(ofp, "\n%s: left height %d, right height %d, difference %d, balanced", root->treeName, heightL, heightR, difference);
    }
    return;
}

// find total number of items in a given tree
int count(itemNode *itemRoot, int counter)
{
    if (itemRoot != NULL)
    {
        counter = count(itemRoot->left, counter);
        counter = counter + itemRoot->count;
        counter = count(itemRoot->right, counter);
    }
    return counter;
}

// command deletes an item from a given tree
itemNode *toDel(itemNode *itemRoot, char item[])
{
    if (itemRoot == NULL)
        return itemRoot;
    // searhc for item to be deleted
    if (strcmp(itemRoot->name, item) > 0)
        itemRoot = toDel(itemRoot->left, item);
    else if (strcmp(itemRoot->name, item) < 0)
        itemRoot = toDel(itemRoot->right, item);
    else
        return itemRoot;
}

itemNode *parent(itemNode *parentItem, itemNode *itemRoot, char item[])
{
    if (strcmp(parentItem->name, item) == 0)
        return NULL;
    // searhc for item to be deleted
    if (strcmp(itemRoot->name, item) > 0)
        parentItem = parent(itemRoot, itemRoot->left, item);
    else if (strcmp(itemRoot->name, item) < 0)
        parentItem = parent(itemRoot, itemRoot->right, item);
    else
        return parentItem;
}

itemNode *delete(treeNameNode *root, itemNode *itemRoot, char item[])
{
    
    struct itemNode *temp = NULL;
    
    // find node to be deleted
    struct itemNode *parentItem = parent(itemRoot, itemRoot, item);
    struct itemNode *curr = toDel(itemRoot, item);
    
    //No Children
    if (curr->left == NULL && root->right == NULL)
    {
        if (parentItem == NULL)
        {
            free(curr);
            curr = NULL;
            return NULL;
        }
        else if (strcmp(parentItem->name, curr->name) > 0)
        {
            free(parentItem->left);
            parentItem->left = NULL;
        }
        else
        {
            free(parentItem->right);
            parentItem->right = NULL;
        }
        return itemRoot;
    }
    // one child
    else if (curr->left == NULL || curr->right == NULL)
    {
        if (curr->left == NULL)
        {
            // if to be deleted is a right child
            if (parentItem == NULL)
            {
                temp = curr->right;
                free(curr);
                curr = NULL;
                return temp;
            }
            else if (strcmp(parentItem->name, curr->name) > 0)
            {      
                temp = parentItem->right;
                parentItem->right = parentItem->right->right;      
            }   
            else 
            {      
                temp = curr->right;
                parentItem->right = parentItem->right->left;      
            }
        }
        else
        {
            // if to be deleted is a left child
            if (parentItem == NULL)
            {
                temp = curr->right;
                free(curr);
                curr = NULL;
                return temp;
            }
            else if (strcmp(parentItem->name, curr->name) > 0)
            {      
                temp = curr->left;
                parentItem->left = parentItem->left->right;     
            }      
            else 
            {      
                temp = curr->left;
                parentItem->left = parentItem->left->left;       
            }
        }
        return itemRoot;
    }
    // node with two children alphabetically use nearby node
    temp = curr->right;

    // Copy the replacemnt
    strcpy(curr->name, temp->name);
    curr->count = temp->count;

    // move others up
    itemRoot->right = delete(root, itemRoot->right, temp->name);

    return itemRoot;
}
// reduces  the  count  of  an  item  in  a  given  tree
// delete node if count becaome <= 0
void reduce(treeNameNode *root, itemNode *itemRoot, itemNode *curr, char item[], int sub)
{
    if (curr != NULL)
    {
        // find item
        if (strcmp(curr->name, item) > 0)
            reduce(root, itemRoot, curr->left, item, sub);
        else if (strcmp(curr->name, item) < 0)
            reduce(root, itemRoot, curr->right, item, sub);
        else
        {
            curr->count = curr->count - sub;
            printf("\n%s reduced", curr->name);
            fprintf(ofp, "\n%s reduced", curr->name);
            if (curr->count <= 0)
            {
                itemRoot = delete(root, itemRoot, item);
            }
        }
    }
    return;
}

// delete the entire tree of a given name
void delete_itemTree(itemNode *itemRoot)
{
    if (itemRoot == NULL)
        return;
    // traverse
    delete_itemTree(itemRoot->left);
    delete_itemTree(itemRoot->right);
    
    //then delete the node bottom up
    free(itemRoot);
}
// delete name from tree
void delete_name(treeNameNode *root, itemNode *itemRoot)
{
    
    if (root == NULL)
        return;
    // delete item tree
    delete_itemTree(itemRoot);
    
    // delete the treenode
    printf("\n%s deleted", root->treeName);
    fprintf(ofp, "\n%s deleted", root->treeName);
    free(root);
}

// compare queries to find correct function
void queries(FILE *ifp, treeNameNode *root, char query[])
{
    struct treeNameNode *temp = NULL;
    char treeName[MAXLEN], item[MAXLEN];
    int counter = 0;
    
    if (strcmp("search", query) == 0)
    {
        fscanf(ifp, "%s", treeName);
        fscanf(ifp, "%s", item);
        temp = searchNameNode(root, treeName);
        search(temp, temp->theTree, treeName, item);
        return;
    }
    else if (strcmp("item_before", query) == 0)
    {
        fscanf(ifp, "%s", treeName);
        fscanf(ifp, "%s", item);
        item_before(root, root->theTree, treeName, item, counter);
        return;
    }
    else if (strcmp("height_balance", query) == 0)
    {
        fscanf(ifp, "%s", treeName);
        temp = searchNameNode(root, treeName);
        height_balance(temp, temp->theTree);
        return;
    }
    else if (strcmp("count", query) == 0)
    {
        fscanf(ifp, "%s", treeName);
        temp = searchNameNode(root, treeName);
        counter = count(temp->theTree, counter);
        printf("\n%s count %d", temp->treeName, counter);
        fprintf(ofp, "\n%s count %d", temp->treeName, counter);
        return;
    }
    else if (strcmp("reduce", query) == 0)
    {
        fscanf(ifp, "%s", treeName);
        fscanf(ifp, "%s", item);
        fscanf(ifp, "%d", &counter);
        temp = searchNameNode(root, treeName);
        reduce(temp, temp->theTree, temp->theTree, item, counter);
        return;
    }
    else if (strcmp("delete", query) == 0)
    {
        fscanf(ifp, "%s", treeName);
        fscanf(ifp, "%s", item);
        temp = searchNameNode(root, treeName);
        temp->theTree = delete(temp, temp->theTree, item);
        printf("\n%s deleted from %s", item, temp->treeName);
        fprintf(ofp, "\n%s deleted from %s", item, temp->treeName);
        return;
    }
    else if (strcmp("delete_name", query) == 0)
    {
        fscanf(ifp, "%s", treeName);
        temp = searchNameNode(root, treeName);
        delete_name(temp, temp->theTree);
        return;
    }
}

// create item node
itemNode* createItemNode(char item[], int count)
{
    itemNode* temp;
    temp = (struct itemNode*)malloc(sizeof(struct itemNode));
    strcpy(temp->name, item);
    temp->count = count;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

// create item tree
itemNode* buildItemTree(struct treeNameNode *root, struct itemNode *itemRoot, char name[], struct itemNode *element)
{
   
    if (root->theTree == NULL)
    {
        itemRoot = element;
        root->theTree = itemRoot;
        return itemRoot;
    }
    else
    {
        // Sort alphabetically
        if (strcmp(itemRoot->name, element->name) > 0)
        {
            if (itemRoot->left != NULL)
                itemRoot->left = buildItemTree(root, itemRoot->left, name, element);
            else
                itemRoot->left = element;
             
        }
        else
        {
            if (itemRoot->right != NULL)
                itemRoot->right = buildItemTree(root, itemRoot->right, name, element);
            else
                itemRoot->right = element;
        }
    }
    return itemRoot;
}

// create tree node
treeNameNode* createtreeNameNode(char name[], int catagory)
{
    treeNameNode* temp;
    temp = (struct treeNameNode*)malloc(sizeof(struct treeNameNode));
    strcpy(temp->treeName, name);
    temp->catagory = catagory;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

// create name tree
treeNameNode* buildNameTree(struct treeNameNode *root, struct treeNameNode *element)
{
    if (root == NULL)
        return element;
    else
    {
        // Sort alphabetically
        if (strcmp(root->treeName, element->treeName) > 0)
        {
            if (root->left != NULL)
                root->left = buildNameTree(root->left, element);
            else
                root->left = element;
        }
        else
        {
            if (root->right != NULL)
                root->right = buildNameTree(root->right, element);
            else
                root->right = element;
        }
    }
    return root;
}


int main()
{
    atexit(report_mem_leak);
    
    FILE *ifp;
    ifp = fopen("in.txt", "r");
    ofp =  fopen("out.txt", "w");
    
    struct treeNameNode *root = NULL, *temp;
    // to make multiple roots to link to correct tree name
    struct itemNode **itemRoot = (struct itemNode**) malloc(MAXLEN * sizeof(struct itemNode*)), *tempItem;
    
    int i, j, k, N, I, Q, count;
    char name[MAXLEN], item[MAXLEN], query[MAXLEN];
    
    fscanf(ifp, "%d", &N); // tree name number
    fscanf(ifp, "%d", &I); // items to be inserted
    fscanf(ifp, "%d", &Q); // number of queries
    for (i = 0; i < N; i++)
    {
        fscanf(ifp, "%s", name);
        temp = createtreeNameNode(name, i);
        root = buildNameTree(root, temp);
    }
    
    traverse_in_traverse(root);
    
    // make item tree
    for (j = 0; j < I; j++)
    {
        fscanf(ifp, "%s", name);
        fscanf(ifp, "%s", item);
        fscanf(ifp, "%d", &count);
        
        tempItem = createItemNode(item, count);
        temp = searchNameNode(root, name);
       
        itemRoot[temp->catagory] = buildItemTree(temp, itemRoot[temp->catagory], name, tempItem);
    }
    
    printItem(root);
 
    for (k = 0; k < Q; k++)
    {
        fscanf(ifp, "%s", query);
        queries(ifp, root, query);
        fflush(stdout);
    }
    fclose(ifp);
    free(itemRoot);
}
