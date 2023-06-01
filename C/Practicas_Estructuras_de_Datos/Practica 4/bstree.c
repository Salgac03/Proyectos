#include <stdlib.h>
#include <stdio.h>
#include "bstree.h"

/* START [_BSTNode] */
typedef struct _BSTNode {
    void * info;
    struct _BSTNode * left;
    struct _BSTNode * right;
    struct _BSTNode * parent;
} BSTNode;
/* END [_BSTNode] */

/* START [_BSTree] */
struct _BSTree {
    BSTNode * root;
    P_tree_ele_print print_ele;
    P_tree_ele_cmp cmp_ele;
};
/* END [_BSTree] */


/*** BSTNode TAD private functions ***/
BSTNode * _bst_node_new(){
    BSTNode * pn = NULL;

    pn = malloc(sizeof(BSTNode));
    if (!pn) {
        return NULL;
    }

    pn->left = pn->right = pn->parent = NULL;
    pn->info = NULL;
    return pn;
}

void _bst_node_free (BSTNode * pn){
    if (!pn) return;

    free(pn);
}

void _bst_node_free_rec (BSTNode * pn){
    if (!pn) return;

    _bst_node_free_rec(pn->left);
    _bst_node_free_rec(pn->right);
    _bst_node_free(pn);

    return;
}

int _bst_depth_rec (BSTNode * pn) {
    int depthR, depthL;
    if (!pn) return 0;

    depthL = _bst_depth_rec(pn->left);
    depthR = _bst_depth_rec(pn->right);

    if (depthR > depthL) {
        return depthR + 1;
    } else {
        return depthL + 1;
    }
}

int _bst_size_rec(BSTNode * pn) {
    int count = 0;
    if (!pn) return count;

    count += _bst_size_rec(pn->left);
    count += _bst_size_rec(pn->right);

    return count + 1;
}

int _bst_preOrder_rec (BSTNode * pn, FILE * pf, P_tree_ele_print print_ele) {
    int count = 0;
    if (!pn) return count;

    count += print_ele(pf, pn->info);
    count += _bst_preOrder_rec(pn->left, pf, print_ele);
    count += _bst_preOrder_rec(pn->right, pf, print_ele);

    return count;
}

int _bst_inOrder_rec (BSTNode * pn, FILE * pf, P_tree_ele_print print_ele) {
    int count = 0;
    if (!pn) return count;

    count += _bst_inOrder_rec(pn->left, pf, print_ele);
    count += print_ele(pf, pn->info);
    count += _bst_inOrder_rec(pn->right, pf, print_ele);

    return count;
}

int _bst_postOrder_rec (BSTNode * pn, FILE * pf, P_tree_ele_print print_ele) {
    int count = 0;
    if (!pn) return count;

    count += _bst_postOrder_rec(pn->left, pf, print_ele);
    count += _bst_postOrder_rec(pn->right, pf, print_ele);
    count += print_ele(pf, pn->info);

    return count;
}

void * _bst_find_min_rec (BSTNode * node){
    if(node==NULL) return NULL;

    void * ele=NULL;

    if(node->left!=NULL){
        ele=_bst_find_min_rec(node->left);
    }else{
        ele=node->info;
    }

    return ele;
}

void * _bst_find_max_rec (BSTNode * node){
    if(node==NULL) return NULL;

    void * ele=NULL;

    if(node->right!=NULL){
        ele=_bst_find_max_rec(node->right);
    }else{
        ele=node->info;
    }

    return ele;
}

Bool _bst_contains_rec (BSTNode * node, const void * elem, P_tree_ele_cmp cmp_ele){
    if(!node || !elem || !cmp_ele) return FALSE;
    Bool ret;

    if(node==NULL){
        ret=FALSE;
    }else if(cmp_ele(elem,node->info)>0){
        ret=_bst_contains_rec (node->right, elem, cmp_ele);
    }else if(cmp_ele(elem,node->info)<0){
        ret=_bst_contains_rec (node->left, elem, cmp_ele);
    }else if(cmp_ele(elem,node->info)==0){
        ret=TRUE;
    }

    return ret;
}

BSTNode * _bst_insert_rec (BSTNode* node, const void * elem, P_tree_ele_cmp cmp_ele){
    BSTNode *aux=NULL;

    if(node==NULL){
        node=_bst_node_new();
        if(node==NULL) return NULL;
        node->info=(void*)elem;
        return node;
    }

    if(cmp_ele(elem,node->info)>0){
        aux=_bst_insert_rec(node->right,elem,cmp_ele);
        if(aux==NULL) return NULL;
        node->right=aux;
        node->right->parent=node;
    }else if(cmp_ele(elem,node->info)<0){
        aux=_bst_insert_rec(node->left,elem,cmp_ele);
        if(aux==NULL) return NULL;
        node->left=aux;
        node->left->parent=node;
    }

    return node;
}

BSTNode * _bst_remove_rec(BSTNode* node, const void * elem, P_tree_ele_cmp cmp_ele){
    if(node==NULL) return NULL;
    void *aux=NULL;
    BSTNode *ret=NULL;

    if(cmp_ele(elem,node->info)>0){
        node->right=_bst_remove_rec(node->right,elem,cmp_ele);
    }else if(cmp_ele(elem,node->info)< 0){
        node->left=_bst_remove_rec(node->left,elem,cmp_ele);
    }else if(cmp_ele(elem,node->info) == 0){

        if(node->left==NULL && node->right==NULL){
            _bst_node_free(node);
            return NULL;
        }else if(node->left!=NULL && node->right==NULL){
            ret=node->left;
            _bst_node_free(node);
            return ret;
        }else if(node->left==NULL && node->right!=NULL){
            ret=node->right;
            _bst_node_free(node);
            return ret;
        }else{
            aux=_bst_find_min_rec(node->right);
            node->info=aux;
            node->right=_bst_remove_rec(node->right,elem,cmp_ele);
        }
    }

    return node;
}

/*** BSTree TAD functions ***/
BSTree * tree_init(P_tree_ele_print print_ele, P_tree_ele_cmp cmp_ele){
    if (!print_ele || !cmp_ele) return NULL;

    BSTree * tree = malloc (sizeof(BSTree));
    if (!tree) {
        return NULL;
    }

    tree->root = NULL;
    tree->print_ele = print_ele;
    tree->cmp_ele = cmp_ele;

    return tree;
}

void tree_destroy (BSTree * tree) {
    if (!tree) return;

    _bst_node_free_rec(tree->root);

    free(tree);
    return;
}

Bool tree_isEmpty( const BSTree * tree){
    if (!tree || !tree->root) return TRUE;
    return FALSE;
}

int tree_depth (const BSTree * tree){
    if (!tree) return -1;

    return _bst_depth_rec(tree->root);
}

size_t tree_size(const BSTree * tree) {
    if (!tree) return -1;

    return _bst_size_rec(tree->root);
}

int tree_preOrder (FILE *f, const BSTree * tree){
    if (!f || !tree) return -1;

    return _bst_preOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_inOrder (FILE *f, const BSTree * tree){
    if (!f || !tree) return -1;

    return _bst_inOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_postOrder (FILE *f, const BSTree * tree){
    if (!f || !tree) return -1;

    return _bst_postOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

void * tree_find_min (BSTree * tree){
    if(!tree) return NULL;

    return _bst_find_min_rec(tree->root);
}

void * tree_find_max (BSTree * tree){
    if(!tree) return NULL;

    return _bst_find_max_rec(tree->root);
}

Bool tree_contains (BSTree * tree, const void * elem){
    if(!tree || !elem) return FALSE;

    return _bst_contains_rec(tree->root,elem,tree->cmp_ele);
}

Status tree_insert (BSTree * tree, const void * elem){
    if(!tree || !elem) return ERROR;

    BSTNode *aux;

    aux=_bst_insert_rec(tree->root,elem,tree->cmp_ele);

    if(aux==NULL) return ERROR;

    tree->root=aux;

    return OK;
}

Status tree_remove (BSTree * tree, const void * elem){
    if(!tree || !elem) return ERROR;

    BSTNode *aux;

    aux=_bst_remove_rec(tree->root,elem,tree->cmp_ele);

    if(aux==NULL) return ERROR;

    tree->root=aux;

    return OK;
}

/**** TODO: find_min, find_max, insert, contains, remove ****/
