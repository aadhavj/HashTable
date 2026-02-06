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
int hashMe(Student* hashable, int maxLen){
	return ( (int) ( (float) hashable->ID * hashable->GPA) % maxLen);
}
void nullify(Student** hashTable, int hashLen){
	for (int i = 0; i < hashLen; i++){
		hashTable[i] = nullptr;
	}
}
Student* getRandomStudent(char** firstNames, char** lastNames, int inputID){
	Student* rando = new Student();
	strcpy(rando->firstName,firstNames[rand() % 1000]);
	strcpy(rando->lastName,lastNames[rand() % 1000]);
	rando->ID = inputID;
	rando->GPA = (((float) (rand()%4001)) / 100.0);
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
	Student* hashTable[100];
	char* firstNames[1000];
	char* lastNames[1000];
	int hashLen = 100;
	int latestID = 100000;
	Student* allStudents[10000];
	srand(static_cast<unsigned int>(time(nullptr)));
	bool doubleHash = false;
	
	//Valid Command prompts
	char del[] = "DELETE";
	char print[] = "PRINT";
	char add[] = "ADD";
	char addRandom[] = "ADDR";
	char quit[] = "QUIT";
	
	//hashTable nullification
	nullify(hashTable, hashLen);

	//allStudents nullification
	nullify(allStudents,10000);

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
			
			//Gets student ID to delete
			cout << "Enter Student ID (Delete): ";
			cin >> inputID;
			
			//Loops through and deletes student with that ID
			int i = 0;
			for (Student* element : hashTable){
				if (element->ID == inputID){
					delete hashTable[i];
					hashTable[i] = nullptr;
				}
				i++;
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
			
			//Assgin to object
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
					cout << "Error: Not adding student because linked has reached max of 3 student objects." << endl;
					doubleHash = true;
				}
				else{
					tempLinked->next = createdStudent;
				}
			}
			else{
				hashTable[hashMe(createdStudent, hashLen)] = createdStudent;
			}
			//Add to quene
			allStudents[createdStudent->ID%10000] = createdStudent;

		}
		else if (strcmp(command, addRandom) == 0){
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
					if (numberOfLinkedStudents == 3){ //Collision Detected
						doubleHash = true;
						cout << "Error: Not adding student because linked has reached max of 3 student objects." << endl;
					}
					else{
						tempLinked->next = createdStudent;
					}
				}
				else{
					hashTable[hashMe(createdStudent, hashLen)] = createdStudent;
				}

			}
			//Add to quene
			allStudents[createdStudent->ID%10000] = createdStudent;
	

		}
		else if (strcmp(command, quit) == 0){

			cout << "Quit command initiated. Program off." << endl;
			//End Program functionality
			runProgram = false;
		}
		else {
			//Invalid comands, print message
			cout << "Command prompt unrecognized." << endl;
		}

		while (doubleHash){
			doubleHash = false;
			hashLen = hashLen*2;
			Student* tempHash[hashLen];
			nullify(tempHash, hashLen);
			for (int i = 0; i < 10000;i++){
				if (allStudents[i] != nullptr){
					Student* addMe = allStudents[i];
					
					//Add to hashTable array
					if (tempHash[hashMe(addMe, hashLen)] != nullptr){
						Student* tempLinked = tempHash[hashMe(addMe, hashLen)];
						int numberOfLinkedStudents = 1;
						while (tempLinked->next != nullptr){
							tempLinked = tempLinked->next;
							numberOfLinkedStudents++;
						}
						if (numberOfLinkedStudents == 3){ //Collision Detected
							doubleHash = true;
							cout << "Error: Not adding student because linked has reached max of 3 student objects." << endl;
						}
						else{
							tempLinked->next = addMe;
						}
					}
					else{
						tempHash[hashMe(addMe, hashLen)] = addMe;
					}

				}	
			}
			if (not doubleHash){
				hashTable = tempHash;
			}

		}
	//Farewell program end text
	//cout << "Thank you for interacting with our student database. Please do so again!";	
	return 0;
}
