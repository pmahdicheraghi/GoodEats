#include "filter.hpp"
#include "error.hpp"

using namespace std;

Filter::Filter(int newUserId) {
	userId = newUserId;
}

Filter::Type Filter::getType() {
	return type;
}

int Filter::getId() {
	return userId;
}


FilterTag::FilterTag(string newTag, int userId) : Filter(userId) {
	type = Filter::Type::tag;
	tag = newTag;
}

vector<Recipe*> FilterTag::applyFilter(const vector<Recipe*>& recipes) {
	vector<Recipe*> filteredRecipes;
	for (int i = 0; i < recipes.size(); i++) {
		for (int j = 0; j < recipes[i]->getRecipeData().tags.size(); j++) {
			if (recipes[i]->getRecipeData().tags[j] == tag) {
				filteredRecipes.push_back(recipes[i]);
			}
		}
	}
	return filteredRecipes;
}



FilterVegterian::FilterVegterian(int userId) : Filter(userId) {
	type = Filter::Type::vegetarian;
}

vector<Recipe*> FilterVegterian::applyFilter(const vector<Recipe*>& recipes) {
	vector<Recipe*> filteredRecipes;
	for (int i = 0; i < recipes.size(); i++) {
		if (recipes[i]->getRecipeData().vegetarian == "Yes") {
			filteredRecipes.push_back(recipes[i]);
		}
	}
	return filteredRecipes;
}



FilterTime::FilterTime(int newMinTime, int newMaxTime, int userId) : Filter(userId) {
	type = Filter::Type::time;
	if (newMaxTime < newMinTime) {
		throw Error(ErrorType::Bad_Request);
	}	
	minTime = newMinTime;
	maxTime = newMaxTime;
}

vector<Recipe*> FilterTime::applyFilter(const vector<Recipe*>& recipes) {
	vector<Recipe*> filteredRecipes;
	for (int i = 0; i < recipes.size(); i++) {
		if (recipes[i]->getRecipeData().minutesToReady <= maxTime && recipes[i]->getRecipeData().minutesToReady >= minTime) {
			filteredRecipes.push_back(recipes[i]);
		}
	}
	return filteredRecipes;
}



FilterRate::FilterRate(int newMinRate, int newMaxRate, int userId) : Filter(userId) {
	type = Filter::Type::rate;
	if (newMinRate < 0 || newMaxRate > 5 || newMaxRate < newMinRate) {
		throw Error(ErrorType::Bad_Request);
	}
	minRate = newMinRate;
	maxRate = newMaxRate;
}

vector<Recipe*> FilterRate::applyFilter(const vector<Recipe*>& recipes) {
	vector<Recipe*> filteredRecipes;
	for (int i = 0; i < recipes.size(); i++) {
		if (recipes[i]->getRecipeData().avarageRate <= maxRate && recipes[i]->getRecipeData().minutesToReady >= minRate) {
			filteredRecipes.push_back(recipes[i]);
		}
	}
	return filteredRecipes;
}