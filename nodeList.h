#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "rng.h"

using namespace std;

#pragma once

template<typename T>
class nodeList {
private:
	struct NODE {
		int priority;
		T value;
		NODE* next;
	};
	NODE* head;
	int size;

public:
	nodeList() {
		head = nullptr;
		size = 0;
	}

	void insert(T value, int priority) {
		NODE* prev = nullptr;
		NODE* cur = head;

		while (cur != nullptr) {
			prev = cur;
			cur = cur->next;
		}

		NODE* n = new NODE();
		n->priority = priority;
		n->value = value;
		n->next = nullptr;

		if (prev == nullptr)
			head = n;
		else
			prev->next = n;
		
		size++;
	}

	void toString(ostream& output) {
		NODE* cur = head;

		while (cur != nullptr) {
			output << cur->priority << " value: " << cur->value << endl;
			cur = cur->next;
		}
	}

	void clear() {
		NODE* curr = head;
		NODE* next = nullptr;

		while (curr != nullptr) {
			next = curr->next;
			delete curr;
			curr = next;
		}
		head = nullptr;
	}
};