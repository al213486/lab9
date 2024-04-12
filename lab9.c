#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

// Define a structure for a node in the linked list
struct Node {
    struct RecordType data;
    struct Node *next;
};

// Fill out this structure
struct HashType
{
    struct Node **buckets; // Array of linked lists (buckets)
    int size; // Size of the hash table
};

// Compute the hash function
int hash(int x, int size)
{
    // Hash function to distribute data evenly in the array
    return x % size;
}

// Parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// Display records in the hash structure
// Skip the indices which are free
// The output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;

    printf("\nRecords in Hash Table:\n");
    for (i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        struct Node *current = pHashArray->buckets[i];
        while (current != NULL)
        {
            printf("%d, %c, %d -> ", current->data.id, current->data.name, current->data.order);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData(argv[1], &pRecords);
    printRecords(pRecords, recordSz);

    // Your hash implementation
    struct HashType hashTable;
    hashTable.size = recordSz * 2; // Adjust the size as needed

    // Allocate memory for array of linked lists (buckets)
    hashTable.buckets = (struct Node **)malloc(sizeof(struct Node *) * hashTable.size);
    if (hashTable.buckets == NULL) {
        printf("Memory allocation failed!\n");
        return 1; // Return error
    }

    // Initialize each bucket to NULL
    for (int i = 0; i < hashTable.size; ++i) {
        hashTable.buckets[i] = NULL;
    }

    // Insert records into hash table
    for (int i = 0; i < recordSz; ++i) {
        int index = hash(pRecords[i].id, hashTable.size);
        // Create a new node
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        if (newNode == NULL) {
            printf("Memory allocation failed!\n");
            return 1; // Return error
        }
        newNode->data = pRecords[i];
        newNode->next = NULL;

        // Insert at the beginning of the linked list (separate chaining)
        if (hashTable.buckets[index] == NULL) {
            hashTable.buckets[index] = newNode;
        } else {
            newNode->next = hashTable.buckets[index];
            hashTable.buckets[index] = newNode;
        }
    }

    // Display records in hash table
    displayRecordsInHash(&hashTable, hashTable.size);

    // Free memory
    for (int i = 0; i < hashTable.size; ++i) {
        struct Node *current = hashTable.buckets[i];
        while (current != NULL) {
            struct Node *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable.buckets);

    return 0;
}
