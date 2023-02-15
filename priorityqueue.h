// priorityqueue.h
//
// Program for priority queue data structure. Elements may be added in any 
// order, but are removed in reverse priority order (priority 1 first, priority 10 last)
//
// Ryder Douglas
//
#pragma once

#include <iostream>
#include <sstream>
#include <set>

using namespace std;

template<typename T>
class priorityqueue {
private:
    struct NODE {
        int priority;  // used to build BST
        T value;  // stored data for the p-queue
        bool dup;  // marked true when there are duplicate priorities
        NODE* parent;  // links back to parent
        NODE* link;  // links to linked list of NODES with duplicate priorities
        NODE* left;  // links to left child
        NODE* right;  // links to right child
    };
    NODE* root;  // pointer to root node of the BST
    int size;  // # of elements in the pqueue
    NODE* curr;  // pointer to next item in pqueue (see begin and next)
    
public:
    //
    // default constructor:
    //
    // Creates an empty priority queue.
    // O(1)
    //
    priorityqueue() {
        root = nullptr;
        size = 0;
        curr = nullptr;
    }

    // copy function for copying one queue into another
    void _copy(NODE* other) {
		if (other == nullptr)
			return;
		else {
			this->enqueue(other->value, other->priority);
			_copy(other->link);
			_copy(other->left);
			_copy(other->right);
		}
	}

    //
    // operator=
    //
    // Clears "this" tree and then makes a copy of the "other" tree.
    // Sets all member variables appropriately.
    // O(n), where n is total number of nodes in custom BST
    //
    priorityqueue& operator=(const priorityqueue& other) {
        clear();

        _copy(other.root);

        return *this;
    }

    //
    // _recursiveClear
    //
    // Recursive helper function to clear tree node by node
    //
    void _recursiveClear(NODE* n) {
        if (n == nullptr)
            return;

        _recursiveClear(n->left);
        _recursiveClear(n->right);
        _recursiveClear(n->link);
        delete n;
        size--;
    }

    //
    // clear:
    //
    // Frees the memory associated with the priority queue but is public.
    // O(n), where n is total number of nodes in custom BST
    //
    void clear() {
        _recursiveClear(root);

        root = nullptr;
    }

    //
    // destructor:
    //
    // Frees the memory associated with the priority queue.
    // O(n), where n is total number of nodes in custom BST
    //
    ~priorityqueue() {
        _recursiveClear(root);

        root = nullptr;
    }
    
    //
    // enqueue:
    //
    // Inserts the value into the custom BST in the correct location based on
    // priority.
    // O(logn + m), where n is number of unique nodes in tree and m is number of
    // duplicate priorities
    //
    void enqueue(T value, int priority) {
        NODE* prev = nullptr;
        NODE* cur = root;

        // Search for key
        while (cur != nullptr) {
            if (priority == cur->priority) {
                // Duplicate, so go through the list till we reach the end
                cur->dup = true;
                prev = cur;
                cur = cur->link;
            } else if (priority < cur->priority) {  // go left
                prev = cur;
                cur = cur->left;
            } else {                                // go right
                prev = cur;
                cur = cur->right;
            }
        }

        // If it wasn't found, insert node where we fell out;
        NODE* n = new NODE();
        n->priority = priority;
        n->value = value;
        n->left = nullptr;
        n->right = nullptr;
        n->link = nullptr;

        if (prev == nullptr)
            root = n;
        else {
            n->parent = prev;

            if (priority == prev->priority) {
                n->dup = true;
                prev->link = n;
            } else if (priority < prev->priority)
                prev->left = n;
            else
                prev->right = n;
        }

        // update size and return
        size++;
    }
    //
    // dequeue:
    //
    // returns the value of the next element in the priority queue and removes
    // the element from the priority queue.
    // O(logn + m), where n is number of unique nodes in tree and m is number of
    // duplicate priorities
    //
    T dequeue() {
        if (size == 0)
            throw runtime_error("priorityqueue: empty");

        NODE* cur;
        NODE* prev;
        T valueOut;

        cur = root;
        prev = nullptr;

        // next node will always be leftmost node
        while (cur->left != nullptr) {
            prev = cur;
            cur = cur->left;
        }

        // Found the node, so copy the value
        valueOut = cur->value;

        // if node we're dequeueing has linked list of duplicates, always
        // replace with next list item
        if (cur->link != nullptr) {
            // can just copy value from link to current, point current at
            // link's link, then move to and delete link
            prev = cur;
            cur = cur->link;
            prev->value = cur->value;
            prev->link = cur->link;
            delete cur;

            cur = prev;
            prev = cur->parent;
            if (prev == nullptr)    // cur is root but tree isn't empty
                root = cur;

            // duplicate handling
            if (cur->link == nullptr)
                cur->dup = false;
            else
                cur->link->parent = cur;
        } else if (cur->right == nullptr) {
            // node to dequeue is leaf node (left child will always be null)
            if (prev == nullptr)    // node is root, and tree IS empty
                root = nullptr;
            else
                prev->left = nullptr;

            delete cur;
        } else {                    // node to dequeue has a child
            (prev == nullptr) ? root = cur->right : prev->left = cur->right;
            delete cur;
        }

        // decrement size and return next node value
        size--;
        return valueOut;
    }
    
    //
    // Size:
    //
    // Returns the # of elements in the priority queue, 0 if empty.
    // O(1)
    //
    int Size() {
        return size;
    }
    
    //
    // begin
    //
    // Resets internal state for an inorder traversal.  After the
    // call to begin(), the internal state denotes the first inorder
    // node; this ensure that first call to next() function returns
    // the first inorder node value.
    //
    // O(logn), where n is number of unique nodes in tree
    //
    // Example usage:
    //    pq.begin();
    //    while (tree.next(value, priority)) {
    //      cout << priority << " value: " << value << endl;
    //    }
    //    cout << priority << " value: " << value << endl;
    void begin() {
        curr = root;

        while (curr->left != nullptr)
            curr = curr->left;
    }
    
    //
    // next
    //
    // Uses the internal state to return the next inorder priority, and
    // then advances the internal state in anticipation of future
    // calls.  If a value/priority are in fact returned (via the reference
    // parameter), true is also returned.
    //
    // False is returned when the internal state has reached null,
    // meaning no more values/priorities are available.  This is the end of the
    // inorder traversal.
    //
    // O(logn), where n is the number of unique nodes in tree
    //
    // Example usage:
    //    pq.begin();
    //    while (pq.next(value, priority)) {
    //      cout << priority << " value: " << value << endl;
    //    }
    //    cout << priority << " value: " << value << endl;
    //
    bool next(T& value, int &priority) {
        value = curr->value;
        priority = curr->priority;

        // If node's priority is a duplicate, we have to navigate list
        if (curr->dup == true) {
            // If we're not at the end of the list, then next node is linked
            if (curr->link != nullptr) {
                curr = curr->link;
                return true;
            } else {
                while (curr->priority == curr->parent->priority) {
                    curr = curr->parent;
                    if (curr == root)   // don't even try
                        break;
                }
            }
        }

        // If right subtree of curr is NOT empty, next inorder node is in subtree
        if (curr->right != nullptr) {
            curr = curr->right;
            while (curr->left != nullptr)
                curr = curr->left;
            return true;
        } else {
            // if right subtree IS empty, next inorder node is ancestor
            while (curr != curr->parent->left) {
                curr = curr->parent;
                if (curr->parent == nullptr) {  // this is the end
                    return false;
                }
            }
            // found node that is left child of parent, parent is next inorder
            curr = curr->parent;
            return true;
        }
    }

    //
    // _recursiveString
    //
    // Recursive helper function for toString(), takes head node of linked list
    // and output stream, prints priority of value of each node, in-order
    void _recursiveString(NODE* n, ostream& output) {
        if (n == nullptr)
            return;
        
        _recursiveString(n->left, output);
        output << n->priority << " value: " << n->value << endl;
        _recursiveString(n->link, output);
        _recursiveString(n->right, output);
    }

    //
    // toString:
    //
    // Returns a string of the entire priority queue, in order.  Format:
    // "1 value: Ben
    //  2 value: Jen
    //  2 value: Sven
    //  3 value: Gwen"
    //
    string toString() {
        stringstream ss;

        _recursiveString(root, ss);
        string str = ss.str();
        return str;
    }
    
    //
    // peek:
    //
    // returns the value of the next element in the priority queue but does not
    // remove the item from the priority queue.
    // O(logn + m), where n is number of unique nodes in tree and m is number of
    // duplicate priorities
    //
    T peek() {
        if (size == 0)
            throw runtime_error("priorityqueue: empty");

        T valueOut;
        NODE* cur;

        cur = root;

        while (cur->left != nullptr)
            cur = cur->left;

        valueOut = cur->value;

        return valueOut;
    }

    //
    // _equal
    //
    // Recursive helper function for operator==, checks equality of two trees
    // node by node, going in order
    //
    bool _equal(NODE* thisCur, NODE* otherCur) const {
        if (thisCur == nullptr && otherCur == nullptr)
            return true;
        else if (thisCur == nullptr || otherCur == nullptr)
            return false;

        if (_equal(thisCur->link, otherCur->link) &&
                thisCur->value == otherCur->value &&
                _equal(thisCur->right, otherCur->right))
            return true;
        return false;
    }
    
    //
    // ==operator
    //
    // Returns true if this priority queue as the priority queue passed in as
    // other.  Otherwise returns false.
    // O(n), where n is total number of nodes in custom BST
    //
    bool operator==(const priorityqueue& other) const {
        return _equal(this->root, other.root);
    }
    
    //
    // getRoot - Do not edit/change!
    //
    // Used for testing the BST.
    // return the root node for testing.
    //
    void* getRoot() {
        return root;
    }

    // Helper functions to fill priority with default or random values/elements
    // Used in testing. randomPopulate functions fill queue with 100 elements

    void _defaultPopulate() {
		enqueue('a', 5);
		enqueue('b', 2);
		enqueue('c', 1);
		enqueue('d', 3);
		enqueue('e', 4);
		enqueue('f', 2);
		enqueue('g', 6);
		enqueue('h', 2);
		enqueue('i', 8);
		enqueue('j', 7);
		enqueue('k', 10);
		enqueue('l', 5);
	}

    void _randomPopulateInt(int seed = 0) {
		int i, j, value, priority;

		value = randInteger(100 + seed, 999);
		enqueue(value, 5);

		for (i = 1; i < 21; i++) {
			for (j = 1; j < 6; j++) {
				priority = randInteger(1, 10 - ((i / 2) - j));
				value = randInteger(100 + seed + j, 999 - i + j);
				enqueue(value, priority);
			}
		}
	}

	void _randomPopulateChar(int seed = 0) {
		int i, j, priority;
		char value;

		value = 'a';
		enqueue(value, 5);

		for (i = 1; i < 21; i++) {
			for (j = 1; j < 6; j++) {
				priority = randInteger(1, 10 - ((i / 2) - j));
				value = randInteger(31 + j, 126 - (seed + i + j));
				enqueue(value, priority);
			}
		}
	}

	void _randomPopulateDouble(double seed = 0) {
		int i, j, priority;
		double value;

		value = randDouble(100.0 + seed, 999.9);
		enqueue(value, 5);
		
		for (i = 1; i < 21; i++) {
			for (j = 1; j < 6; j++) {
				priority = randInteger(1, 10 - ((i / 2) - j));
				value = randDouble(99.0 + seed + j, 999.9 - (i + j));
				enqueue(value, priority);
			}
		}
	}

	string _randomString(int seed = 0) {
		int i;
		char ch;
		string str;

		str = "";

		for (i = 0; i < 3; i++) {
			ch = randInteger(35 - i, 125 - (i + seed));
			str += ch;
		}
		return str;
	}

	void _randomPopulateString(int seed = 0) {
		int i, j, priority;
		string value;

		value = _randomString(seed);
		enqueue(value, 5);

		for (i = 1; i < 21; i++) {
			for (j = 1; j < 6; j++) {
				priority = randInteger(1, 10 - ((i / 2) - j));
				value = _randomString(seed);
				enqueue(value, priority);
			}
		}
	}
};
