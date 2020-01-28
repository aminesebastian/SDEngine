#include "Tree.h"
#include "Core/DataStructures/DataStructures.h"

template<typename T>
STree<T>::STree() {
	Root = nullptr;
}
template<typename T>
STree<T>::~STree() {

}

template<typename T>
void STree<T>::Add(T* NewData, T* Root) {
	if (Root == nullptr) {
		Root = new STreeNode(NewNode);
	}
}

template<typename T>
bool STree<T>::Remove(T* DataToRemove) {
	if (Root->GetData() == NewData) {
		if (Root->GetChildren().size() > 0) {
			STreeNode* temp = Root->GetChildren()[0];
			Root->GetChildren().erase(Root->Begin());
			Root = temp;
		} else {
			Root = nullptr;
		}
		return true;
	} else {
		for (STreeNode* node : Root->GetChildren()) {
			bool check = RemoveInternal(NewData, node, Root);
			if (check) {
				return true;
			}
		}
	}
	return false;
}

template<typename T>
bool STree<T>::RemoveInternal(T* DataToRemove, STreeNode<T>* Node, STreeNode<T>* PreviousNode) {
	if (Node == nullptr || PreviousNode == nullptr) {
		return false;
	}
	if (Node->GetData() == DataToRemove) {
		for (STreeNode* node : Node->GetChildren()) {
			PreviousNode->GetChildren().push_back(node);
		}
		for (int i = 0; i < PreviousNode->GetChildren().size(); i++) {
			if (PreviousNode->GetChildren()[i] == Node) {
				PreviousNode->GetChildren().erase(PreviousNode->GetChildren().begin() + i);
			}
		}
	} else {
		for (STreeNode* node : Root->GetChildren()) {
			bool check = RemoveInternal(NewData, node, Node);
			if (check) {
				return true;
			}
		}
	}
}
template<typename T>
SArray<T*> STree<T>::GetAllNodes() {
	SArray<T*> output;

}

/////////////
//Tree Node//
/////////////
template<typename T>
STreeNode<T>::STreeNode(T* Data) : Data(Data) {}
template<typename T>
T* STreeNode<T>::GetData() {
	return Data;
}
template<typename T>
SSet<STreeNode<T>*> STreeNode<T>::GetChildren() {
	return Children;
}
template<typename T>
bool STreeNode<T>::AddChild(T* NewData) {
	Children.push_back(new STreeNode(NewData));
	return true;
}
