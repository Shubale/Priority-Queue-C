#pragma once
#include <stdio.h>
#include <stdlib.h>
#define LOWEST_PRIORITY 10
#define HIGHEST_PRIORITY 0

struct node {
	const char* name;
	int priority;
	struct node* next;
	struct node* prev;
};

struct queue {
	int queue_number;
	struct node* head;
	struct node* tail;
};
struct queue* construct_queue(int queue_number); // done
void push(struct queue* q, const char* name); // done
void pop(struct queue* q); // done
void print(struct queue* q); // done
void insert(struct queue* q, int priority, const char* name); // done
void delete_by_priority(struct queue* q, struct node* n, int priority);
//void delete_by_priority2(queue* q, node* n, int priority);
void queue_merge_two(struct queue* q1, struct queue* q2); // done


struct queue* construct_queue(int queue_number) {
	// allocate memory and set all of queue's parameters, then return new queue
	struct queue* q = malloc(sizeof(struct queue));
	q->head = NULL;
	q->tail = NULL;
	q->queue_number = queue_number;
	return q;
}


void push(struct queue* q, const char* name) {
	// new node is created and it's parameters set, exept for next element, since we don't know yet if next element is NULL or tail
	struct node* n = malloc(sizeof(struct node));
	n->priority = LOWEST_PRIORITY;
	n->name = name;
	n->prev = NULL;

	// when queue is empty next element is NULL
	if (q->head == NULL) {
		n->next = NULL;
		n->priority = HIGHEST_PRIORITY;
		q->head = n;
		q->tail = n;
		return;
	}

	// and if not next element is tail, then joints are fixed
	n->next = q->tail;
	q->tail->prev = n;
	q->tail = n;

}

void pop(struct queue* q) {
	// if queue is empty no action is needed
	if (q->head == NULL) {
		printf("Queue %d is empty.\n", q->queue_number);
		return;
	}

	// 1 element queue is simple enough
	if (q->head == q->tail) {
		free(q);
		return;
	}

	// otherwise first element has to be deleted and head adjusted
	q->head = q->head->prev;
	free(q->head->next);
	q->head->next = NULL;
}


void print(struct queue* q) {
	// if either head or tail is NULL then queue is empty (in case of empty queue tail and head are both equal to NULL)
	if (q->head == NULL) {
		printf("Queue %d is empty.\n", q->queue_number);
		return;
	}

	// otherwise traversal temporary node is created which will print node untill traversal node is NULL
	struct node* temp = q->head;
	printf("Queue %d:\n", q->queue_number);
	while (temp != NULL) {
		printf("\t%s %d\n", temp->name, temp->priority);
		temp = temp->prev;
	}
}



void insert(struct queue* q, int priority, const char* name) {
	// empty queue
	if (q->head == NULL) {
		push(q, name);
		return;
	}

	// temporary traversing node
	struct node* temp = q->head;

	// check if priority is lower than current node, if you couldn't find suitable place before, this has to be the correct position
	while (temp != NULL && temp->priority < priority) {
		temp = temp->prev;
	}

	// if node has lowest priority (since lower priority hadn't been found) it is pushed to the queue
	if (temp == NULL) {
		push(q, name);
		return;
	}

	// 2 special scenarios have been considered, regular new node is created in correct position
	struct node* n = malloc(sizeof(struct node));
	if (n == NULL) {
		printf("Allocation error!\n");
		return;
	}
	n->name = name;
	n->priority = priority;

	// however, different rules apply if current element is head of the queue, then the first element has to be corrected
	if (temp == q->head) {
		n->prev = q->head;
		n->next = NULL;
		q->head->next = n;
		q->head = n;
		return;
	}

	// if element wasn't head pointers are corrected here
	n->next = temp->next;
	n->prev = temp;
	n->next->prev = n;
	n->prev->next = n;

}

/*void delete_by_priority2(struct queue* q, int priority) {
	struct node* temp = q->head;
	struct node* temp_temp = temp;
	while (temp != NULL) {
		if (temp->priority > priority)
			return;
		if (temp->priority == priority) {
			if (temp == q->head) {
				pop(q);
			}
			else if (temp == q->tail) {
				temp->next->prev = NULL;
				temp->next = q->tail;
				temp_temp = temp;
				free(temp);
				temp_temp;
				return;
			}
			else {

			}

		}
		temp = temp->prev;
	}
}*/

void delete_by_priority(struct queue* q, struct node* n, int priority) {
	struct node* temp_n;
	if (n->prev != NULL)
		temp_n = n->prev;
	else
		temp_n = n;
	if (n->priority != priority) {
		if (n->prev == NULL)
			return;
		delete_by_priority(q, temp_n, priority);
		return;
	}
	if (n == q->head) {
		pop(q);
		delete_by_priority(q, temp_n, priority);
		return;
	}
	if (n == q->tail) {

		q->tail = q->tail->next;
		free(q->tail->prev);
		q->tail->prev = NULL;
		return;
	}
	n->prev->next = n->next;
	n->next->prev = n->prev;
	free(n);
	delete_by_priority(q, temp_n, priority);
}


void queue_merge_two(struct queue* q1, struct queue* q2) {
	// this function merges queue 2 with queue 1 (q2 to q1).
	// traversal node is created from queue 2 head, then while it's not NULL that node is inserted into queue 1 then deleted
	// second temporary node is created to save next element to traverse
	struct node* temp = q2->head;
	while (temp != NULL) {
		insert(q1, temp->priority, temp->name);
		struct node* temp_temp = temp->prev;
		free(temp);
		temp = temp_temp;
	}
}