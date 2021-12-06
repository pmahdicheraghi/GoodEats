#include "recipe.hpp"

#ifndef __SHELF__
#define __SHELF__

struct ShelfData {
	vector<Recipe*> recipes;
	string name;
	int userId;
	int id;
};

class Shelf {
	public:
	Shelf(string shelfName, int ShelfId, int makerId);
	string getName();
	int getUserId();
	int getId();
	void addRecipe(Recipe* recipe);
	void deleteRecipe(Recipe* recipe);
	vector<RecipeData> getRecipes();
	ShelfData getData();
	private:
	void sortRecipes();
	ShelfData data;
};

#endif