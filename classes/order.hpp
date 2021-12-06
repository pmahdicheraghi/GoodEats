#include <vector>
#include <string>
#include "recipe.hpp"
#include "offer.hpp"
#include "user.hpp"

using namespace std;

#ifndef __ORDER__
#define __ORDER__

struct MiniOrder{
	string supplireName;
	vector<string> ingredients;
};

struct OrderData {
	vector<MiniOrder> miniOrders;
	float totalPrice;
	int recipeId;
};

class Order {
	public:
	Order(int newRecipeId, int newBuyerId);
	void proccesOrder(const vector<Recipe*> &recipes, const vector<Offer*> &offers, const vector<User*> &users);
	OrderData getData();
	int getBuyerId();
	private:
	Offer* findCheepestOffer(string ingredient, const vector<Offer*> &offers);
	float calculatePrice(const vector<string> &ingredients, vector<Offer*> &cheepestOffers, const vector<User*> &users, const vector<Offer*> &offers);
    void createReport(const vector<Offer*> &cheepestOffers, float totalPrice, const vector<User*> &users);
    void buyFromSupplier(const vector<Offer*> &cheepestOffers, const vector<User*> &users, float totalPrice);
	Recipe* findRecipeById (int recipeId, const vector<Recipe*> &recipes);
    User* findUserById(int userId, const vector<User*> &users);
	OrderData data;
	int buyerId;
};

#endif