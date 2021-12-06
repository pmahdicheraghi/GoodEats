#include <vector>
#include <string>

using namespace std;

#ifndef __RECIPE__
#define __RECIPE__

struct Rate {
    int score;
    int userId;
};

struct RecipeData {
    string title;
    vector<string> ingredients;
    string vegetarian;
    int minutesToReady;
    vector<string> tags;
    string imageAddress;
    vector<Rate> Rates;
    float avarageRate;
    int id;
};

class Recipe {
    public:
    Recipe(string title, vector<string> ingredients, string vegetarian, int minutsToReady,
           vector<string> tags, string imageAddress, int newId, int newChefId);
    RecipeData getRecipeData();
    void addScore(int score, int userId);
    void editScore(int score, int userId);
    int getChefId();
    int getId();
    private:
    float calculateAvarageRate();
    RecipeData data;
    int chefId;
};

#endif