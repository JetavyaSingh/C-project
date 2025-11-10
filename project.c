#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_TABLES 5
#define NAME_LEN 50

typedef struct {
    int id;
    int isOccupied;
    char customerName[NAME_LEN];
} Table;

typedef struct Node {
    char name[NAME_LEN];
    struct Node* next;
} Node;

Node *front = NULL, *rear = NULL;
Table tables[MAX_TABLES];

void setColor(int color);
void welcomeScreen();
void displayMenu();
void addCustomer();
void releaseTable();
void displayStatus();
void searchCustomer();
void enqueue(char name[]);
void dequeue(char name[]);
int isQueueEmpty();
int tableAvailable();
void assignTable(char name[]);
void markTableFree(int id);
void displayQueue();
void clearScreen();
void pause();

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void clearScreen() {
    system("cls");
}

void welcomeScreen() {
    setColor(11);
    printf("\n\n\n\t\t=============================================\n");
    setColor(14);
    printf("\t\t   WELCOME TO RESTAURANT MANAGEMENT SYSTEM\n");
    setColor(11);
    printf("\t\t=============================================\n");
    setColor(10);
    printf("\n\t\tDeveloped in C Language\n");
    printf("\t\tManages Tables and Customer Queue Smartly\n\n");
    setColor(15);
    Sleep(1500);
    clearScreen();
}

void pause() {
    setColor(8);
    printf("\nPress any key to continue...");
    getchar();
    getchar();
}

void initializeTables() {
    for (int i = 0; i < MAX_TABLES; i++) {
        tables[i].id = i + 1;
        tables[i].isOccupied = 0;
        strcpy(tables[i].customerName, "None");
    }
}

void enqueue(char name[]) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->name, name);
    newNode->next = NULL;
    if (rear == NULL)
        front = rear = newNode;
    else {
        rear->next = newNode;
        rear = newNode;
    }
}

void dequeue(char name[]) {
    if (isQueueEmpty())
        return;
    Node* temp = front;
    strcpy(name, temp->name);
    front = front->next;
    if (front == NULL)
        rear = NULL;
    free(temp);
}

int isQueueEmpty() {
    return (front == NULL);
}

int tableAvailable() {
    for (int i = 0; i < MAX_TABLES; i++)
        if (!tables[i].isOccupied)
            return 1;
    return 0;
}

void assignTable(char name[]) {
    for (int i = 0; i < MAX_TABLES; i++) {
        if (!tables[i].isOccupied) {
            tables[i].isOccupied = 1;
            strcpy(tables[i].customerName, name);
            setColor(10);
            printf("\n✅ Table %d assigned to %s successfully!\n", tables[i].id, name);
            return;
        }
    }
}

void markTableFree(int id) {
    if (id < 1 || id > MAX_TABLES || !tables[id - 1].isOccupied) {
        setColor(12);
        printf("\n❌ Invalid or already free table ID!\n");
        return;
    }
    setColor(14);
    printf("\n🍽️ Table %d is now free! (Previous Customer: %s)\n", id, tables[id - 1].customerName);
    tables[id - 1].isOccupied = 0;
    strcpy(tables[id - 1].customerName, "None");
    if (!isQueueEmpty()) {
        char nextName[NAME_LEN];
        dequeue(nextName);
        assignTable(nextName);
    }
}

void addCustomer() {
    char name[NAME_LEN];
    setColor(14);
    printf("\nEnter Customer Name: ");
    fflush(stdin);
    gets(name);
    if (tableAvailable()) {
        assignTable(name);
    } else {
        enqueue(name);
        setColor(13);
        printf("\n⏳ All tables full! %s added to waiting queue.\n", name);
    }
}

void displayQueue() {
    Node* temp = front;
    if (temp == NULL) {
        printf("No customers waiting.\n");
        return;
    }
    int pos = 1;
    while (temp != NULL) {
        printf("%d. %s\n", pos++, temp->name);
        temp = temp->next;
    }
}

void displayStatus() {
    setColor(11);
    printf("\n================ TABLE STATUS ================\n");
    for (int i = 0; i < MAX_TABLES; i++) {
        if (tables[i].isOccupied) {
            setColor(10);
            printf("Table %d → Occupied by %s\n", tables[i].id, tables[i].customerName);
        } else {
            setColor(8);
            printf("Table %d → Available\n", tables[i].id);
        }
    }
    setColor(11);
    printf("\n================ WAITING QUEUE ================\n");
    setColor(15);
    displayQueue();
}

void searchCustomer() {
    char name[NAME_LEN];
    setColor(14);
    printf("\nEnter Customer Name to Search: ");
    fflush(stdin);
    gets(name);
    for (int i = 0; i < MAX_TABLES; i++) {
        if (tables[i].isOccupied && strcmp(tables[i].customerName, name) == 0) {
            setColor(10);
            printf("\n✅ %s is seated at Table %d.\n", name, tables[i].id);
            return;
        }
    }
    Node* temp = front;
    int pos = 1;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            setColor(13);
            printf("\n⏳ %s is waiting in queue (Position: %d)\n", name, pos);
            return;
        }
        temp = temp->next;
        pos++;
    }
    setColor(12);
    printf("\n❌ Customer not found!\n");
}

void releaseTable() {
    int id;
    setColor(14);
    printf("\nEnter Table ID to release: ");
    scanf("%d", &id);
    markTableFree(id);
}

void displayMenu() {
    int choice;
    do {
        setColor(11);
        printf("\n\n================ MAIN MENU ================\n");
        setColor(14);
        printf("1. Add Customer\n");
        printf("2. Release Table\n");
        printf("3. Display Status\n");
        printf("4. Search Customer\n");
        printf("5. Exit\n");
        setColor(11);
        printf("===========================================\n");
        setColor(15);
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearScreen();
        switch (choice) {
            case 1: addCustomer(); break;
            case 2: releaseTable(); break;
            case 3: displayStatus(); break;
            case 4: searchCustomer(); break;
            case 5: 
                setColor(10);
                printf("\nThank you for using the Restaurant Management System!\n");
                Sleep(1000);
                break;
            default:
                setColor(12);
                printf("\nInvalid Choice! Try again.\n");
        }
        pause();
        clearScreen();
    } while (choice != 5);
}

int main() {
    system("color 0F");
    welcomeScreen();
    initializeTables();
    displayMenu();
    return 0;
}
