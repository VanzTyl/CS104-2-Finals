// Define Libraries & Imports
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <conio.h>
#endif

using namespace std;

// Define Hardcoded Limits
#define MAX_COUNTRIES 37
#define MAX_PURCHASES 500
#define MAX_QUEUE 100
#define MAX_EVENTS 10
#define MAX_MATCHES 50
#define MAX_NAMES 30

// Define Structs
typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int matchID;
    char team1[50];
    char team2[50];
    int score1;
    int score2;
} Match;

typedef struct {
    int eventID;
    char eventName[100];
    char format[50];
    int matchCount;
    Match matches[MAX_MATCHES];
    int teamCount;
    char teams[10][50];
} Event;

typedef struct {
    int purchaseID;
    int ticketID;
    char customerName[50];
    char eventName[100];
    char matchInfo[100];
    double amountPaid;
    Date purchaseDate;
    char refundStatus[20];
} Purchase;

typedef struct {
    int requestID;
    Purchase purchaseRecord;
    char reason[100];
    bool isValidReason;
} RefundRequest;

typedef struct BSTNode {
    int teamKey;
    char teamName[50];
    int wins;
    int losses;
    int draws;
    int points;
    int goalsFor;
    int goalsAgainst;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

// Define Array of Structures
Purchase masterPurchases[MAX_PURCHASES];
RefundRequest refundQueueArray[MAX_QUEUE];
Event activeEvents[MAX_EVENTS];
Event completedEvents[MAX_EVENTS * 10];
RefundRequest expiredRefunds[MAX_QUEUE];

// Define Global Variables
int purchaseCount = 0;
int queueFront = 0;
int queueRear = -1;
int queueSize = 0;
int activeEventCount = 0;
int completedEventCount = 0;
int expiredRefundCount = 0;

int currentDay = 1;
int organizerReputation = 80;
int totalEventsCreated = 0;
int totalMatchesGenerated = 0;
int totalTicketsSold = 0;
double totalRevenue = 0.0;
int totalRefundRequests = 0;
int approvedRefunds = 0;
int rejectedRefunds = 0;
int highestReputation = 80;
int lowestReputation = 80;
bool isSortedByID = false;
bool tutorialShown = false;
int genPurchasesMin = 3;
int genPurchasesMax = 7;
double genPricePerMatch = 100.0;
double genPriceVariance = 20.0;
int genRefundCount = 1;
int saveFileCounter = 1;
char currentSaveFile[50] = "";
bool gameRunning = true;

BSTNode* standingsRoot = NULL;

// Define Terminal User Interface Variables
string ur_br = "╮";
string ul_br = "╭";
string ver_br = "│";
string hor_br = "─";
string br_br = "╯";
string bl_br = "╰";

string countries[] = {
    "Afghanistan",
    "Albania",
    "Algeria",
    "Andorra",
    "Angola",
    "Argentina",
    "Australia",
    "Austria",
    "Belgium",
    "Brazil",
    "Canada",
    "China",
    "Colombia",
    "Croatia",
    "Denmark",
    "Egypt",
    "England",
    "France",
    "Germany",
    "Ghana",
    "Greece",
    "Italy",
    "Japan",
    "Mexico",
    "Netherlands",
    "Nigeria",
    "Norway",
    "Philippines",
    "Portugal",
    "South Korea",
    "Spain",
    "Sweden",
    "Switzerland",
    "Thailand",
    "Turkey",
    "United States",
    "Uruguay"
};

string firstNames[MAX_NAMES] = {
    "James", "Mary", "John", "Patricia", "Robert", "Jennifer",
    "Michael", "Linda", "David", "Elizabeth", "William", "Barbara",
    "Richard", "Susan", "Joseph", "Jessica", "Thomas", "Sarah",
    "Charles", "Karen", "Christopher", "Lisa", "Daniel", "Nancy",
    "Matthew", "Betty", "Anthony", "Margaret", "Mark", "Sandra"
};

string lastNames[MAX_NAMES] = {
    "Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
    "Miller", "Davis", "Rodriguez", "Martinez", "Hernandez", "Lopez",
    "Gonzalez", "Wilson", "Anderson", "Thomas", "Taylor", "Moore",
    "Jackson", "Martin", "Lee", "Perez", "Thompson", "White",
    "Harris", "Sanchez", "Clark", "Ramirez", "Lewis", "Robinson"
};

// Define Functions
void clearScreen();
void runTutorial();
void displayDashboard();
void configureEvents();
void generateTicketPurchases();
void generateRefundRequests();
void reviewRefundRequests();
void viewStatistics();
void searchPurchaseRecord();
void endDaySummary();
void saveGame(const char* filename);
void loadGame(const char* filename);
void optionsAndExtraSettings();
void configureGenerationSettings();
void managePurchases();
void showAllPurchases();
void manageRefunds();
void viewRefundQueue();
void viewRefundHistory();
void viewExpiredRefunds();
void generatePurchasesAndRequests();
int searchCountriesByPrefix(const char prefix[], int results[], int &count, int &comparisons);
void selectTeamsManual(Event &e, int numTeams);
int showStartupMenu();
void saveAndExit();
void computeStandingsForEvent(Event &e);

// Custom Queue Functions
void enqueueRefund(RefundRequest req);
RefundRequest dequeueRefund();
bool isQueueFull();
bool isQueueEmpty();

// BST Functions
BSTNode* createBSTNode(int key, const char* name, int w, int l, int d, int pts, int gf, int ga);
BSTNode* insertBSTNode(BSTNode* root, int key, const char* name, int w, int l, int d, int pts, int gf, int ga);
void inOrderTraversal(BSTNode* root);
void freeBST(BSTNode* root);

// Sorting & Searching Functions
void selectionSortPurchasesByID();
int binarySearchPurchaseByID(int targetID);

// Terminal UI Helpers
void drawHeader(string title);
void drawFooter();
void printBorderRow(string text);
void printColumns(string col1, string col2, string col3);
void printInfoLine(string label, string value);
void printInfoSpacer();
string formatDecimal(double amount);
string formatAmount(double amount);
string truncateForHeader(string text, int maxLen);
string getReputationRank(int rep);

int main(){

#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    srand((unsigned int)time(0));

    int startChoice = showStartupMenu();
    if (startChoice == -1) {
        freeBST(standingsRoot);
        return 0;
    }
    if (startChoice == 0) {
        runTutorial();
        tutorialShown = true;
    }

    int choice = 0;
    while (gameRunning && choice != 7) {
        displayDashboard();
        cout << "\nSelect Choice [1-7]: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                configureEvents();
                break;
            case 2:
                generatePurchasesAndRequests();
                break;
            case 3:
                managePurchases();
                break;
            case 4:
                manageRefunds();
                break;
            case 5:
                optionsAndExtraSettings();
                break;
            case 6:
                endDaySummary();
                break;
            case 7:
                saveAndExit();
                break;
            default:
                cout << "\nInvalid selection. Try again.\n";
        }
    }

    freeBST(standingsRoot);
    return 0;
}

// Function Definitions

void drawHeader(string title) {
    cout << ul_br;
    for (int i = 0; i < 78; i++) cout << hor_br;
    cout << ur_br << "\n";

    int pad = (78 - (int)title.length()) / 2;
    int rightPad = 78 - (int)title.length() - pad;

    cout << ver_br;
    for (int i = 0; i < pad; i++) cout << " ";
    cout << title;
    for (int i = 0; i < rightPad; i++) cout << " ";
    cout << ver_br << "\n";

    cout << bl_br;
    for (int i = 0; i < 78; i++) cout << hor_br;
    cout << br_br << "\n";
}

void printBorderRow(string text) {
    cout << ver_br << " ";
    cout << left << setw(77) << text;
    cout << ver_br << "\n";
}

void drawFooter() {
    cout << bl_br;
    for (int i = 0; i < 78; i++) cout << hor_br;
    cout << br_br << "\n";
}

void printColumns(string col1, string col2, string col3) {
    cout << ver_br << " ";
    cout << left << setw(15) << col1;
    cout << left << setw(27) << col2;
    cout << right << setw(15) << col3;
    cout << ver_br << "\n";
}

void printInfoLine(string label, string value) {
    string line = " " + label;
    while ((int)line.length() < 1 + 22) line += " ";
    line += value;
    while ((int)line.length() < 1 + 77) line += " ";
    cout << ver_br << line << ver_br << "\n";
}

void printInfoSpacer() {
    cout << ver_br << " " << string(77, ' ') << ver_br << "\n";
}

string formatDecimal(double amount) {
    char buffer[20];
    sprintf(buffer, "%.2f", amount);
    return string(buffer);
}

string formatAmount(double amount) {
    return "$" + formatDecimal(amount);
}

string truncateForHeader(string text, int maxLen) {
    if ((int)text.length() <= maxLen) return text;
    return text.substr(0, maxLen - 3) + "...";
}

string getReputationRank(int rep) {
    if (rep >= 95) return "Elite Organizer";
    if (rep >= 80) return "Trusted Organizer";
    if (rep >= 60) return "Established Organizer";
    if (rep >= 40) return "Under Review";
    return "Poor Organizer";
}

void waitForEnter() {
    cout << "\nPress Enter to return...";
    cout.flush();
#ifdef _WIN32
    while (_getch() != '\r') { }
#else
    cin.ignore(cin.rdbuf()->in_avail(), '\n');
    cin.get();
#endif
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displayDashboard() {
    clearScreen();
    drawHeader("SOCCER SEAT TICKETS REFUND MANAGEMENT SYSTEM");

    string dayStr = "Current Day: Day " + to_string(currentDay);
    string repStr = "Reputation: " + to_string(organizerReputation) +
                    " (" + getReputationRank(organizerReputation) + ")";
    string tixStr = "Total Tickets Sold: " + to_string(totalTicketsSold);
    string reqStr = "Pending Refund Requests in Queue: " + to_string(queueSize);
    string activeEvStr = "Active Tournament Events: " + to_string(activeEventCount);

    printBorderRow(dayStr);
    printBorderRow(repStr);
    printBorderRow(tixStr);
    printBorderRow(reqStr);
    printBorderRow(activeEvStr);

    drawFooter();

    drawHeader("MAIN MENU");

    printBorderRow("1. Create New Event");
    printBorderRow("2. Generate Purchases & Requests");
    printBorderRow("3. Manage Purchases");
    printBorderRow("4. Manage Refunds");
    printBorderRow("5. Options & Extra Settings");
    printBorderRow("6. End Day & View Daily Summary");
    printBorderRow("7. Save & Exit Simulation");

    drawFooter();
}

int showStartupMenu() {
    clearScreen();
    drawHeader("SOCCER SEAT TICKETS REFUND MANAGEMENT SYSTEM");
    printBorderRow("Welcome to the Simulation!");
    drawFooter();

    drawHeader("STARTUP MENU");
    printBorderRow("1. New Game");
    printBorderRow("2. Load Game");
    printBorderRow("3. Exit");
    drawFooter();

    int choice;
    cout << "\nSelect Choice [1-3]: ";
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(1000, '\n');
    }

    if (choice == 1) return 0;
    if (choice == 3) return -1;

    if (choice == 2) {
        char saveFiles[10][50];
        int saveCount = 0;

#ifdef _WIN32
        struct _finddata_t fileInfo;
        long handle = _findfirst("*_SMTMS.txt", &fileInfo);
        if (handle != -1) {
            do {
                strncpy(saveFiles[saveCount], fileInfo.name, 50);
                saveFiles[saveCount][49] = '\0';
                saveCount++;
            } while (_findnext(handle, &fileInfo) == 0 && saveCount < 10);
            _findclose(handle);
        }
#endif

        if (saveCount == 0) {
            cout << "\nNo save files found. Starting new game.\n";
            return 0;
        }

        drawHeader("SAVED GAMES");
        for (int i = 0; i < saveCount; i++) {
            printBorderRow(to_string(i + 1) + ". " + string(saveFiles[i]));
        }
        drawFooter();

        cout << "\nSelect save file [1-" << saveCount << "]: ";
        int fileChoice;
        if (!(cin >> fileChoice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nInvalid input. Starting new game.\n";
            return 0;
        }
        if (fileChoice < 1 || fileChoice > saveCount) {
            cout << "Invalid choice. Starting new game.\n";
            return 0;
        }

        strncpy(currentSaveFile, saveFiles[fileChoice - 1], 50);
        loadGame(currentSaveFile);
        return 1;
    }

    return 0;
}

void runTutorial() {
    clearScreen();
    drawHeader("SYSTEM TUTORIAL");
    printBorderRow("Welcome to the Soccer Seat Tickets Refund Management");
    printBorderRow("Simulation! As an event organizer, your goal is to");
    printBorderRow("create tournaments, sell tickets, process refunds,");
    printBorderRow("and maintain your Organizer Reputation.");
    printBorderRow("");
    printBorderRow("MAIN MENU OVERVIEW:");
    printBorderRow("[1] Create New Event - Set up tournament events.");
    printBorderRow("    Choose Manual or Auto event naming. Select");
    printBorderRow("    format: Round Robin (teams play each other),");
    printBorderRow("    Winner & Losers Bracket, or Single Elimination.");
    printBorderRow("    Pick teams via Binary Search or auto-generate.");
    printBorderRow("[2] Generate Purchases & Requests - Simulate");
    printBorderRow("    ticket sales and generate refund requests.");
    printBorderRow("    Customize in Options & Extra Settings.");
    printBorderRow("[3] Manage Purchases - View all purchases sorted");
    printBorderRow("    via Selection Sort. Search by ID using Binary");
    printBorderRow("    Search for fast lookups.");
    printBorderRow("[4] Manage Refunds - Review pending refunds in");
    printBorderRow("    FIFO Queue order. Approve/Reject wisely to");
    printBorderRow("    maintain reputation. View queue or history.");
    printBorderRow("[5] Options & Extra Settings - View statistics");
    printBorderRow("    and Team Standings (BST Leaderboard). Re-watch");
    printBorderRow("    this tutorial. Configure Generation Settings");
    printBorderRow("    (adjust ticket prices, purchase counts, etc).");
    printBorderRow("[6] End Day & View Daily Summary - See today's");
    printBorderRow("    performance. Optionally view Team Standings");
    printBorderRow("    per event before advancing to next day.");
    printBorderRow("[7] Save & Exit - Save progress to a file.");
    printBorderRow("");
    printBorderRow("REPUTATION SYSTEM:");
    printBorderRow("Correct decisions (approve valid, reject invalid)");
    printBorderRow("increase reputation by +3. Incorrect decisions");
    printBorderRow("decrease it by -5. Higher reputation unlocks");
    printBorderRow("better organizer ranks (Elite, Trusted, etc).");
    printBorderRow("");
    printBorderRow("DATA STRUCTURES DEMONSTRATED:");
    printBorderRow("Array: Master purchase storage");
    printBorderRow("Nested Structs: Purchase contains Date");
    printBorderRow("Selection Sort: Purchase records by ID");
    printBorderRow("Binary Search: Find purchases and search teams");
    printBorderRow("Queue (FIFO): Refund request processing");
    printBorderRow("BST: Team standings leaderboard");
    printBorderRow("File I/O: Save and load simulation state");
    drawFooter();
    waitForEnter();
}

int searchCountriesByPrefix(const char prefix[], int results[], int &count, int &comparisons) {
    int low = 0;
    int high = MAX_COUNTRIES - 1;
    comparisons = 0;
    int firstMatch = -1;
    int prefixLen = strlen(prefix);

    if (prefixLen == 0) return -1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        comparisons++;
        int cmp = strncmp(prefix, countries[mid].c_str(), prefixLen);
        if (cmp == 0) {
            firstMatch = mid;
            high = mid - 1;
        } else if (cmp < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    if (firstMatch == -1) {
        count = 0;
        return -1;
    }

    count = 0;
    for (int i = firstMatch; i < MAX_COUNTRIES && strncmp(prefix, countries[i].c_str(), prefixLen) == 0; i++) {
        results[count++] = i;
    }
    return firstMatch;
}

void selectTeamsManual(Event &e, int numTeams) {
    int selected = 0;

    while (selected < numTeams) {
        clearScreen();
        drawHeader("TEAM SELECTION (" + to_string(selected) + "/" + to_string(numTeams) + ")");
        for (int i = 0; i < MAX_COUNTRIES; i++) {
            bool used = false;
            for (int j = 0; j < selected; j++) {
                if (strcmp(e.teams[j], countries[i].c_str()) == 0) {
                    used = true;
                    break;
                }
            }
            if (!used) {
                printBorderRow(to_string(i + 1) + ". " + countries[i]);
            }
        }
        drawFooter();

        drawHeader("SELECTION METHOD");
        printBorderRow("1. Pick by number from list");
        printBorderRow("2. Search by name (Binary Search)");
        drawFooter();
        cout << "Choice: ";
        int method;
        cin >> method;

        if (method == 1) {
            cout << "Enter country number (1-" << MAX_COUNTRIES << "): ";
            int idx;
            cin >> idx;
            idx--;
            if (idx < 0 || idx >= MAX_COUNTRIES) {
                cout << "Invalid number.\n";
                continue;
            }
            bool duplicate = false;
            for (int j = 0; j < selected; j++) {
                if (strcmp(e.teams[j], countries[idx].c_str()) == 0) {
                    duplicate = true;
                    break;
                }
            }
            if (duplicate) {
                cout << "Already selected. Pick another.\n";
                continue;
            }
            strncpy(e.teams[selected], countries[idx].c_str(), 50);
            selected++;
            cout << "Added " << countries[idx] << ".\n";
        } else if (method == 2) {
            cout << "Enter country name or prefix to search: ";
            char searchName[50];
            cin.ignore(1000, '\n');
            cin.getline(searchName, 50);
            int results[10];
            int resultCount = 0;
            int comps = 0;
            int firstIdx = searchCountriesByPrefix(searchName, results, resultCount, comps);
            if (firstIdx == -1) {
                cout << "No countries found with prefix '" << searchName << "' after " << comps << " comparisons.\n";
                continue;
            }
            cout << "Found " << resultCount << " country(ies) after " << comps << " comparisons:\n";
            drawHeader("SEARCH RESULTS");
            for (int r = 0; r < resultCount; r++) {
                printBorderRow(to_string(r + 1) + ". " + countries[results[r]]);
            }
            drawFooter();
            cout << "Select country (1-" << resultCount << "): ";
            int pick;
            cin >> pick;
            if (pick < 1 || pick > resultCount) { cout << "Invalid.\n"; continue; }
            int idx = results[pick - 1];
            bool duplicate = false;
            for (int j = 0; j < selected; j++) {
                if (strcmp(e.teams[j], countries[idx].c_str()) == 0) {
                    duplicate = true;
                    break;
                }
            }
            if (duplicate) {
                cout << "Already selected. Pick another.\n";
                continue;
            }
            strncpy(e.teams[selected], countries[idx].c_str(), 50);
            selected++;
            cout << "Added " << countries[idx] << ".\n";
        }
    }

    e.teamCount = numTeams;
}

void configureEvents() {
    if (activeEventCount >= MAX_EVENTS) {
        cout << "\nMaximum event capacity reached for today.\n";
        waitForEnter();
        return;
    }

    clearScreen();
    drawHeader("EVENT CONFIGURATION");

    Event newEvent;
    newEvent.eventID = 101 + totalEventsCreated;
    newEvent.teamCount = 0;

    drawHeader("SETUP MODE");
    printBorderRow("1. Manual Setup");
    printBorderRow("2. Auto Setup");
    drawFooter();
    cout << "Choice: ";
    int modeChoice;
    cin >> modeChoice;

    if (modeChoice == 1) {
        cout << "Enter Event Name: ";
        cin.ignore(1000, '\n');
        cin.getline(newEvent.eventName, 100);
    } else {
        string randomName = countries[rand() % MAX_COUNTRIES] + " Championship Cup";
        strncpy(newEvent.eventName, randomName.c_str(), sizeof(newEvent.eventName));
    }

    clearScreen();
    drawHeader("TOURNAMENT FORMAT");
    printBorderRow("1. Round Robin");
    printBorderRow("2. Winner & Losers Bracket (Two Lose Out)");
    printBorderRow("3. Single Elimination (One Lose Out)");
    drawFooter();
    cout << "Choice: ";
    int fmtChoice;
    cin >> fmtChoice;

    if (fmtChoice == 1) strncpy(newEvent.format, "Round Robin", 50);
    else if (fmtChoice == 2) strncpy(newEvent.format, "Winner & Losers Bracket", 50);
    else strncpy(newEvent.format, "Single Elimination", 50);

    int maxMatchesPerEvent = (fmtChoice == 1) ? MAX_MATCHES : 5;

    cout << "\nEnter number of teams (2-10): ";
    int numTeams;
    cin >> numTeams;
    if (numTeams < 2 || numTeams > 10) {
        numTeams = 10;
        cout << "Adjusted to 10 teams.\n";
    }
    if (fmtChoice != 1 && numTeams % 2 != 0) {
        numTeams--;
        cout << "Bracket formats require an even number. Adjusted to " << numTeams << " teams.\n";
    }

    if (fmtChoice == 1) {
        clearScreen();
        drawHeader("TEAM SELECTION MODE");
        printBorderRow("1. Manual Selection");
        printBorderRow("2. Auto Selection");
        drawFooter();
        cout << "Choice: ";
        int teamChoice;
        cin >> teamChoice;

        if (teamChoice == 1) {
            selectTeamsManual(newEvent, numTeams);
        } else {
            newEvent.teamCount = numTeams;
            for (int i = 0; i < numTeams; i++) {
                string t = countries[rand() % MAX_COUNTRIES];
                bool duplicate = false;
                for (int j = 0; j < i; j++) {
                    if (strcmp(newEvent.teams[j], t.c_str()) == 0) {
                        duplicate = true;
                        i--;
                        break;
                    }
                }
                if (!duplicate) {
                    strncpy(newEvent.teams[i], t.c_str(), 50);
                }
            }
        }

        int matchIdx = 0;
        for (int i = 0; i < newEvent.teamCount; i++) {
            for (int j = i + 1; j < newEvent.teamCount; j++) {
                if (matchIdx >= MAX_MATCHES) break;
                newEvent.matches[matchIdx].matchID = 500 + totalMatchesGenerated + matchIdx;
                strncpy(newEvent.matches[matchIdx].team1, newEvent.teams[i], 50);
                strncpy(newEvent.matches[matchIdx].team2, newEvent.teams[j], 50);
                newEvent.matches[matchIdx].score1 = rand() % 5;
                newEvent.matches[matchIdx].score2 = rand() % 5;
                matchIdx++;
            }
            if (matchIdx >= MAX_MATCHES) break;
        }
        newEvent.matchCount = matchIdx;
    } else {
        int matchSlots = numTeams / 2;
        if (matchSlots > maxMatchesPerEvent) matchSlots = maxMatchesPerEvent;
        newEvent.matchCount = 0;

        for (int i = 0; i < matchSlots; i++) {
            newEvent.matches[i].matchID = 500 + totalMatchesGenerated + i;
            newEvent.matches[i].team1[0] = '\0';
            newEvent.matches[i].team2[0] = '\0';
        }

        bool allFilled = false;
        while (!allFilled) {
            clearScreen();
            drawHeader("MATCH ASSIGNMENT (" + to_string(matchSlots) + " slots)");
            for (int m = 0; m < matchSlots; m++) {
                string t1 = (strlen(newEvent.matches[m].team1) > 0) ? string(newEvent.matches[m].team1) : "_____";
                string t2 = (strlen(newEvent.matches[m].team2) > 0) ? string(newEvent.matches[m].team2) : "_____";
                printBorderRow("Match " + to_string(m + 1) + ": " + t1 + " vs " + t2);
            }
            drawFooter();

            bool allAssigned = true;
            for (int m = 0; m < matchSlots; m++) {
                if (newEvent.matches[m].team1[0] == '\0') allAssigned = false;
            }
            if (allAssigned) break;

            drawHeader("OPTIONS");
            for (int m = 0; m < matchSlots; m++) {
                if (newEvent.matches[m].team1[0] == '\0') {
                    printBorderRow(to_string(m + 1) + ". Assign Match " + to_string(m + 1));
                }
            }
            printBorderRow("A. Auto-fill all remaining matches");
            printBorderRow("D. Done");
            drawFooter();
            cout << "Choice: ";
            string pick;
            cin >> pick;

            if (pick == "A" || pick == "a") {
                for (int i = 0; i < matchSlots; i++) {
                    if (newEvent.matches[i].team1[0] == '\0') {
                        int c1 = rand() % MAX_COUNTRIES;
                        int c2 = rand() % MAX_COUNTRIES;
                        while (c1 == c2) c2 = rand() % MAX_COUNTRIES;
                        strncpy(newEvent.matches[i].team1, countries[c1].c_str(), 50);
                        strncpy(newEvent.matches[i].team2, countries[c2].c_str(), 50);
                        newEvent.matches[i].score1 = rand() % 5;
                        newEvent.matches[i].score2 = rand() % 5;
                    }
                }
                break;
            }
            if (pick == "D" || pick == "d") break;

            int matchNum = atoi(pick.c_str());
            if (matchNum < 1 || matchNum > matchSlots || newEvent.matches[matchNum - 1].team1[0] != '\0') {
                cout << "Invalid match number.\n";
                continue;
            }
            matchNum--;

            cout << "\nSelect Team A for Match " << (matchNum + 1) << ":\n";
            int idxA = -1;
            while (idxA == -1) {
                cout << "1. Pick by number from list\n";
                cout << "2. Search by name (Binary Search)\n";
                cout << "Choice: ";
                int m;
                cin >> m;
                if (m == 1) {
                    cout << "Enter team number (1-" << MAX_COUNTRIES << "): ";
                    int n;
                    cin >> n;
                    n--;
                    if (n < 0 || n >= MAX_COUNTRIES) { cout << "Invalid.\n"; continue; }
                    bool used = false;
                    for (int u = 0; u < matchSlots; u++) {
                        if (strcmp(newEvent.matches[u].team1, countries[n].c_str()) == 0) used = true;
                        if (strcmp(newEvent.matches[u].team2, countries[n].c_str()) == 0) used = true;
                    }
                    if (used) { cout << "Team already assigned to a match.\n"; continue; }
                    idxA = n;
                } else if (m == 2) {
                    char sn[50];
                    cin.ignore(1000, '\n');
                    cout << "Enter country name or prefix: ";
                    cin.getline(sn, 50);
                    int results[10], resCount = 0, comps = 0;
                    searchCountriesByPrefix(sn, results, resCount, comps);
                    if (resCount == 0) { cout << "No matches after " << comps << " comparisons.\n"; continue; }
                    cout << "Found " << resCount << " match(es) after " << comps << " comparisons.\n";
                    for (int r = 0; r < resCount; r++) {
                        printBorderRow(to_string(r + 1) + ". " + countries[results[r]]);
                    }
                    cout << "Select (1-" << resCount << "): ";
                    int pick;
                    cin >> pick;
                    if (pick < 1 || pick > resCount) { cout << "Invalid.\n"; continue; }
                    idxA = results[pick - 1];
                    bool used = false;
                    for (int u = 0; u < matchSlots; u++) {
                        if (strcmp(newEvent.matches[u].team1, countries[idxA].c_str()) == 0) used = true;
                        if (strcmp(newEvent.matches[u].team2, countries[idxA].c_str()) == 0) used = true;
                    }
                    if (used) { cout << "Team already assigned.\n"; idxA = -1; continue; }
                }
            }
            strncpy(newEvent.matches[matchNum].team1, countries[idxA].c_str(), 50);

            cout << "\nSelect Team B for Match " << (matchNum + 1) << ":\n";
            int idxB = -1;
            while (idxB == -1) {
                cout << "1. Pick by number from list\n";
                cout << "2. Search by name (Binary Search)\n";
                cout << "Choice: ";
                int m;
                cin >> m;
                if (m == 1) {
                    cout << "Enter team number (1-" << MAX_COUNTRIES << "): ";
                    int n;
                    cin >> n;
                    n--;
                    if (n < 0 || n >= MAX_COUNTRIES) { cout << "Invalid.\n"; continue; }
                    if (n == idxA) { cout << "Same as Team A. Pick another.\n"; continue; }
                    bool used = false;
                    for (int u = 0; u < matchSlots; u++) {
                        if (strcmp(newEvent.matches[u].team1, countries[n].c_str()) == 0) used = true;
                        if (strcmp(newEvent.matches[u].team2, countries[n].c_str()) == 0) used = true;
                    }
                    if (used) { cout << "Team already assigned.\n"; continue; }
                    idxB = n;
                } else if (m == 2) {
                    char sn[50];
                    cin.ignore(1000, '\n');
                    cout << "Enter country name or prefix: ";
                    cin.getline(sn, 50);
                    int results[10], resCount = 0, comps = 0;
                    searchCountriesByPrefix(sn, results, resCount, comps);
                    if (resCount == 0) { cout << "No matches after " << comps << " comparisons.\n"; continue; }
                    cout << "Found " << resCount << " match(es) after " << comps << " comparisons.\n";
                    for (int r = 0; r < resCount; r++) {
                        if (results[r] == idxA) continue;
                        printBorderRow(to_string(r + 1) + ". " + countries[results[r]]);
                    }
                    cout << "Select (1-" << resCount << "): ";
                    int pick;
                    cin >> pick;
                    if (pick < 1 || pick > resCount) { cout << "Invalid.\n"; continue; }
                    if (results[pick - 1] == idxA) { cout << "Same as Team A.\n"; continue; }
                    idxB = results[pick - 1];
                    bool used = false;
                    for (int u = 0; u < matchSlots; u++) {
                        if (strcmp(newEvent.matches[u].team1, countries[idxB].c_str()) == 0) used = true;
                        if (strcmp(newEvent.matches[u].team2, countries[idxB].c_str()) == 0) used = true;
                    }
                    if (used) { cout << "Team already assigned.\n"; idxB = -1; continue; }
                }
            }
            strncpy(newEvent.matches[matchNum].team2, countries[idxB].c_str(), 50);
            newEvent.matches[matchNum].score1 = rand() % 5;
            newEvent.matches[matchNum].score2 = rand() % 5;
        }

        newEvent.matchCount = matchSlots;
        for (int i = 0; i < matchSlots; i++) {
            if (newEvent.matches[i].team1[0] == '\0' || newEvent.matches[i].team2[0] == '\0') {
                int c1 = rand() % MAX_COUNTRIES;
                int c2 = rand() % MAX_COUNTRIES;
                while (c1 == c2) c2 = rand() % MAX_COUNTRIES;
                strncpy(newEvent.matches[i].team1, countries[c1].c_str(), 50);
                strncpy(newEvent.matches[i].team2, countries[c2].c_str(), 50);
                newEvent.matches[i].score1 = rand() % 5;
                newEvent.matches[i].score2 = rand() % 5;
            }
        }

        int ti = 0;
        for (int i = 0; i < matchSlots && ti < 10; i++) {
            bool dup1 = false, dup2 = false;
            for (int j = 0; j < ti; j++) {
                if (strcmp(newEvent.teams[j], newEvent.matches[i].team1) == 0) dup1 = true;
                if (strcmp(newEvent.teams[j], newEvent.matches[i].team2) == 0) dup2 = true;
            }
            if (!dup1) strncpy(newEvent.teams[ti++], newEvent.matches[i].team1, 50);
            if (!dup2 && ti < 10) strncpy(newEvent.teams[ti++], newEvent.matches[i].team2, 50);
        }
        newEvent.teamCount = ti;
    }

    activeEvents[activeEventCount++] = newEvent;
    totalEventsCreated++;
    totalMatchesGenerated += newEvent.matchCount;

    cout << "\nSuccessfully created event: " << newEvent.eventName << " with "
         << newEvent.matchCount << " generated matches.\n";
    waitForEnter();
}

void generateTicketPurchases() {
    clearScreen();
    if (activeEventCount == 0) {
        cout << "\nNo active events configured. Please configure events first.\n";
        waitForEnter();
        return;
    }

    int numToGenerate = genPurchasesMin + rand() % (genPurchasesMax - genPurchasesMin + 1);
    for (int i = 0; i < numToGenerate; i++) {
        if (purchaseCount >= MAX_PURCHASES) break;

        Purchase p;
        p.purchaseID = 1000 + purchaseCount + 1;
        p.ticketID = 8000 + purchaseCount + 1;

        string custName = firstNames[rand() % MAX_NAMES] + " " + lastNames[rand() % MAX_NAMES];
        strncpy(p.customerName, custName.c_str(), 50);

        Event selectedEvent = activeEvents[rand() % activeEventCount];
        strncpy(p.eventName, selectedEvent.eventName, 100);

        Match selectedMatch = selectedEvent.matches[rand() % selectedEvent.matchCount];
        string matchDesc = "Match ID " + to_string(selectedMatch.matchID) + ": " + string(selectedMatch.team1) + " vs " + string(selectedMatch.team2);
        strncpy(p.matchInfo, matchDesc.c_str(), 100);

        double variance = (rand() % ((int)(2 * genPriceVariance) + 1)) - genPriceVariance;
        p.amountPaid = genPricePerMatch + variance;
        if (p.amountPaid < 0) p.amountPaid = 0;

        p.purchaseDate.day = 1 + rand() % 28;
        p.purchaseDate.month = 1 + rand() % 12;
        p.purchaseDate.year = 2026;

        strncpy(p.refundStatus, "Purchased", 20);

        masterPurchases[purchaseCount++] = p;
        totalTicketsSold++;
        totalRevenue += p.amountPaid;
    }

    isSortedByID = false;
    cout << "\nSuccessfully simulated " << numToGenerate << " customer ticket purchases.\n";
    waitForEnter();
}

void generateRefundRequests() {
    clearScreen();
    if (purchaseCount == 0) {
        cout << "\nNo ticket purchases exist to generate refund requests.\n";
        waitForEnter();
        return;
    }

    int generated = 0;
    for (int attempt = 0; attempt < genRefundCount * 3 && generated < genRefundCount; attempt++) {
        int targetIdx = rand() % purchaseCount;
        if (strcmp(masterPurchases[targetIdx].refundStatus, "Refund Ongoing") == 0 ||
            strcmp(masterPurchases[targetIdx].refundStatus, "Refunded") == 0) continue;

        if (isQueueFull()) {
            cout << "\nRefund Queue is full!\n";
            break;
        }

        RefundRequest req;
        req.requestID = 2000 + totalRefundRequests + 1;

        const char* ongoingValidReasons[] = {
            "Match was rescheduled without prior notice",
            "Purchased the wrong seat category",
            "Medical emergency with valid documentation",
            "Duplicate charge detected on payment method",
            "Event date was changed after purchase"
        };

        const char* ongoingInvalidReasons[] = {
            "Customer simply changed their mind",
            "Did not enjoy the match outcome",
            "Forgot about the event date entirely",
            "Attempted to resell ticket at inflated price",
            "Claimed ticket was lost but electronic record exists"
        };

        int validCount = 5;
        int invalidCount = 5;
        int total = validCount + invalidCount;
        int reasonIdx = rand() % total;
        if (reasonIdx < validCount) {
            strncpy(req.reason, ongoingValidReasons[reasonIdx], 100);
            req.isValidReason = true;
        } else {
            strncpy(req.reason, ongoingInvalidReasons[reasonIdx - validCount], 100);
            req.isValidReason = false;
        }

        strncpy(masterPurchases[targetIdx].refundStatus, "Refund Ongoing", 20);
        req.purchaseRecord = masterPurchases[targetIdx];

        enqueueRefund(req);
        totalRefundRequests++;
        generated++;
    }
    if (generated > 0) {
        cout << "\nGenerated " << generated << " refund request(s) and added to the review queue.\n";
    } else {
        cout << "\nNo new refund requests generated (all purchases already have pending/processed status).\n";
    }
    waitForEnter();
}

void generatePurchasesAndRequests() {
    clearScreen();
    drawHeader("GENERATE PURCHASES & REQUESTS");
    printBorderRow("1. Generate Ticket Purchases Only");
    printBorderRow("2. Generate Refund Requests Only");
    printBorderRow("3. Generate Both (Purchases then Refunds)");
    printBorderRow("4. Back to Main Menu");
    drawFooter();

    int choice;
    cout << "\nChoice: ";
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nInvalid input.\n";
        waitForEnter();
        return;
    }
    cin.ignore(1000, '\n');

    if (choice == 1) {
        generateTicketPurchases();
    } else if (choice == 2) {
        generateRefundRequests();
    } else if (choice == 3) {
        generateTicketPurchases();
        generateRefundRequests();
    }
}

void reviewRefundRequests() {
    clearScreen();
    if (isQueueEmpty()) {
        cout << "\nNo pending refund requests in queue.\n";
        waitForEnter();
        return;
    }

    RefundRequest req = dequeueRefund();

    drawHeader("REFUND REQUEST REVIEW");

    printInfoLine("Request ID", to_string(req.requestID));
    printInfoLine("Customer Name", string(req.purchaseRecord.customerName));
    printInfoSpacer();
    printInfoLine("Event Name", string(req.purchaseRecord.eventName));
    printInfoLine("Match Info", string(req.purchaseRecord.matchInfo));
    printInfoSpacer();
    printInfoLine("Amount Paid", formatAmount(req.purchaseRecord.amountPaid));
    printInfoLine("Refund Reason", string(req.reason));

    drawFooter();

    drawHeader("DECISION");
    printBorderRow("1. Approve Refund");
    printBorderRow("2. Reject Refund");
    drawFooter();
    cout << "Choice: ";
    int choice;
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nInvalid input.\n";
        waitForEnter();
        return;
    }

    bool approved = (choice == 1);

    if (approved == req.isValidReason) {
        organizerReputation += 3;
        if (organizerReputation > 100) organizerReputation = 100;
        cout << "\nCorrect decision! Organizer Reputation increased (+3).\n";
    } else {
        organizerReputation -= 5;
        if (organizerReputation < 0) organizerReputation = 0;
        cout << "\nIncorrect decision! Organizer Reputation decreased (-5).\n";
    }

    if (organizerReputation > highestReputation) highestReputation = organizerReputation;
    if (organizerReputation < lowestReputation) lowestReputation = organizerReputation;

    int idx = binarySearchPurchaseByID(req.purchaseRecord.purchaseID);
    if (idx != -1) {
        if (approved) {
            strncpy(masterPurchases[idx].refundStatus, "Refunded", 20);
            approvedRefunds++;
        } else {
            strncpy(masterPurchases[idx].refundStatus, "Refunded", 20);
            rejectedRefunds++;
        }
    }
    waitForEnter();
}

void selectionSortPurchasesByID() {
    for (int i = 0; i < purchaseCount - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < purchaseCount; j++) {
            if (masterPurchases[j].purchaseID < masterPurchases[minIdx].purchaseID) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            Purchase temp = masterPurchases[i];
            masterPurchases[i] = masterPurchases[minIdx];
            masterPurchases[minIdx] = temp;
        }
    }
    isSortedByID = true;
}

int binarySearchPurchaseByID(int targetID) {
    if (!isSortedByID) {
        selectionSortPurchasesByID();
    }

    int low = 0;
    int high = purchaseCount - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (masterPurchases[mid].purchaseID == targetID) {
            return mid;
        }
        if (masterPurchases[mid].purchaseID < targetID) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

void searchPurchaseRecord() {
    clearScreen();
    if (purchaseCount == 0) {
        cout << "\nNo purchases recorded yet.\n";
        waitForEnter();
        return;
    }

    cout << "\nEnter Purchase ID to search: ";
    int searchID;
    if (!(cin >> searchID)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nInvalid input.\n";
        waitForEnter();
        return;
    }

    int resultIdx = binarySearchPurchaseByID(searchID);

    if (resultIdx != -1) {
        Purchase p = masterPurchases[resultIdx];
        drawHeader("PURCHASE RECORD FOUND");

        printInfoLine("Purchase ID", to_string(p.purchaseID));
        printInfoLine("Ticket ID", to_string(p.ticketID));
        printInfoSpacer();
        printInfoLine("Customer Name", string(p.customerName));
        printInfoSpacer();
        printInfoLine("Event Name", string(p.eventName));
        printInfoLine("Match Info", string(p.matchInfo));
        printInfoSpacer();
        printInfoLine("Amount Paid", formatAmount(p.amountPaid));

        string date =
            to_string(p.purchaseDate.year) + "-" +
            (p.purchaseDate.month < 10 ? "0" : "") + to_string(p.purchaseDate.month) + "-" +
            (p.purchaseDate.day < 10 ? "0" : "") + to_string(p.purchaseDate.day);

        printInfoLine("Date", date);
        printInfoSpacer();
        printInfoLine("Status", string(p.refundStatus));

        drawFooter();
    } else {
        cout << "\nPurchase ID " << searchID << " not found in database.\n";
    }
    waitForEnter();
}

void viewStatistics() {
    clearScreen();
    drawHeader("SIMULATION STATISTICS OVERVIEW");

    printBorderRow("Days Played: " + to_string(currentDay));
    printBorderRow("Total Events Created: " + to_string(totalEventsCreated));
    printBorderRow("Total Matches Generated: " + to_string(totalMatchesGenerated));
    printBorderRow("Total Tickets Sold: " + to_string(totalTicketsSold));
    printBorderRow("Total Revenue: " + formatAmount(totalRevenue));
    printBorderRow("Total Refund Requests: " + to_string(totalRefundRequests));
    printBorderRow("Approved Refunds: " + to_string(approvedRefunds));
    printBorderRow("Rejected Refunds: " + to_string(rejectedRefunds));
    printBorderRow("Current Reputation: " + to_string(organizerReputation));
    printBorderRow("Highest Reputation: " + to_string(highestReputation));
    printBorderRow("Lowest Reputation: " + to_string(lowestReputation));

    drawFooter();
    waitForEnter();
}

void endDaySummary() {
    clearScreen();

    int refundPenalty = 0;
    if (queueSize > 0) {
        refundPenalty = queueSize * 3;
        organizerReputation -= refundPenalty;
        if (organizerReputation < 0) organizerReputation = 0;
    }

    drawHeader("DAILY SUMMARY REPORT");

    printBorderRow("Completed Day: " + to_string(currentDay));
    printBorderRow("Tickets Sold Today: " + to_string(totalTicketsSold));
    printBorderRow("Revenue Earned: " + formatAmount(totalRevenue));
    if (refundPenalty > 0) {
        printBorderRow("Unmanaged Refunds: " + to_string(queueSize));
        printBorderRow("Reputation Penalty: -" + to_string(refundPenalty));
    }
    printBorderRow("Organizer Reputation: " + to_string(organizerReputation));

    drawFooter();
    waitForEnter();

    if (activeEventCount > 0) {
        cout << "\nWould you like to view team standings? (1 = Yes, 2 = No): ";
        int viewChoice = 2;
        if (!(cin >> viewChoice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nInvalid input. Skipping standings.\n";
            waitForEnter();
        }
        if (viewChoice == 1) {
            clearScreen();
            drawHeader("SELECT EVENT");
            for (int i = 0; i < activeEventCount; i++) {
                string eName = string(activeEvents[i].eventName) + " (" + string(activeEvents[i].format) + ")";
                printBorderRow(to_string(i + 1) + ". " + truncateForHeader(eName, 54));
            }
            drawFooter();
            cout << "Choice: ";
            int evChoice = 1;
            if (!(cin >> evChoice)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "\nInvalid input. Returning to menu.\n";
                waitForEnter();
            }
            if (evChoice >= 1 && evChoice <= activeEventCount) {
                clearScreen();
                drawHeader("TEAM STANDINGS LEADERBOARD");
                printBorderRow(truncateForHeader(string(activeEvents[evChoice - 1].eventName) +
                               " (" + string(activeEvents[evChoice - 1].format) + ")", 57));
                drawFooter();
                computeStandingsForEvent(activeEvents[evChoice - 1]);
                if (standingsRoot != NULL) {
                    inOrderTraversal(standingsRoot);
                }
                drawFooter();
                waitForEnter();
            }
        }
    }

    for (int i = 0; i < purchaseCount; i++) {
        if (strcmp(masterPurchases[i].refundStatus, "Purchased") == 0) {
            strncpy(masterPurchases[i].refundStatus, "Match Finished", 20);
        }
    }

    for (int i = 0; i < activeEventCount && completedEventCount < MAX_EVENTS * 10; i++) {
        completedEvents[completedEventCount++] = activeEvents[i];
    }

    while (!isQueueEmpty() && expiredRefundCount < MAX_QUEUE) {
        expiredRefunds[expiredRefundCount++] = dequeueRefund();
    }

    currentDay++;
    activeEventCount = 0;
    cout << "\nAdvanced to Day " << currentDay << ".\n";
}

void viewEventsAndStandings() {
    clearScreen();
    int totalEvents = activeEventCount + completedEventCount;
    if (totalEvents == 0) {
        cout << "\nNo events found. Create an event first.\n";
        waitForEnter();
        return;
    }
    for (int e = 0; e < totalEvents; e++) {
        Event &ev = (e < activeEventCount) ? activeEvents[e] : completedEvents[e - activeEventCount];
        computeStandingsForEvent(ev);
        if (standingsRoot == NULL) continue;
        string hdr = "TEAM STANDINGS - " + string(ev.eventName) + " (" + string(ev.format) + ")";
        drawHeader(truncateForHeader(hdr, 55));
        if (standingsRoot != NULL) inOrderTraversal(standingsRoot);
        drawFooter();
    }
    waitForEnter();
}

void optionsAndExtraSettings() {
    int choice = 0;
    while (choice != 5) {
        clearScreen();
        drawHeader("OPTIONS & EXTRA SETTINGS");
        printBorderRow("1. View Detailed Statistics");
        printBorderRow("2. View Events & Team Standings (BST)");
        printBorderRow("3. View System Tutorial");
        printBorderRow("4. Configure Generation Settings");
        printBorderRow("5. Back to Main Menu");
        drawFooter();

        cout << "\nChoice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nInvalid input.\n";
            waitForEnter();
            continue;
        }
        cin.ignore(1000, '\n');

        switch (choice) {
            case 1:
                viewStatistics();
                break;
            case 2:
                viewEventsAndStandings();
                break;
            case 3:
                runTutorial();
                break;
            case 4:
                configureGenerationSettings();
                break;
        }
    }
}

void configureGenerationSettings() {
    int choice = 0;
    while (choice != 7) {
        clearScreen();
        drawHeader("GENERATION SETTINGS");
        printBorderRow("1. Min Purchases Per Batch: " + to_string(genPurchasesMin));
        printBorderRow("2. Max Purchases Per Batch: " + to_string(genPurchasesMax));
        printBorderRow("3. Base Ticket Price Per Match: " + formatAmount(genPricePerMatch));
        printBorderRow("4. Price Variance (+/-): " + formatAmount(genPriceVariance));
        printBorderRow("5. Refunds Per Generation: " + to_string(genRefundCount));
        printBorderRow("6. Reset to Defaults");
        printBorderRow("7. Back");
        drawFooter();

        cout << "\nChoice: ";
        cin >> choice;
        cin.ignore(1000, '\n');
        cin.clear();

        if (choice == 1) {
            cout << "Enter min purchases (1-20): ";
            int val;
            cin >> val;
            if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid.\n"; continue; }
            if (val >= 1 && val <= 20 && val <= genPurchasesMax) {
                genPurchasesMin = val;
                cout << "Updated.\n";
            } else {
                cout << "Invalid. Must be 1-20 and <= max.\n";
            }
        } else if (choice == 2) {
            cout << "Enter max purchases (1-50): ";
            int val;
            cin >> val;
            if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid.\n"; continue; }
            if (val >= 1 && val <= 50 && val >= genPurchasesMin) {
                genPurchasesMax = val;
                cout << "Updated.\n";
            } else {
                cout << "Invalid. Must be 1-50 and >= min.\n";
            }
        } else if (choice == 3) {
            cout << "Enter base price per match ($): ";
            double val;
            cin >> val;
            if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid.\n"; continue; }
            if (val >= 1.0 && val <= 1000.0) {
                genPricePerMatch = val;
                cout << "Updated.\n";
            } else {
                cout << "Invalid. Must be $1-$1000.\n";
            }
        } else if (choice == 4) {
            cout << "Enter price variance ($0-$100): ";
            double val;
            cin >> val;
            if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid.\n"; continue; }
            if (val >= 0 && val <= 100 && val <= genPricePerMatch) {
                genPriceVariance = val;
                cout << "Updated.\n";
            } else {
                cout << "Invalid. Must be $0-$100 and <= base price.\n";
            }
        } else if (choice == 5) {
            cout << "Enter refunds per generation (1-10): ";
            int val;
            cin >> val;
            if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid.\n"; continue; }
            if (val >= 1 && val <= 10) {
                genRefundCount = val;
                cout << "Updated.\n";
            } else {
                cout << "Invalid. Must be 1-10.\n";
            }
        } else if (choice == 6) {
            genPurchasesMin = 3;
            genPurchasesMax = 7;
            genPricePerMatch = 100.0;
            genPriceVariance = 20.0;
            genRefundCount = 1;
            cout << "Settings reset to defaults.\n";
        }
    }
}

void showAllPurchases() {
    clearScreen();
    if (purchaseCount == 0) {
        cout << "\nNo purchases recorded yet.\n";
        waitForEnter();
        return;
    }

    drawHeader("PURCHASE HISTORY (Sorted via Selection Sort) (" + to_string(purchaseCount) + " records)");
    for (int i = 0; i < purchaseCount; i++) {
        Purchase p = masterPurchases[i];
        string status = string(p.refundStatus);
        printInfoLine("Purchase #" + to_string(p.purchaseID), string(p.customerName) + " | " + formatAmount(p.amountPaid));
        printInfoLine("Event", string(p.eventName));
        printInfoLine(status, "");
        if (i < purchaseCount - 1) {
            printBorderRow(string(77, '-'));
        }
    }
    drawFooter();
    waitForEnter();
}

void managePurchases() {
    int choice = 0;
    while (choice != 3) {
        clearScreen();
        drawHeader("MANAGE PURCHASES");
        printBorderRow("1. Show All Purchase History");
        printBorderRow("2. Search Purchase by ID (Binary Search)");
        printBorderRow("3. Back to Main Menu");
        drawFooter();

        cout << "\nChoice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nInvalid input.\n";
            waitForEnter();
            continue;
        }
        cin.ignore(1000, '\n');

        if (choice == 1) {
            showAllPurchases();
        } else if (choice == 2) {
            searchPurchaseRecord();
        }
    }
}

void viewRefundQueue() {
    clearScreen();
    if (isQueueEmpty()) {
        cout << "\nNo pending refund requests in queue.\n";
        waitForEnter();
        return;
    }

    drawHeader("PENDING REFUND QUEUE (" + to_string(queueSize) + " requests)");
    int idx = queueFront;
    for (int i = 0; i < queueSize; i++) {
        RefundRequest req = refundQueueArray[idx];
        printInfoLine("Request #" + to_string(req.requestID), string(req.purchaseRecord.customerName));
        printInfoLine("Amount", formatAmount(req.purchaseRecord.amountPaid));
        printInfoLine("Reason", string(req.reason));
        printInfoLine("Match", string(req.purchaseRecord.matchInfo));
        if (i < queueSize - 1) {
            printBorderRow(string(77, '-'));
        }
        idx = (idx + 1) % MAX_QUEUE;
    }
    drawFooter();
    waitForEnter();
}

void viewRefundHistory() {
    clearScreen();
    bool found = false;
    drawHeader("REFUND HISTORY (Sorted via Selection Sort)");
    int entryCount = 0;
    for (int i = 0; i < purchaseCount; i++) {
        if (strcmp(masterPurchases[i].refundStatus, "Refunded") == 0) {
            if (entryCount > 0) printBorderRow(string(77, '-'));
            Purchase p = masterPurchases[i];
            printInfoLine("Purchase #" + to_string(p.purchaseID), string(p.customerName));
            printInfoLine("Amount", formatAmount(p.amountPaid));
            printInfoLine("Status", string(p.refundStatus));
            found = true;
            entryCount++;
        }
    }
    if (!found) {
        printBorderRow("No processed refunds found.");
    }
    drawFooter();
    waitForEnter();
}

void viewExpiredRefunds() {
    clearScreen();
    if (expiredRefundCount == 0) {
        cout << "\nNo expired refunds.\n";
        waitForEnter();
        return;
    }

    drawHeader("EXPIRED REFUNDS (" + to_string(expiredRefundCount) + ")");
    for (int i = 0; i < expiredRefundCount; i++) {
        if (i > 0) printBorderRow(string(77, '-'));
        RefundRequest req = expiredRefunds[i];
        printInfoLine("Request #" + to_string(req.requestID), string(req.purchaseRecord.customerName));
        printInfoLine("Amount", formatAmount(req.purchaseRecord.amountPaid));
        printInfoLine("Reason", string(req.reason));
        printInfoLine("Status", "Expired (Day Ended)");
    }
    drawFooter();
    waitForEnter();
}

void searchRefundByID() {
    clearScreen();
    cout << "\nEnter Refund Request ID to search: ";
    int searchID;
    if (!(cin >> searchID)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nInvalid input.\n";
        waitForEnter();
        return;
    }

    bool found = false;

    int idx = queueFront;
    for (int i = 0; i < queueSize; i++) {
        if (refundQueueArray[idx].requestID == searchID) {
            RefundRequest req = refundQueueArray[idx];
            drawHeader("REFUND FOUND (Pending Queue)");
            printInfoLine("Request ID", to_string(req.requestID));
            printInfoLine("Customer", string(req.purchaseRecord.customerName));
            printInfoLine("Event", string(req.purchaseRecord.eventName));
            printInfoLine("Amount", formatAmount(req.purchaseRecord.amountPaid));
            printInfoLine("Reason", string(req.reason));
            printInfoLine("Status", string(req.purchaseRecord.refundStatus));
            drawFooter();
            found = true;
            break;
        }
        idx = (idx + 1) % MAX_QUEUE;
    }

    if (!found) {
        for (int i = 0; i < expiredRefundCount; i++) {
            if (expiredRefunds[i].requestID == searchID) {
                RefundRequest req = expiredRefunds[i];
                drawHeader("REFUND FOUND (Expired)");
                printInfoLine("Request ID", to_string(req.requestID));
                printInfoLine("Customer", string(req.purchaseRecord.customerName));
                printInfoLine("Event", string(req.purchaseRecord.eventName));
                printInfoLine("Amount", formatAmount(req.purchaseRecord.amountPaid));
                printInfoLine("Reason", string(req.reason));
                printInfoLine("Status", "Expired (Day Ended)");
                drawFooter();
                found = true;
                break;
            }
        }
    }

    if (!found) {
        for (int i = 0; i < purchaseCount; i++) {
            if (strcmp(masterPurchases[i].refundStatus, "Refunded") == 0 &&
                masterPurchases[i].purchaseID == searchID) {
                Purchase p = masterPurchases[i];
                drawHeader("REFUND FOUND (Processed History)");
                printInfoLine("Purchase ID", to_string(p.purchaseID));
                printInfoLine("Customer", string(p.customerName));
                printInfoLine("Event", string(p.eventName));
                printInfoLine("Amount", formatAmount(p.amountPaid));
                printInfoLine("Status", string(p.refundStatus));
                drawFooter();
                found = true;
                break;
            }
        }
    }

    if (!found) {
        cout << "\nRefund Request ID " << searchID << " not found.\n";
    }
    waitForEnter();
}

void manageRefunds() {
    int choice = 0;
    while (choice != 6) {
        clearScreen();
        drawHeader("MANAGE REFUNDS");
        printBorderRow("1. Review Refund Request (FIFO Queue)");
        printBorderRow("2. View Refund Queue (Pending)");
        printBorderRow("3. View Refund History (Processed)");
        printBorderRow("4. View Expired Refunds");
        printBorderRow("5. Search Refund by ID");
        printBorderRow("6. Back to Main Menu");
        drawFooter();

        cout << "\nChoice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nInvalid input.\n";
            waitForEnter();
            continue;
        }
        cin.ignore(1000, '\n');

        if (choice == 1) {
            reviewRefundRequests();
        } else if (choice == 2) {
            viewRefundQueue();
        } else if (choice == 3) {
            viewRefundHistory();
        } else if (choice == 4) {
            viewExpiredRefunds();
        } else if (choice == 5) {
            searchRefundByID();
        }
    }
}

void enqueueRefund(RefundRequest req) {
    if (isQueueFull()) return;
    queueRear = (queueRear + 1) % MAX_QUEUE;
    refundQueueArray[queueRear] = req;
    queueSize++;
}

RefundRequest dequeueRefund() {
    RefundRequest emptyReq;
    memset(&emptyReq, 0, sizeof(RefundRequest));
    if (isQueueEmpty()) return emptyReq;

    RefundRequest req = refundQueueArray[queueFront];
    queueFront = (queueFront + 1) % MAX_QUEUE;
    queueSize--;
    return req;
}

bool isQueueFull() {
    return queueSize >= MAX_QUEUE;
}

bool isQueueEmpty() {
    return queueSize == 0;
}

BSTNode* createBSTNode(int key, const char* name, int w, int l, int d, int pts, int gf, int ga) {
    BSTNode* newNode = new BSTNode();
    newNode->teamKey = key;
    strncpy(newNode->teamName, name, sizeof(newNode->teamName));
    newNode->wins = w;
    newNode->losses = l;
    newNode->draws = d;
    newNode->points = pts;
    newNode->goalsFor = gf;
    newNode->goalsAgainst = ga;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

BSTNode* insertBSTNode(BSTNode* root, int key, const char* name, int w, int l, int d, int pts, int gf, int ga) {
    if (root == NULL) {
        return createBSTNode(key, name, w, l, d, pts, gf, ga);
    }
    if (key < root->teamKey) {
        root->left = insertBSTNode(root->left, key, name, w, l, d, pts, gf, ga);
    } else if (key > root->teamKey) {
        root->right = insertBSTNode(root->right, key, name, w, l, d, pts, gf, ga);
    }
    return root;
}

void inOrderTraversal(BSTNode* root) {
    if (root == NULL) return;
    inOrderTraversal(root->right);
    string record = string(root->teamName) + "  W:" + to_string(root->wins) +
                    " L:" + to_string(root->losses) + " D:" + to_string(root->draws) +
                    "  Pts:" + to_string(root->points) +
                    "  GF:" + to_string(root->goalsFor) + " GA:" + to_string(root->goalsAgainst);
    printBorderRow(record);
    inOrderTraversal(root->left);
}

void freeBST(BSTNode* root) {
    if (root == NULL) return;
    freeBST(root->left);
    freeBST(root->right);
    delete root;
}

void computeStandingsForEvent(Event &e) {
    freeBST(standingsRoot);
    standingsRoot = NULL;

    if (e.teamCount == 0 || e.matchCount == 0) return;

    for (int t = 0; t < e.teamCount; t++) {
        int wins = 0, losses = 0, draws = 0;
        int gf = 0, ga = 0;

        for (int m = 0; m < e.matchCount; m++) {
            bool isTeam1 = (strcmp(e.matches[m].team1, e.teams[t]) == 0);
            bool isTeam2 = (strcmp(e.matches[m].team2, e.teams[t]) == 0);
            if (!isTeam1 && !isTeam2) continue;

            int teamScore = isTeam1 ? e.matches[m].score1 : e.matches[m].score2;
            int oppScore = isTeam1 ? e.matches[m].score2 : e.matches[m].score1;

            gf += teamScore;
            ga += oppScore;

            if (teamScore > oppScore) wins++;
            else if (teamScore < oppScore) losses++;
            else draws++;
        }

        int points = wins * 3 + draws;
        int key = points * 1000 + t;
        standingsRoot = insertBSTNode(standingsRoot, key, e.teams[t], wins, losses, draws, points, gf, ga);
    }
}

void saveGame(const char* filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "\nError: Could not open " << filename << " for writing.\n";
        return;
    }

    outFile << currentDay << "|" << organizerReputation << "|" << totalEventsCreated << "|"
            << totalMatchesGenerated << "|" << totalTicketsSold << "|" << totalRevenue << "|"
            << totalRefundRequests << "|" << approvedRefunds << "|" << rejectedRefunds << "|"
            << highestReputation << "|" << lowestReputation << "|"
            << genPurchasesMin << "|" << genPurchasesMax << "|"
            << genPricePerMatch << "|" << genPriceVariance << "|"
            << genRefundCount << "|" << saveFileCounter << "|"
            << tutorialShown << "\n";

    outFile << purchaseCount << "\n";
    for (int i = 0; i < purchaseCount; i++) {
        outFile << masterPurchases[i].purchaseID << "|"
                << masterPurchases[i].ticketID << "|"
                << masterPurchases[i].amountPaid << "|"
                << masterPurchases[i].purchaseDate.day << "|"
                << masterPurchases[i].purchaseDate.month << "|"
                << masterPurchases[i].purchaseDate.year << "|"
                << masterPurchases[i].refundStatus << "|"
                << masterPurchases[i].customerName << "|"
                << masterPurchases[i].eventName << "|"
                << masterPurchases[i].matchInfo << "\n";
    }

    outFile << activeEventCount << "\n";
    for (int i = 0; i < activeEventCount; i++) {
        outFile << activeEvents[i].eventID << "|" << activeEvents[i].eventName << "|"
                << activeEvents[i].format << "|" << activeEvents[i].matchCount << "|"
                << activeEvents[i].teamCount << "\n";
        for (int t = 0; t < activeEvents[i].teamCount; t++) {
            if (t > 0) outFile << "|";
            outFile << activeEvents[i].teams[t];
        }
        outFile << "\n";
        for (int m = 0; m < activeEvents[i].matchCount; m++) {
            outFile << activeEvents[i].matches[m].matchID << "|"
                    << activeEvents[i].matches[m].team1 << "|"
                    << activeEvents[i].matches[m].team2 << "|"
                    << activeEvents[i].matches[m].score1 << "|"
                    << activeEvents[i].matches[m].score2 << "\n";
        }
    }

    int idx = queueFront;
    outFile << queueSize << "\n";
    for (int i = 0; i < queueSize; i++) {
        RefundRequest req = refundQueueArray[idx];
        outFile << req.requestID << "|"
                << req.purchaseRecord.purchaseID << "|"
                << req.purchaseRecord.ticketID << "|"
                << req.purchaseRecord.amountPaid << "|"
                << req.purchaseRecord.purchaseDate.day << "|"
                << req.purchaseRecord.purchaseDate.month << "|"
                << req.purchaseRecord.purchaseDate.year << "|"
                << req.purchaseRecord.refundStatus << "|"
                << req.purchaseRecord.customerName << "|"
                << req.purchaseRecord.eventName << "|"
                << req.purchaseRecord.matchInfo << "|"
                << req.reason << "|"
                << req.isValidReason << "\n";
        idx = (idx + 1) % MAX_QUEUE;
    }

    outFile << completedEventCount << "\n";
    for (int i = 0; i < completedEventCount; i++) {
        outFile << completedEvents[i].eventID << "|" << completedEvents[i].eventName << "|"
                << completedEvents[i].format << "|" << completedEvents[i].matchCount << "|"
                << completedEvents[i].teamCount << "\n";
        for (int t = 0; t < completedEvents[i].teamCount; t++) {
            if (t > 0) outFile << "|";
            outFile << completedEvents[i].teams[t];
        }
        outFile << "\n";
        for (int m = 0; m < completedEvents[i].matchCount; m++) {
            outFile << completedEvents[i].matches[m].matchID << "|"
                    << completedEvents[i].matches[m].team1 << "|"
                    << completedEvents[i].matches[m].team2 << "|"
                    << completedEvents[i].matches[m].score1 << "|"
                    << completedEvents[i].matches[m].score2 << "\n";
        }
    }

    outFile << expiredRefundCount << "\n";
    for (int i = 0; i < expiredRefundCount; i++) {
        outFile << expiredRefunds[i].requestID << "|"
                << expiredRefunds[i].purchaseRecord.purchaseID << "|"
                << expiredRefunds[i].purchaseRecord.ticketID << "|"
                << expiredRefunds[i].purchaseRecord.amountPaid << "|"
                << expiredRefunds[i].purchaseRecord.purchaseDate.day << "|"
                << expiredRefunds[i].purchaseRecord.purchaseDate.month << "|"
                << expiredRefunds[i].purchaseRecord.purchaseDate.year << "|"
                << expiredRefunds[i].purchaseRecord.refundStatus << "|"
                << expiredRefunds[i].purchaseRecord.customerName << "|"
                << expiredRefunds[i].purchaseRecord.eventName << "|"
                << expiredRefunds[i].purchaseRecord.matchInfo << "|"
                << expiredRefunds[i].reason << "|"
                << expiredRefunds[i].isValidReason << "\n";
    }

    outFile.close();
    cout << "\nProgress saved to " << filename << "\n";
}

void loadGame(const char* filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) return;

    string buf;
    getline(inFile, buf, '|'); currentDay = atoi(buf.c_str());
    getline(inFile, buf, '|'); organizerReputation = atoi(buf.c_str());
    getline(inFile, buf, '|'); totalEventsCreated = atoi(buf.c_str());
    getline(inFile, buf, '|'); totalMatchesGenerated = atoi(buf.c_str());
    getline(inFile, buf, '|'); totalTicketsSold = atoi(buf.c_str());
    getline(inFile, buf, '|'); totalRevenue = atof(buf.c_str());
    getline(inFile, buf, '|'); totalRefundRequests = atoi(buf.c_str());
    getline(inFile, buf, '|'); approvedRefunds = atoi(buf.c_str());
    getline(inFile, buf, '|'); rejectedRefunds = atoi(buf.c_str());
    getline(inFile, buf, '|'); highestReputation = atoi(buf.c_str());
    getline(inFile, buf, '|'); lowestReputation = atoi(buf.c_str());
    getline(inFile, buf, '|'); genPurchasesMin = atoi(buf.c_str());
    getline(inFile, buf, '|'); genPurchasesMax = atoi(buf.c_str());
    getline(inFile, buf, '|'); genPricePerMatch = atof(buf.c_str());
    getline(inFile, buf, '|'); genPriceVariance = atof(buf.c_str());
    getline(inFile, buf, '|'); genRefundCount = atoi(buf.c_str());
    getline(inFile, buf, '|'); saveFileCounter = atoi(buf.c_str());
    getline(inFile, buf);      tutorialShown = (buf == "1");

    getline(inFile, buf); purchaseCount = atoi(buf.c_str());
    for (int i = 0; i < purchaseCount; i++) {
        getline(inFile, buf, '|'); masterPurchases[i].purchaseID = atoi(buf.c_str());
        getline(inFile, buf, '|'); masterPurchases[i].ticketID = atoi(buf.c_str());
        getline(inFile, buf, '|'); masterPurchases[i].amountPaid = atof(buf.c_str());
        getline(inFile, buf, '|'); masterPurchases[i].purchaseDate.day = atoi(buf.c_str());
        getline(inFile, buf, '|'); masterPurchases[i].purchaseDate.month = atoi(buf.c_str());
        getline(inFile, buf, '|'); masterPurchases[i].purchaseDate.year = atoi(buf.c_str());
        getline(inFile, buf, '|'); strncpy(masterPurchases[i].refundStatus, buf.c_str(), 20);
        getline(inFile, buf, '|'); strncpy(masterPurchases[i].customerName, buf.c_str(), 50);
        getline(inFile, buf, '|'); strncpy(masterPurchases[i].eventName, buf.c_str(), 100);
        getline(inFile, buf);      strncpy(masterPurchases[i].matchInfo, buf.c_str(), 100);
    }

    getline(inFile, buf); activeEventCount = atoi(buf.c_str());
    for (int i = 0; i < activeEventCount; i++) {
        getline(inFile, buf, '|'); activeEvents[i].eventID = atoi(buf.c_str());
        getline(inFile, buf, '|'); strncpy(activeEvents[i].eventName, buf.c_str(), 100);
        getline(inFile, buf, '|'); strncpy(activeEvents[i].format, buf.c_str(), 50);
        getline(inFile, buf, '|'); activeEvents[i].matchCount = atoi(buf.c_str());
        getline(inFile, buf);      activeEvents[i].teamCount = atoi(buf.c_str());
        for (int t = 0; t < activeEvents[i].teamCount; t++) {
            if (t < activeEvents[i].teamCount - 1)
                getline(inFile, buf, '|');
            else
                getline(inFile, buf);
            strncpy(activeEvents[i].teams[t], buf.c_str(), 50);
        }
        for (int m = 0; m < activeEvents[i].matchCount; m++) {
            getline(inFile, buf, '|'); activeEvents[i].matches[m].matchID = atoi(buf.c_str());
            getline(inFile, buf, '|'); strncpy(activeEvents[i].matches[m].team1, buf.c_str(), 50);
            getline(inFile, buf, '|'); strncpy(activeEvents[i].matches[m].team2, buf.c_str(), 50);
            getline(inFile, buf, '|'); activeEvents[i].matches[m].score1 = atoi(buf.c_str());
            getline(inFile, buf);      activeEvents[i].matches[m].score2 = atoi(buf.c_str());
        }
    }

    queueSize = 0;
    getline(inFile, buf); int savedQueueSize = atoi(buf.c_str());
    for (int i = 0; i < savedQueueSize; i++) {
        RefundRequest req;
        getline(inFile, buf, '|'); req.requestID = atoi(buf.c_str());
        getline(inFile, buf, '|'); req.purchaseRecord.purchaseID = atoi(buf.c_str());
        getline(inFile, buf, '|'); req.purchaseRecord.ticketID = atoi(buf.c_str());
        getline(inFile, buf, '|'); req.purchaseRecord.amountPaid = atof(buf.c_str());
        getline(inFile, buf, '|'); req.purchaseRecord.purchaseDate.day = atoi(buf.c_str());
        getline(inFile, buf, '|'); req.purchaseRecord.purchaseDate.month = atoi(buf.c_str());
        getline(inFile, buf, '|'); req.purchaseRecord.purchaseDate.year = atoi(buf.c_str());
        getline(inFile, buf, '|'); strncpy(req.purchaseRecord.refundStatus, buf.c_str(), 20);
        getline(inFile, buf, '|'); strncpy(req.purchaseRecord.customerName, buf.c_str(), 50);
        getline(inFile, buf, '|'); strncpy(req.purchaseRecord.eventName, buf.c_str(), 100);
        getline(inFile, buf, '|'); strncpy(req.purchaseRecord.matchInfo, buf.c_str(), 100);
        getline(inFile, buf, '|'); strncpy(req.reason, buf.c_str(), 100);
        getline(inFile, buf);      req.isValidReason = (buf == "1");
        enqueueRefund(req);
    }

    completedEventCount = 0;
    expiredRefundCount = 0;

    getline(inFile, buf); int savedCompletedCount = atoi(buf.c_str());
    for (int i = 0; i < savedCompletedCount && i < MAX_EVENTS * 10; i++) {
        getline(inFile, buf, '|'); completedEvents[i].eventID = atoi(buf.c_str());
        getline(inFile, buf, '|'); strncpy(completedEvents[i].eventName, buf.c_str(), 100);
        getline(inFile, buf, '|'); strncpy(completedEvents[i].format, buf.c_str(), 50);
        getline(inFile, buf, '|'); completedEvents[i].matchCount = atoi(buf.c_str());
        getline(inFile, buf);      completedEvents[i].teamCount = atoi(buf.c_str());
        for (int t = 0; t < completedEvents[i].teamCount; t++) {
            if (t < completedEvents[i].teamCount - 1)
                getline(inFile, buf, '|');
            else
                getline(inFile, buf);
            strncpy(completedEvents[i].teams[t], buf.c_str(), 50);
        }
        for (int m = 0; m < completedEvents[i].matchCount; m++) {
            getline(inFile, buf, '|'); completedEvents[i].matches[m].matchID = atoi(buf.c_str());
            getline(inFile, buf, '|'); strncpy(completedEvents[i].matches[m].team1, buf.c_str(), 50);
            getline(inFile, buf, '|'); strncpy(completedEvents[i].matches[m].team2, buf.c_str(), 50);
            getline(inFile, buf, '|'); completedEvents[i].matches[m].score1 = atoi(buf.c_str());
            getline(inFile, buf);      completedEvents[i].matches[m].score2 = atoi(buf.c_str());
        }
        completedEventCount++;
    }

    getline(inFile, buf); int savedExpiredCount = atoi(buf.c_str());
    for (int i = 0; i < savedExpiredCount && i < MAX_QUEUE; i++) {
        getline(inFile, buf, '|'); expiredRefunds[i].requestID = atoi(buf.c_str());
        getline(inFile, buf, '|'); expiredRefunds[i].purchaseRecord.purchaseID = atoi(buf.c_str());
        getline(inFile, buf, '|'); expiredRefunds[i].purchaseRecord.ticketID = atoi(buf.c_str());
        getline(inFile, buf, '|'); expiredRefunds[i].purchaseRecord.amountPaid = atof(buf.c_str());
        getline(inFile, buf, '|'); expiredRefunds[i].purchaseRecord.purchaseDate.day = atoi(buf.c_str());
        getline(inFile, buf, '|'); expiredRefunds[i].purchaseRecord.purchaseDate.month = atoi(buf.c_str());
        getline(inFile, buf, '|'); expiredRefunds[i].purchaseRecord.purchaseDate.year = atoi(buf.c_str());
        getline(inFile, buf, '|'); strncpy(expiredRefunds[i].purchaseRecord.refundStatus, buf.c_str(), 20);
        getline(inFile, buf, '|'); strncpy(expiredRefunds[i].purchaseRecord.customerName, buf.c_str(), 50);
        getline(inFile, buf, '|'); strncpy(expiredRefunds[i].purchaseRecord.eventName, buf.c_str(), 100);
        getline(inFile, buf, '|'); strncpy(expiredRefunds[i].purchaseRecord.matchInfo, buf.c_str(), 100);
        getline(inFile, buf, '|'); strncpy(expiredRefunds[i].reason, buf.c_str(), 100);
        getline(inFile, buf);      expiredRefunds[i].isValidReason = (buf == "1");
        expiredRefundCount++;
    }

    inFile.close();
}

void saveAndExit() {
    char filename[50];
    cout << "\nEnter save filename (default: save" + to_string(saveFileCounter) + "): ";
    string input;
    cin.ignore(1000, '\n');
    getline(cin, input);

    if (input.empty()) {
        sprintf(filename, "save%03d_SMTMS.txt", saveFileCounter);
        saveFileCounter++;
    } else {
        string baseName = input;
        size_t pos = input.find("_SMTMS.txt");
        if (pos != string::npos) {
            baseName = input.substr(0, pos);
        }
        sprintf(filename, "%s_SMTMS.txt", baseName.c_str());

        bool exists = false;
#ifdef _WIN32
        struct _finddata_t fileInfo;
        long handle = _findfirst(filename, &fileInfo);
        if (handle != -1) {
            exists = true;
            _findclose(handle);
        }
#endif
        if (exists) {
            cout << "File already exists. Overwrite? (1 = Yes, 2 = No): ";
            int overwrite = 2;
            if (!(cin >> overwrite)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "\nInvalid input. Using new filename.\n";
                overwrite = 2;
            }
            if (overwrite == 1) {
                // use the existing filename as-is (will overwrite)
            } else {
                int num = 1;
                while (true) {
                    sprintf(filename, "%s_%03d_SMTMS.txt", baseName.c_str(), num);
#ifdef _WIN32
                    handle = _findfirst(filename, &fileInfo);
                    if (handle == -1) break;
                    _findclose(handle);
#endif
                    num++;
                }
            }
        }
    }

    strncpy(currentSaveFile, filename, 50);
    saveGame(currentSaveFile);
    gameRunning = false;
}
