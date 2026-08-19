#ifndef _LIST_H__
#define _LIST_H__

#include "Node.h"

class List
{
public:
	List();

	int size;
	Node *prim;
	
	//retorna o no com menor custo F = G + H
	Node *getMinF(void);
	Node *insert(Node *node, bool aloca);
	void remove(Node *node, bool libera);
	void clear();
	bool isEmpty();
	
	void print();
	//diz se o vertice (x,y) pertence a lista
	bool match(int x, int y); 
};

#endif

