#include "schedule.h"

// Function to display the main menu and get the user's choice
char showMenu() {
    char choice;
    std::cout << "\nSchedule of classes for the Summer 2022 session\n";
    std::cout << "1 - Print all entries\n";
    std::cout << "2 - Print all the records with a subject\n";
    std::cout << "3 - Print all the records with a subject and catalog number\n";
    std::cout << "4 - Prints all the records given the instructor's last name\n";
    std::cout << "5 - Quit\n";
    std::cout << "Enter your choice: ";
    std::cin >> choice;
    return choice;
}

int main(void) {
    const std::string fileName = "STEM+-+Summer+2022+Schedule+of+Classes+as+of+05-02-22%281%29.csv";
    std::ifstream stream(fileName);
    if (!stream.is_open()) {
        std::cerr << "Error reading from file." << std::endl;
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
            std::string subject;
            std::cout << "Enter Subject: ";
            std::cin >> subject;
            obj.findBySubject(subject);
            break;
        }
        case '3': { // Search records by subject and catalog number
            std::string subject;
            std::string catalog;
            std::cout << "Enter Subject: ";
            std::cin >> subject;
            std::cout << "Enter Catalog Number: ";
            std::cin >> catalog;
            obj.findBySubjectCatalog(subject, catalog);
            break;
        }
        case '4': { // Search records by instructor's last name
            std::string lastName;
            std::cout << "Enter Instructor's Last Name: ";
            std::cin >> lastName;
            obj.findByInstructorLastName(lastName);
            break;
        }
        case '5': // Exit the program
            std::cout << "Exiting program..." << std::endl;
            break;
        default: // Handle invalid choices
            std::cout << "\nInvalid choice. " << choice << " Please try again." << std::endl;
        }
    } while (choice != '5'); // Loop until the user chooses to quit
}
