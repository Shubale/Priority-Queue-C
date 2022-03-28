#include "Priority_Queue.h"

int main() {
	struct queue *queue1 = construct_queue(1);
	for (int i = 0; i < 5; i++)
		push(queue1, "Czarek");
	print(queue1);

	return 0;
}