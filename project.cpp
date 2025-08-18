#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// =======================
//  Event Class (CSV Support)
// =======================
class Event {
private:
    string name, date, time, type, location;

public:
    Event() {}
    Event(string n, string d, string t, string ty, string l)
        : name(n), date(d), time(t), type(ty), location(l) {}

    string getName() const { return name; }
    string getDate() const { return date; }
    string getTime() const { return time; }
    string getType() const { return type; }
    string getLocation() const { return location; }

    void display() const {
        cout << "\n Event: " << name
             << "\n Date: " << date
             << "\n Time: " << time
             << "\n Type: " << type
             << "\n Location: " << location << "\n";
    }

    string toCSV() const {
        return name + "," + date + "," + time + "," + type + "," + location;
    }

    static Event fromCSV(const string& line) {
        vector<string> parts;
        stringstream ss(line);
        string token;
        while (getline(ss, token, ',')) {
            parts.push_back(token);
        }

        if (parts.size() < 5) return Event();
        return Event(parts[0], parts[1], parts[2], parts[3], parts[4]);
    }
};

// =======================
//  EventManager Class
// =======================
class EventManager {
private:
    vector<Event> events;

    string toLower(const string& s) const {
        string result = s;
        transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

public:
    void loadEvents(const string& filename) {
        events.clear();
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                Event e = Event::fromCSV(line);
                events.push_back(e);
            }
        }
        file.close();
    }

    void saveEvents(const string& filename) const {
        ofstream file(filename);
        for (const auto& e : events) {
            file << e.toCSV() << endl;
        }
        file.close();
    }

    bool isConflict(const string& date, const string& time) const {
        for (const auto& e : events) {
            if (e.getDate() == date && e.getTime() == time) {
                return true;
            }
        }
        return false;
    }

    void addEvent() {
        cin.ignore();
        string name, date, time, type, location;
        cout << "\nEnter Event Name: ";
        getline(cin, name);
        cout << "Enter Date (DD-MM-YYYY): ";
        getline(cin, date);
        cout << "Enter Time (HH:MM): ";
        getline(cin, time);
        cout << "Enter Type: ";
        getline(cin, type);
        cout << "Enter Location (optional): ";
        getline(cin, location);

        if (isConflict(date, time)) {
            cout << "Conflict detected! Another event is scheduled at the same time.\n";
            return;
        }

        for (const auto& e : events) {
            if (e.getName() == name && e.getDate() == date && e.getTime() == time) {
                cout << "Duplicate event found! Not adding.\n";
                return;
            }
        }

        events.emplace_back(name, date, time, type, location);
        saveEvents("events.csv");
        cout << "Event added successfully.\n";
    }

    void viewEvents() const {
        if (events.empty()) {
            cout << "No events scheduled.\n";
            return;
        }

        cout << "\n Scheduled Events:\n";
        for (const auto& e : events) {
            e.display();
            cout << "----------------------\n";
        }
    }

    void deleteEvent() {
        if (events.empty()) {
            cout << "No events to delete.\n";
            return;
        }

        cin.ignore();
        string name;
        cout << "Enter Event Name to Delete: ";
        getline(cin, name);

        auto it = remove_if(events.begin(), events.end(), [&](const Event& e) {
            return e.getName() == name;
        });

        if (it != events.end()) {
            events.erase(it, events.end());
            saveEvents("events.csv");
            cout << "Event deleted successfully.\n";
        } else {
            cout << "Event not found.\n";
        }
    }

    void searchEvents() const {
        cin.ignore();
        string keyword;
        cout << "Enter keyword to search (name/type): ";
        getline(cin, keyword);

        string kw = toLower(keyword);
        bool found = false;

        for (const auto& e : events) {
            string name = toLower(e.getName());
            string type = toLower(e.getType());

            if (name.find(kw) != string::npos || type.find(kw) != string::npos) {
                e.display();
                cout << "----------------------\n";
                found = true;
            }
        }

        if (!found) {
            cout << " No matching events found.\n";
        }
    }

    // ======================
    //  Timeline Feature
    // ======================
    void displayTimelineForDate() {
        cin.ignore();
        string date;
        cout << "\nEnter date to view timeline (DD-MM-YYYY): ";
        getline(cin, date);

        vector<Event> dayEvents;
        for (const auto& e : events) {
            if (e.getDate() == date) {
                dayEvents.push_back(e);
            }
        }

        if (dayEvents.empty()) {
            cout << "No events scheduled on " << date << ".\n";
            return;
        }

        cout << "\n Timeline for " << date << "\n";
        cout << "----------------------------\n";

        for (int hour = 8; hour <= 20; ++hour) {
            stringstream ss;
            ss << (hour < 10 ? "0" : "") << hour << ":00";
            string slotTime = ss.str();

            bool found = false;
            for (const auto& e : dayEvents) {
                if (e.getTime().substr(0, 2) == slotTime.substr(0, 2)) {
                    cout << slotTime << " | " << e.getName() << "\n";
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << slotTime << " | \n";
            }
        }
    }

    // ======================
    // Edit Event Feature
    // ======================
    void editEvent() {
        if (events.empty()) {
            cout << "No events to edit.\n";
            return;
        }
        
        cin.ignore();
        string nameToEdit;
        cout << "Enter the name of the event to edit: ";
        getline(cin, nameToEdit);

        auto it = find_if(events.begin(), events.end(), [&](const Event& e) {
            return e.getName() == nameToEdit;
        });

        if (it == events.end()) {
            cout << "Event not found.\n";
            return;
        }

        // Show current details
        cout << "\nCurrent details:\n";
        it->display();

        // Get new details
        string name, date, time, type, location;

        cout << "Enter new Event Name (or press enter to keep \"" << it->getName() << "\"): ";
        getline(cin, name);
        if (name.empty()) name = it->getName();

        cout << "Enter new Date (DD-MM-YYYY) (or press enter to keep \"" << it->getDate() << "\"): ";
        getline(cin, date);
        if (date.empty()) date = it->getDate();

        cout << "Enter new Time (HH:MM) (or press enter to keep \"" << it->getTime() << "\"): ";
        getline(cin, time);
        if (time.empty()) time = it->getTime();

        cout << "Enter new Type (or press enter to keep \"" << it->getType() << "\"): ";
        getline(cin, type);
        if (type.empty()) type = it->getType();

        cout << "Enter new Location (or press enter to keep \"" << it->getLocation() << "\"): ";
        getline(cin, location);
        if (location.empty()) location = it->getLocation();

        // Check for conflicts except this event itself
        for (const auto& e : events) {
            if (&e != &(*it) && e.getDate() == date && e.getTime() == time) {
                cout << "Conflict detected with another event at this date and time.\n";
                return;
            }
        }

        // Update event with new details
        *it = Event(name, date, time, type, location);
        saveEvents("events.csv");
        cout << "Event updated successfully.\n";
    }
};

// =======================
// Admin Login
// =======================
bool adminLogin() {
    string username, password;
    cout << "\n Admin Login\nUsername: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    return (username == "vinayak" && password == "pccoepune");
}

// =======================
// Menus
// =======================
void adminMenu(EventManager& manager) {
    int choice;
    while (true) {
        cout << "\n=== Admin Menu ===\n";
        cout << "1. Add Event\n2. View Events\n3. Delete Event\n4. Search Events\n5. View Timeline\n6. Edit Event\n7. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: manager.addEvent(); break;
            case 2: manager.viewEvents(); break;
            case 3: manager.deleteEvent(); break;
            case 4: manager.searchEvents(); break;
            case 5: manager.displayTimelineForDate(); break;
            case 6: manager.editEvent(); break;
            case 7: return;
            default: cout << "Invalid choice.\n";
        }
    }
}

void userMenu(EventManager& manager) {
    int choice;
    while (true) {
        cout << "\n=== User Menu ===\n";
        cout << "1. View Events\n2. Search Events\n3. View Timeline\n4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: manager.viewEvents(); break;
            case 2: manager.searchEvents(); break;
            case 3: manager.displayTimelineForDate(); break;
            case 4: return;
            default: cout << "Invalid choice.\n";
        }
    }
}

// =======================
//  Main Program
// =======================
int main() {
    EventManager manager;
    manager.loadEvents("events.csv");

    cout << " Welcome to Smart Event Manager \n";
    cout << "Persistent Storage Format: CSV (events.csv)\n";
    cout << "Are you admin? (y/n): ";
    char isAdmin;
    cin >> isAdmin;

    if (isAdmin == 'y' || isAdmin == 'Y') {
        if (adminLogin()) {
            adminMenu(manager);
        } else {
            cout << " Invalid credentials.\n";
        }
    } else {
        userMenu(manager);
    }

    cout << " Goodbye!\n";
    return 0;
}


    cout << "Goodbye!\n";
    return 0;
}
