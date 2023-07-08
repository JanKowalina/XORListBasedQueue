# XOR List Based Queue

This project implements a FIFO (First-In-First-Out) queue using a doubly or singly linked list. It extends the functionality of the XOR linked list assignment and can reuse the list implemented for that assignment. Alternatively, an STL `<list>` can be used.

I decided to implement it based on my own implementation of XOR linked list.

## Compilation and Execution

To compile the program, follow these steps:

1. Use a C++ compiler (such as g++) to compile the code:

   ```shell
   g++ -o XORListBasedQueue XORListBasedQueue.cpp
   ```

2. Run the program using the following command:

   ```shell
   ./XORListBasedQueue
   ```

   The program will start running and wait for user input.


## Functionality

In addition to the functions of the XOR list (ADD_BEG, ADD_END, DEL_BEG, DEL_END, PRINT_FORWARD, PRINT_BACKWARD), the queue implements the following commands:

- `SIZE`: Returns the number of elements in the list.
- `PUSH N`: Adds element N to the end of the queue.
- `POP`: Removes the first element from the queue.
- `PRINT_QUEUE`: Prints the queue from the first element to the lastly added.
- `COUNT`: Returns the number of elements in the queue (≤ SIZE).
- `GARBAGE_SOFT`: Changes the values of all elements in the list that do not belong to the queue to 0.
- `GARBAGE_HARD`: Removes all elements from the list that do not belong to the queue.

The FIFO queue extends the list with pointers to the beginning (`FRONT`) and the end (`BACK`) of the queue. After inserting an element to the queue, the pointer to the end of the queue (`BACK`) is updated to point to the next element in the list. When removing an element from the queue (`FRONT`), the pointer starts pointing at the element after the previously pointed element. To avoid memory loss and enable reusability, a cyclical list or starting from the beginning after reaching the end of the queue can be used.

## Handling Exceptions

The implementation handles the following exceptions:

- Inserting an element into an empty queue (no elements).
- Adding elements to a full list (the number of elements in the queue equals the number of elements in the list).
- Trying to remove elements from an empty queue.

## List State

- When the list is empty (no elements), the pointers (`FRONT` and `BACK`) point to `nullptr`.
- When adding an element (`PUSH N`) to the queue and the list is empty, one element is added to the list before executing the operation.
- When the list is not empty but all the elements are occupied by the queue, the list is extended by inserting the element before the element pointed to by the `BACK` pointer.

## Garbage Collection

From time to time, it may be necessary to remove elements from the list that do not belong to the queue. The `GARBAGE_SOFT` operation changes the values of such elements to 0. The `GARBAGE_HARD` operation removes all elements from the list that do not belong to the queue.

## Input and Output

### Input

The program accepts a series of commands for the list or the queue.

### Output

The program outputs the results of the operations of the commands on the queue or directly on the list. The initial state of the list and the queue is empty, and their state evolves based on the executed commands. Some commands do not generate any output but influence the state of the list, the queue, and subsequent commands that print information.

## Examples

### Example 1

Input:

```
PUSH 1
PRINT_FORWARD
PRINT_QUEUE
SIZE
COUNT
PUSH 2
PRINT_FORWARD
PRINT_QUEUE
SIZE
COUNT
PUSH 3
PRINT_FORWARD
PRINT_QUEUE
SIZE
COUNT
POP
PRINT_FORWARD
PRINT_QUEUE
SIZE
COUNT
PUSH 4
PRINT_FORWARD
PRINT_QUEUE
SIZE
COUNT
POP
PRINT_FORWARD
PRINT_QUEUE
SIZE
COUNT
POP
PRINT_FORWARD
PRINT_QUEUE
SIZE
COUNT
POP
PRINT_FORWARD
PRINT_QUEUE
SIZE
COUNT
POP
PRINT_FORWARD
PRINT_QUEUE
SIZE
COUNT
```

Output:

```
1 
1
1
1
2 1 
1 2 
2
2
3 2 1 
1 2 3 
3
3
1
3 2 1 
2 3 
3
2
3
3 2 4 
2 3 4 
3
3
2
3 2 4 
3 4 
3
2
3
3 2 4 
4 
3
1
4
3 2 4 
NULL
3
0
NULL
3 2 4 
NULL
3
0
```

### Example 2

Input:

```
ADD_BEG 1
ADD_BEG 2
ADD_BEG 3
ADD_BEG 4
ADD_BEG 5
PRINT_FORWARD
PRINT_QUEUE
PUSH 10
PRINT_FORWARD
PRINT_QUEUE
PUSH 11
PRINT_FORWARD
PRINT_QUEUE
PUSH 12
PRINT_FORWARD
PRINT_QUEUE
PUSH 13
PRINT_FORWARD
PRINT_QUEUE
PUSH 14
PRINT_FORWARD
PRINT_QUEUE
PUSH 15
PRINT_FORWARD
PRINT_QUEUE
POP
POP
POP
PRINT_FORWARD
PRINT_QUEUE
POP
POP
POP
PRINT_FORWARD
PRINT_QUEUE
```

Output:

```
5 4 3 2 1 
NULL
5 4 3 2 10 
10 
5 4 3 11 10 
10 11 
5 4 12 11 10 
10 11 12 
5 13 12 11 10 
10 11 12 13 
14 13 12 11 10 
10 11 12 13 14 
15 14 13 12 11 10 
10 11 12 13 14 15 
10
11
12
15 14 13 12 11 10 
13 14 15 
13
14
15
15 14 13 12 11 10 
NULL
```

### Example 3

Input:

```
ADD_END 42
PRINT_FORWARD
GARBAGE_SOFT
PRINT_FORWARD
PUSH 10
PRINT_FORWARD
GARBAGE_SOFT
PRINT_FORWARD
ADD_END 6
ADD_END 5
ADD_END 4
ADD_END 3
ADD_END 2
ADD_END 1
ADD_BEG 8
ADD_BEG 9
PRINT_FORWARD
PUSH 11
PUSH 12
PUSH 13
PUSH 14
PRINT_FORWARD
GARBAGE_SOFT
PRINT_FORWARD
```

Output:

```
42 
0 
10 
10 
9 8 10 6 5 4 3 2 1 
12 11 10 6 5 4 3 14 13 
12 11 10 0 0 0 0 14 13 
```