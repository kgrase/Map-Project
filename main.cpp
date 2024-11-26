#include "schedule.h"

// Function to display the main menu and get the user's choice
char showMenu() {
    char choice;
    cout << "\nSchedule of classes for the Summer 2022 session\n";
    cout << "1 - Print all entries\n";
    cout << "2 - Print all the records with a subject\n";
    cout << "3 - Print all the records with a subject and catalog number\n";
    cout << "4 - Prints all the records given the instructor's last name\n";
    cout << "5 - Quit\n";
    cout << "Enter your choice: ";
    cin >> choice;
    return choice;
}

int main(void) {
    const string fileName = "STEM+-+Summer+2022+Schedule+of+Classes+as+of+05-02-22%281%29.csv";
    ifstream stream(fileName);
    if (!stream.is_open()) {
        cerr << "Error reading from file." << endl;
        return 0;
    }

    schedule obj; // Create a schedule object
    obj.initSchedule(&stream); // Initialize the schedule object with data from the file
    stream.close();

    char choice;
    do {
        choice = showMenu(); // Display menu and get user choice
        switch (choice) {
        case '1': // Print all schedule entries
            obj.print();
            break;
        case '2': { // Search records by subject
            string subject;
            cout << "Enter Subject: ";
            cin >> subject;
            obj.findBySubject(subject);
            break;
        }
        case '3': { // Search records by subject and catalog number
            string subject;
            string catalog;
            cout << "Enter Subject: ";
            cin >> subject;
            cout << "Enter Catalog Number: ";
            cin >> catalog;
            obj.findBySubjectCatalog(subject, catalog);
            break;
        }
        case '4': { // Search records by instructor's last name
            string lastName;
            cout << "Enter Instructor's Last Name: ";
            cin >> lastName;
            obj.findByInstructorLastName(lastName);
            break;
        }
        case '5': // Exit the program
            cout << "Exiting program..." << endl;
            break;
        default: // Handle invalid choices
            cout << "\nInvalid choice. " << choice << " Please try again." << endl;
        }
    } while (choice != '5'); // Loop until the user chooses to quit
}
