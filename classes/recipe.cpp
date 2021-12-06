#include "recipe.hpp"
#include "error.hpp"

Recipe::Recipe(string title, vector<string> ingredients, string vegetarian, int minutsToReady,
               vector<string> tags, string imageAddress, int newId, int newChefId) {
    data.title = title;
    data.ingredients = ingredients;
    data.vegetarian = vegetarian;
    data.minutesToReady = minutsToReady;
    data.tags = tags;
    data.imageAddress = imageAddress;
    data.avarageRate = 0.0;
    data.id = newId;
    chefId = newChefId;
}

RecipeData Recipe::getRecipeData() {
    return data;
}

int Recipe::getId() {
    return data.id;
}

int Recipe::getChefId() {
    return chefId;
}

float Recipe::calculateAvarageRate() {
    float avarageRate = 0;
    for (int i = 0; i < data.Rates.size(); i++) {
        avarageRate += data.Rates[i].score;
    }
    (data.Rates.size() == 0) ? avarageRate : (avarageRate /= data.Rates.size());
    return avarageRate;
}

void Recipe::addScore(int score, int userId) {
    for (int i = 0; i < data.Rates.size(); i++) {
        if (data.Rates[i].userId == userId) {
            throw Error(ErrorType::Bad_Request);
        }
    }
    if (score > 5 || score < 1) {
        throw Error(ErrorType::Bad_Request);
    }
    Rate rate = {score, userId};
    data.Rates.push_back(rate);
    data.avarageRate = calculateAvarageRate();
}

void Recipe::editScore(int score, int userId) {
    if (score > 5 || score < 1) {
        throw Error(ErrorType::Bad_Request);
    }
    for (int i = 0; i < data.Rates.size(); i++) {
        if (data.Rates[i].userId == userId) {
            data.Rates[i].score = score;
            data.avarageRate = calculateAvarageRate();
            return;
        }
    }
    throw Error(ErrorType::Bad_Request);
}