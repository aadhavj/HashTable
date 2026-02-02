#include <iostream>
#include <cstring>
#include <vector>
#include <iomanip>
#include <fstream>
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
Student* getRandomStudent(){
	Student* rando = new Student();
	//rando->firstName = 
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

	//Valid Command prompts
	char del[] = "DELETE";
	char print[] = "PRINT";
	char add[] = "ADD";
	char quit[] = "QUIT";
	
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
	
	for (int i = 0; i < 1000; i++){
		cout << firstNames[i] << " " << lastNames[i] << endl;
	}



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
				}
				else{
					tempLinked->next = createdStudent;
				}
			}
			else{
				hashTable[hashMe(createdStudent, hashLen)] = createdStudent;
			}

			cout << "Student HASH: " << hashMe(createdStudent, hashLen) << endl;

			cout << "Student Created!" << endl;
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
	}

	//Farewell program end text
	cout << "Thank you for interacting with our student database. Please do so again!";
	

	return 0;
}
