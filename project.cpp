#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Event class
class Event {
private:
    string name;
    string date;   // DD-MM-YYYY
    string time;   // HH:MM
    string type;
    string location;

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
        cout << "\nEvent: " << name
             << "\nDate: " << date
             << "\nTime: " << time
             << "\nType: " << type
             << "\nLocation: " << location << "\n";
    }

    string serialize() const {
        return name + "|" + date + "|" + time + "|" + type + "|" + location + "\n";
    }

    static Event deserialize(const string& line) {
        size_t pos = 0;
        string temp = line;
        vector<string> parts;

        while ((pos = temp.find('|')) != string::npos) {
            parts.push_back(temp.substr(0, pos));
            temp.erase(0, pos + 1);
        }
        parts.push_back(temp);

        if (parts.size() < 5) {
            return Event();
        }

        return Event(parts[0], parts[1], parts[2], parts[3], parts[4]);
    }
};

// Event Manager class
class EventManager {
private:
    vector<Event> events;

    static string toLower(const string& s) {
        string result = s;
        transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

public:
    void loadEvents(const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            if (!line.empty())
                events.push_back(Event::deserialize(line));
        }
        file.close();
    }

    void saveEvents(const string& filename) {
        ofstream file(filename);
        for (const auto& e : events) {
            file << e.serialize();
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
            cout << " Conflict detected! An event is already scheduled at this time.\n";
            return;
        }

        // Check duplicates by name+date+time
        for (const auto& e : events) {
            if (e.getName() == name && e.getDate() == date && e.getTime() == time) {
                cout << " Duplicate event found! Not adding.\n";
                return;
            }
        }

        events.emplace_back(name, date, time, type, location);
        saveEvents("events.txt");
        cout << " Event added successfully!\n";
    }

    void viewEvents() const {
        if (events.empty()) {
            cout << "No events scheduled.\n";
            return;
        }
        cout << "\nAll Events:\n";
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
        string nameToDelete;
        cout << "\nEnter the Event Name to delete: ";
        getline(cin, nameToDelete);

        auto it = find_if(events.begin(), events.end(), [&](const Event& e) {
            return e.getName() == nameToDelete;
        });

        if (it != events.end()) {
            events.erase(it);
            saveEvents("events.txt");
            cout << " Event '" << nameToDelete << "' deleted successfully.\n";
        } else {
            cout << " Event with name '" << nameToDelete << "' not found.\n";
        }
    }

    void searchEvents() const {
        cin.ignore();
        string keyword;
        cout << "\nEnter keyword to search (name or type): ";
        getline(cin, keyword);

        string kwLower = toLower(keyword);
        bool found = false;

        for (const auto& e : events) {
            if (toLower(e.getName()).find(kwLower) != string::npos ||
                toLower(e.getType()).find(kwLower) != string::npos) {
                e.display();
                cout << "----------------------\n";
                found = true;
            }
        }

        if (!found) {
            cout << "No matching events found.\n";
        }
    }
};

// Admin login helper
bool adminLogin() {
    string username, password;
    cout << "Admin Login\nUsername: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    return (username == "vinayak" && password == "pccoepune");
}

void adminMenu(EventManager& manager) {
    int choice;
    while (true) {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Add Event\n2. View Events\n3. Delete Event\n4. Search Events\n5. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: manager.addEvent(); break;
            case 2: manager.viewEvents(); break;
            case 3: manager.deleteEvent(); break;
            case 4: manager.searchEvents(); break;
            case 5: return;
            default: cout << "Invalid choice.\n";
        }
    }
}

void userMenu(EventManager& manager) {
    int choice;
    while (true) {
        cout << "\n--- User Menu ---\n";
        cout << "1. View Events\n2. Search Events\n3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: manager.viewEvents(); break;
            case 2: manager.searchEvents(); break;
            case 3: return;
            default: cout << "Invalid choice.\n";
        }
    }
}

int main() {
    EventManager manager;
    manager.loadEvents("events.txt");

    cout << "Welcome to Smart Event Manager\n";
    cout << "Are you admin? (y/n): ";
    char isAdmin;
    cin >> isAdmin;

    if (isAdmin == 'y' || isAdmin == 'Y') {
        if (adminLogin()) {
            adminMenu(manager);
        } else {
            cout << "Access denied. Exiting...\n";
        }
    } else {
        userMenu(manager);
    }

    cout << "Goodbye!\n";
    return 0;
}
