#include <iostream>
#include <cstring>
#include <vector>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

//Instantiate student structure
struct Student {
	char firstName[100];
	char lastName[100];
	int ID;
	float GPA;
	Student* next;
};
//hash function
int hashMe(Student* hashable, int maxLen){
	return ( (int) ( (float) hashable->ID * hashable->GPA) % maxLen);
}
//Sets all spots in array to nullptr
void nullify(Student** hashTable, int hashLen){
	for (int i = 0; i < hashLen; i++){
		hashTable[i] = nullptr;
	}
}
//Gets number of student objects in array
int getLength(Student** hashTable, int hashLen){
	int numberOfStudents = 0;
	for (int i = 0; i < hashLen; i++){
		Student* current = hashTable[i];
		while (current != nullptr){
			numberOfStudents++;
			current = current->next;
		}
	}
	return numberOfStudents;
}
//Makes a random student
Student* getRandomStudent(char** firstNames, char** lastNames, int inputID){
	Student* rando = new Student();
	strcpy(rando->firstName,firstNames[rand() % 1000]);
	strcpy(rando->lastName,lastNames[rand() % 1000]);
	rando->ID = inputID;
	rando->GPA = (((float) (rand()%4001)) / 1000.0);
	rando->next = nullptr;
	return rando;
}
int main(){

	//Basic var instantiation
	char command[100];
	char fName[100];
	char lName[100];
	int inputID;
	float GPA;
	bool runProgram = true;
	int hashLen = 100;
	int tempHashLen = 100;
	Student** hashTable = new Student*[hashLen];
	char* firstNames[1000];
	char* lastNames[1000];
	int latestID = 100000;
	srand(static_cast<unsigned int>(time(nullptr)));

	bool doubleHash = false;
	
	//Valid Command prompts
	char del[] = "DELETE";
	char print[] = "PRINT";
	char add[] = "ADD";
	char addRandom[] = "ADDR";
	char quit[] = "QUIT";
	char num[] = "NUM";
	
	//hashTable nullification
	nullify(hashTable, hashLen);


	//Name instantiation
	ifstream firstNameFile("firstNames.txt");
	for (int i = 0; i < 1000; i++){
		firstNames[i] = new char[1000];
		firstNameFile >> firstNames[i];
	}
	firstNameFile.close();
	
	ifstream lastNameFile("lastNames.txt");
	for (int i = 0; i < 1000; i++){
		lastNames[i] = new char[1000];
		lastNameFile >> lastNames[i];
		lastNames[i] = lastNames[i];
	}
	lastNameFile.close();



	while (runProgram){
		//Command parsing and prompting
		cin >> command;
		command[6] = '\0';

		//Command recognition
		if (strcmp(command, del) == 0){
			cout << "Delete command initiated" << endl;
			
			bool objectDeleted = false;

			//Gets student ID to delete
			cout << "Enter Student ID (Delete): ";
			cin >> inputID;
			
			//search through hashTable and deletes and relinks objects
			for (int i = 0; i < hashLen;i++){
				Student* current = hashTable[i];
				Student* prev = current;
				if (current != nullptr){
					while (current != nullptr){
						if (current->ID == inputID){
							if (prev != current){
								prev->next = current->next;
							}
							else{
								if (current->next == nullptr){
									hashTable[i] = nullptr;
								}
								else{
									hashTable[i] = current->next;
								}
							}
							delete current;
							objectDeleted = true;
							break;
						}
						prev = current;
						current = current->next;
					}
				}
			}
			
			//If student ID not in list, let user know
			if (not objectDeleted){
				cout << "Invalid student ID. An object with that ID does not currently exist." << endl;
			}
		}
		else if (strcmp(command, print) == 0) {
			cout << "Print command initiated" << endl;
			
			//Loops through all objects and lists their attributes
			for (int i = 0; i < hashLen;i++){
				if (hashTable[i] != nullptr){
					Student* tempLinked = hashTable[i];
					while (tempLinked != nullptr){
						cout << hashMe(tempLinked, hashLen) << ": " << tempLinked->firstName << " "  << tempLinked->lastName << " " << tempLinked->ID << " " << fixed << setprecision(2) << tempLinked->GPA << " -> ";
						tempLinked = tempLinked->next;
					}
					cout << endl;
				}
			}
		}
		else if (strcmp(command, add) == 0){

			cout << "Add command initiated" << endl;

			//Instantiate new student object
			Student *createdStudent = new Student();
			
			//Get input data
			cout << "First name: ";
			cin >> fName;
			cout << "\nLast name: ";
			cin >> lName;
			cout << "\nStudent ID: ";
			cin >> inputID;
			cout << "\nStudent GPA: ";
			cin >> GPA;
			
			//Assign to object
			strcpy(createdStudent->firstName,fName);
			strcpy(createdStudent->lastName,lName);
			createdStudent->ID = inputID;
			createdStudent->GPA = GPA;
			createdStudent->next = nullptr;

			//Add to hashTable array
			if (hashTable[hashMe(createdStudent, hashLen)] != nullptr){
				Student* tempLinked = hashTable[hashMe(createdStudent, hashLen)];
				int numberOfLinkedStudents = 1;
				while (tempLinked->next != nullptr){
					tempLinked = tempLinked->next;
					numberOfLinkedStudents++;
				}
				if (numberOfLinkedStudents == 3){ //Collision Detected
					doubleHash = true;
				}
				tempLinked->next = createdStudent;
			}
			else{
				hashTable[hashMe(createdStudent, hashLen)] = createdStudent;
			}

		}
		else if (strcmp(command, addRandom) == 0){
			//Adds a random student
			cout << "Adding random student initiated" << endl;

			int numRepitition;
			cout << "\nHow many random students do you want to add?: " << endl;
			cin >> numRepitition;
			
			for (int i = 0; i < numRepitition;i++){
				//Instantiate random student
				Student *createdStudent = getRandomStudent(firstNames,lastNames,latestID);
				latestID++;
				
				//Add to hashTable array
				if (hashTable[hashMe(createdStudent, hashLen)] != nullptr){
					Student* tempLinked = hashTable[hashMe(createdStudent, hashLen)];
					int numberOfLinkedStudents = 1;
					while (tempLinked->next != nullptr){
						tempLinked = tempLinked->next;
						numberOfLinkedStudents++;
					}
					if (numberOfLinkedStudents >= 3){ //Collision Detected
						doubleHash = true;
					}
					
					tempLinked->next = createdStudent;
					
				}
				else{
					hashTable[hashMe(createdStudent, hashLen)] = createdStudent;
				}
			}	

		}
		else if (strcmp(command, num) == 0){
			//Get the number of objects in hashTable
			cout << "There are " << getLength(hashTable, hashLen) << " student objects. "<< endl;
		}
		else if (strcmp(command, quit) == 0){

			//End Program functionality
			runProgram = false;
		}
		else {
			//Invalid comands, print message
			cout << "Command prompt unrecognized." << endl;
		}
		while (doubleHash){
			
			//set doublehash false and instantiate tempHash
			doubleHash = false;
			tempHashLen = tempHashLen*2;
			Student** tempHash = new Student*[tempHashLen];
			nullify(tempHash, tempHashLen);
			Student* current;

			//Loops through hashTable and reorganizes into bigger temp hash table
			for (int i = 0; i < hashLen; i++){
				current = hashTable[i];
				while (current != nullptr){
		
					//Add to tempHashTable array
					if (tempHash[hashMe(current, tempHashLen)] != nullptr){
						int lengthOfLinks = 1;
						Student* tempLinked = tempHash[hashMe(current, tempHashLen)];
						while (tempLinked->next != nullptr){
							tempLinked = tempLinked->next;
							lengthOfLinks++;
						}
						if (lengthOfLinks >= 3){
							doubleHash = true;
						}
						tempLinked->next = current;
					}
					else{
						tempHash[hashMe(current, tempHashLen)] = current;
					}
					Student* temp = current;
					current = current->next;
					temp->next = nullptr;
				}
			}
			hashLen = tempHashLen;
			hashTable = tempHash;
			
		}
	}
	//Farewell program end text
	cout << "Thank you for interacting with our student database. Please do so again!";	
	return 0;
}
