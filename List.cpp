#include "List.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

void addHead(Node*& head, int value)
{
	Node* newNode = new Node;
	newNode->data = value;
	newNode->next = head;
	head = newNode;
}

void addTail(Node*& head, int value)
{
	Node* newNode = new Node;
	newNode->data = value;
	newNode->next = nullptr;

	if (head == nullptr)
	{
		head = newNode;
		return;
	}
	 
	Node* current = head;
	while (current->next != nullptr)
	{
		current = current->next;
	}
	current->next = newNode;
}

bool isEmpty(Node* head)
{
	return head == nullptr;
}

void printList(Node* head)
{
	Node* current = head;
	while (current != nullptr)
	{
		cout << current->data << " ";
		current = current->next;
	}
	cout << endl;
}

void clearList(Node*& head)
{
	while (head != nullptr)
	{
		Node* temp = head;
		head = head->next;
		delete temp;
	}
}

int getLength(Node* head)
{
	int length = 0;
	Node* current = head;
	while (current != nullptr)
	{
		length++;
		current = current->next;
	}
	return length;
}

void insertAfterFirstPositive(Node* head, int newElement)
{
	cout << "\n============================================================\n";
	cout << "        TASK A: insertAfterFirstPositive\n";
	cout << "============================================================\n";
	cout << left
		<< setw(6) << "Step"
		<< setw(15) << "Current"
		<< setw(10) << "Data"
		<< setw(15) << "Next"
		<< "Action" << endl;
	cout << "------------------------------------------------------------\n";

	Node* current = head;
	int step = 1;
	bool inserted = false;

	while (current != nullptr)
	{
		cout << left << setw(6) << step;
		cout << setw(15) << ("Node(" + to_string(current->data) + ")");
		cout << setw(10) << current->data;

		if (current->next != nullptr)
			cout << setw(15) << ("Node(" + to_string(current->next->data) + ")");
		else
			cout << setw(15) << "NULL";

		if (!inserted && current->data > 0)
		{
			Node* newNode = new Node;
			newNode->data = newElement;
			newNode->next = current->next;
			current->next = newNode;

			cout << "Inserted Node(" << newElement << ") after Node(" << current->data << ")" << endl;
			inserted = true;
			break; // Завершуємо роботу після першої вставки
		}
		else
		{
			cout << "Move current -> next" << endl;
			current = current->next;
		}

		step++;
	}

	cout << "------------------------------------------------------------\n";
	if (!inserted)
	{
		cout << "No positive elements found in the list.\n";
	}
	cout << "Result: ";
	printList(head);
	cout << "============================================================\n";
}

void deleteIthElement(Node*& head, int index)
{
	cout << "\n============================================================\n";
	cout << "        TASK B: deleteIthElement (Index: " << index << ")\n";
	cout << "============================================================\n";
	cout << left
		<< setw(6) << "Step"
		<< setw(15) << "Current"
		<< setw(15) << "Previous"
		<< "Action" << endl;
	cout << "------------------------------------------------------------\n";

	int length = getLength(head);
	if (index < 1 || index > length)
	{
		cout << "Invalid index: " << index << ". (List length is " << length << ")\n";
		cout << "============================================================\n";
		return;
	}

	Node* current = head;
	Node* previous = nullptr;
	int step = 1;

	while (current != nullptr && step < index)
	{
		cout << left << setw(6) << step;
		cout << setw(15) << ("Node(" + to_string(current->data) + ")");

		if (previous != nullptr)
			cout << setw(15) << ("Node(" + to_string(previous->data) + ")");
		else
			cout << setw(15) << "NULL";

		cout << "Move to next node" << endl;

		previous = current;
		current = current->next;
		step++;
	}

	// Табличний вивід для кроку видалення
	cout << left << setw(6) << step;
	cout << setw(15) << ("Node(" + to_string(current->data) + ")");
	if (previous != nullptr)
		cout << setw(15) << ("Node(" + to_string(previous->data) + ")");
	else
		cout << setw(15) << "NULL";

	cout << "Deleting Node(" << current->data << ")" << endl;

	if (previous == nullptr)
	{
		head = head->next;
	}
	else
	{
		previous->next = current->next;
	}

	delete current;

	cout << "------------------------------------------------------------\n";
	cout << "Result: ";
	printList(head);
	cout << "============================================================\n";
}

void reverseList(Node*& head)
{
	cout << "\n============================================================\n";
	cout << "        TASK C: reverseList\n";
	cout << "============================================================\n";
	cout << left
		<< setw(6) << "Step"
		<< setw(15) << "Previous"
		<< setw(15) << "Current"
		<< setw(15) << "Next" << endl;
	cout << "------------------------------------------------------------\n";

	Node* prev = nullptr;
	Node* current = head;
	Node* next = nullptr;
	int step = 1;

	while (current != nullptr)
	{
		next = current->next;

		cout << left << setw(6) << step;

		if (prev != nullptr)
			cout << setw(15) << ("Node(" + to_string(prev->data) + ")");
		else
			cout << setw(15) << "NULL";

		cout << setw(15) << ("Node(" + to_string(current->data) + ")");

		if (next != nullptr)
			cout << setw(15) << ("Node(" + to_string(next->data) + ")");
		else
			cout << setw(15) << "NULL";

		cout << endl;

		current->next = prev;
		prev = current;
		current = next;

		step++;
	}

	head = prev;

	cout << "------------------------------------------------------------\n";
	cout << "Result: ";
	printList(head);
	cout << "============================================================\n";
}