/************************************
 *		Splay Tree header			*
 ************************************/


#pragma once

template<class T, class K> class SplayTree {
public:
	SplayTree();
	virtual ~SplayTree();
	void Insert(const T & key, const K & Val);
	void Remove(const T & key);
	bool Search(const T & key);
	bool isEmpty() const;
	T Successor(const T & key);
	T Predecessor(const T & key);
	K Find(const T &key);
	void Clear();

private:

	struct SplayNode{
		SplayNode* leftChild;
		SplayNode* rightChild;
		SplayNode* parent;
		T indexKey;
		K data;

		SplayNode(const T & Key = T(), const K & Data = K())
			: leftChild(nullptr), rightChild(nullptr), parent(nullptr), indexKey(Key), data(Data) {}

		~SplayNode() {
			if (leftChild)
				delete leftChild;
			if (rightChild)
				delete rightChild;
			if (parent)
				delete parent;
		}
	};

	SplayNode* root;
	SplayNode* _Successor(SplayNode* localRoot) const;
	SplayNode* _Predecessor(SplayNode* localRoot) const;
	SplayNode* _Minimum(SplayNode* localRoot) const;
	SplayNode* _Maximum(SplayNode* localRoot) const;
	SplayNode* _Search(const T & key);
	void _LeftRotate(SplayNode* localRoot);
	void _RightRotate(SplayNode* localRoot);
	void _Transplant(SplayNode* localParent, SplayNode* localChild);
	void _Splay(SplayNode* pivotElement);
};


template<typename T, typename K> SplayTree<T, K>::SplayTree() { root = nullptr; }
template<typename T, typename K> SplayTree<T, K>::~SplayTree() { delete root; }
template<typename T, typename K> void SplayTree<T, K>::Insert(const T & key, const K & Val) {
	SplayNode * preInsertPlace = nullptr;
	SplayNode * insertPlace = root;

	while (insertPlace != nullptr) {
		preInsertPlace = insertPlace;

		if (insertPlace->indexKey < key)
			insertPlace = insertPlace->rightChild;
		else if (key < insertPlace->indexKey) {
			insertPlace = insertPlace->leftChild;
		}

		SplayNode * insertElement = new SplayNode(key, Val);
		insertElement->parent = preInsertPlace;

		if (preInsertPlace == nullptr)
			root = insertElement;
		else if (preInsertPlace->indexKey < insertElement->indexKey)
			preInsertPlace->rightChild = insertElement;
		else if (insertElement->indexKey < preInsertPlace->indexKey)
			preInsertPlace->leftChild = insertElement;

		_Splay(insertElement);
	}
}
template<typename T, typename K> void SplayTree<T, K>::Remove(const T & key) {
	SplayNode * removeElement = _Search(key);

	if (removeElement != nullptr) {
		if (removeElement->rightChild == nullptr)
			_Transplant(removeElement, removeElement->leftChild);
		else if (removeElement->leftChild == nullptr)
			_Transplant(removeElement, removeElement->leftChild);
		else {
			SplayNode * newLocalRoot = _Minimum(removeElement->rightChild);

			if (newLocalRoot->parent != removeElement) {

				_Transplant(newLocalRoot, newLocalRoot->rightChild);

				newLocalRoot->rightChild = removeElement->rightChild;
				newLocalRoot->rightChild->parent = newLocalRoot;
			}

			_Transplant(removeElement, newLocalRoot);

			newLocalRoot->leftChild = removeElement->leftChild;
			newLocalRoot->leftChild->parent = newLocalRoot;

			_Splay(newLocalRoot);
		}

		delete removeElement;
	}
}
template<typename T, typename K> bool SplayTree<T, K>::Search(const T &key) { return _Search(key) != nullptr; }
template<typename T, typename K> bool SplayTree<T, K>::isEmpty() const { return root == nullptr; }
template<typename T, typename K> T SplayTree<T, K>::Successor(const T & key) {
	if (_Successor(_Search(key)) != nullptr) {
		return _Successor(_Search(key))->indexKey;
	}
	else {
		return -1;
	}
}
template<typename T, typename K> T SplayTree<T, K>::Predecessor(const T & key) {
	if (_Predecessor(_Search(key)) != nullptr) {
		return _Predecessor(_Search(key))->indexKey;
	}
	else {
		return -1;
	}
}
template<class T, class K> K SplayTree<T, K>::Find(const T & key)
{
	return _Search(key)->data;
}
template<typename T, typename K> void SplayTree<T, K>::Clear() {
	delete root;
	root = nullptr;
}

template<typename T, typename K> typename SplayTree<T, K>::SplayNode* SplayTree<T, K>::_Successor(SplayNode * localRoot) const {
	SplayNode * successor = localRoot;

	if (successor->rightChild != nullptr) {
		successor = _Minimum(successor->rightChild);
	}
	else {
		while (successor != successor->parent->leftChild
			|| successor != root) {
			successor = successor->parent;
		}
	}

	return successor;
}
template<typename T, typename K> typename SplayTree<T, K>::SplayNode* SplayTree<T, K>::_Predecessor(SplayNode * localRoot) const {
	SplayNode * predecessor = localRoot;

	if (predecessor->leftChild != nullptr) {
		predecessor = _Maximum(predecessor->leftChild);
	}
	else {
		while (predecessor != predecessor->parent->rightChild
			|| predecessor != root) {
			predecessor = predecessor->parent;
		}
	}

	return predecessor;
}
template<typename T, typename K> typename SplayTree<T, K>::SplayNode* SplayTree<T, K>::_Minimum(SplayNode * localRoot) const {
	SplayNode * minimum = localRoot;

	while (minimum != nullptr)
		minimum = minimum->leftChild;

	return minimum;
}
template<typename T, typename K> typename SplayTree<T, K>::SplayNode* SplayTree<T, K>::_Maximum(SplayNode * localRoot) const {
	SplayNode * maximum = localRoot;

	while (maximum != nullptr)
		maximum = maximum->rightChild;

	return maximum;
}
template<typename T, typename K> typename SplayTree<T, K>::SplayNode* SplayTree<T, K>::_Search(const T& key) {
	SplayNode* searchedElement = root;

	while (searchedElement != nullptr) {
		if (searchedElement->indexKey < key)
			searchedElement = searchedElement->rightChild;
		else if (key < searchedElement->indexKey)
			searchedElement = searchedElement->leftChild;
		else if (searchedElement->indexKey == key) {
			_Splay(searchedElement);
			return searchedElement;
		}
	}

	return nullptr;
}
template<typename T, typename K> void SplayTree<T, K>::_LeftRotate(SplayNode * localRoot) {
	SplayNode * rightChild = localRoot->rightChild;

	localRoot->rightChild = rightChild->leftChild;
	if (rightChild->leftChild != nullptr)
		rightChild->leftChild->parent = localRoot;

	_Transplant(localRoot, rightChild);

	rightChild->leftChild = localRoot;
	rightChild->leftChild->parent = rightChild;
}
template<typename T, typename K> void SplayTree<T, K>::_RightRotate(SplayNode * localRoot) {
	SplayNode * leftChild = localRoot->leftChild;

	localRoot->leftChild = leftChild->rightChild;
	if (leftChild->rightChild != nullptr)
		leftChild->rightChild->parent = localRoot;

	_Transplant(localRoot, leftChild);

	leftChild->rightChild = localRoot;
	leftChild->rightChild->parent = leftChild;
}
template<typename T, typename K> void SplayTree<T, K>::_Transplant(SplayNode* localParent, SplayNode* localChild) {
	if (localParent->parent == nullptr)
		root = localChild;
	else if (localParent == localParent->parent->leftChild)
		localParent->parent->leftChild = localChild;
	else if (localParent == localParent->parent->rightChild)
		localParent->parent->rightChild = localChild;

	if (localChild != nullptr)
		localChild->parent = localParent->parent;
}
template<typename T, typename K> void SplayTree<T, K>::_Splay(SplayNode * pivotElement) {
	while (pivotElement != root) {
		if (pivotElement->parent == root) {

			if (pivotElement == pivotElement->parent->leftChild)
				_RightRotate(pivotElement);
			else if (pivotElement == pivotElement->parent->rightChild) {
				_LeftRotate(pivotElement);

			}
			else {
				// Zig-Zig step.
				if (pivotElement == pivotElement->parent->leftChild &&
					pivotElement->parent == pivotElement->parent->parent->leftChild) {

					_RightRotate(pivotElement->parent->parent);
					_RightRotate(pivotElement->parent);

				}
				else if (pivotElement == pivotElement->parent->rightChild &&
					pivotElement->parent == pivotElement->parent->parent->rightChild) {

					_LeftRotate(pivotElement->parent->parent);
					_LeftRotate(pivotElement->parent);
				}
				// Zig-Zag step.
				else if (pivotElement == pivotElement->parent->rightChild &&
					pivotElement->parent == pivotElement->parent->parent->leftChild) {

					_LeftRotate(pivotElement->parent);
					_RightRotate(pivotElement->parent->parent);

				}
				else if (pivotElement == pivotElement->parent->leftChild &&
					pivotElement->parent == pivotElement->parent->parent->rightChild) {

					_RightRotate(pivotElement->parent);
					_LeftRotate(pivotElement->parent->parent);
				}
			}
		}
	}
}

