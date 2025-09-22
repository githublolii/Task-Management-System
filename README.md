# Task Management System

This project implements a simple Task Management System in C, allowing users to manage their tasks through a command-line interface. It supports loading tasks from a file, adding new tasks, deleting tasks, searching for tasks, marking tasks as performed, undoing performed tasks, viewing different task lists, and generating a summary report.

## Features

*   **Load Tasks from File:** Load initial tasks from a specified text file (`tasks.txt`).
*   **Add New Task:** Create and add a new task with an ID, name, date, and duration.
*   **Delete Task:** Remove a task from the unperformed tasks list by its ID.
*   **Search for Task:** Find tasks by either their ID or name.
*   **View Unperformed Tasks:** Display all tasks that have not yet been marked as performed.
*   **Perform Task:** Move a task from the unperformed list to the performed tasks stack.
*   **Undo Last Performed Task:** Revert the last performed task back to the unperformed list.
*   **View Performed Tasks:** Display all tasks that have been marked as performed (from the stack).
*   **Generate Summary Report:** Create a `Report.txt` file containing lists of performed, undone, and remaining unperformed tasks.
*   **Duplicate ID Check:** Prevents adding tasks with an ID that already exists in either the unperformed list or performed stack.
*   **Memory Management:** Proper allocation and deallocation of memory for tasks.

## Data Structures

The system uses two primary data structures:

1.  **`TasksList` (Singly Linked List):** Used to store unperformed and undone tasks.
    ```c
    typedef struct Task {
        int id;
        char name[SIZE];
        char date[DATE_LENGTH];
        float duration;
        int IsUndone; // Flag to indicate if a task was undone
        struct Task *next;
    } Task;

    typedef struct TasksList {
        Task *head;
    } TasksList;
    ```
2.  **`PerformedTasksStack` (Singly Linked List acting as a Stack):** Used to store tasks that have been performed, allowing for easy undo functionality.
    ```c
    typedef struct PerformedTasksStack {
        Task *top; // Points to the top of the stack (most recently performed task)
    } PerformedTasksStack;
    ```

## How to Compile and Run

### Prerequisites

*   A C compiler (e.g., GCC)

### Compilation

1.  Save the provided C code as `main.c`.
2.  Open a terminal or command prompt.
3.  Navigate to the directory where you saved `main.c`.
4.  Compile the code using a C compiler:

    ```bash
    gcc main.c -o task_manager
    ```

### Running the Program

1.  After successful compilation, run the executable:

    ```bash
    ./task_manager
    ```

2.  The program will present a menu of options. Follow the prompts to interact with the task management system.

### `tasks.txt` File Format

The `loadTasks` function expects a `tasks.txt` file with each task on a new line, using `#` as a delimiter:
## Functions Overview

*   `initStack(PerformedTasksStack *stack)`: Initializes an empty stack.
*   `addTask(TasksList *list, int id, char *name, char *date, float duration)`: Adds a new task to the unperformed tasks list.
*   `addUndoneTask(TasksList *list, Task* task)`: Adds an undone task back to the unperformed tasks list.
*   `deleteTask(TasksList *list, int id)`: Deletes a task by ID from the unperformed tasks list.
*   `performTask(TasksList *unperformedTasks, PerformedTasksStack *performedTasks, int id)`: Moves a task from unperformed to performed.
*   `undoLastPerformedTask(TasksList *unperformedTasks, PerformedTasksStack *performedTasks)`: Moves the top task from the performed stack back to unperformed.
*   `searchById(TasksList *list, int id)`: Searches for a task by its ID.
*   `searchByName(TasksList *list, char name[])`: Searches for a task by its name.
*   `loadTasks(TasksList *list, const char *fileName)`: Loads tasks from a specified file.
*   `displayUnperformedTasks(TasksList *list)`: Prints all unperformed tasks.
*   `performedTasksPrint(PerformedTasksStack *stack)`: (Note: The actual implementation uses `printStack`) Prints all performed tasks.
*   `generateSummaryReport(TasksList *unperformedTasks, PerformedTasksStack *performedTasks)`: Creates a report file.
*   `printTask(Task* task)`: Helper function to print a single task's details to console.
*   `printStack(PerformedTasksStack *stack)`: Prints all tasks in the performed stack to console.
*   `statusCheck(Task* task)`: (Currently unused in `main`, but could be used for specific status checks).
*   `CheckDuplicateId(TasksList *unperformedTasks, PerformedTasksStack *performedTasks, int id)`: Checks for duplicate task IDs.
*   `pop(PerformedTasksStack *stack)`: Removes and returns the top task from the stack.
*   `push(PerformedTasksStack *stack, Task *task)`: Adds a task to the top of the stack.
*   `printStackToFile(PerformedTasksStack *stack, FILE* output)`: Prints performed tasks to a file.
*   `destroy(TasksList* taskList, PerformedTasksStack* stack)`: Frees all allocated memory for both lists/stacks.
*   `printTaskToFile(Task* task, FILE* output)`: Helper function to print a single task's details to a file.
*   `Menu()`: Displays the main menu options to the user.
## Author

*   Laila Awad
