
    //set<interval_t> _intervals;
	struct node* root;
};
struct node{
	int value;
	int which;
	int other;
	int min;
	int max;
	int leftmin;
	int rightmax;
	int submeasure;
	
	int height;
	node* left;
	node* right;
};

typedef struct node end_pt;

//Function declaration
end_pt* insert_node(end_pt* node, int val, int other, int which);
end_pt* delete_node(end_pt* node, int val, int other, int which);
end_pt* single_rotate_right(end_pt* node);
end_pt* single_rotate_left(end_pt* node);
end_pt* double_rotate_left(end_pt* node);
end_pt* double_rotate_right(end_pt* node);
void compute_leftmin_rightmax(end_pt* node);
void compute_min_max(end_pt* node);
void compute_submeasure(end_pt* node);

int height(end_pt* node);
int submeasure(end_pt* node);
int min(end_pt* node,int);
int max(end_pt* node,int);
int leftmin(end_pt* node,int);
int rightmax(end_pt* node,int);
void free_node(end_pt* node);

//Function implementation
m_tree_t* create_m_tree(){
	m_tree_t* tree = (m_tree_t *) malloc(sizeof(m_tree_t));
	tree->root = NULL;
	return tree;
}

void destroy_m_tree(m_tree_t *tree)
{
	free_node(tree->root);
}

void free_node(end_pt* node){
	if(node != NULL){
		free_node(node->left);
		free_node(node->right);
		free(node);
	}
}


void insert_interval(m_tree_t * tree, int a, int b)
{
	if(tree == NULL || a > b)
		return;
	tree->root = insert_node(tree->root,a,b,0); // left-end-point
	tree->root = insert_node(tree->root,b,a,1); //right-end-point
	
}


void delete_interval(m_tree_t * tree, int a, int b)
{
	if(tree == NULL || a > b)
		return;
	tree->root = delete_node(tree->root,a,b,0); // left-end-point
	tree->root = delete_node(tree->root,b,a,1); // right-end-point
	
}


int query_length(m_tree_t * tree)
{
	if(tree == NULL || tree->root == NULL )
		return 0;
	return submeasure(tree->root);
}

end_pt* insert_node(end_pt* node, int val,int other,int which){
	if(node == NULL){
		node = (end_pt *) malloc(sizeof(end_pt));
		node->left = node->right = NULL;
		node->value = val;
		node->other = other;
		node->which = which;
		
		compute_min_max(node);
		
		compute_leftmin_rightmax(node);
		
		node->submeasure = 0;
		node->height = 0;
		
		return node;
	}
	if( val < node->value || (val == node->value && other < node->other)){
		node->left = insert_node(node->left,val,other,which);
		if(height(node->left) > height(node->right) + 1){
			if(height(node->left->left) > height(node->left->right))
				node = single_rotate_right(node);
			else
				node = double_rotate_left(node);
		}
		
	}else if( val > node->value || (val == node->value && other > node->other)){
		node->right = insert_node(node->right,val,other,which);
		if(height(node->right) > height(node->left) + 1){
			if(height(node->right->right) > height(node->right->left))
				node = single_rotate_left(node);
			else
				node = double_rotate_right(node);
		}
	}
	
	//computes new min and max after insertion. This trace back to root.
	compute_min_max(node);
	
	//computes leftmin and rightmax after insertion. This trace backs to root.
	compute_leftmin_rightmax(node);
	
	//computing submeasure after insertion. Area covered by this subtree rooted at node.
	compute_submeasure(node);
	
	//computes the height w.r.t altered structure of subtree.
	node->height = 1 + max(height(node->left), height(node->right));
	return node;
}

end_pt* delete_node(end_pt* node, int val, int other, int which){
	if(node == NULL)
		return node;
	if(val == node->value && other == node->other){
		end_pt* tmp = node;
		if(node->left == NULL && node->right == NULL){
			free(node);
			return NULL;
		}else if(node->right == NULL){
			node = node->left;
			free(tmp);
		}else if(node->left == NULL){
			node = node->right;
			free(tmp);
		}else{
			end_pt* replace = node->right;
			while(replace->left != NULL)
				replace = replace->left;
			node->value = replace->value;
			node->other = replace->other;
			node->which = replace->which;
			node->right = delete_node(node->right,replace->value,replace->other,replace->which);
			
		}
		
	}else if( val < node->value || (val == node->value && other < node->other)){
		node->left = delete_node(node->left,val,other,which);
	}else if( val > node->value || (val == node->value && other > node->other)){
		node->right = delete_node(node->right,val,other,which);
	}
	
	//Balances the tree
	if(height(node->left) > height(node->right) + 1){
		if(height(node->left->left) > height(node->left->right))
				node = single_rotate_right(node);
			else
				node = double_rotate_left(node);
	}else if(height(node->right) > height(node->left) + 1){
		if(height(node->right->right) > height(node->right->left))
			node = single_rotate_left(node);
		else
			node = double_rotate_right(node);
	}
	
	//computes new min and max after deletion. This trace back to root.
	compute_min_max(node);
	
	//computes leftmin and rightmax after deletion. This trace backs to root.
	compute_leftmin_rightmax(node);
	
	//computing submeasure after deletion. Area covered by this subtree rooted at node.
	compute_submeasure(node);
	
	//computes the height w.r.t altered structure of subtree.
	node->height = 1 + max(height(node->left), height(node->right));
	return node;
}

end_pt* single_rotate_right(end_pt* node){
	end_pt* new_rt = node->left;
	node-> left = new_rt->right;
	new_rt->right = node;
	
	node->height = 1 + max(height(node->left), height(node->right));
	new_rt->height = 1 + max(height(new_rt->left), height(new_rt->right));
	
	//computes new min and max after rotation
	compute_min_max(node);
	compute_min_max(new_rt);
	
	//computes leftmin and rightmax after rotation.
	compute_leftmin_rightmax(node);
	compute_leftmin_rightmax(new_rt);
	
	//computing submeasure after rotation. 
	compute_submeasure(node);
	compute_submeasure(new_rt);
	
	return new_rt;
}

end_pt* single_rotate_left(end_pt* node){
	end_pt* new_rt = node->right;
	node-> right = new_rt->left;
	new_rt->left = node;
	
	node->height = 1 + max(height(node->left), height(node->right));
	new_rt->height = 1 + max(height(new_rt->left), height(new_rt->right));
	
	//computes new min and max after rotation
	compute_min_max(node);
	compute_min_max(new_rt);
	
	
	//computes leftmin and rightmax after rotation.
	compute_leftmin_rightmax(node);
	compute_leftmin_rightmax(new_rt);
	
	//computing submeasure after rotation. 
	compute_submeasure(node);
	compute_submeasure(new_rt);
	
	return new_rt;
}

end_pt* double_rotate_left(end_pt* node){
	node->left = single_rotate_left(node->left);
	return single_rotate_right(node);
}

end_pt* double_rotate_right(end_pt* node){
	node->right = single_rotate_right(node->right);
	return single_rotate_left(node);
}

//Computes min and max of a subtree of a node 
void compute_min_max(end_pt* node){
	node->min = (node->left == NULL)? node->value : node->left->min;
	node->max = (node->right == NULL)? node->value : node->right->max;
}

//Computes leftmin and rightmax of a subtree of a node.
void compute_leftmin_rightmax(end_pt* node){
	
	node->leftmin = (node->which == 0)? node->value : node->other;
	node->rightmax = (node->which == 1)? node->value: node->other;
	if(node->left != NULL){
		node->leftmin = min(node->leftmin, node->left->leftmin);
		node->rightmax = max(node->rightmax, node->left->rightmax);
	}
	if(node->right != NULL){
		node->leftmin = min(node->leftmin, node->right->leftmin);
		node->rightmax = max(node->rightmax, node->right->rightmax);
	}
}

//computes submeasure of a node
void compute_submeasure(end_pt* node){
	int left = 0;
	int right = 1;
	if(node == NULL)
		return;
	node->submeasure = 0;
	if(node->which == 0){
		if((leftmin(node,right) == min(node,right) || leftmin(node,right) == node->value) && rightmax(node,left) == max(node,left)){
			if(node->other < max(node,right))
				node->submeasure = submeasure(node->left) + submeasure(node->right) + min(node,right) - node->value;
			else
				node->submeasure = submeasure(node->left) +  max(node,right) - node->value;
		}else{
			if(node->other < max(node,right) && rightmax(node,left) < max(node,right))
				node->submeasure = submeasure(node->left) + submeasure(node->right) + min(node,right) - max(node,left);
			else
				node->submeasure = submeasure(node->left) +  max(node,right) - max(node,left);
		}
	}else{
		if((rightmax(node,left) == max(node,left) || rightmax(node,left) == node->value) && leftmin(node,right) == min(node,right)){
			if(node->other > min(node,left))
				node->submeasure = submeasure(node->left) + submeasure(node->right) + node->value - max(node,left);
			else
				node->submeasure = submeasure(node->right) +  node->value - min(node,left);
		}else{
			if(node->other > min(node,left) && leftmin(node,right) > min(node,left))
				node->submeasure = submeasure(node->left) + submeasure(node->right) + min(node,right) - max(node,left);
			else
				node->submeasure = submeasure(node->right) +  min(node,right) - min(node,left);
		}
	}
}
int height(end_pt* node){
	if(node == NULL)
		return -1;
	return node->height;
}


int submeasure(end_pt* node){
	if(node == NULL)
		return 0;
	return node->submeasure;
}
int min(end_pt* node, int left){
	if(left == 0)
		if(node->left == NULL)
			return node->value;
		else
			return node->left->min;
	else
		if(node->right == NULL)
			return node->value;
		else
			return node->right->min;
}
int max(end_pt* node, int left){
	if(left == 0)
		if(node->left == NULL)
			return node->value;
		else
			return node->left->max;
	else
		if(node->right == NULL)
			return node->value;
		else
			return node->right->max;
}

int leftmin(end_pt* node, int left){
	if(left == 0)
		if(node->left == NULL)
			return node->value;
		else
			return node->left->leftmin;
	else
		if(node->right == NULL)
			return node->value;
		else
			return node->right->leftmin;
}

int rightmax(end_pt* node, int left){
	if(left == 0)
		if(node->left == NULL)
			return node->value;
		else
			return node->left->rightmax;
	else
		if(node->right == NULL)
			return node->value;
		else
			return node->right->rightmax;
}