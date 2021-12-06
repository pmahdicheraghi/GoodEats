#include <bits/stdc++.h>
#include "error.hpp"
#include "shelf.hpp"

using namespace std;

Shelf::Shelf(string shelfName, int shelfId, int makerId) {
	data.name = shelfName;
	data.id = shelfId;
	data.userId = makerId;
}

string Shelf::getName() {
	return data.name;
}

int Shelf::getId() {
	return data.id;
}

int Shelf::getUserId() {
	return data.userId;
}

void Shelf::addRecipe(Recipe* recipe) {
	for (int i = 0; i < data.recipes.size(); i++) {
		if (data.recipes[i]->getId() == recipe->getId()) {
			throw Error(ErrorType::Bad_Request);
		}
	}
	data.recipes.push_back(recipe);
}

void Shelf::deleteRecipe(Recipe* recipe) {
	for (int i = 0; i < data.recipes.size(); i++) {
		if (data.recipes[i]->getId() == recipe->getId()) {
			data.recipes.erase(data.recipes.begin() + i);
			return;
		}
	}
	throw Error(ErrorType::Bad_Request);
}

vector<RecipeData> Shelf::getRecipes() {
	if (data.recipes.size() == 0) {
    	throw Error(ErrorType::Empty);
    }
	sortRecipes();
    vector<RecipeData> recipesData;
    for (int i = 0; i < data.recipes.size(); i++) {
        recipesData.push_back(data.recipes[i]->getRecipeData());
    }
    return recipesData;
}

ShelfData Shelf::getData() {
	return data;
}

void Shelf::sortRecipes() {
	sort(data.recipes.begin(), data.recipes.end(), [](Recipe* a, Recipe* b){return a->getRecipeData().title < b->getRecipeData().title;});
}