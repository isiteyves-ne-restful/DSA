#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

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
        ss << "Item ID: " << itemID << "\tItem Name: " << itemName << "\tQuantity : " << quantity << "\tReg Date: " << registrationDate;
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
        // Check if the ID is already taken
        for (const auto &item : items)
        {
            if (item.getItemID() == id)
            {
                cout << "Error: Item with ID " << id << " already exists." << endl;
                return;
            }
        }

        Item item(id, name, quantity, regDate);
        ofstream file(fileName, ios::app);
        if (file.is_open())
        {
            file << id << "," << name << "," << quantity << "," << regDate << "\n";
            file.close();
            // add item to existing items
            items.push_back(item);
            cout << "Item saved successfully!" << endl;
        }
        else
        {
            cout << "Unable to open the file." << endl;
        }
    }

    // Function to list items in ascending order of their ID
    void listItems()
    {
        // Check if there are any items and display a message if not
        if (items.empty())
        {
            cout << "No items are recorded yet." << endl;
            return;
        }

        // Sort items in ascending order of their name
        sort(items.begin(), items.end(), [](const Item &a, const Item &b)
             { return a.getItemName() < b.getItemName(); });

        // Display items by looping through the vector
        for (const auto &item : items)
        {
            cout << item.toString() << endl;
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
            cout << "Stored Items have been loaded successfully!" << endl;
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

// function to check if a date is valid
bool isValidDate(const string &dateStr, const string &format)
{
    tm tm = {};
    istringstream ss(dateStr);
    ss >> get_time(&tm, format.c_str());
    return !ss.fail();
}

int main()
{
    // create an inventory object instance
    Inventory inventory("items.csv");

    string command;
    cout << "--------------------------------------" << endl;
    cout << "*       RCA INVENTORY SYSTEM            *" << endl;
    cout << "--------------------------------------" << endl;
    cout << "Developed and maintained by: SW Engineer. ISITE Yves" << endl;

    // load items from the file and tell the user when they are ready
    inventory.loadItems();

    // display help about supported commands and their syntaxes
    displayHelp();

    while (true)
    {
        cout << "\nEnter a command> ";
        getline(cin, command);

        // handle different cases according to the entered command (case insensitive)
        if (toLowercase(command) == "itemadd" || toLowercase(command.substr(0, 8)) == "itemadd ")
        {
            if (toLowercase(command) == "itemadd")
            {
                cout << "Invalid format. Enter data in the following format:\n";
                cout << "itemadd <item_id> <item_name> <quantity> <registration_date>\n";
                continue;
            }
            string addCommand = command.substr(8);
            size_t firstSpacePos = addCommand.find(' ');
            size_t secondSpacePos = addCommand.find(' ', firstSpacePos + 1);
            size_t thirdSpacePos = addCommand.find(' ', secondSpacePos + 1);

            if (firstSpacePos != string::npos && secondSpacePos != string::npos)
            {
                int id = 0;

                try
                {
                    id = stoi(addCommand.substr(0, firstSpacePos));
                }
                catch (const exception &e)
                {
                    // display an error message
                    cout << "Error: Invalid ID.Must be a valid integer" << endl;
                    // continue because user entered invalid data
                    continue;
                }

                string name = addCommand.substr(firstSpacePos + 1, secondSpacePos - firstSpacePos - 1);
                int quantity = 0;

                try
                {
                    quantity = stoi(addCommand.substr(secondSpacePos + 1, thirdSpacePos - secondSpacePos - 1));
                }
                catch (const exception &e)
                {
                    // display an error message
                    cout << "Error: Invalid quantity.Must be a valid integer." << endl;
                    // continue because user entered invalid data
                    continue;
                }

                string regDate = addCommand.substr(thirdSpacePos + 1);
                // Validate regDate as a valid date (format: YYYY-MM-DD)
                if (!isValidDate(regDate, "%Y-%m-%d"))
                {
                    cout << "Invalid date format. Please enter the date in the format YYYY-MM-DD." << endl;
                    continue;
                }

                // Call the recordDiseaseCases function with the provided arguments
                inventory.addItem(id, name, quantity, regDate);
            }
            else
            {
                cout << "Invalid format. Enter data in the following format:\n";
                cout << "itemadd <item_id> <item_name> <quantity> <registration_date>\n";
            }
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
