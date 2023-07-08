#include <iostream>
#include <cstring>

using namespace std;

typedef struct Cell {
    int val;       // Value stored in the cell
    Cell* Xor;     // XOR of the previous and next cells
} Cell;

typedef struct List {
    Cell* beg;     // Beginning of the list
    Cell* end;     // End of the list
    int size;      // Size of the list
    int mode;      // Mode of traversal (0 for forward, 1 for backward)
} List;

typedef struct Queue {
    int count;          // Count of elements in the queue
    Cell* front;        // Front of the queue
    Cell* back;         // Back of the queue
    Cell* before_back;  // Cell before the back cell
    Cell* before_front; // Cell before the front cell
} Queue;

// Function declarations
Cell* XOR(Cell* First, Cell* Second) {
    return (Cell*)((uintptr_t)(First) ^ (uintptr_t)(Second));
}

void add(List* list, Queue* queue);                 // Add a cell to the list
void del(List* list, Queue* queue);                 // Delete a cell from the list
void print(List* list);                             // Print the list

void push(List* list, Queue* queue);                // Push a cell to the queue
void pop(Queue* queue);                             // Pop a cell from the queue
void garbage_soft(Queue* queue, List* list);         // Perform soft garbage collection
void garbage_hard(Queue* queue, List* list);         // Perform hard garbage collection
void print_queue(Queue* queue);                      // Print the queue

void deallocate(List* list);                         // Deallocate the memory used by the list

int main() {
    char s[15];
    List list;
    list.beg = NULL;
    list.end = NULL;
    list.size = 0;
    list.mode = 0;
    Queue queue;
    queue.front = NULL;
    queue.back = NULL;
    queue.before_front = NULL;
    queue.before_back = NULL;
    queue.count = 0;

    while (cin >> s) {
        // Check the input and perform the corresponding operation
        if (strcmp(s, "PUSH") == 0)
            push(&list, &queue);  // Call push function

        else if (strcmp(s, "POP") == 0)
            pop(&queue);          // Call pop function

        else if (strcmp(s, "PRINT_QUEUE") == 0)
            print_queue(&queue);  // Call print_queue function

        else if (strcmp(s, "COUNT") == 0)
            cout << queue.count << endl;  // Print queue count

        else if (strcmp(s, "GARBAGE_SOFT") == 0)
            garbage_soft(&queue, &list);  // Call garbage_soft function

        else if (strcmp(s, "GARBAGE_HARD") == 0)
            garbage_hard(&queue, &list);  // Call garbage_hard function

        else if (strcmp(s, "SIZE") == 0)
            cout << list.size << endl;  // Print list size

        else if (strcmp(s, "ADD_BEG") == 0) {
            list.mode = 0;
            add(&list, &queue);  // Call add function at the beginning
        }

        else if (strcmp(s, "ADD_END") == 0) {
            list.mode = 1;
            add(&list, &queue);  // Call add function at the end
        }

        else if (strcmp(s, "DEL_BEG") == 0) {
            list.mode = 0;
            del(&list, &queue);  // Call delete function from the beginning
        }

        else if (strcmp(s, "DEL_END") == 0) {
            list.mode = 1;
            del(&list, &queue);  // Call delete function from the end
        }

        else if (strcmp(s, "PRINT_FORWARD") == 0) {
            list.mode = 0;
            print(&list);  // Call print function in forward mode
        }

        else if (strcmp(s, "PRINT_BACKWARD") == 0) {
            list.mode = 1;
            print(&list);  // Call print function in backward mode
        }
    }

    deallocate(&list);  // Call deallocate function

    return 0;
}

void garbage_hard(Queue* queue, List* list) {
    if (queue->count != list->size) {
        if (queue->count == 0) {
            deallocate(list);
            list->beg = NULL;
            list->end = NULL;
            queue->front = NULL;
            queue->back = NULL;
            queue->before_front = NULL;
            queue->before_back = NULL;
        }
        else {
            if (list->size == 2) {
                Cell* Help = queue->before_front;
                list->beg = queue->front;
                list->end = queue->front;
                list->end->Xor = NULL;
                queue->before_front = NULL;
                queue->before_back = NULL;
                delete Help;
            }
            else {
                Cell* Help = XOR(queue->front->Xor, queue->before_front);
                Cell* Dealloc1 = queue->back;
                Cell* Dealloc2 = XOR(queue->back->Xor, queue->before_back);
                Cell* Dealloc3 = XOR(Dealloc1, Dealloc2->Xor);
                while (XOR(Dealloc3, queue->front) != NULL) {
                    if (XOR(Dealloc1, queue->back) != NULL)
                        delete Dealloc1;
                    Dealloc1 = Dealloc2;
                    Dealloc2 = Dealloc3;
                    Dealloc3 = XOR(Dealloc1, Dealloc2->Xor);
                }

                if (XOR(Dealloc1, queue->back) != NULL)
                    delete Dealloc1;

                delete Dealloc2;

                if (queue->count == 1) {
                    queue->before_back = NULL;
                    queue->before_front = NULL;
                    list->beg = queue->front;
                    list->end = queue->front;
                    list->beg->Xor = NULL;
                }
                else if (queue->count == 2) {
                    if (XOR(queue->front, list->beg) != NULL) {
                        list->beg = queue->back;
                        list->end = queue->front;
                        queue->before_front = list->beg;
                    }
                    else {
                        list->beg = queue->front;
                        list->end = queue->back;
                        queue->before_front = list->end;
                    }
                    list->beg->Xor = XOR(queue->back, queue->front);
                    list->end->Xor = XOR(queue->back, queue->front);
                }
                else {
                    int end_beg_in_queue = 0;
                    Cell* Mover;
                    Cell* Current = queue->before_back;
                    Cell* Previous = queue->back;
                    if (XOR(Previous, list->beg) == NULL || XOR(Previous, list->end) == NULL)
                        end_beg_in_queue++;

                    for (int i = 0; i < queue->count - 1; i++) {
                        if (XOR(Current, list->beg) == NULL || XOR(Current, list->end) == NULL)
                            end_beg_in_queue++;

                        Mover = Current;
                        Current = XOR(Current->Xor, Previous);
                        Previous = Mover;
                    }

                    if (end_beg_in_queue == 2) {
                        Cell* Help2 = XOR(queue->front->Xor, queue->before_front);
                        queue->before_front = queue->back;
                        queue->front->Xor = XOR(Help2, queue->back);
                        queue->back->Xor = XOR(queue->before_back, queue->front);
                    }
                    else {
                        list->beg = queue->back;
                        list->end = queue->front;
                        list->beg->Xor = XOR(queue->before_back, queue->front);
                        list->end->Xor = XOR(queue->back, Help);
                        queue->before_front = list->beg;
                    }
                }
            }
        }
        list->size = queue->count;
    }
}



void garbage_soft(Queue* queue, List* list) {
    if (queue->count != list->size) {
        if (queue->count == 0) {
            if (list->size == 1)
                list->beg->val = 0;
            else if (list->size == 2) {
                list->beg->val = 0;
                list->end->val = 0;
            }
            else {
                Cell* Previous = list->end;
                Cell* Current = list->beg;
                Cell* Mover;
                while (XOR(Current, list->end) != NULL) {
                    Current->val = 0;
                    Mover = Current;
                    Current = XOR(Current->Xor, Previous);
                    Previous = Mover;
                }
                Current->val = 0;
            }
        }
        else {
            if (list->size == 2)
                queue->before_front->val = 0;
            else {
                Cell* Previous = queue->back;
                Cell* Current = XOR(queue->back->Xor, queue->before_back);
                Cell* Mover;
                while (XOR(Current, queue->front) != NULL) {
                    Current->val = 0;
                    Mover = Current;
                    Current = XOR(Current->Xor, Previous);
                    Previous = Mover;
                }
            }
        }
    }
}



void print_queue(Queue* queue) {
    if (queue->count == 0)
        cout << "NULL" << endl;
    else if (queue->count == 1)
        cout << queue->front->val << endl;
    else if (queue->count == 2)
        cout << queue->front->val << " " << queue->back->val << endl;
    else {
        Cell* Previous = queue->before_front;
        Cell* Current = queue->front;
        Cell* Mover;
        while (XOR(Current, queue->back) != NULL) {
            cout << Current->val << " ";
            Mover = Current;
            Current = XOR(Current->Xor, Previous);
            Previous = Mover;
        }
        cout << Current->val << " ";
        cout << endl;
    }
}



void pop(Queue* queue) {
    if (queue->count == 0)
        cout << "NULL" << endl;
    else if (queue->count == 1)
        cout << queue->front->val << endl;
    else if (queue->count == 2) {
        cout << queue->front->val << endl;
        queue->front = queue->back;
        queue->before_front = queue->front;
    }
    else {
        cout << queue->front->val << endl;
        Cell* Help = XOR(queue->front->Xor, queue->before_front);
        queue->before_front = queue->front;
        queue->front = Help;
    }

    if (queue->count != 0)
        (queue->count)--;
}



void push(List* list, Queue* queue) {
    if (list->size == 0) {
        add(list, queue);
        queue->back = list->beg;
        queue->front = list->beg;
    }
    else if (queue->count == list->size) {
        if (queue->count == 1 || XOR(list->beg, queue->back) == NULL) {
            list->mode = 0;
            add(list, queue);
            queue->before_back = queue->back;
            queue->back = list->beg;
            queue->before_front = list->beg;
        }
        else {
            Cell* New_Cell = new Cell;
            cin >> New_Cell->val;
            if (queue->count > 2) {
                Cell* Help;
                New_Cell->Xor = XOR(queue->front, queue->back);
                Help = XOR(queue->front->Xor, queue->before_front);
                queue->front->Xor = XOR(Help, New_Cell);
                queue->back->Xor = XOR(queue->before_back, New_Cell);
            }
            else {
                New_Cell->Xor = XOR(queue->back, queue->front);
                queue->back->Xor = XOR(queue->front, New_Cell);
                queue->front->Xor = XOR(queue->back, New_Cell);
            }
            queue->before_back = queue->back;
            queue->before_front = New_Cell;
            queue->back = New_Cell;
            (list->size)++;
        }
    }
    else {
        if (queue->count == 0) {
            if (queue->front == NULL) {
                cin >> list->end->val;
                queue->front = list->end;
                queue->back = list->end;
                if (list->size >= 2) {
                    queue->before_front = list->beg;
                    queue->before_back = list->beg;
                }
            }
            else
                cin >> queue->front->val;
        }
        else {
            if (list->size == 2) {
                cin >> queue->before_back->val;
                queue->back = queue->before_back;
                queue->before_back = queue->front;
            }
            else {
                Cell* Temp_Cell = XOR(queue->back->Xor, queue->before_back);
                cin >> Temp_Cell->val;
                queue->before_back = queue->back;
                queue->back = Temp_Cell;
            }
        }
    }
    (queue->count)++;
}



void add(List* list, Queue* queue) {
    Cell* New_Cell = new Cell;
    cin >> New_Cell->val;
    if (list->size == 0) {
        New_Cell->Xor = NULL;
        list->beg = New_Cell;
        list->end = New_Cell;
    }
    else {
        if (list->size == 1) {
            list->beg->Xor = XOR(list->beg, New_Cell);
            New_Cell->Xor = XOR(list->beg, New_Cell);
            if (queue->count == 1) {
                queue->before_back = New_Cell;
                queue->before_front = New_Cell;
            }
        }
        else if (list->size == 2) {
            New_Cell->Xor = XOR(list->beg, list->end);
            list->beg->Xor = XOR(list->end, New_Cell);
            list->end->Xor = XOR(list->beg, New_Cell);
            if (XOR(queue->front, list->beg) == NULL && XOR(queue->back, list->end) == NULL) {
                (queue->count)++;
                queue->before_back = New_Cell;
            }
            else {
                if (XOR(queue->front, list->end) == NULL)
                    queue->before_front = New_Cell;
                if (XOR(queue->back, list->end) == NULL)
                    queue->before_back = New_Cell;
            }
        }
        else {
            if (queue->count > 1) {
                Cell* Previous = queue->front;
                Cell* Current = XOR(queue->before_front, queue->front->Xor);
                Cell* Mover;
                int end_inside = 0;
                while (XOR(Current, queue->back) != NULL) {
                    if (XOR(Current, list->end) == NULL) {
                        end_inside++;
                        break;
                    }
                    Mover = Current;
                    Current = XOR(Current->Xor, Previous);
                    Previous = Mover;
                }

                if (XOR(Current, list->end) == NULL)
                    end_inside++;
                if (end_inside != 0)
                    (queue->count)++;

                if (XOR(queue->back, list->end) == NULL)
                    queue->before_back = New_Cell;
                else if (XOR(queue->front, list->end) == NULL)
                    queue->before_front = New_Cell;
            }
            list->beg->Xor = XOR(XOR(list->beg->Xor, list->end), New_Cell);
            list->end->Xor = XOR(XOR(list->end->Xor, list->beg), New_Cell);
            New_Cell->Xor = XOR(list->beg, list->end);
        }

        if (list->mode == 0)
            list->beg = New_Cell;
        else
            list->end = New_Cell;
    }
    (list->size)++;
}



void del(List* list, Queue* queue) {
    if (list->size != 0) {
        if (list->size == 1) {
            delete list->beg;
            queue->front = NULL;
            queue->back = NULL;
            queue->count = 0;
            list->beg = NULL;
            list->end = NULL;
        }
        else {
            if (list->mode == 0) {
                if (list->size >= 3) {
                    if (XOR(list->beg, queue->front) == NULL) {
                        if (queue->count == 1 || queue->count == 0)
                            queue->back = list->end;
                        if (queue->count == 2)
                            queue->before_back = queue->before_front;

                        queue->front = list->end;

                        if (queue->count != 0)
                            (queue->count)--;
                    }
                    else if (XOR(list->beg, queue->back) == NULL) {
                        Cell* Help = queue->before_back;
                        queue->before_back = XOR(queue->back, queue->before_back->Xor);
                        queue->back = Help;

                        if (XOR(queue->before_front, list->beg) == NULL)
                            queue->before_front = Help;
                        if (queue->count != 0)
                            (queue->count)--;
                    }
                    else if (XOR(list->beg, queue->before_front) == NULL) {
                        queue->before_front = XOR(queue->front, queue->before_front->Xor);
                        if (queue->count == 1 || queue->count == 0)
                            queue->before_back = XOR(queue->back, queue->before_back->Xor);
                    }
                    else if (XOR(list->beg, queue->before_back) == NULL) {
                        queue->before_back = XOR(queue->back, queue->before_back->Xor);
                        if (queue->count > 2)
                            (queue->count)--;
                    }

                    Cell* New_Beg = XOR(list->beg->Xor, list->end);
                    New_Beg->Xor = XOR(XOR(New_Beg->Xor, list->beg), list->end);
                    list->end->Xor = XOR(XOR(list->end->Xor, list->beg), New_Beg);
                    delete list->beg;
                    list->beg = New_Beg;

                    if (list->size == 3) {
                        list->end->Xor = XOR(list->end, list->beg);
                        list->beg->Xor = XOR(list->end, list->beg);
                    }
                }
                else if (list->size == 2) {
                    if (XOR(list->beg, queue->front) == NULL || XOR(list->beg, queue->back) == NULL) {
                        (queue->count)--;
                        queue->front = list->end;
                        queue->before_back = NULL;
                        queue->before_front = NULL;
                        queue->back = list->end;
                        delete list->beg;
                        list->beg = list->end;
                        list->beg->Xor = NULL;
                    }
                }
            }
            else {
                if (list->size >= 3) {
                    if (XOR(list->end, queue->front) == NULL) {
                        if (queue->count == 1 || queue->count == 0)
                            queue->back = XOR(queue->front->Xor, queue->before_front);
                        if (queue->count == 2)
                            queue->before_back = XOR(queue->before_back->Xor, queue->back);
                        queue->front = XOR(queue->front->Xor, queue->before_front);
                        if (queue->count != 0)
                            (queue->count)--;
                    }
                    else if (XOR(list->end, queue->back) == NULL) {
                        Cell* Help = queue->before_back;
                        queue->before_back = XOR(queue->back, queue->before_back->Xor);
                        queue->back = Help;

                        if (XOR(queue->before_front, list->end) == NULL)
                            queue->before_front = Help;
                        if (queue->count != 0)
                            (queue->count)--;
                    }
                    else if (XOR(list->end, queue->before_front) == NULL) {
                        queue->before_front = XOR(queue->front, queue->before_front->Xor);
                        if (queue->count == 1 || queue->count == 0)
                            queue->before_back = XOR(queue->back, queue->before_back->Xor);
                    }
                    else if (XOR(list->end, queue->before_back) == NULL) {
                        queue->before_back = list->beg;
                        if (queue->count > 2)
                            (queue->count)--;
                    }

                    Cell* New_End = XOR(list->end->Xor, list->beg);
                    New_End->Xor = XOR(XOR(New_End->Xor, list->end), list->beg);
                    list->beg->Xor = XOR(XOR(list->beg->Xor, list->end), New_End);
                    delete list->end;
                    list->end = New_End;

                    if (list->size == 3) {
                        list->end->Xor = XOR(list->end, list->beg);
                        list->beg->Xor = XOR(list->end, list->beg);
                    }
                }
                else if (list->size == 2) {
                    if (XOR(list->end, queue->front) == NULL || XOR(list->end, queue->back) == NULL) {
                        (queue->count)--;
                        queue->front = list->beg;
                        queue->back = list->beg;
                        queue->before_back = NULL;
                        queue->before_front = NULL;
                        delete list->end;
                        list->end = list->beg;
                        list->end->Xor = NULL;
                    }
                }
            }
        }
        (list->size)--;
    }
}



void print(List* list) {
    if (list->size != 0) {
        int s = list->size;
        Cell* Previous;
        Cell* Current;
        if (list->mode == 0) {
            Previous = list->end;
            Current = list->beg;
        }
        else {
            Previous = list->beg;
            Current = list->end;
        }
        Cell* Mover;
        if (list->size == 1)
            cout << Current->val;
        else if (list->size == 2) {
            cout << Current->val << " " << Previous->val;
        }
        else {
            for (int i = 0; i < s; i++) {
                cout << Current->val << " ";
                Mover = Current;
                Current = XOR(Previous, Current->Xor);
                Previous = Mover;
            }
        }
        cout << endl;
    }
    else
        cout << "NULL" << endl;
}



void deallocate(List* list) {
    if (list->size != 0) {
        Cell* Dealloc1 = list->end;
        Cell* Dealloc2 = list->beg;
        Cell* Dealloc3 = XOR(Dealloc1, Dealloc2->Xor);
        while (XOR(Dealloc3, list->end) != NULL) {
            if (XOR(Dealloc1, list->end) != NULL)
                delete Dealloc1;
            Dealloc1 = Dealloc2;
            Dealloc2 = Dealloc3;
            Dealloc3 = XOR(Dealloc1, Dealloc2->Xor);
        }
        if (XOR(Dealloc1, list->end) != NULL)
            delete Dealloc1;
        delete Dealloc2;
    }
}