#include "order.hpp"
#include <bits/stdc++.h>
#include "error.hpp"

using namespace std;

Order::Order(int newRecipeId, int newBuyerId) {
	data.recipeId = newRecipeId;
	buyerId = newBuyerId;
}

User* Order::findUserById(int userId, const vector<User*> &users) {
        for (int i = 0; i < users.size(); i++) {
        if (users[i]->getId() == userId) {
            return users[i];
        }
    }
    throw Error(ErrorType::Not_Found);
}

Recipe* Order::findRecipeById (int recipeId, const vector<Recipe*> &recipes) {
    for (int i = 0; i < recipes.size(); i++) {
        if (recipes[i]->getId() == recipeId) {
            return recipes[i];
        }
    }
    throw Error(ErrorType::Not_Found);
}


Offer* Order::findCheepestOffer(string ingredeint, const vector<Offer*> &offers) {
    Offer* cheepestOffer = nullptr;
    for (int i = 0; i < offers.size(); i++) {
        if (offers[i]->getIngredient() == ingredeint && offers[i]->canBuy()) {
            if (cheepestOffer == nullptr) {
                cheepestOffer = offers[i];
            }
            else if (cheepestOffer->getPrice() > offers[i]->getPrice()) {
                cheepestOffer = offers[i];
            }
        }
    }
    if (cheepestOffer == nullptr) {
        throw Error(ErrorType::Not_Found);
    }
    return cheepestOffer;
}


float Order::calculatePrice(const vector<string> &ingredients,vector<Offer*> &cheepestOffers, const vector<User*> &users, const vector<Offer*> &offers) {
    float totalPrice = 0;
    for (int i = 0; i < ingredients.size(); i++) {
        Offer* offer = findCheepestOffer(ingredients[i], offers);
        totalPrice += offer->getPrice();
        totalPrice += offer->getPackaging();
        cheepestOffers.push_back(offer);
    }
    if (totalPrice > findUserById(buyerId, users)->getMoney()) {
        throw Error(ErrorType::Bad_Request);
    }
    return totalPrice;
}

void Order::buyFromSupplier(const vector<Offer*> &cheepestOffers, const vector<User*> &users, float totalPrice) {
    for (int i = 0; i < cheepestOffers.size(); i++) {
        User* supplier = findUserById(cheepestOffers[i]->getSupplierId(), users);
        supplier->changeMoney(cheepestOffers[i]->getPrice() + cheepestOffers[i]->getPackaging());
        cheepestOffers[i]->buy();
    }
    findUserById(buyerId, users)->changeMoney(-totalPrice);
}

void Order::createReport(const vector<Offer*> &cheepestOffers, float totalPrice, const vector<User*> &users) {
    for (int i = 0; i < cheepestOffers.size(); i++) {
        bool hasSameSupplier = false;   
        for (int j = 0; j < data.miniOrders.size(); j++) {
            if (data.miniOrders[j].supplireName == findUserById(cheepestOffers[i]->getSupplierId(), users)->getUsername()) {
                data.miniOrders[j].ingredients.push_back(cheepestOffers[i]->getIngredient());
                hasSameSupplier = true;
                break;
            }
        }
        if (!hasSameSupplier) {
            MiniOrder miniOrder;
            miniOrder.supplireName = findUserById(cheepestOffers[i]->getSupplierId(), users)->getUsername();
            miniOrder.ingredients.push_back(cheepestOffers[i]->getIngredient());
            data.miniOrders.push_back(miniOrder);
        }
    }
    data.totalPrice = totalPrice;
    sort(data.miniOrders.begin(), data.miniOrders.end(), [](MiniOrder a, MiniOrder b) {return a.supplireName < b.supplireName;});
    for (int i = 0; i < data.miniOrders.size(); i++) {
        sort(data.miniOrders[i].ingredients.begin(), data.miniOrders[i].ingredients.end());
    }
}

void Order::proccesOrder(const vector<Recipe*> &recipes, const vector<Offer*> &offers, const vector<User*> &users) {
	vector<string> ingredients = findRecipeById(data.recipeId, recipes)->getRecipeData().ingredients;
    vector<Offer*> cheepestOffers;
    float totalPrice = calculatePrice(ingredients, cheepestOffers, users, offers);
    buyFromSupplier(cheepestOffers, users, totalPrice);
    createReport(cheepestOffers, totalPrice, users);
}

OrderData Order::getData() {
    return data;
}

int Order::getBuyerId() {
    return buyerId;
}