// Define Libraries & Imports
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// Define Hardcoded Limits
#define MAX_COUNTRIES 37
#define MAX_PURCHASES 500
#define MAX_QUEUE 100
#define MAX_EVENTS 10
#define MAX_MATCHES 50

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
} Event;

typedef struct {
    int purchaseID;
    int ticketID;
    char customerName[50];
    char eventName[100];
    char matchInfo[100];
    double amountPaid;
    Date purchaseDate; // Nested structure
    char refundStatus[20];
} Purchase;

typedef struct {
    int requestID;
    Purchase purchaseRecord; // Nested structure
    char reason[100];
    bool isValidReason;
} RefundRequest;

typedef struct BSTNode {
    int regionID;
    char regionName[100];
    int capacity;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

// Define Array of Structures
Purchase masterPurchases[MAX_PURCHASES];
RefundRequest refundQueueArray[MAX_QUEUE];
Event activeEvents[MAX_EVENTS];

// Define Global Variables
int purchaseCount = 0;
int queueFront = 0;
int queueRear = -1;
int queueSize = 0;
int activeEventCount = 0;

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

BSTNode* categoryTreeRoot = NULL;

// Define Terminal User Interface Variables
string ur_br = "╮";
string ul_br = "╭";
string ver_br = "│";
string hor_br = "─";
string br_br = "╯";
string bl_br = "╰";

string countries[] = { // FIFA-Style Countries
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

// Define Functions
void runTutorial();
void displayDashboard();
void startNewDay();
void configureEvents();
void generateTicketPurchases();
void generateRefundRequests();
void reviewRefundRequests();
void viewStatistics();
void searchPurchaseRecord();
void endDaySummary();
void saveGame();
void loadGame();

// Custom Queue Functions
void enqueueRefund(RefundRequest req);
RefundRequest dequeueRefund();
bool isQueueFull();
bool isQueueEmpty();

// BST Functions
BSTNode* createBSTNode(int id, const char* name, int cap);
BSTNode* insertBSTNode(BSTNode* root, int id, const char* name, int cap);
void inOrderTraversal(BSTNode* root);
void freeBST(BSTNode* root);
void initializeRegionBST();

// Sorting & Searching Functions
void selectionSortPurchasesByID();
int binarySearchPurchaseByID(int targetID);

// Terminal UI Helpers
void drawHeader(string title);
void drawFooter();
void printBorderRow(string text);
void printColumns(string col1, string col2, string col3);
string getReputationRank(int rep);

int main(){

#ifdef _WIN32
    // Enable UTF-8 encoding in Windows Console
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    srand((unsigned int)time(0));
    initializeRegionBST();
    loadGame();

    int choice = 0;
    while(choice != 9){
        displayDashboard();
        cout << "\nSelect Choice [1-9]: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch(choice){
            case 1:
                runTutorial();
                break;
            case 2:
                configureEvents();
                break;
            case 3:
                generateTicketPurchases();
                break;
            case 4:
                generateRefundRequests();
                break;
            case 5:
                reviewRefundRequests();
                break;
            case 6:
                viewStatistics();
                break;
            case 7:
                searchPurchaseRecord();
                break;
            case 8:
                endDaySummary();
                break;
            case 9:
                saveGame();
                cout << "\nProgress saved automatically. Exiting system...\n";
                break;
            default:
                cout << "\nInvalid selection. Try again.\n";
        }
    }

    freeBST(categoryTreeRoot);
    return 0;
}

// Function Definitions

void drawHeader(string title) {
    cout << ul_br;
    for (int i = 0; i < 58; i++) cout << hor_br;
    cout << ur_br << "\n";

    int pad = (58 - (int)title.length()) / 2;
    int rightPad = 58 - (int)title.length() - pad;

    cout << ver_br;
    for (int i = 0; i < pad; i++) cout << " ";
    cout << title;
    for (int i = 0; i < rightPad; i++) cout << " ";
    cout << ver_br << "\n";

    cout << bl_br; 
    for (int i = 0; i < 58; i++) cout << hor_br; 
    cout << br_br << "\n";
}

void printBorderRow(string text) {
    cout << ver_br << " ";
    cout << left << setw(57) << text;
    cout << ver_br << "\n";
}

void drawFooter() {
    cout << bl_br;
    for (int i = 0; i < 58; i++) cout << hor_br;
    cout << br_br << "\n";
}

void printColumns(string col1, string col2, string col3) {
    cout << ver_br << " ";
    cout << left << setw(15) << col1;
    cout << left << setw(27) << col2;
    cout << right << setw(15) << col3;
    cout << ver_br << "\n";
}

string getReputationRank(int rep) {
    if (rep >= 95) return "Elite Organizer";
    if (rep >= 80) return "Trusted Organizer";
    if (rep >= 60) return "Established Organizer";
    if (rep >= 40) return "Under Review";
    return "Poor Organizer";
}

void displayDashboard() {
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

    printBorderRow("1. View System Tutorial (One-Time)");
    printBorderRow("2. Configure Tournament Events");
    printBorderRow("3. Generate Ticket Purchases");
    printBorderRow("4. Generate Refund Requests");
    printBorderRow("5. Review Refund Requests (Queue)");
    printBorderRow("6. View Detailed Statistics & Venue BST");
    printBorderRow("7. Search Purchase Record (Binary Search)");
    printBorderRow("8. End Business Day & Daily Summary");
    printBorderRow("9. Save & Exit Simulation");

    drawFooter();
}

void runTutorial() {
    drawHeader("SYSTEM TUTORIAL");
    printBorderRow("Welcome to the Ticket Refund Management Simulation!");
    printBorderRow("");
    printBorderRow("1. Configure Tournaments: Set up match brackets.");
    printBorderRow("2. Sell Tickets: Simulate ticket sales for matches.");
    printBorderRow("3. Refund Queue: Handle refund requests in FIFO order.");
    printBorderRow("4. Maintain Reputation: Correct reviews increase rep!");
    drawFooter();
    cout << "\nPress Enter to return to main menu...";
    cin.ignore(1000, '\n');
    cin.get();
}

void configureEvents() {
    if (activeEventCount >= MAX_EVENTS) {
        cout << "\nMaximum event capacity reached for today.\n";
        return;
    }

    drawHeader("EVENT CONFIGURATION");

    Event newEvent;
    newEvent.eventID = 101 + totalEventsCreated;

    cout << "Select Setup Mode:\n1. Manual Setup\n2. Auto Setup\nChoice: ";
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

    cout << "\nSelect Tournament Format:\n1. Round Robin\n2. Winner & Losers Bracket\n3. Single Elimination\nChoice: ";
    int fmtChoice;
    cin >> fmtChoice;

    if (fmtChoice == 1) strncpy(newEvent.format, "Round Robin", 50);
    else if (fmtChoice == 2) strncpy(newEvent.format, "Winner & Losers Bracket", 50);
    else strncpy(newEvent.format, "Single Elimination", 50);

    int numTeams = 4 + rand() % 5;
    newEvent.matchCount = numTeams / 2;

    for (int i = 0; i < newEvent.matchCount; i++) {
        newEvent.matches[i].matchID = 500 + totalMatchesGenerated + i;
        string t1 = countries[rand() % MAX_COUNTRIES];
        string t2 = countries[rand() % MAX_COUNTRIES];
        while (t1 == t2) t2 = countries[rand() % MAX_COUNTRIES];

        strncpy(newEvent.matches[i].team1, t1.c_str(), 50);
        strncpy(newEvent.matches[i].team2, t2.c_str(), 50);
        newEvent.matches[i].score1 = rand() % 5;
        newEvent.matches[i].score2 = rand() % 5;
    }

    activeEvents[activeEventCount++] = newEvent;
    totalEventsCreated++;
    totalMatchesGenerated += newEvent.matchCount;

    cout << "\nSuccessfully created event: " << newEvent.eventName << " with " << newEvent.matchCount << " generated matches.\n";
}

void generateTicketPurchases() {
    if (activeEventCount == 0) {
        cout << "\nNo active events configured. Please configure events first.\n";
        return;
    }

    int numToGenerate = 3 + rand() % 5;
    for (int i = 0; i < numToGenerate; i++) {
        if (purchaseCount >= MAX_PURCHASES) break;

        Purchase p;
        p.purchaseID = 1000 + purchaseCount + 1;
        p.ticketID = 8000 + purchaseCount + 1;

        string custName = "Customer_" + to_string(p.purchaseID);
        strncpy(p.customerName, custName.c_str(), 50);

        Event selectedEvent = activeEvents[rand() % activeEventCount];
        strncpy(p.eventName, selectedEvent.eventName, 100);

        Match selectedMatch = selectedEvent.matches[rand() % selectedEvent.matchCount];
        string matchDesc = string(selectedMatch.team1) + " vs " + string(selectedMatch.team2);
        strncpy(p.matchInfo, matchDesc.c_str(), 100);

        p.amountPaid = 50.0 + (rand() % 150);
        p.purchaseDate.day = 1 + rand() % 28;
        p.purchaseDate.month = 1 + rand() % 12;
        p.purchaseDate.year = 2026;

        strncpy(p.refundStatus, "None", 20);

        masterPurchases[purchaseCount++] = p;
        totalTicketsSold++;
        totalRevenue += p.amountPaid;
    }

    isSortedByID = false;
    cout << "\nSuccessfully simulated " << numToGenerate << " customer ticket purchases.\n";
}

void generateRefundRequests() {
    if (purchaseCount == 0) {
        cout << "\nNo ticket purchases exist to generate refund requests.\n";
        return;
    }

    int targetIdx = rand() % purchaseCount;
    if (strcmp(masterPurchases[targetIdx].refundStatus, "None") != 0) {
        cout << "\nSelected ticket already processed for refund request.\n";
        return;
    }

    if (isQueueFull()) {
        cout << "\nRefund Queue is full!\n";
        return;
    }

    RefundRequest req;
    req.requestID = 2000 + totalRefundRequests + 1;
    req.purchaseRecord = masterPurchases[targetIdx];

    const char* reasons[] = {
        "Match Rescheduled",
        "Bought wrong seat category",
        "Personal medical emergency",
        "Changed mind without valid reason",
        "Duplicate purchase attempt",
        "Scalper attempt flag"
    };

    int reasonIdx = rand() % 6;
    strncpy(req.reason, reasons[reasonIdx], 100);
    req.isValidReason = (reasonIdx < 3);

    strncpy(masterPurchases[targetIdx].refundStatus, "Pending", 20);
    req.purchaseRecord = masterPurchases[targetIdx];

    enqueueRefund(req);
    totalRefundRequests++;

    cout << "\nNew refund request generated and added to the review queue (Request ID: " << req.requestID << ").\n";
}

void reviewRefundRequests() {
    if (isQueueEmpty()) {
        cout << "\nNo pending refund requests in queue.\n";
        return;
    }

    RefundRequest req = dequeueRefund();

    drawHeader("REFUND REQUEST REVIEW");

    printBorderRow("Request ID: " + to_string(req.requestID));
    printBorderRow("Customer: " + string(req.purchaseRecord.customerName));
    printBorderRow("Event: " + string(req.purchaseRecord.eventName));
    printBorderRow("Match: " + string(req.purchaseRecord.matchInfo));
    printBorderRow("Amount: $" + to_string(req.purchaseRecord.amountPaid));
    printBorderRow("Reason: " + string(req.reason));

    drawFooter();

    cout << "\nDecision:\n1. Approve Refund\n2. Reject Refund\nChoice: ";
    int choice;
    cin >> choice;

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
            strncpy(masterPurchases[idx].refundStatus, "Approved", 20);
            approvedRefunds++;
        } else {
            strncpy(masterPurchases[idx].refundStatus, "Rejected", 20);
            rejectedRefunds++;
        }
    }
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
    if (purchaseCount == 0) {
        cout << "\nNo purchases recorded yet.\n";
        return;
    }

    cout << "\nEnter Purchase ID to search: ";
    int searchID;
    cin >> searchID;

    int resultIdx = binarySearchPurchaseByID(searchID);

    if (resultIdx != -1) {
        Purchase p = masterPurchases[resultIdx];
        drawHeader("PURCHASE RECORD FOUND");

        printBorderRow("Purchase ID: " + to_string(p.purchaseID));
        printBorderRow("Ticket ID: " + to_string(p.ticketID));
        printBorderRow("Customer: " + string(p.customerName));
        printBorderRow("Event Name: " + string(p.eventName));
        printBorderRow("Match Info: " + string(p.matchInfo));
        printBorderRow("Amount Paid: $" + to_string(p.amountPaid));

        string date =
            to_string(p.purchaseDate.year) + "-" +
            (p.purchaseDate.month < 10 ? "0" : "") + to_string(p.purchaseDate.month) + "-" +
            (p.purchaseDate.day < 10 ? "0" : "") + to_string(p.purchaseDate.day);

        printBorderRow("Date: " + date);
        printBorderRow("Status: " + string(p.refundStatus));

        drawFooter();
    } else {
        cout << "\nPurchase ID " << searchID << " not found in database.\n";
    }
}

void viewStatistics() {
    drawHeader("SIMULATION STATISTICS OVERVIEW");

    printBorderRow("Days Played: " + to_string(currentDay));
    printBorderRow("Total Events Created: " + to_string(totalEventsCreated));
    printBorderRow("Total Matches Generated: " + to_string(totalMatchesGenerated));
    printBorderRow("Total Tickets Sold: " + to_string(totalTicketsSold));
    printBorderRow("Total Revenue: $" + to_string(totalRevenue));
    printBorderRow("Total Refund Requests: " + to_string(totalRefundRequests));
    printBorderRow("Approved Refunds: " + to_string(approvedRefunds));
    printBorderRow("Rejected Refunds: " + to_string(rejectedRefunds));
    printBorderRow("Current Reputation: " + to_string(organizerReputation));
    printBorderRow("Highest Reputation: " + to_string(highestReputation));
    printBorderRow("Lowest Reputation: " + to_string(lowestReputation));

    drawFooter();

    drawHeader("REGIONAL VENUE HIERARCHY (BST IN-ORDER TRAVERSAL)");
    inOrderTraversal(categoryTreeRoot);
    drawFooter();
}

void endDaySummary() {
    drawHeader("DAILY SUMMARY REPORT");

    printBorderRow("Completed Day: " + to_string(currentDay));
    printBorderRow("Tickets Sold Today: " + to_string(totalTicketsSold));
    printBorderRow("Revenue Earned: $" + to_string(totalRevenue));
    printBorderRow("Organizer Reputation: " + to_string(organizerReputation));

    drawFooter();

    currentDay++;
    activeEventCount = 0;
    cout << "\nAdvanced to Day " << currentDay << ".\n";
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

BSTNode* createBSTNode(int id, const char* name, int cap) {
    BSTNode* newNode = new BSTNode();
    newNode->regionID = id;
    strncpy(newNode->regionName, name, sizeof(newNode->regionName));
    newNode->capacity = cap;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

BSTNode* insertBSTNode(BSTNode* root, int id, const char* name, int cap) {
    if (root == NULL) {
        return createBSTNode(id, name, cap);
    }
    if (id < root->regionID) {
        root->left = insertBSTNode(root->left, id, name, cap);
    } else if (id > root->regionID) {
        root->right = insertBSTNode(root->right, id, name, cap);
    }
    return root;
}

void inOrderTraversal(BSTNode* root) {
    if (root == NULL) return;
    inOrderTraversal(root->left);
    printColumns(
        "ID: " + to_string(root->regionID),
        root->regionName,
        "Cap: " + to_string(root->capacity)
    );
    inOrderTraversal(root->right);
}

void freeBST(BSTNode* root) {
    if (root == NULL) return;
    freeBST(root->left);
    freeBST(root->right);
    delete root;
}

void initializeRegionBST() {
    categoryTreeRoot = insertBSTNode(categoryTreeRoot, 300, "Europe Division", 85000);
    categoryTreeRoot = insertBSTNode(categoryTreeRoot, 100, "Americas Division", 75000);
    categoryTreeRoot = insertBSTNode(categoryTreeRoot, 200, "Asia-Pacific Division", 65000);
    categoryTreeRoot = insertBSTNode(categoryTreeRoot, 400, "Africa Division", 50000);
}

void saveGame() {
    ofstream outFile("save_data.txt");
    if (!outFile.is_open()) return;

    outFile << currentDay << "\n";
    outFile << organizerReputation << "\n";
    outFile << totalEventsCreated << "\n";
    outFile << totalMatchesGenerated << "\n";
    outFile << totalTicketsSold << "\n";
    outFile << totalRevenue << "\n";
    outFile << totalRefundRequests << "\n";
    outFile << approvedRefunds << "\n";
    outFile << rejectedRefunds << "\n";
    outFile << highestReputation << "\n";
    outFile << lowestReputation << "\n";

    outFile << purchaseCount << "\n";
    for (int i = 0; i < purchaseCount; i++) {
        outFile << masterPurchases[i].purchaseID << " "
                << masterPurchases[i].ticketID << " "
                << masterPurchases[i].amountPaid << " "
                << masterPurchases[i].purchaseDate.day << " "
                << masterPurchases[i].purchaseDate.month << " "
                << masterPurchases[i].purchaseDate.year << " "
                << masterPurchases[i].refundStatus << "\n";
    }

    outFile.close();
}

void loadGame() {
    ifstream inFile("save_data.txt");
    if (!inFile.is_open()) return;

    inFile >> currentDay;
    inFile >> organizerReputation;
    inFile >> totalEventsCreated;
    inFile >> totalMatchesGenerated;
    inFile >> totalTicketsSold;
    inFile >> totalRevenue;
    inFile >> totalRefundRequests;
    inFile >> approvedRefunds;
    inFile >> rejectedRefunds;
    inFile >> highestReputation;
    inFile >> lowestReputation;

    inFile >> purchaseCount;
    for (int i = 0; i < purchaseCount; i++) {
        masterPurchases[i].purchaseID = 1000 + i + 1;
        masterPurchases[i].ticketID = 8000 + i + 1;
        inFile >> masterPurchases[i].purchaseID
               >> masterPurchases[i].ticketID
               >> masterPurchases[i].amountPaid
               >> masterPurchases[i].purchaseDate.day
               >> masterPurchases[i].purchaseDate.month
               >> masterPurchases[i].purchaseDate.year
               >> masterPurchases[i].refundStatus;

        string custName = "Customer_" + to_string(masterPurchases[i].purchaseID);
        strncpy(masterPurchases[i].customerName, custName.c_str(), 50);
        strncpy(masterPurchases[i].eventName, "Loaded Championship Cup", 100);
        strncpy(masterPurchases[i].matchInfo, "Team A vs Team B", 100);
    }

    inFile.close();
}