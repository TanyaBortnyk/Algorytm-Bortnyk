#include <iostream>
#include "List.h"

using namespace std;

int main()
{
	Node* head = nullptr;
	int n;

	cout << "Enter number of elements: ";
	cin >> n;

	cout << "Enter " << n << " elements:" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << i + 1 << ") ";
		int value;
		cin >> value;
		addTail(head, value);
	}

	cout << "\nInitial list: ";
	printList(head);
	cout << "Length: " << getLength(head) << endl;

	int E;
	cout << "\n--- TASK A ---" << endl;
	cout << "Enter element E to insert after the first positive element: ";
	cin >> E;

	insertAfterFirstPositive(head, E);
	cout << "Length after Task A: " << getLength(head) << endl;

	int idx;
	cout << "\n--- TASK B ---" << endl;
	cout << "Enter index i of element to delete (1-based): ";
	cin >> idx;

	deleteIthElement(head, idx);
	cout << "Length after Task B: " << getLength(head) << endl;

	cout << "\n--- TASK C ---" << endl;
	reverseList(head);
	cout << "Length after Task C: " << getLength(head) << endl;

	clearList(head);

	return 0;
}
