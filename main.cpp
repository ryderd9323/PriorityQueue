#include <vector>
#include "nodelist.h"
#include "rng.h"
#include "priorityqueue.h"

using namespace std;

int main() {
	priorityqueue<char> pq, other;
	char pqValue, otherValue;
	int otherPriority, pqSize;

	pq._defaultPopulate();
	other = pq;
	pqSize = pq.Size();
	
	other.begin();

	while (other.next(otherValue, otherPriority)) {
		if (otherValue == 'i')
			cout << "## stop here" << endl;
		pqValue = pq.dequeue();
		pqSize--;
		cout << "pqValue (" << pqValue << ") should equal otherValue (" << otherValue << ")\n"
			 << "pq.Size() (" << pq.Size() << ") should equal pqSize (" << pqSize << ")" << endl;
	}

	pqValue = pq.dequeue();
	cout << "pqValue (" << pqValue << ") should equal otherValue (" << otherValue << ")\n"
		 << "pqSize (" << pqSize << ") should equal 0" << endl;

	pq.clear();
	other.clear();
}