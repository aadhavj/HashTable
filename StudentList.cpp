#include <iostream>
#include <cstring>
#include <vector>
#include <iomanip>
using namespace std;

//Instantiate student structure
struct Student {
	char firstName[100];
	char lastName[100];
	int ID;
	float GPA;
};

int main(){

	//Basic var instantiation
	char command[100];
	char fName[100];
	char lName[100];
	int inputID;
	float GPA;
	bool runProgram = true;
	vector<Student*> studentList;

	//Valid Command prompts
	char del[] = "DELETE";
	char print[] = "PRINT";
	char add[] = "ADD";
	char quit[] = "QUIT";

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
			for (Student* element : studentList){
				if (element->ID == inputID){
					studentList.erase(studentList.begin()+i);
				}
				i++;
			}
		}
		else if (strcmp(command, print) == 0) {
			cout << "Print command initiated" << endl;
			
			//Loops through all objects and lists their attributes
			for (Student* element : studentList){
				cout << "\nFirst Name: " << element->firstName << endl;
				cout << "Last Name: " << element->lastName << endl;
				cout << "Student ID: " << element->ID << endl;
				cout << "Student GPA: " << fixed << setprecision(2) << element->GPA << endl;
				cout << endl;
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
			
			//Add to studentList array
			studentList.push_back(createdStudent);

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
