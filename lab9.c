/*Programmer: Joshua Jaggernauth
Date: 04/12/2024
Project: Lab Assignment 9 (Hash Functions)
*/
//The code implements hashing with chaining using separate chaining in C. It reads records from an input file, 
//hashes them based on their IDs, stores them in a hash table where collisions are handled by chaining, and then 
//displays the records stored in the hash table.

#include <stdio.h>

#include <stdlib.h>

//RecordType struct to represent individual records
struct RecordType {
  int id; //Unique identifier
  char name; //Name
  int order; //Order
};

//Node struct for linked list implementation
struct Node {
  struct RecordType data; //Data of the node
  struct Node * next; //Pointer to the next node
};

//Size of the hash table
#define HASH_SIZE 10

//Structure for each entry in the hash table
struct HashType {
  struct Node * head; //Head of the linked list for separate chaining
};

int hash(int x) {
  return x % HASH_SIZE; //Simple modulo operation to distribute keys evenly
}

//Parse input file to read records into memory
int parseData(char * inputFileName, struct RecordType ** ppData) {
  FILE * inFile = fopen(inputFileName, "r"); //Open input file for reading
  int dataSz = 0; //Number of records
  int i, n;
  char c;
  struct RecordType * pRecord;
  * ppData = NULL; //Initialize pointer to data as NULL

  if (inFile) {
    //Read number of records
    fscanf(inFile, "%d\n", & dataSz);
    //Allocate memory for records
    * ppData = (struct RecordType * ) malloc(sizeof(struct RecordType) * dataSz);
    //Check if memory allocation was successful
    if ( * ppData == NULL) {
      printf("Cannot allocate memory\n");
      exit(-1);
    }
    //Loop through each record in the file
    for (i = 0; i < dataSz; ++i) {
      pRecord = * ppData + i;
      //Read ID, name, and order from file
      fscanf(inFile, "%d ", & n);
      pRecord -> id = n;
      fscanf(inFile, "%c ", & c);
      pRecord -> name = c;
      fscanf(inFile, "%d ", & n);
      pRecord -> order = n;
    }

    fclose(inFile);
  }

  return dataSz; //Return the number of records read
}

//Print records
void printRecords(struct RecordType pData[], int dataSz) {
  int i;
  printf("\nRecords:\n");
  //Loop through each record and print its ID, name, and order
  for (i = 0; i < dataSz; ++i) {
    printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
  }
  printf("\n\n");
}

//Display records stored in the hash table
void displayRecordsInHash(struct HashType hashArray[], int hashSz) {
  int i;
  struct Node * temp;

  //Loop through each entry in the hash table
  for (i = 0; i < hashSz; ++i) {
    printf("Index %d -> ", i); //Print index
    temp = hashArray[i].head; //Get the head of the linked list at this index
    //Loop through the linked list and print each record
    while (temp != NULL) {
      printf("%d %c %d -> ", temp -> data.id, temp -> data.name, temp -> data.order);
      temp = temp -> next; //Move to the next node
    }
    printf("\n");
  }
}

int main(void) {
  struct RecordType * pRecords; //Pointer to store records
  int recordSz = 0; //Number of records

  //Initialize hash table
  struct HashType hashArray[HASH_SIZE];
  for (int i = 0; i < HASH_SIZE; ++i) {
    hashArray[i].head = NULL; //Initialize each head of the linked list to NULL
  }

  //Parse data from input file
  recordSz = parseData("input.txt", & pRecords);
  //Print the parsed records
  printRecords(pRecords, recordSz);

  //Hash records and insert into the hash table
  for (int i = 0; i < recordSz; ++i) {
    int index = hash(pRecords[i].id); //Get the hash index for the record
    struct Node * newNode = (struct Node * ) malloc(sizeof(struct Node)); //Allocate memory for new node
    newNode -> data = pRecords[i]; //Assign record data to the node
    newNode -> next = NULL; //Initialize next pointer to NULL

    //If the head of the linked list at this index is NULL, assign the new node as the head
    if (hashArray[index].head == NULL) {
      hashArray[index].head = newNode;
    } else {
      //Otherwise, traverse to the end of the linked list and append the new node
      struct Node * temp = hashArray[index].head;
      while (temp -> next != NULL) {
        temp = temp -> next;
      }
      temp -> next = newNode;
    }
  }

  //Display records stored in the hash table
  displayRecordsInHash(hashArray, HASH_SIZE);

  //Free allocated memory
  for (int i = 0; i < HASH_SIZE; ++i) {
    struct Node * temp = hashArray[i].head;
    while (temp != NULL) {
      struct Node * next = temp -> next;
      free(temp); //Free memory allocated for each node
      temp = next; //Move to the next node
    }
  }
  free(pRecords); //Free memory allocated for records

  return 0;
}