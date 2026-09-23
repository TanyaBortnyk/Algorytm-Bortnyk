#pragma once

struct Node
{
	int data;
	Node* next;
};

// Базові функції
void addHead(Node*& head, int value);
void addTail(Node*& head, int value);
bool isEmpty(Node* head);
void printList(Node* head);
void clearList(Node*& head);
int getLength(Node* head);

void insertAfterFirstPositive(Node* head, int newElement); 
void deleteIthElement(Node*& head, int index);            
void reverseList(Node*& head);                            
