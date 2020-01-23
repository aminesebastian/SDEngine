#pragma once
#include "Utilities/TypeDefenitions.h"

template <typename T>
class STreeNode {
public:
	STreeNode(T* Data);
	T* GetData();
	SSet<STreeNode<T>*> GetChildren();
	bool AddChild(T* NewData);

private:
	T* Data;
	SSet<STreeNode<T>*> Children;
};

template <typename T>
class STree {
public:
	STree();
	~STree();

	void Add(T* NewData, T* Root);
	bool Remove(T* DataToRemove);
	SArray<T*> GetAllNodes();

private:
	STreeNode* Root;

	bool RemoveInternal(T* DataToRemove, STreeNode<T>* Node, STreeNode<T>* PreviousNode);
};

