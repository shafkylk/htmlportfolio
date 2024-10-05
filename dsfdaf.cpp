#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm> // for std::find

using namespace std;

struct Player {
    int registrationNumber;
    string firstName;
    string lastName;
    string dateOfBirth;
    int runsScored;
    vector<string> teams; // Player can select up to 2 teams
};

vector<Player> players;
vector<string> teams; // Global variable to store teams

void displayPlayers();
void addPlayer();
void searchPlayers();
void manageTeams();
bool login();
void logout();
void exitProgram();
void saveTeams(const vector<string>& teams); // Declaration of saveTeams function

int main() {
    if (!login()) {
        cout << "Login failed!" << endl;
        return 1;
    }

    int choice;
    do {
        cout << "\n--- Upcountry Warriors ---\n";
        cout << "1. Display Players Information\n";
        cout << "2. Add New Player\n";
        cout << "3. Manage Teams\n";
        cout << "4. Search Players\n";
        cout << "5. View Team Details\n"; // Corrected menu option
        cout << "6. Logout\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayPlayers();
                break;
            case 2:
                addPlayer();
                break;
            case 3:
                manageTeams();
                break;
            case 4:
                searchPlayers();
                break;
            case 5:
                // Placeholder for view team details (not fully implemented in original code)
                cout << "View Team Details\n";
                break;
            case 6:
                logout();
                break;
            case 7:
                exitProgram();
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 7);

    return 0;
}

bool login() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    // For simplicity, let's use hardcoded username and password
    if (username == "shafkybro" && password == "shafky@12") {
        return true;
    } else {
        return false;
    }
}

void logout() {
    cout << "Logged out successfully!" << endl;
}

void exitProgram() {
    cout << "Exiting program..." << endl;
    // Perform any cleanup operations if necessary
}

void displayPlayers() {
    // Load player data from file
    ifstream inFile("players.txt");
    if (!inFile) {
        cout << "No player data found!" << endl;
        return;
    }

    players.clear();
    Player player;
    while (inFile >> player.registrationNumber >> player.firstName >> player.lastName >> player.dateOfBirth >> player.runsScored) {
        string team;
        while (inFile >> team) {
            if (team == "|") break;
            player.teams.push_back(team);
        }
        players.push_back(player);
        player.teams.clear(); // Clear teams vector for next player
    }
    inFile.close();

    // Display player information
    cout << "\n--- Player Information ---\n";
    for (size_t i = 0; i < players.size(); ++i) {
        cout << "Registration Number: " << players[i].registrationNumber << "\n";
        cout << "First Name: " << players[i].firstName << "\n";
        cout << "Last Name: " << players[i].lastName << "\n";
        cout << "Date of Birth: " << players[i].dateOfBirth << "\n";
        cout << "Runs Scored: " << players[i].runsScored << "\n";
        cout << "Teams: ";
        for (size_t j = 0; j < players[i].teams.size(); ++j) {
            cout << players[i].teams[j] << " ";
        }
        cout << "\n------------------------\n";
    }
}

void addPlayer() {
    Player newPlayer;
    cout << "Enter registration number: ";
    cin >> newPlayer.registrationNumber;
    cout << "Enter first name: ";
    cin >> newPlayer.firstName;
    cout << "Enter last name: ";
    cin >> newPlayer.lastName;
    cout << "Enter date of birth (YYYY-MM-DD): ";
    cin >> newPlayer.dateOfBirth;
    cout << "Enter runs scored: ";
    cin >> newPlayer.runsScored;

    cout << "Enter teams (max 2, separated by space, end with '|'): ";
    string team;
    while (newPlayer.teams.size() < 2 && cin >> team) {
        if (team == "|") break;
        newPlayer.teams.push_back(team);
    }

    // Save player data to file
    ofstream outFile("players.txt", ios::app);
    if (!outFile) {
        cout << "Error saving player data!" << endl;
        return;
    }

    outFile << newPlayer.registrationNumber << " "
            << newPlayer.firstName << " "
            << newPlayer.lastName << " "
            << newPlayer.dateOfBirth << " "
            << newPlayer.runsScored << " ";
    for (size_t i = 0; i < newPlayer.teams.size(); ++i) {
        outFile << newPlayer.teams[i] << " ";
    }
    outFile << "|\n";
    outFile.close();

    cout << "Player added successfully!" << endl;
}

void searchPlayers() {
    int regNo;
    cout << "Enter player registration number to search: ";
    cin >> regNo;

    bool found = false;
    for (size_t i = 0; i < players.size(); ++i) {
        if (players[i].registrationNumber == regNo) {
            cout << "Registration Number: " << players[i].registrationNumber << "\n";
            cout << "First Name: " << players[i].firstName << "\n";
            cout << "Last Name: " << players[i].lastName << "\n";
            cout << "Date of Birth: " << players[i].dateOfBirth << "\n";
            cout << "Runs Scored: " << players[i].runsScored << "\n";
            cout << "Teams: ";
            for (size_t j = 0; j < players[i].teams.size(); ++j) {
                cout << players[i].teams[j] << " ";
            }
            cout << "\n";
            found = true;
            break; // Exit loop once player is found
        }
    }

    if (!found) {
        cout << "Player not found!\n";
    }
}

void manageTeams() {
    int choice;
    string team;

    cout << "\n--- Manage Teams ---\n";
    cout << "1. Add New Team\n2. View Team Details\n";
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore(); // Ignore newline character left by cin

    switch (choice) {
        case 1:
            cout << "Enter new team name: ";
            getline(cin, team);
            if (find(teams.begin(), teams.end(), team) == teams.end()) {
                teams.push_back(team);
                saveTeams(teams);
                cout << "Team added successfully.\n";
            } else {
                cout << "Team already exists.\n";
            }
            break;
        case 2:
            cout << "Enter team name to view details: ";
            getline(cin, team);
            if (find(teams.begin(), teams.end(), team) != teams.end()) {
                cout << "Players in team " << team << ":\n";
                for (size_t i = 0; i < players.size(); ++i) {
                    const Player& player = players[i];
                    if (find(player.teams.begin(), player.teams.end(), team) != player.teams.end()) {
                        cout << player.firstName << " " << player.lastName << "\n";
                    }
                }
            } else {
                cout << "Team not found.\n";
            }
            break;
        default:
            cout << "Invalid choice.\n";
            break;
    }
}

void saveTeams(const vector<string>& teams) {
    ofstream outFile("teams.txt");
    if (!outFile) {
        cout << "Error saving teams!" << endl;
        return;
    }

    // Using traditional for loop instead of range-based for loop
    for (size_t i = 0; i < teams.size(); ++i) {
        outFile << teams[i] << "\n";
    }
    outFile.close();
}