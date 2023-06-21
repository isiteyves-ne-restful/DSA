#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Item class representing an inventory item
class Item
{
private:
    int itemID;
    string itemName;
    int quantity;
    string registrationDate;

public:
    // Constructor
    Item(int id, const string &name, int qty, const string &regDate)
        : itemID(id), itemName(name), quantity(qty), registrationDate(regDate) {}

    // Getter methods
    int getItemID() const
    {
        return itemID;
    }

    string getItemName() const
    {
        return itemName;
    }

    int getQuantity() const
    {
        return quantity;
    }

    string getRegistrationDate() const
    {
        return registrationDate;
    }

    // Function to format item details as a string
    string toString() const
    {
        stringstream ss;
        ss << "Item ID: " << itemID << "\tItem Name: " << itemName << "\tQuantity: " << quantity << "\tReg Date: " << registrationDate;
        return ss.str();
    }
};

// Inventory class representing the inventory system
class Inventory
{
private:
    vector<Item> items;
    string fileName;

public:
    // Constructor
    Inventory(const string &file)
        : fileName(file) {}

    // Function to add an item to the inventory
    void addItem(int id, const string &name, int quantity, const string &regDate)
    {
        Item item(id, name, quantity, regDate);
        items.push_back(item);
    }

    // Function to list items in alphabetical order
    void listItems()
    {
        // Check if there are any items and display  message if not
        if (items.empty())
        {
            cout << "No items are recorded yet." << endl;
            return;
        }

        // OTHERWISE (IF THERE ARE ITEMS)
        // Sort items in alphabetical order
        sort(items.begin(), items.end(), [](const Item &a, const Item &b)
             { return a.getItemName() < b.getItemName(); });

        // Display items looping through the vector
        for (const auto &item : items)
        {
            cout << item.toString() << endl;
        }
    }

    // Function to save items to a file
    void saveItems()
    {
        ofstream file(fileName);
        if (file.is_open())
        {
            for (const auto &item : items)
            {
                file << item.getItemID() << "," << item.getItemName() << "," << item.getQuantity() << "," << item.getRegistrationDate() << "\n";
            }
            file.close();
            cout << "Items saved successfully!" << endl;
        }
        else
        {
            cout << "Unable to open the file." << endl;
        }
    }

    // Function to load items from a file
    void loadItems()
    {
        ifstream file(fileName);
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                stringstream ss(line);
                string token;
                vector<string> tokens;
                while (getline(ss, token, ','))
                {
                    tokens.push_back(token);
                }
                if (tokens.size() == 4)
                {
                    int id = stoi(tokens[0]);
                    string name = tokens[1];
                    int quantity = stoi(tokens[2]);
                    string regDate = tokens[3];
                    Item item(id, name, quantity, regDate);
                    items.push_back(item);
                }
            }
            file.close();
            cout << "Stored Items loaded successfully!" << endl;
        }
        else
        {
            cout << "Unable to open the file." << endl;
        }
    }
};

// Function to clear the console screen
void clearScreen()
{
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Linux and macOS
#endif
}

// Function to display help
void displayHelp()
{
    cout << "--------------------------------------\n";
    cout << "*       Commands syntaxes              *\n";
    cout << "--------------------------------------\n";
    cout << "itemadd <item_id> <item_name> <quantity> <registration_date>\n";
    cout << "itemslist\n";
    cout << "help\n";
    cout << "exit\n";
}

// Function to convert a string to lowercase
string toLowercase(const string &str)
{
    string lowercaseStr = str;
    transform(lowercaseStr.begin(), lowercaseStr.end(), lowercaseStr.begin(), ::tolower);
    return lowercaseStr;
}

int main()
{
    // create an inventory object instance
    Inventory inventory("items.csv");

    string command;
    cout << "--------------------------------------" << endl;
    cout << "*          INVENTORY SYSTEM            *" << endl;
    cout << "--------------------------------------" << endl;

    // load items from the file and tell the user when they are ready
    inventory.loadItems();

    // display help about supported commands and their syntaxes
    displayHelp();

    while (true)
    {
        cout << "\nEnter a command> ";
        getline(cin, command);

        // Convert the command to lowercase
        command = toLowercase(command);

        // handle different cases according to the entered command
        if (command == "itemadd")
        {
            int id, quantity;
            string name, regDate;

            cout << "Enter item ID: ";
            cin >> id;
            cin.ignore();

            cout << "Enter item name: ";
            getline(cin, name);

            cout << "Enter item quantity: ";
            cin >> quantity;
            cin.ignore();

            cout << "Enter item registration date: ";
            getline(cin, regDate);

            inventory.addItem(id, name, quantity, regDate);
            inventory.saveItems();

            cout << "Item added successfully!\n";
        }
        else if (command == "itemslist")
        {
            inventory.listItems();
        }
        else if (command == "help")
        {
            displayHelp();
        }
        else if (command == "clear" || command == "cls")
        {
            clearScreen(); // Clear the console screen
        }
        else if (command == "exit")
        {
            cout << "Exiting the program...\n";
            break;
        }
        else
        {
            cout << "Invalid command. Please try again.\n";
        }
    }

    return 0;
}
