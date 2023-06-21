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
        sort(items.begin(), items.end(), [](const Item &a, const Item &b)
             { return a.getItemName() < b.getItemName(); });

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
            cout << "Items loaded successfully!" << endl;
        }
        else
        {
            cout << "Unable to open the file." << endl;
        }
    }
};

int main()
{
    return 0;
}