//Libraries required for all functionalities included.
#include<iostream>
#include<string>
#include <fstream>
#include <vector>
#include<cstdio>
#include <iomanip>
using namespace std;

//prototypes for the functions used.
void printalldetails();
void printloc();
void printpositive();

//structure to create symptoms file and handle symptoms.
struct sympt {
    const static int symptonSize = 30;
    char level;
    char sym[symptonSize];
};
int temp_id2;
int IDcheck(int);

//class created to efficiently store and handle data within the text file.
class Patient {
public:

    const static int firstNameSize = 30;
    const static int lastNameSize = 30;
    const static int visitedLocationSize = 30;
    const static int addressSize = 30;
    const static int symptonSize = 15;

    //Enteries of the class.
    int patientId;
    char firstName[firstNameSize];
    char lastName[lastNameSize];
    int age;
    int dob;
    char visitedLocation[visitedLocationSize];
    time_t timeNow = time(0);
    char Address[addressSize];
    bool overseasTravel;
    bool  covidTest;
    bool isAlive;
    char symptom[symptonSize];

    //Functions of the class.
    void write();
    void testStatusUpdate();
    void updatelocation(string);
    void updateDetails();
    void recommendation(string, string);
};


int main() {
    cout << "---------------COVID RECOMMENDATION SYSTEM---------------\n" << endl;
    Patient obj;
    int choice = 0;
    //Displaying all the options and asking user for the function they want to perform.
    while (choice != 7) {
        cout << "Please select one of the following functions :- \n";
        cout << "1 - Enter your detail for COVID - Test Recommendation." << endl;
        cout << "2 - Submit Your Covid test status & Update the Location database." << endl;
        cout << "3 - Display the Updated location (High Risk for COVID)" << endl;
        cout << "4 - Update the COVID Patient Details." << endl;
        cout << "5 - Display the COVID Positive Patient Details." << endl;
        cout << "6 - Display all records." << endl;
        cout << "7 - Quit." << endl << endl;
        cout << "Enter choice:- ";
        cin >> choice;

        //Validation check on the choice entered.
        while ((choice < 1) || (choice > 7)) {
            cout << "Invalid selection, please enter again. \n";
            cin >> choice;
        }

        //Selecting the operations to perform and calling of the functions based on user's input.
        switch (choice) {
        case 1:
            obj.write();
            break;
        case 2:
            obj.testStatusUpdate();

            break;
        case 3:
            printloc();
            break;
        case 4:
            obj.updateDetails();
            break;
        case 5:
            printpositive();
            break;
        case 7:
            cout << "Good bye, have a greeat day !!!!" << endl;
            break;
        case 6:
            printalldetails();
        }
        cout << "\n \n";
    }
    return 0;
}

//Write function to take all the required inputs and store it in the file.
void Patient::write() {
    Patient obj;
    ofstream file_obj;
    int temp_id;
    file_obj.open("Input.txt", ios::app);
    int repeat = 1;
    cout << "Please enter patient id: - ";
    cin >> temp_id;
    repeat = IDcheck(temp_id);   //Sending the ID for duplicate check.

    while (repeat == 1) {
        cout << "ID already exists, please enter a new patient ID: - ";
        cin >> temp_id2;
        repeat = IDcheck(temp_id2);
        temp_id = temp_id2;
    }
    obj.patientId = temp_id;
    cout << "Please enter patient's first name: - ";
    cin >> obj.firstName;
    cout << "Please enter patient's last name: - ";
    cin >> obj.lastName;
    cout << "Please enter your age: - ";
    cin >> obj.age;
    cout << "Please enter patient's DOB (DDMMYYYY): - ";
    cin >> obj.dob;
    cout << "Please enter patient's Address : - ";
    cin >> obj.Address;
    cout << "Enter the symptom (Fever, Headache, Tiredness, Aches, Breathlessness, ChestPain, Diarrhoea) :- ";
    cin >> obj.symptom;
    cout << "Please enter visited Locations: - ";
    cin >> obj.visitedLocation;
    cout << "Please enter if patient travelled overseas(1: yes or 0: no): - ";
    cin >> obj.overseasTravel;
    cout << "Please enter Covid Test Status: (1: Positive or 0: Negative) - ";
    cin >> obj.covidTest;
    string aliveOrDead;
    cout << "Please enter if patient is alive or dead (1: Alive or 0: Dead) : - ";
    cin >> aliveOrDead;
    obj.isAlive = aliveOrDead == "alive";
    file_obj.write((char*)&obj, sizeof(obj));
    file_obj.close();
    Patient anyname;
    anyname.recommendation(obj.symptom, obj.visitedLocation); //calling the recommendation function to recommend COVID testing based on the inputs.
}

//Function to update the test result status of the patient using ID to search for record.
void Patient::testStatusUpdate() {
    cout << "Please enter your patient ID :-  ";
    int id;
    cin >> id;
    cout << "Please enter your Covid test Status (positive/negative) :-  ";
    string status;
    cin >> status;
    fstream file_obj;
    fstream temp_file;
    file_obj.open("Input.txt", ios::in);
    temp_file.open("temp.txt", ios::app);

    /*Data in files can not be changed so to update the record, the data is copied into another temporary file where the updated
    data is added and then the original file is deleted and made again to copy back the data from temporary file.*/
    Patient temp;
    file_obj.read((char*)&temp, sizeof(temp));
    while (!file_obj.eof()) {
        if (temp.patientId == id) {
            temp.covidTest = status == "positive";
            if (temp.covidTest) {
                updatelocation(temp.visitedLocation);
            };
        };
        temp_file.write((char*)&temp, sizeof(temp));
        file_obj.read((char*)&temp, sizeof(temp));
    }
    file_obj.close();
    temp_file.close();
    remove("Input.txt");
    fstream file_obj2;
    Patient temp_obj;
    file_obj2.open("Input.txt", ios::app);
    temp_file.open("temp.txt", ios::in);
    temp_file.read((char*)&temp_obj, sizeof(temp_obj));

    while (!temp_file.eof()) {
        file_obj2.write((char*)&temp_obj, sizeof(temp_obj));
        temp_file.read((char*)&temp_obj, sizeof(temp_obj));
    }
    file_obj2.close();
    temp_file.close();
    remove("temp.txt");
}

//Function to update the location database based on the covid status of user and visited location.
void Patient::updatelocation(string loc) {

    fstream loc_file;
    loc_file.open("location.txt", ios::in);
    int flag = 0;
    string temp_loc;
    while (getline(loc_file, temp_loc)) { //Reading till the end of file.
        if (loc == temp_loc) {
            cout << "Visited location already exists in the high risk location database." << endl;
            flag = 1;
            break;
        }
    }
    if (flag == 0) {
        loc_file.close();
        loc_file.open("location.txt", ios::app);
        loc_file << loc << endl;
        cout << "location database updated. " << endl;
        loc_file.close();
    }

}

//Funtion to update the personal details of the user.
void Patient::updateDetails() {
    cout << "Enter the Patient ID :-";
    int temp_id;
    cin >> temp_id;
    cout << "Enter the choice you want to update : -" << endl << "1. First Name" << endl << "2. Last Name" << endl << "3. Age" << endl << "4. DOB" << endl << "5. Address" << endl << "6. visited location " << endl << "7. overseas travel(yes/no)" << endl << "8. Status(alive/dead)" << endl;
    int choice;
    cin >> choice;
    cout << "Enter the updated deatil : - ";

    fstream file_obj;
    fstream temp_file;
    file_obj.open("Input.txt", ios::in);
    temp_file.open("temp.txt", ios::app);


    //Giving user options to enter the entity they want to change and then changing them by creating a temporary file, adding all the data and copying back the new one.
    Patient temp;
    file_obj.read((char*)&temp, sizeof(temp));
    while (!file_obj.eof()) {
        if (temp.patientId == temp_id) {
            switch (choice) {
            case 1:
                char firstName[Patient::firstNameSize];
                cin.clear();
                cin >> firstName;
                strcpy_s(temp.firstName, firstName);
                break;

            case 2:
                char lastName[Patient::lastNameSize];
                cin >> lastName;
                strcpy_s(temp.lastName, lastName);
                break;
            case 3:
                cin >> age;
                break;
            case 4:
            {
                int updatedDOB;
                string updatedDetail;
                cin >> updatedDetail;
                updatedDOB = stoi(updatedDetail);
                temp.dob = updatedDOB;
                break;
            }
            case 5:
                char address[Patient::addressSize];
                cin >> address;
                strcpy_s(temp.Address, address);
                break;

            case 6:
                char visitedLoc[Patient::visitedLocationSize];
                cin >> visitedLoc;
                strcpy_s(temp.visitedLocation, visitedLoc);
                break;

            case 7:
            {
                string updatedDetail;
                cin >> updatedDetail;
                temp.overseasTravel = updatedDetail == "y";
                break;
            }
            case 8:
                string updatedDetail;
                cin >> updatedDetail;
                temp.isAlive = updatedDetail == "alive";
                break;
            }
        };
        temp_file.write((char*)&temp, sizeof(temp));
        file_obj.read((char*)&temp, sizeof(temp));
    }

    file_obj.close();
    temp_file.close();
    remove("Input.txt");
    fstream file_obj2;
    Patient temp_obj;
    file_obj2.open("Input.txt", ios::app);
    temp_file.open("temp.txt", ios::in);
    temp_file.read((char*)&temp_obj, sizeof(temp_obj));

    while (!temp_file.eof()) {
        file_obj2.write((char*)&temp_obj, sizeof(temp_obj));
        temp_file.read((char*)&temp_obj, sizeof(temp_obj));
    }
    file_obj2.close();
    temp_file.close();
    remove("temp.txt");
    cout << "Detail updated" << endl;
}

//Covid testing recommendation function.
void Patient::recommendation(string symp, string location) {

    sympt s1;
    fstream file_obj;
    int high = 0;
    int low = 0;
    int medium = 0;
    int count = 0;

    file_obj.open("symptom.txt", ios::in);
    file_obj.read((char*)&s1, sizeof(s1));
    //Checking if the symptom entered by the user is High, Low, or Medium, based on symptoms stored in the symptoms file.
    while (!file_obj.eof()) {
        count++;
        if (s1.sym == symp) {
            if (s1.level == 'H') {
                high = 1;
            }
            else if (s1.level == ',') {
                medium = 1;
            }
            else if (s1.level == 'L') {
                low = 1;
            }
        }

        auto test = (char*)&s1;
        auto test2 = sizeof(s1);

        file_obj.read((char*)&s1, sizeof(s1));
    }
    if (count == 0) {
        cout << "Unable to recommend COVID test - required data missing" << endl; //Prompting if the symptoms file is empty.
    }
    file_obj.close();


    fstream loc_file;
    loc_file.open("location.txt", ios::in);
    int flag = 0;
    string temp_loc;
    while (getline(loc_file, temp_loc)) {
        if (location == temp_loc) {
            flag = 1;
            break;
        }
    }
    cin.ignore();
    string level = "";
    if (high == 1) {
        level = "H";
    }
    else if (medium == 1) {
        level = "M";
    }
    else if (low == 1) {
        level = "L";
    }

    if (flag == 1) {
    //Recommending COVID testing or precautions based on the severity of the symptoms and the location.

        if (level == "H") {
            cout << "\n";
            cout << "You have high risk symtoms and visited a high risk location, please get a PCR test and isolate immediately " << endl;
        }
        else if (level == "M") {
            cout << "\n";
            cout << "You have medium risk symtoms and visited a high risk location, please get a PCR test and isolate immediately " << endl;
        }
        else if (level == "L") {
            cout << "\n";
            cout << "You have low risk symtoms and visited a high risk location, please get a PCR test and isolate immediately " << endl;
        }
    }
    else {
        if (level == "H") {
            cout << "\n";
            cout << "You have high risk symtoms please take a PCR test! " << endl;
        }
        else if (level == "M") {
            cout << "\n";
            cout << "Take a RAT test and isolate yourself for 7 days" << endl;
        }
        else if (level == "L") {
            cout << "\n";
            cout << "Isolate for 7 days" << endl;
        }
    }
}

//Function to print high risk locations.
void printloc() {
    fstream file_obj;
    file_obj.open("Location.txt", ios::in);
    string loc;
    getline(file_obj, loc);
    cout << "The high risk locations are:- " << endl;
    while (!file_obj.eof()) {
        cout << loc << endl;
        getline(file_obj, loc);
    }
    file_obj.close();
}

//Function to print the details of COVID psotive patients.
void printpositive() {
    Patient obj;
    int count = 0;
    fstream file_obj;
    file_obj.open("Input.txt", ios::in);
    file_obj.read((char*)&obj, sizeof(obj));
        count++;
        if (obj.covidTest) {
            cout << obj.patientId << "  " << obj.firstName << "  " << obj.lastName << "  " << obj.dob << "   "<< obj.dob << "  " << obj.visitedLocation << "  " << obj.Address << "  " << (obj.overseasTravel ? "travelled_overseas" : "not travelled overseas ") << "  " << obj.symptom << "  " << (obj.isAlive ? "alive" : "dead") << endl;
        }
        file_obj.read((char*)&obj, sizeof(obj));
    if (count == 0) {
        cout << "[] - the database is empty" << endl;
    }
    file_obj.close();
}


//Function to print details of all the patients.
void printalldetails() {
    Patient obj;
    int count = 0;
    fstream file_obj;
    file_obj.open("Input.txt", ios::in);
    file_obj.read((char*)&obj, sizeof(obj));
    while (!file_obj.eof()) {
        count++;
        cout << obj.patientId << "  " << obj.firstName << "  " << obj.lastName << "  " << obj.dob << "  " << obj.visitedLocation << "  " << obj.Address << "  " << (obj.overseasTravel ? "travelled_overseas" : "not travelled overseas ") << "  " << obj.symptom << "  " << (obj.isAlive ? "alive" : "dead") << endl;
        file_obj.read((char*)&obj, sizeof(obj));
    }
    if (count == 0) {
        cout << "[] - the database is empty" << endl;
    }
    file_obj.close();
}

//Function to create the symptoms file.
void write_sym() {
    sympt sym1;
    fstream file_obj;
    file_obj.open("symptom.txt", ios::app);
    char choice = 'y';
    //Adding enteries until the user enters n.
    while (choice == 'y') {
        cout << "Enter the severity level :- ";
        cin >> sym1.level;
        cout << "Enter the symptom :- ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> sym1.sym;
        file_obj.write((char*)&sym1, sizeof(sym1));
        cout << "Do you want to enter more symptoms? (y/n):-  ";
        cin >> choice;
        while (!((choice == 'y') || (choice == 'n'))) {
            cout << "Incorrect choice, please enter y or n \n";
            cin >> choice;
        }
    }
    file_obj.close();
}

//Function to perform duplicate ID check on the ID entered.
int IDcheck(int tempID) {
    int ID;
    int exist = 0;
    Patient obj;
    fstream file_obj;
    file_obj.open("Input.txt", ios::in);
    file_obj.read((char*)&obj, sizeof(obj));
    while (!file_obj.eof()) {
        if (tempID == obj.patientId) {
            exist = 1;
        }
        file_obj.read((char*)&obj, sizeof(obj));
    }
    return exist;
}

/*Appropriate variable and function names were used.

Five different combinations were created for the severity of the symptoms and locations. For example, if a patient has been
to a high risk location, he/she will be recommended a COVID test regardless of the symptom. If the location they visisted isn't a high risk
location then the symptoms will be checked

The test data used was the location text file, which included the list of the high risk locations and the symptoms data which contained
different COVID symptoms and their severity.*/