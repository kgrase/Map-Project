#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include <fstream>
#include <vector>

// Class representing a single schedule entry
class scheduleItem {
public:
    // Constructors for scheduleItem
    scheduleItem() :
        _i_units(0), _i_totEnrl(0), _i_capEnrol(0),
        _str_subject(""), _str_catalog(""), _str_section(""),
        _str_session(""), _str_component(""), _str_instructor("") {}

    scheduleItem(int units, int totEnrl, int capEnrol,
        const std::string& subject, const std::string& catalog,
        const std::string& section, const std::string& session,
        const std::string& component, const std::string& instructor) :
        _i_units(units), _i_totEnrl(totEnrl), _i_capEnrol(capEnrol),
        _str_subject(subject), _str_catalog(catalog), _str_section(section),
        _str_session(session), _str_component(component), _str_instructor(instructor) {}

private:
    // Attributes of a schedule entry
    int _i_units; // Number of units
    int _i_totEnrl; // Total enrollment
    int _i_capEnrol; // Capacity enrollment
    std::string _str_subject; // Subject
    std::string _str_catalog; // Catalog number
    std::string _str_section; // Section number
    std::string _str_session; // Session type
    std::string _str_component; // Component type (e.g., lecture, lab)
    std::string _str_instructor; // Instructor name

public:
    // Accessor methods for schedule attributes
    int getUnits() const { return this->_i_units; }
    int getTotEnrl() const { return this->_i_totEnrl; }
    int getCapEnrol() const { return this->_i_capEnrol; }
    const std::string& getSubject() const { return this->_str_subject; }
    const std::string& getCatalog() const { return this->_str_catalog; }
    const std::string& getSection() const { return this->_str_section; }
    const std::string& getSession() const { return this->_str_session; }
    const std::string& getComponent() const { return this->_str_component; }
    const std::string& getInstructor() const { return this->_str_instructor; }
    std::string getKey() const { return genKey(this->_str_subject, this->_str_catalog, this->_str_section); }

    // Get the instructor's last name
    std::string getInstructorLastName() const {
        size_t commaPos = _str_instructor.find(',');
        if (commaPos != std::string::npos) {
            return _str_instructor.substr(0, commaPos);
        }
        return _str_instructor;
    }

    // Generate a unique key for a schedule entry
    static std::string genKey(const std::string& subject, const std::string& catalog, const std::string& section) {
        return subject + "_" + catalog + "_" + section;
    }

    // Overload operators for comparison
    bool operator == (const scheduleItem& other) const { return this->getKey() == other.getKey(); }
    bool operator != (const scheduleItem& other) const { return this->getKey() != other.getKey(); }
    bool operator >= (const scheduleItem& other) const { return this->getKey() >= other.getKey(); }

    // Print the schedule item in a formatted way
    void print(void) const {
        std::cout   << std::setfill(' ') << std::setw(10) << this->_str_subject
                    << std::setfill(' ') << std::setw(10) << this->_str_catalog
                    << std::setfill(' ') << std::setw(10) << this->_str_section
                    << std::setfill(' ') << std::setw(10) << this->_str_component
                    << std::setfill(' ') << std::setw(10) << this->_str_session
                    << std::setfill(' ') << std::setw(6) << this->_i_units
                    << std::setfill(' ') << std::setw(10) << this->_i_totEnrl
                    << std::setfill(' ') << std::setw(10) << this->_i_capEnrol
                    << std::setfill(' ') << std::setw(20) << this->_str_instructor
                    << std::endl;
    }
};

// Class representing the schedule system
class schedule {
private:
    std::map<std::string, scheduleItem> _dict; // Map to store schedule entries

    // Add a new entry to the schedule
    void addEntry(const scheduleItem&);

public:
    // Initialize the schedule from an input file
    void initSchedule(std::ifstream*);

    // Print the entire schedule
    void print() const;
    void printHeader() const;

    // Methods to search and print specific entries
    void findByKey(const std::string&, const std::string&, const std::string&) const;
    void findBySubject(const std::string&) const;
    void findBySubjectCatalog(const std::string&, const std::string&) const;
    void findByInstructorLastName(const std::string&) const;
};

// Method definitions for schedule class...

// Initialize the schedule from a file stream
void schedule::initSchedule(std::ifstream* stream) {
    if (!stream->is_open()) { // Check if the file stream is open
        std::cerr << "Error reading from file." << std::endl;
        return;
    }

    std::string row;
    std::getline(*stream, row); // Skip the header row

    int rowNum = 1;
    while (std::getline(*stream, row)) { // Read each row of the file
        size_t pos = 0;
        size_t idx = 0;
        std::vector<std::string> rec;

        idx = row.find_first_of(",\"", pos);
        while (idx != std::string::npos) { // Parse the row into individual fields
            if (row[idx] == '\"') { // Handle fields enclosed in double quotes
                pos++;
                idx = row.find_first_of("\"", pos);
            }
            if (idx == std::string::npos) { // Handle errors in the record
                std::cerr << "Error: Invalid Record at line " << rowNum << ", position " << pos << std::endl;
                break;
            }
            rec.push_back(row.substr(pos, idx - pos)); // Add the parsed field to the record
            pos = idx + 1;
            idx = row.find_first_of(",\"", pos);
        }
        if (rec.size() != 25) { // Ensure the record has the expected number of fields
            std::cerr << "Error: Invalid Record." << std::endl;
            continue;
        }

        // Create a scheduleItem object and add it to the schedule
        scheduleItem item(std::stoi(rec[6]), std::stoi(rec[7]), std::stoi(rec[8]), rec[0], rec[1], rec[2], rec[4], rec[3], rec[10]);
        addEntry(item);
        rowNum++;
    }
}

void schedule::addEntry(const scheduleItem& item) {
    _dict.insert(std::make_pair(item.getKey(), item));
}

void schedule::print() const {
    printHeader();
    for (const auto& entry : _dict) {
        const scheduleItem& value = entry.second;
        value.print();
    }
}

void schedule::printHeader() const {
    std::cout << std::left
              << std::setw(10) << std::setfill(' ') << "Subject"
              << std::setw(10) << std::setfill(' ') << "Catalog"
              << std::setw(10) << std::setfill(' ') << "Section"
              << std::setw(10) << std::setfill(' ') << "Component"
              << std::setw(10) << std::setfill(' ') << "Session"
              << std::setw(6) << std::setfill(' ') << "Units"
              << std::setw(10) << std::setfill(' ') << "TotEnrl"
              << std::setw(10) << std::setfill(' ') << "CapEnrol"
              << std::setw(20) << std::setfill(' ') << "Instructor"
              << std::endl;
}

void schedule::findByKey(const std::string& subject, const std::string& catalog, const std::string& section) const {
    bool found = false;
    std::string key = scheduleItem::genKey(subject, catalog, section);
    std::map<std::string, scheduleItem>::const_iterator it = _dict.find(key);
    if (it != _dict.end()) {
        it->second.print();
    }
    else {
        std::cout << "No records found with Key: " << key << std::endl;
    }
}

void schedule::findBySubject(const std::string& subject) const {
    bool found = false;
    for (std::map<std::string, scheduleItem>::const_iterator it = _dict.cbegin(); it != _dict.cend(); ++it) {
        const scheduleItem& item = it->second;    // Access the value
        if (item.getSubject() == subject) {
            item.print();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No records found with Subject: " << subject << std::endl;
    }
}

void schedule::findBySubjectCatalog(const std::string& subject, const std::string& catalog) const {
    bool found = false;
    for (std::map<std::string, scheduleItem>::const_iterator it = _dict.cbegin(); it != _dict.cend(); ++it) {
        const scheduleItem& item = it->second;    // Access the value
        if (item.getSubject() == subject && item.getCatalog() == catalog) {
            item.print();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No records found with Subject: " << subject << " and Catalog: " << catalog << std::endl;
    }
}

void schedule::findByInstructorLastName(const std::string& lastName) const {
    bool found = false;
    for (std::map<std::string, scheduleItem>::const_iterator it = _dict.cbegin(); it != _dict.cend(); ++it) {
        const scheduleItem& item = it->second;    // Access the value
        if (item.getInstructorLastName() == lastName) {
            item.print();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No records found with Instructor's Last Name: " << lastName << std::endl;
    }
}

#endif
