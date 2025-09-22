// Laila Awad - 1230085 - Section 4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 70
#define DATE_LENGTH 11

typedef struct Task {
    int id;
    char name[SIZE];
    char date[DATE_LENGTH];
    float duration;
    int IsUndone;
    struct Task *next;
} Task;


typedef struct TasksList {
    Task *head;
} TasksList;

typedef struct PerformedTasksStack {
    Task *top;
} PerformedTasksStack;

void initStack(PerformedTasksStack *stack) {
    stack->top = NULL;
}
int isLoaded = 0;
void initStack(PerformedTasksStack *stack);
void addTask(TasksList *list, int id, char *name, char *date, float duration);
void addUndoneTask(TasksList *list, Task* task);
int deleteTask(TasksList *list, int id);
void performTask(TasksList *unperformedTasks, PerformedTasksStack *performedTasks, int id);
void undoLastPerformedTask(TasksList *unperformedTasks, PerformedTasksStack *performedTasks);
void searchById(TasksList *list, int id);
void searchByName(TasksList *list, char name[]);
void loadTasks(TasksList *list, const char *fileName);
void displayUnperformedTasks(TasksList *list);
void performedTasksPrint(PerformedTasksStack *stack);
void generateSummaryReport(TasksList *unperformedTasks, PerformedTasksStack *performedTasks);
void printTask(Task* task);
void printStack(PerformedTasksStack *stack);
void statusCheck(Task* task);
int CheckDuplicateId(TasksList *unperformedTasks, PerformedTasksStack *performedTasks, int id);
Task* pop(PerformedTasksStack *stack);
void push(PerformedTasksStack *stack, Task *task);
void printStackToFile(PerformedTasksStack *stack, FILE* output);
void destroy(TasksList* taskList, PerformedTasksStack* stack);
void printTaskToFile(Task* task, FILE* output);
void Menu();


int main() {
    int id;
    char name[SIZE];
    char date[DATE_LENGTH];
    float duration;


    PerformedTasksStack performedTasks;
    TasksList unperformedTasks;
    unperformedTasks.head = NULL;
    performedTasks.top = NULL;

    printf("Hello! Welcome to My Task Management System.\n");

    int choice;

    while(1) {
        Menu();
        printf("\n");
        scanf("%d", &choice);

        switch(choice) {
        case 1:
            loadTasks(&unperformedTasks, "tasks.txt");
            break;

        case 2:
            printf("Please Avoid Using Zeros or Spaces!\n");

            do {
                printf("Enter Task ID: ");
                scanf("%d", &id);

                if(CheckDuplicateId(&unperformedTasks, &performedTasks, id)) {
                    printf("This ID already exists.Could not add task\n");
                }
            } while(CheckDuplicateId(&unperformedTasks, &performedTasks, id));

            printf("Enter Task Name: ");
            fgetchar();
            fgets(name, sizeof(name), stdin);
            strcpy(name, strtok(name, "\n"));

            printf("Enter Task Date (D/M/Y): ");
            fgets(date, sizeof(date), stdin);
            strcpy(date, strtok(date, "\n"));

            printf("Enter Task Duration in hours: ");
            scanf("%f", &duration);
            addTask(&unperformedTasks, id, name, date, duration);
            break;

        case 3:
            printf("Enter Task ID to delete: ");
            scanf("%d", &id);

            if(deleteTask(&unperformedTasks, id)) {
                printf("Task %d deleted.\n", id);
                printf("\n");
            } else
                printf("Task %d not found.\n", id);

            break;

        case 4: {
            int search;
            printf("Search by 1. Task ID or 2. Task Name: ");
            scanf("%d", &search);

            if(search == 1) {
                printf("Enter Task ID: ");
                scanf("%d", &id);
                searchById(&unperformedTasks, id);
            } else if(search == 2) {
                printf("Enter Task Name: ");
                fgetchar();
                fgets(name, sizeof(name), stdin);
                strcpy(name, strtok(name, "\n"));
                searchByName(&unperformedTasks, name);
            } else {
                printf("Invalid choice.\n");
            }

            break;
        }

        case 5:
            displayUnperformedTasks(&unperformedTasks);
            break;

        case 6:
            printf("Enter Task ID to perform: ");
            scanf("%d", &id);
            performTask(&unperformedTasks, &performedTasks, id);
            break;

        case 7:
            undoLastPerformedTask(&unperformedTasks, &performedTasks);
            break;

        case 8:
            printStack(&performedTasks);
            break;

        case 9:
            generateSummaryReport(&unperformedTasks, &performedTasks);
            break;

        case 10:
            destroy(&unperformedTasks, &performedTasks);


            printf("Exiting the program.\n");
            return 0;

        default:
            printf("Invalid choice. Please pick a valid choice from the options.\n");
        }
    }

    return 0;
}


void Menu() {
    printf("\n");
    printf("Please Enter the number of the wanted action from the following list: \n");
    printf("1. Load Tasks File\n");
    printf("2. Add a New Task\n");
    printf("3. Delete a Task\n");
    printf("4. Search for a Task\n");
    printf("5. View Unperformed Tasks\n");
    printf("6. Perform a Task\n");
    printf("7. Undo Last Performed Task\n");
    printf("8. View Performed Tasks\n");
    printf("9. Generate Summary Report\n");
    printf("10. Exit\n");
}

void loadTasks(TasksList *list, const char *tasks) {
    if(isLoaded) {
        printf("File already loaded!\n");
        return;
    }

    FILE *file = fopen(tasks, "r");

    if(file == NULL) {
        printf("Task list is empty or there's a problem in reading file!\n");

    }

    int id;
    float duration;
    char name[SIZE], date[DATE_LENGTH];

    int c = '1';
    int n = 0;
    float temp = 0;

    int length = 200;
    char buffer[length];
    char* token;

    while(fgets(buffer, length, file) != NULL) {
        if(buffer[0] == '\0' || buffer[0] == '\n')
            continue;

        token = strtok(buffer, "#");
        c = token[0];
        id = 0;
        n = 0;

        while(c != '\0') {
            id *= 10;
            id += c - '0';
            n++;
            c = token[n];
        }

        if(CheckDuplicateId(list, NULL, id)) {
            printf("Skipping duplicate with id: %d\n", id);
            continue;
        }

        token = strtok(NULL, "#");

        strcpy(name, token);

        token = strtok(NULL, "#");

        strcpy(date, token);

        token = strtok(NULL, "#");
        c = token[0];
        duration = 0;
        n = 0;

        while(c != '\0' && c != '.' && c != '\n') {
            duration *= 10.0;
            duration += c - '0';
            n++;
            c = token[n];
        }

        if(c == '.') {
            token = strtok(token, ".");
            token = strtok(NULL, ".");
            c = token[0];
            n = 0;
            temp = 1.0;

            while(c != '\0' && c != '\n') {
                temp *= 10.0;
                duration += ((c - '0') / temp);
                n++;
                c = token[n];
            }
        }

        addTask(list, id, name, date, duration);
    }

    fclose(file);
    isLoaded = 1;
}


void addTask(TasksList *list, int id, char *name, char *date, float duration) {
    Task *addedTask;
    addedTask = (Task *)malloc(sizeof(Task));

    if(addedTask == NULL) {
        printf("No more memory left to allocate for this task!\n");
        return;
    } else {
        addedTask->id = id;
        strcpy(addedTask->name, name);
        strcpy(addedTask->date, date);
        addedTask->duration = duration;
        addedTask->next = list->head;
        addedTask->IsUndone = 0;
        list->head = addedTask;
        printf("Task %d is now added.\n", id);
    }
}

void addUndoneTask(TasksList *list, Task* task) {
    if(task == NULL) {
        printf("No more memory left to allocate for this task!\n");
        return;
    } else {
        Task* undo = list->head;
        task->next = NULL;

        if(undo  == NULL)
            list->head = task;

        while(undo != NULL) {
            if(undo ->next == NULL) {
                undo ->next = task;
                return;
            }

            undo  = undo ->next;
        }
    }
}


int deleteTask(TasksList *list, int id) {

    Task *deletedTask = list->head;
    Task *prev = list->head;

    while(deletedTask != NULL) {
        if(deletedTask->id == id) {
            if(deletedTask == list->head) {
                list->head = deletedTask->next;
            } else {
                prev->next = deletedTask->next;
            }

            free(deletedTask);
            return 1;
        }

        prev = deletedTask;
        deletedTask = deletedTask->next;
    }

    return 0;
}


void searchById(TasksList *list, int id) {
    Task *searchedTask = list->head;

    while(searchedTask != NULL) {
        if(searchedTask->id == id) {
            printTask(searchedTask);
            return;
        }

        searchedTask = searchedTask->next;
    }

    printf("Task not found\n");
    printf("\n");
}
void searchByName(TasksList *list, char name[SIZE]) {
    Task *searchedTask = list->head;

    while(searchedTask != NULL) {
        if(strcmp(searchedTask->name, name) == 0) {
            printTask(searchedTask);
            return;
        }

        searchedTask = searchedTask->next;
    }

    printf("Task not found\n");
    printf("\n");
}



void performTask(TasksList *unperformedTasks, PerformedTasksStack *performedTasks, int id) {
    Task *current = unperformedTasks->head;

    while(current != NULL) {
        if(current->id == id) {
            printf("\nPerforming Task:\n");
            printTask(current);
            Task *copy;
            copy = (Task *)malloc(sizeof(Task));
            copy->id = current->id;
            copy->duration = current->duration;
            strcpy(copy->date, current->date);
            strcpy(copy->name, current->name);
            copy->next = NULL;

            deleteTask(unperformedTasks, id);

            push(performedTasks, copy);
            return;
        }

        current = current->next;
    }

    printf("Task with ID %d not found in unperformed tasks.\n", id);
}



void displayUnperformedTasks(TasksList *list) {
    Task *taskNode = list->head;
    int taskNumber = 1;

    if(taskNode == NULL) {
        printf("No unperformed tasks in the list.\n");
        return;
    } else {
        printf("\nUnperformed Tasks:\n");

        while(taskNode != NULL) {
            printTask(taskNode);

            taskNode = taskNode->next;
            taskNumber++;
        }
    }
}

void undoLastPerformedTask(TasksList *unperformedTasks, PerformedTasksStack *performedTasks) {

    if(performedTasks->top == NULL) {
        printf("No tasks got performed.\n");
    } else {
        Task *undo = pop(performedTasks);

        if(CheckDuplicateId(unperformedTasks, performedTasks, undo->id)) {
            printf("Task with ID %d already exists in unperformed list.\n", undo->id);

            return;
        }

        undo->IsUndone = 1;
        undo->next = NULL;
        addUndoneTask(unperformedTasks, undo);
        printf("Undo complete. Task with ID %d is now unperformed.\n", undo->id);
    }
}





void generateSummaryReport(TasksList *unperformedTasks, PerformedTasksStack *performedTasks) {
    FILE* file = fopen("Report.txt", "w");
    printStack(performedTasks);
    printStackToFile(performedTasks, file);
    printf("Undone Tasks:\n");
    fprintf(file, "Undone Tasks:\n");
    Task *task = unperformedTasks->head;

    while(task != NULL) {
        if(task->IsUndone == 1) {
            printTask(task);
            printTaskToFile(task, file);
        }

        if(task->next == NULL) {
            break;
        }

        task = task->next;
    }

    printf("\nUnperformed Tasks:\n");
    fprintf(file, "\nUnperformed Tasks:\n");
    task = unperformedTasks->head;

    while(task != NULL) {
        if(task->IsUndone == 0) {
            printTask(task);
            printTaskToFile(task, file);
        }

        if(task->next == NULL) {
            break;
        }

        task = task->next;
    }

    fclose(file);
}

void printTask(Task* task) {
    if(task != NULL)
        printf("Task details: ID: %d | Name: %s | Date: %s | Duration: %.1f hours\n",
               task->id, task->name, task->date, task->duration);
}

void push(PerformedTasksStack *stack, Task *task) {
    task->next = stack->top;
    stack->top = task;
}

Task* pop(PerformedTasksStack *stack) {
    if(stack->top == NULL) {
        printf("The stack is empty!\n");
        return NULL;
    } else {
        Task *topTask = stack->top;
        stack->top = topTask->next;
        topTask->next = NULL;
        return topTask;
    }
}


void statusCheck(Task* task) {
    while(task != NULL) {
        if(task->IsUndone == 1) {

            printTask(task);
        }

        task = task->next;
    }

}

int CheckDuplicateId(TasksList *unperformedTasks, PerformedTasksStack *performedTasks, int id) {
    Task *task = unperformedTasks->head;

    while(task != NULL) {
        if(task->id == id) return 1;

        task = task->next;
    }

    if(performedTasks != NULL) {
        task = performedTasks->top;

        while(task != NULL) {
            if(task->id == id) return 1;

            task = task->next;
        }
    }

    return 0;
}

void printStack(PerformedTasksStack *stack) {
    Task *task = stack->top;

    if(task == NULL) {
        printf("No performed tasks.\n");
        return;
    } else {
        printf("Performed Tasks:\n");

        while(task != NULL) {
            printTask(task);
            task = task->next;
        }

        printf("\n");
    }
}

void printStackToFile(PerformedTasksStack *stack, FILE* output) {
    Task *task = stack->top;

    if(task == NULL) {
        fprintf(output, "No performed tasks.\n\n");
        return;
    } else {
        fprintf(output, "Performed Tasks:\n");

        while(task != NULL) {
            printTaskToFile(task, output);
            task = task->next;
        }

        fprintf(output, "\n");
    }
}

void printTaskToFile(Task* task, FILE* output) {
    if(task != NULL)
        fprintf(output, "Task details: ID: %d | Name: %s | Date: %s | Duration: %.1f hours\n",
                task->id, task->name, task->date, task->duration);
}
void destroy(TasksList* taskList, PerformedTasksStack* stack) {
    Task *task = taskList->head;
    Task *nextTask;

    while(task != NULL) {
        nextTask = task->next;
        free(task);
        task = nextTask;
    }

    taskList->head = NULL;
    task = stack->top;

    while(task != NULL) {
        nextTask = task->next;
        free(task);
        task = nextTask;
    }

    stack->top = NULL;
}



