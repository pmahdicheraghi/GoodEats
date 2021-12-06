#include <string>
#include <vector>

using namespace std;

#ifndef __USER__
#define __USER__

enum UserType {user, chef, supplier, unknown};

class User {
    public:
    User(string newUsername, string newPassword, string type, int userId);
    virtual void addOffer(string ingredient, float price, int count, float packaging);
    virtual void editOffer(string ingredient, float price, int count);
    virtual void deleteOffer(string ingredient);
    virtual string getEmail();
    string getUsername();
    bool isPassword(string enteredPass);
    void changeMoney(float change);
    float getMoney();
    UserType getType();
    int getId();
    protected:
    string username;
    string password;
    UserType type;
    float money;
    int id;
};

class Supplier : public User {
    public:
    Supplier(string newName, string newPassword, string newEmail, int userId);
    string getEmail();
    private:
    string email;
};

#endif