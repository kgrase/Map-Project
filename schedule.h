#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include <fstream>
#include <vector>

using namespace std;

// Class representing a single schedule entry
class scheduleItem {
public:
    // Constructors for scheduleItem
    scheduleItem() :
        _i_units(0), _i_totEnrl(0), _i_capEnrol(0),
        _str_subject(""), _str_catalog(""), _str_section(""),
        _str_session(""), _str_component(""), _str_instructor("") {}

    scheduleItem(int units, int totEnrl, int capEnrol,
        const string& subject, const string& catalog,
        const string& section, const string& session,
        const string& component, const string& instructor) :
        _i_units(units), _i_totEnrl(totEnrl), _i_capEnrol(capEnrol),
        _str_subject(subject), _str_catalog(catalog), _str_section(section),
        _str_session(session), _str_component(component), _str_instructor(instructor) {}

private:
    // Attributes of a schedule entry
    int _i_units; // Number of units
    int _i_totEnrl; // Total enrollment
    int _i_capEnrol; // Capacity enrollment
    string _str_subject; // Subject
    string _str_catalog; // Catalog number
    string _str_section; // Section number
    string _str_session; // Session type
    string _str_component; // Component type (e.g., lecture, lab)
    string _str_instructor; // Instructor name

public:
    // Accessor methods for schedule attributes
    int getUnits() const { return this->_i_units; }
    int getTotEnrl() const { return this->_i_totEnrl; }
    int getCapEnrol() const { return this->_i_capEnrol; }
    const string& getSubject() const { return this->_str_subject; }
    const string& getCatalog() const { return this->_str_catalog; }
    const string& getSection() const { return this->_str_section; }
    const string& getSession() const { return this->_str_session; }
    const string& getComponent() const { return this->_str_component; }
    const string& getInstructor() const { return this->_str_instructor; }
    string getKey() const { return genKey(this->_str_subject, this->_str_catalog, this->_str_section); }

    // Get the instructor's last name
    string getInstructorLastName() const {
        size_t commaPos = _str_instructor.find(',');
        if (commaPos != string::npos) {
            return _str_instructor.substr(0, commaPos);
        }
        return _str_instructor;
    }

    // Generate a unique key for a schedule entry
    static string genKey(const string& subject, const string& catalog, const string& section) {
        return subject + "_" + catalog + "_" + section;
    }

    // Overload operators for comparison
    bool operator == (const scheduleItem& other) const { return this->getKey() == other.getKey(); }
    bool operator != (const scheduleItem& other) const { return this->getKey() != other.getKey(); }
    bool operator >= (const scheduleItem& other) const { return this->getKey() >= other.getKey(); }

    // Print the schedule item in a formatted way
    void print(void) const {
        cout << setfill(' ') << setw(10) << this->_str_subject
             << setfill(' ') << setw(10) << this->_str_catalog
             << setfill(' ') << setw(10) << this->_str_section
             << setfill(' ') << setw(10) << this->_str_component
             << setfill(' ') << setw(10) << this->_str_session
             << setfill(' ') << setw(6) << this->_i_units
             << setfill(' ') << setw(10) << this->_i_totEnrl
             << setfill(' ') << setw(10) << this->_i_capEnrol
             << setfill(' ') << setw(20) << this->_str_instructor
             << endl;
    }
};

// Class representing the schedule system
class schedule {
private:
    map<string, scheduleItem> _dict; // Map to store schedule entries

    // Add a new entry to the schedule
    void addEntry(const scheduleItem&);

public:
    // Initialize the schedule from an input file
    void initSchedule(ifstream*);

    // Print the entire schedule
    void print() const;
    void printHeader() const;

    // Methods to search and print specific entries
    void findByKey(const string&, const string&, const string&) const;
    void findBySubject(const string&) const;
    void findBySubjectCatalog(const string&, const string&) const;
    void findByInstructorLastName(const string&) const;
};

// Method definitions for schedule class...

// Initialize the schedule from a file stream
void schedule::initSchedule(ifstream* stream) {
    if (!stream->is_open()) { // Check if the file stream is open
        cerr << "Error reading from file." << endl;
        return;
    }

    string row;
    getline(*stream, row); // Skip the header row

    int rowNum = 1;
    while (getline(*stream, row)) { // Read each row of the file
        size_t pos = 0;
        size_t idx = 0;
        vector<string> rec;

        idx = row.find_first_of(",\"", pos);
        while (idx != string::npos) { // Parse the row into individual fields
            if (row[idx] == '\"') { // Handle fields enclosed in double quotes
                pos++;
                idx = row.find_first_of("\"", pos);
            }
            if (idx == string::npos) { // Handle errors in the record
                cerr << "Error: Invalid Record at line " << rowNum << ", position " << pos << endl;
                break;
            }
            rec.push_back(row.substr(pos, idx - pos)); // Add the parsed field to the record
            pos = idx + 1;
            idx = row.find_first_of(",\"", pos);
        }
        if (rec.size() != 25) { // Ensure the record has the expected number of fields
            cerr << "Error: Invalid Record." << endl;
            continue;
        }

        // Create a scheduleItem object and add it to the schedule
        scheduleItem item(stoi(rec[6]), stoi(rec[7]), stoi(rec[8]), rec[0], rec[1], rec[2], rec[4], rec[3], rec[10]);
        addEntry(item);
        rowNum++;
    }
}

void schedule::addEntry(const scheduleItem& item) {
    _dict.insert(make_pair(item.getKey(), item));
}

void schedule::print() const {
    printHeader();
    for (const auto& entry : _dict) {
        const scheduleItem& value = entry.second;
        value.print();
    }
}

void schedule::printHeader() const {
    cout << left
         << setw(10) << setfill(' ') << "Subject"
         << setw(10) << setfill(' ') << "Catalog"
         << setw(10) << setfill(' ') << "Section"
         << setw(10) << setfill(' ') << "Component"
         << setw(10) << setfill(' ') << "Session"
         << setw(6) << setfill(' ') << "Units"
         << setw(10) << setfill(' ') << "TotEnrl"
         << setw(10) << setfill(' ') << "CapEnrol"
         << setw(20) << setfill(' ') << "Instructor"
         << endl;
}

void schedule::findByKey(const string& subject, const string& catalog, const string& section) const {
    bool found = false;
    string key = scheduleItem::genKey(subject, catalog, section);
    map<string, scheduleItem>::const_iterator it = _dict.find(key);
    if (it != _dict.end()) {
        it->second.print();
    }
    else {
        cout << "No records found with Key: " << key << endl;
    }
}

void schedule::findBySubject(const string& subject) const {
    bool found = false;
    for (map<string, scheduleItem>::const_iterator it = _dict.cbegin(); it != _dict.cend(); ++it) {
        const scheduleItem& item = it->second;    // Access the value
        if (item.getSubject() == subject) {
            item.print();
            found = true;
        }
    }
    if (!found) {
        cout << "No records found with Subject: " << subject << endl;
    }
}

void schedule::findBySubjectCatalog(const string& subject, const string& catalog) const {
    bool found = false;
    for (map<string, scheduleItem>::const_iterator it = _dict.cbegin(); it != _dict.cend(); ++it) {
        const scheduleItem& item = it->second;    // Access the value
        if (item.getSubject() == subject && item.getCatalog() == catalog) {
            item.print();
            found = true;
        }
    }
    if (!found) {
        cout << "No records found with Subject: " << subject << " and Catalog: " << catalog << endl;
    }
}

void schedule::findByInstructorLastName(const string& lastName) const {
    bool found = false;
    for (map<string, scheduleItem>::const_iterator it = _dict.cbegin(); it != _dict.cend(); ++it) {
        const scheduleItem& item = it->second;    // Access the value
        if (item.getInstructorLastName() == lastName) {
            item.print();
            found = true;
        }
    }
    if (!found) {
        cout << "No records found with Instructor's Last Name: " << lastName << endl;
    }
}

#endif
