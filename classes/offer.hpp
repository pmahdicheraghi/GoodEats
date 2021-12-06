#include <string>

using namespace std;

#ifndef __OFFER__
#define __OFFER__

class Offer {
	public:
	Offer(string newIngredient, float newPrice, int newCount, float newPackaging, int id);
	string getIngredient();
	void changePrice(float newPrice);
	void changeCount(float newCount);
	void changeDiscount(int newDiscount);
	int getSupplierId();
	float getPrice();
	float getPackaging();
	int getCount();
	bool canBuy();
	void buy();
	private:
	string ingredient;
	float price;
	int count;
	float packaging;
	int supplierId;
	int discountPercent;
};



#endif