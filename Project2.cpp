/*Final Project
* Name: Parker Burkard
* Date: 10/19/2025
* Class:  CS 300
*/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

//Create a structure for the courses
struct Course {
	string courseId;
	string courseName;
	vector<string> Prereqs;
};

class BinarySearchTree {
//Private constructors
private:
	//Structure for the tree
	struct Node {
		Course course;
		Node* right;
		Node* left;
		//defaults
		Node() {
			left = nullptr;
			right = nullptr;
		}
		//Initialize the course
		Node(Course aCourse) {
			course = aCourse;
			left = nullptr;
			right = nullptr;
		}
	};

	Node* root;
	void inOrder(Node* node);
	int size = 0;
//Public constructors
public:
	BinarySearchTree();
	void InOrder();
	void Insert(Course aCourse);
	Course Search(string courseId);
	int Size();
};

BinarySearchTree::BinarySearchTree() {
	//Establish the root as null to start
	this->root = nullptr;
}
//Organize the tree to be in order
void BinarySearchTree::InOrder() {
	inOrder(root);
}

void BinarySearchTree::Insert(Course aCourse) {
	Node* currentNode = root;

	if (root == NULL) {
		root = new Node(aCourse);
	}
	else {
		while (currentNode != NULL) {
			if (aCourse.courseId < currentNode->course.courseId) {
				if (currentNode->left == nullptr) {
					currentNode->left = new Node(aCourse);
					currentNode = NULL;
				}
				else {
					currentNode = currentNode->left;
				}
			}
			else {
				if (currentNode->right == nullptr) {
					currentNode->right = new Node(aCourse);
					currentNode = NULL;
				}
				else {
					currentNode = currentNode->right;
				}
			}
		}
	}
	size++;
}


//Searching for a course
Course BinarySearchTree::Search(string courseId) {
	Course aCourse;
	Node* currentNode = root;
	while (currentNode != NULL) {
		if (currentNode->course.courseId == courseId) {
			return currentNode->course;
		}
		else if (courseId < currentNode->course.courseId) {
			currentNode = currentNode->left;
		}
		else {
			currentNode = currentNode->right;
		}
	}
	return aCourse;
}

void BinarySearchTree::inOrder(Node* node) {
	if (node == NULL) {
		return;
	}
	inOrder(node->left);
	//printing the node
	cout << node->course.courseId << ", " << node->course.courseName << endl;
	inOrder(node->right);
}

//get the size of the tree
int BinarySearchTree::Size() {
	return size;
}

vector<string> Split(string lineFeed) {

	char delim = ',';

	lineFeed += delim; //includes a delimiter at the end so last word is also read
	vector<string> lineTokens;
	string temp = "";
	for (int i = 0; i < lineFeed.length(); i++)
	{
		if (lineFeed[i] == delim)
		{
			lineTokens.push_back(temp); //store words in token vector
			temp = "";
			i++;
		}
		temp += lineFeed[i];
	}
	return lineTokens;
}

//load courses
void loadCourses(string csvPath, BinarySearchTree* courseList) {
	ifstream inFS;
	string line;
	vector<string> stringTokens;

	inFS.open(csvPath);
	if (!inFS.is_open()) {
		cout << "File could not open. Please try again. " << endl;
		return;
	}
	while (!inFS.eof()) {
		Course aCourse;
		getline(inFS, line);
		stringTokens = Split(line);

		if (stringTokens.size() < 2) {
			cout << "Error. Skipping Line." << endl;
		}
		else {
			aCourse.courseId = stringTokens.at(0);
			aCourse.courseName = stringTokens.at(1);

			for (unsigned int i = 2; i < stringTokens.size(); i++) {
				aCourse.Prereqs.push_back(stringTokens.at(i));
			}
			courseList->Insert(aCourse);
		}
	}
	inFS.close();
}

//display the courses
void displayCourse(Course aCourse) {
	cout << aCourse.courseId << ", " << aCourse.courseName << endl;
	cout << "Prerequisites: ";

	if (aCourse.Prereqs.empty()) {
		cout << "none" << endl;
	}
	else {
		for (unsigned int i = 0; i < aCourse.Prereqs.size(); i++) {
			cout << aCourse.Prereqs.at(i);
			//Add in a comma for more than 1 prerequisite
			if (aCourse.Prereqs.size() > 1 && i < aCourse.Prereqs.size() - 1) {
				cout << ", ";
			}
		}
	}
	cout << endl;
}

void convertCase(string& toConvert) {
	for (unsigned int i = 0; i < toConvert.length(); i++) {
		if (isalpha(toConvert[i])) {
			toConvert[i] = toupper(toConvert[i]);
		}
	}
}

int main(int argc, char* argv[]) {
	string csvPath, aCourseKey;

	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		aCourseKey = argv[2];
		break;
	default:
		csvPath = "ABCU_Advising_Program_Input.csv";
	}

	BinarySearchTree* courseList = new BinarySearchTree();
	Course course;
	bool goodInput;
	int choice = 0;

	while (choice != 9) {
		cout << "Welcome to the course planner!" << endl;
		cout << "1: Load All Courses" << endl;
		cout << "2: Display All Courses" << endl;
		cout << "3: Find A Course" << endl;
		cout << "9: Exit Program" << endl;
		cout << "Please Enter A Choice: ";

		aCourseKey = "";
		string anyKey = " ";
		choice = 0;

		try {
			cin >> choice;

			if ((choice > 0 && choice < 5) || (choice == 9)) {
				goodInput = true;
			}
			else {
				goodInput = false;
				throw 1;
			}
			switch (choice) {
			case 1:
				loadCourses(csvPath, courseList);
				cout << courseList->Size() << " courses read" << endl;
				break;

			case 2:
				courseList->InOrder();
				break;

			case 3:
				cout << "\nWhat course would you like to know more about? " << endl;
				cin >> aCourseKey;
				convertCase(aCourseKey);
				course = courseList->Search(aCourseKey);
				if (!course.courseId.empty()) {
					displayCourse(course);
				}
				else {
					cout << "\nCourse ID " << aCourseKey << "Could not be found" << endl;
				}
				break;

			case 9:
				exit;
				break;

			default:
				throw 2;
			}
		}
		catch (int err) {
			std::cout << "\nPlease check your input!" << endl;
		}
		cin.clear();
		cin.ignore();

		system("cls");
	}
	cout << "Thank you for accessing my program. Good bye." << endl;
	return 0;
}