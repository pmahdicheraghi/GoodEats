#include "user.hpp"

using namespace std;

User::User(string newUsername, string newPassword, string newType, int userId) {
    username = newUsername;
    password = newPassword;
    newType == "user" ? type = UserType::user : type = UserType::chef;
    money = 0;
    id = userId;
}

void User::addOffer(string ingredient, float price, int count, float packaging) {}

void User::editOffer(string ingredient, float price, int count) {}

void User::deleteOffer(string ingredient) {}

UserType User::getType() {
    return type;
}

string User::getUsername() {
    return username;
}

string User::getEmail() {
    return "";
}

bool User::isPassword(string enteredPass) {
    return (password == enteredPass);
}

int User::getId() {
    return id;
}

void User::changeMoney(float change) {
    money += change;
}

float User::getMoney() {
    return money;
}



Supplier::Supplier(string newName, string newPassword, string newEmail, int userId) : User("", newPassword, "", userId) {
    username = newName;
    email = newEmail;
    type = UserType::supplier;
}

string Supplier::getEmail() {
    return email;
}