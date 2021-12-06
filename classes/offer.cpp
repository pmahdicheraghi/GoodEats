#include "offer.hpp"
#include "error.hpp"

Offer::Offer(string newIngredient, float newPrice, int newCount, float newPackaging, int id) {
	ingredient = newIngredient;
	price = newPrice;
	count = newCount;
	packaging = newPackaging;
	supplierId = id;
	discountPercent = 0;
}

string Offer::getIngredient() {
	return ingredient;
}

void Offer::changePrice(float newPrice) {
	price = newPrice;
}

void Offer::changeCount(float newCount) {
	count = newCount;
}

int Offer::getSupplierId() {
	return supplierId;
}

float Offer::getPrice() {
	return price - price * discountPercent / 100;
}

float Offer::getPackaging() {
	return packaging - packaging * discountPercent / 100;
}

bool Offer::canBuy() {
	if (count > 0) {
		return true;
	}
	return false;
}

int Offer::getCount() {
	return count;
}

void Offer::buy() {
	if (count <= 0) {
		throw Error(ErrorType::Bad_Request);
	}
	count--;
}

void Offer::changeDiscount(int newDiscount) {
	discountPercent = newDiscount;
}