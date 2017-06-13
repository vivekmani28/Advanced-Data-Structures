#include <vector>
#include <iostream>
#include <stdlib.h>
#include<API.h>

struct node{
    node* left;
    node* right;
    int lineNo;
	int height;
	char *line;
};

struct text_t
{
	struct node *root_node;
	int lineCnt;
};

typedef struct node text_node;

text_node* remove_node(text_node*, int);
text_node* singleRotateLeft(text_node* );
text_node* singleRotateRight(text_node* );
text_node* doubleRotateLeft(text_node* );
int max(int leftHeight, int rightHeight);
text_node* doubleRotateRight(text_node* );
int getHeight(text_node*);
void insert_line(text_t*,int,char*);
void decrement(text_node*, int);
void increment(text_node*, int);
text_node* addNode(text_node* , int, char* );

text_node * find_node(text_t  *t,int index){
	text_node *tmpNode = t->root_node;
	while(tmpNode!=NULL){
	  if(tmpNode->lineNo==index)
		return (tmpNode);
	  else if(tmpNode->lineNo<index)
		tmpNode=tmpNode->right;
	  else
		tmpNode=tmpNode->left;
	}
	return NULL;
}
int len_subTree(node* n){
    if(n == NULL || n->left == NULL)
        return 0;
	if(n->left != NULL && n->right == NULL)
		return 1;
    return 1 + len_subTree(n->left) + len_subTree(n->right);
}



text_t* create_text(){
    text_t  *newNode;
	newNode=(text_t *)malloc(sizeof(text_t));
	newNode->root_node=NULL;
	newNode->lineCnt=0;
	return newNode;
}


int length_text(text_t *t){
    	if(t != NULL)
		return t->lineCnt;
	else
		return -1;
}


char * get_line( text_t *txt, int index){
    	if(txt != NULL){
		text_node *tmpNode =  find_node(txt,index);
		if(tmpNode != NULL){
			//tmpNode->left = NULL;
			return tmpNode-> line;
		}
	}
	return NULL;
}

void append_line( text_t *txt, char * new_line)
{
    txt->lineCnt++;
	txt->root_node=addNode(txt->root_node,txt->lineCnt,new_line);
}


char * set_line( text_t *txt, int index, char * new_line){
     if(txt != NULL){
		text_node *tmpNode =  find_node(txt,index);
		char * old_line;
		if(tmpNode != NULL){
			old_line = tmpNode->line;
			tmpNode -> line = new_line;
			return old_line;
		}
	}
	return NULL;
}

int getHeight(text_node* node){
	if(node == NULL)
		return 0;
	return node->height;
}

void increment(text_node *tmpNode, int index){
	if(tmpNode!=NULL){
	    if(tmpNode->lineNo>=index){
		    tmpNode->lineNo++;
			increment(tmpNode->left,index);
		}
		increment(tmpNode->right,index);
	}
}

void decrement(text_node *tmpNode, int index){
	if(tmpNode!=NULL){
	    if(tmpNode->lineNo>index){
		    tmpNode->lineNo--;
			decrement(tmpNode->left,index);
		}
		decrement(tmpNode->right,index);
	}
}

text_node* remove_node(text_node *delNode, int index ){
	text_node *tmpNode;
	if(delNode == NULL)
		return NULL;
	if(index < delNode->lineNo)
		delNode->left = remove_node(delNode->left,index);
	else if(index > delNode->lineNo)
		delNode->right = remove_node(delNode->right,index);
	else{

		if(delNode-> left == NULL || delNode->right == NULL){
			tmpNode = delNode;
			if(delNode->left == NULL)
				delNode = delNode->right;
			else
				delNode = delNode->left;
			free(tmpNode);

		}
		else{

			tmpNode = delNode -> right;
			while(tmpNode->left != NULL)
				tmpNode = tmpNode->left;
			delNode->lineNo = tmpNode->lineNo;
			delNode->line = tmpNode->line;
			delNode->right = remove_node( delNode->right,tmpNode->lineNo);
		}
	}

	if(delNode == NULL)
		return NULL;
	delNode->height = 1 + max(getHeight(delNode->left),getHeight(delNode->right));
	if(getHeight(delNode->left) > getHeight(delNode->right) + 1){
		//RotateLeft
		if(getHeight(delNode->left->left) > getHeight(delNode->left->right))
			delNode = singleRotateLeft(delNode);
		else
			delNode = doubleRotateLeft(delNode);
	}else if(getHeight(delNode->right) > getHeight(delNode->left) + 1){
		//RotateRight
		if(getHeight(delNode->right->right) > getHeight(delNode->right->left))
			delNode = singleRotateRight(delNode);
		else
			delNode = doubleRotateRight(delNode);
	}
	return delNode;
}

int max(int leftHeight, int rightHeight){
	if(leftHeight > rightHeight)
		return leftHeight;
	else
		return rightHeight;
}

text_node* singleRotateRight(text_node* nodeA){
	text_node* nodeB;
	nodeB = nodeA->right;
	nodeA->right = nodeB->left;
	nodeB->left =nodeA;
	//TODO - set height for nodeA and nodeB
	nodeA->height = max( getHeight( nodeA->left ), getHeight( nodeA->right ) ) + 1;
    nodeB->height = max( getHeight( nodeB->right ), nodeA->height ) + 1;
	return nodeB;
}

text_node* singleRotateLeft(text_node* nodeC){
	text_node* nodeB;
	nodeB = nodeC->left;
	nodeC->left = nodeB->right;
	nodeB->right =nodeC;
	//TODO - set height for nodeC and nodeB
	nodeC->height = max( getHeight( nodeC->left ), getHeight( nodeC->right ) ) + 1;
    nodeB->height = max( getHeight( nodeB->left ), nodeC->height ) + 1;
	return nodeB;
}

text_node* doubleRotateLeft(text_node* nodeC){
	nodeC->left = singleRotateRight(nodeC->left);
	return singleRotateLeft(nodeC);
}

text_node* doubleRotateRight(text_node* nodeA){
	nodeA->right = singleRotateLeft(nodeA->right);
	return singleRotateRight(nodeA);
}

text_node* addNode(text_node* node, int index, char* new_line){
	if(node == NULL){
		node = (text_node*) malloc(sizeof(text_node));
		node->lineNo = index;
		node->line = new_line;
		node->height = 0;
		node -> left = node -> right = NULL;
	}
	else if(index > node->lineNo){
		node->right = addNode(node->right,index,new_line);
		if(getHeight(node->right)- getHeight(node->left)==2){
			if(index> node->right->lineNo)
				node=singleRotateRight(node);
			else
				node=doubleRotateRight(node);
		}
	}
	else if(index < node->lineNo){
		node->left = addNode(node->left,index,new_line);
		if(getHeight(node->left)- getHeight(node->right)==2){
			if(index < node->left->lineNo)
				node=singleRotateLeft(node);
			else
				node=doubleRotateLeft(node);
		}
	}
	node->height=max(getHeight(node->left),getHeight(node->right))+1;
	return node;
}

void insert_line( text_t *txt, int index, char * new_line){
   text_node *tmpNode = find_node(txt,index);
	if(tmpNode == NULL)
		append_line(txt,new_line);
	else{
		int tmpLineNo = tmpNode->lineNo;
		increment(txt->root_node,tmpLineNo);
		txt->lineCnt++;
		txt->root_node=addNode(txt->root_node,tmpLineNo,new_line);
	}
}

char * delete_line( text_t *txt, int index)
{
    if(txt != NULL){
		text_node *tmpNode =  find_node(txt,index);
		char * old_line;
		if(tmpNode != NULL){
			old_line = tmpNode->line;
			txt->lineCnt --;
			txt->root_node = remove_node(txt->root_node, index);
			decrement(txt->root_node, index);
			return old_line;
		}
	}
	return NULL;
}
