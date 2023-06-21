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
    string toString(const vector<Item> &items) const
    {
        stringstream ss;
        ss << "Item ID:" << itemID << "\tItem Name:" << itemName;

        // Calculate the length of the longest name in the list
        size_t longestNameLength = 0;
        for (const auto &item : items)
        {
            size_t nameLength = item.getItemName().length();
            if (nameLength > longestNameLength)
            {
                longestNameLength = nameLength;
            }
        }

        // Calculate the number of tabs needed to align the "Quantity" string
        size_t numTabs = (longestNameLength / 8 + 1) * 2; // Assuming tab width is 8 characters

        // Append the tabs before the "Quantity" string
        for (size_t i = 0; i < numTabs; ++i)
        {
            ss << "\t";
        }

        ss << "Quantity :" << quantity << "\tReg Date :" << registrationDate;
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
        // Check if the ID is already taken in the CSV file
        ifstream file1(fileName);
        if (file1.is_open())
        {
            string line;
            while (getline(file1, line))
            {
                stringstream ss(line);
                string itemIdStr;
                getline(ss, itemIdStr, ',');
                int itemId = stoi(itemIdStr);
                if (itemId == id)
                {
                    cout << "Error: Item with ID " << id << " already exists." << endl;
                    file1.close();
                    return;
                }
            }
            file1.close();
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
        // Load items from the CSV file
        loadItems();

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
            cout << item.toString(items) << endl;
        }
    }

    // Function to load items from a file
    void loadItems()
    {
        items.clear();
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
            cout << "Stored Items have been loaded successfully!\n"
                 << endl;
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
    try
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
                        // check if the ID is a positive integer
                        if (id < 0)
                        {
                            cout << "Error: Invalid ID.Must be a positive integer" << endl;
                            continue;
                        }
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
                        // check if the quantity is a positive integer
                        if (quantity < 0)
                        {
                            cout << "Error: Invalid quantity.Must be a positive integer" << endl;
                            continue;
                        }
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
    }
    // catch any error that may occur
    catch (const exception &e)
    {
        // store the error in file called logs.txt
        ofstream file("logs.txt", ios::app);
        if (file.is_open())
        {
            file << e.what() << "\n";
            file.close();
        }
    }

    return 0;
}
