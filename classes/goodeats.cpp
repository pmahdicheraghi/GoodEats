#include <bits/stdc++.h>
#include "goodeats.hpp"
#include "error.hpp"

using namespace std;

struct Product {
    string ingredientName;
    int count;
};

bool USERNAME_SORT_FUNCTION(User* a, User* b) {return a->getUsername() < b->getUsername();}
bool TITLE_SORT_FUNCTION(Recipe* a, Recipe* b) {return a->getRecipeData().title < b->getRecipeData().title;}

Goodeats* Goodeats::instance = nullptr;

Goodeats* Goodeats::getInstance() {
    if (!instance) {
        instance = new Goodeats();
    }
    return instance;
}

Goodeats::Goodeats() {
    lastRecipeId = 0;
    lastUserId = 0;
    lastShelfId = 0;
    enteredUser = nullptr;
}

UserType Goodeats::userIdToType(int id) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getId() == id) {
            return users[i]->getType();
        }
    }
    return UserType::unknown;
}

void Goodeats::sortRecipes() {
    sort(recipes.begin(), recipes.end(), TITLE_SORT_FUNCTION);
}

void Goodeats::sortUsers() {
    sort(users.begin(), users.end(), USERNAME_SORT_FUNCTION);
}

User* Goodeats::findUserByUsername(string username) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getUsername() == username) {
            return users[i];
        }
    }
    return nullptr;
}

User* Goodeats::findUserById(int userId) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getId() == userId) {
            return users[i];
        }
    }
    throw Error(ErrorType::Not_Found);
}

Recipe* Goodeats::findRecipeById (int recipeId) {
    for (int i = 0; i < recipes.size(); i++) {
        if (recipes[i]->getId() == recipeId) {
            return recipes[i];
        }
    }
    throw Error(ErrorType::Not_Found);
}

Shelf* Goodeats::findShelfById (int shelfId) {
    for (int i = 0; i < shelves.size(); i++) {
        if (shelves[i]->getId() == shelfId) {
            return shelves[i];
        }
    }
    throw Error(ErrorType::Not_Found);
}

void Goodeats::checkUserPermission(vector<UserType> neededTypes, int userId) {
    enteredUser = findUserById(userId);
    for (int i = 0; i < neededTypes.size(); i++) {
        if (enteredUser != nullptr && enteredUser->getType() == neededTypes[i]) {
            return;
        }
    }
    throw Error(ErrorType::Permission_Denied);
}

int Goodeats::signup(string username, string password, string type) {
    if (findUserByUsername(username) != nullptr) {
        throw Error(ErrorType::Bad_Request);
    }
    else {
        lastUserId++;
        User* newUser = new User(username, password, type, lastUserId);
        users.push_back(newUser);
        sortUsers();
    }
    return lastUserId;
}

int Goodeats::login(string username, string password) {
    User* user = findUserByUsername(username);
    if (user == nullptr) {
        throw Error(ErrorType::Bad_Request);
    }
    if (!user->isPassword(password)) {
        throw Error(ErrorType::Bad_Request);
    }
    return user->getId();
}

float Goodeats::calculateChefRate(const vector<RecipeData>& recipesData) {
    float avarageRate = 0;
    int ratedRecipe = 0;
    for (int i = 0; i < recipesData.size(); i++) {
        if (recipesData[i].Rates.size() > 0) {
            avarageRate += recipesData[i].avarageRate;
            ratedRecipe++;
        }
    }
    (ratedRecipe == 0) ? avarageRate : (avarageRate /= ratedRecipe);
    return avarageRate;
}

vector<Recipe*> Goodeats::applyFilter(int userId) {
    vector<Recipe*> filteredRecipes = recipes;
    for (int i = 0; i < filters.size(); i++) {
        if (filters[i]->getId() == userId) {
            filteredRecipes = filters[i]->applyFilter(filteredRecipes);
        }
    }
    return filteredRecipes;
}

vector<string> Goodeats::getUsers(int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    vector<string> usernames;
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getType() == UserType::user) {
            if (users[i]->getUsername() != enteredUser->getUsername()) {
                usernames.push_back(users[i]->getUsername());
            }
        }
    }
    return usernames;
}

vector<ChefData> Goodeats::getChefs(int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    vector<ChefData> chefsdata;
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getType() == UserType::chef) {
            vector<RecipeData> recipesData = getRecipe(users[i]);
            ChefData data = {
                users[i]->getUsername(),
                calculateChefRate(recipesData),
                recipesData
            };
            chefsdata.push_back(data);
        }
    }
    return chefsdata;
}

ChefData Goodeats::getChefs(string username, int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    User* user = findUserByUsername(username);
    if (user == nullptr || user->getType() == UserType::user) {
        throw Error(ErrorType::Not_Found);
    }
    vector<RecipeData> recipesData = getRecipe(user);
    ChefData data = {
        user->getUsername(),
        calculateChefRate(recipesData),
        recipesData
    };
    return data;
}

int Goodeats::addRecipe(string title, vector<string> ingredients, string vegetarian, int minutsToReady, vector<string> tags, string imageAddress, int userId) {
    checkUserPermission(vector<UserType>{UserType::chef}, userId);
    lastRecipeId++;
    recipes.push_back(new Recipe(title, ingredients, vegetarian, minutsToReady, tags, imageAddress, lastRecipeId, enteredUser->getId()));
    sortRecipes();
    return lastRecipeId;
}

RecipeData Goodeats::getRecipeByRecipeId(int recipeId, int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    Recipe* recipe = findRecipeById(recipeId);
    return recipe->getRecipeData();
}

vector<RecipeData> Goodeats::getMyRecipes(int userId) {
    checkUserPermission(vector<UserType>{UserType::chef}, userId);
    vector<RecipeData> recipesData;
    for (int i = 0; i < recipes.size(); i++) {
        if (recipes[i]->getChefId() == enteredUser->getId()) {
            recipesData.push_back(recipes[i]->getRecipeData());
        }
    }
    // if (recipesData.size() == 0) {
    //     throw Error(ErrorType::Empty);
    // }
    return recipesData;
}

vector<RecipeData> Goodeats::getRecipe(User* user) {
    vector<RecipeData> recipesData;
    for (int i = 0; i < recipes.size(); i++) {
        if (recipes[i]->getChefId() == user->getId()) {
            recipesData.push_back(recipes[i]->getRecipeData());
        }
    }
    if (recipesData.size() == 0) {
        throw Error(ErrorType::Empty);
    }
    return recipesData;
}

vector<RecipeData> Goodeats::getAllRecipes(int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    vector<Recipe*> filteredRecipes = applyFilter(userId);
    vector<RecipeData> recipesData;
    for (int i = 0; i < filteredRecipes.size(); i++) {
        recipesData.push_back(filteredRecipes[i]->getRecipeData());
    }
    return recipesData;
}

void Goodeats::deleteRecipe(int recipeId, int userId) {
    checkUserPermission(vector<UserType>{UserType::chef}, userId);
    for (int i = 0; i < recipes.size(); i++) {
        if (recipes[i]->getId() == recipeId) {
            if (recipes[i]->getChefId() != enteredUser->getId()) {
                throw Error(ErrorType::Permission_Denied);
            }
            else {
                //dont need delete bec shelves need pointers
                recipes.erase(recipes.begin() + i);
                return;
            }
        }
    }
    throw Error(ErrorType::Not_Found);
}

void Goodeats::addRate(int recipeId, int score, int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    Recipe* recipe = findRecipeById(recipeId);
    recipe->addScore(score, enteredUser->getId());
}

void Goodeats::editRate(int recipeId, int score, int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    Recipe* recipe = findRecipeById(recipeId);
    recipe->editScore(score, enteredUser->getId());
}

int Goodeats::addShelf(string name, int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    lastShelfId++;
    for (int i = 0; i < shelves.size(); i++) {
        if (shelves[i]->getUserId() == enteredUser->getId() && shelves[i]->getName() == name) {
            throw Error(ErrorType::Bad_Request);
        }
    }
    shelves.push_back(new Shelf(name, lastShelfId, enteredUser->getId()));
    return lastShelfId;
}

void Goodeats::putRecipeInShelf(int shelfId, int recipeId, int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    Recipe* recipe = findRecipeById(recipeId);
    Shelf* shelf = findShelfById(shelfId);
    if (shelf->getUserId() != enteredUser->getId()) {
        throw Error(ErrorType::Permission_Denied);
    }
    shelf->addRecipe(recipe);
}

void Goodeats::deleteRecipeFromShelf(int shelfId, int recipeId, int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    Recipe* recipe = findRecipeById(recipeId);
    Shelf* shelf = findShelfById(shelfId);
    if (shelf->getUserId() != enteredUser->getId()) {
        throw Error(ErrorType::Permission_Denied);
    }
    shelf->deleteRecipe(recipe);
}

ShelfData Goodeats::getShelf(int shelfId, int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    Shelf* shelf = findShelfById(shelfId);
    return shelf->getData();
}

vector<ShelfData> Goodeats::getShelves(string username, int limit, int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    vector<ShelfData> shelvesData;
    User* user = findUserByUsername(username);
    int shelvesCount = 0;
    for (int i = 0; i < shelves.size() && shelvesCount < limit; i++) {
        if (shelves[i]->getUserId() == user->getId()) {
            shelvesData.push_back(shelves[i]->getData());
            shelvesCount++;
        }
    }
    return shelvesData;
}

vector<ShelfData> Goodeats::getMyShelves(int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    vector<ShelfData> shelvesData;
    int shelvesCount = 0;
    for (int i = 0; i < shelves.size(); i++) {
        if (shelves[i]->getUserId() == enteredUser->getId()) {
            shelvesData.push_back(shelves[i]->getData());
            shelvesCount++;
        }
    }
    return shelvesData;
}

void Goodeats::addFilter(Filter* filter, int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    for (int i = 0; i < filters.size(); i++) {
        if (filters[i]->getId() == userId) {
            if (filters[i]->getType() == filter->getType()) {
                delete filters[i];
                filters.at(i) = filter;
                return;
            }
        }
    }
    filters.push_back(filter);
}

void Goodeats::addTagFilter(string tag, int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    FilterTag* filter = new FilterTag(tag, userId);
    addFilter(filter, userId);
}

void Goodeats::addVegetarianFilter(int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    FilterVegterian* filter = new FilterVegterian(userId);
    addFilter(filter, userId);
}

void Goodeats::addTimeFilter(int minTime, int maxTime, int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    FilterTime* filter = new FilterTime(minTime, maxTime, userId);
    addFilter(filter, userId);
}

void Goodeats::addRateFilter(int minRate, int maxRate, int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    FilterRate* filter = new FilterRate(minRate, maxRate, userId);
    addFilter(filter, userId);
}

void Goodeats::removeFilters(int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    for (int i = 0; i < filters.size(); i++) {
        if (filters[i]->getId() == userId) {
            delete filters[i];
            filters.erase(filters.begin() + i);
        }
    }
}

int Goodeats::signupSupplier(string name, string email, string password) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getUsername() == name || users[i]->getEmail() == email) {
            throw Error(ErrorType::Bad_Request);
        }
    }
    lastUserId++;
    Supplier* newSupplier = new Supplier(name, password, email, lastUserId);
    users.push_back(newSupplier);
    return lastUserId;
}

int Goodeats::loginSupplier(string email, string password) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getEmail() == email) {
            if (users[i]->isPassword(password)) {
                return users[i]->getId();
            }
        }
    }
    throw Error(ErrorType::Bad_Request);
}

void Goodeats::addOffer(string ingredient, float price, int count, float packaging, int userId) {
    checkUserPermission(vector<UserType>{UserType::supplier}, userId);
    for (int i = 0; i < offers.size(); i++) {
        if (offers[i]->getSupplierId() == enteredUser->getId()) {
            if (offers[i]->getIngredient() == ingredient) {
                throw Error(ErrorType::Bad_Request);
            }
        }
    }
    Offer* newOffer = new Offer(ingredient, price, count, packaging, enteredUser->getId());
    offers.push_back(newOffer);
    calculateDiscount();
}

void Goodeats::editOffer(string ingredient, float price, int count, int userId) {
    checkUserPermission(vector<UserType>{UserType::supplier}, userId);
    for (int i = 0; i < offers.size(); i++) {
        if (offers[i]->getSupplierId() == enteredUser->getId()) {
            if (offers[i]->getIngredient() == ingredient) {
                offers[i]->changePrice(price);
                offers[i]->changeCount(count);
                calculateDiscount();
                return;
            }
        }
    }
    throw Error(ErrorType::Bad_Request);
}

void Goodeats::deleteOffer(string ingredient, int userId) {
    checkUserPermission(vector<UserType>{UserType::supplier}, userId);
    for (int i = 0; i < offers.size(); i++) {
        if (offers[i]->getSupplierId() == enteredUser->getId()) {
            if (offers[i]->getIngredient() == ingredient) {
                delete offers[i];
                offers.erase(offers.begin() + i);
                calculateDiscount();
                return;
            }
        }
    }
    throw Error(ErrorType::Bad_Request);
}

void Goodeats::addCredit(float money, int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    enteredUser->changeMoney(money);
}

float Goodeats::getCredit(int userId) {
    checkUserPermission(vector<UserType>{UserType::user, UserType::supplier}, userId);
    return enteredUser->getMoney();
}

OrderData Goodeats::addOrder(int recipeId, int userId) {
    checkUserPermission(vector<UserType>{UserType::user}, userId);
    Order* newOrder = new Order(recipeId, enteredUser->getId());
    newOrder->proccesOrder(recipes, offers, users);
    orders.push_back(newOrder);
    calculateDiscount();
    return newOrder->getData(); 
}

vector<OrderData> Goodeats::getMyOrders(int UserId) {
    checkUserPermission(vector<UserType>{UserType::user}, UserId);
    vector<OrderData> ordersData;
    for (int i = 0; i < orders.size(); i++) {
        if (orders[i]->getBuyerId() == enteredUser->getId()) {
            ordersData.push_back(orders[i]->getData());
        }
    }
    return ordersData;
}

void Goodeats::calculateDiscount() {
    vector<Product> productList;
    for (int i = 0; i < offers.size(); i++) {
        bool sameIngredientName = false;
        for (int j = 0; j < productList.size(); j++) {
            if (productList[j].ingredientName == offers[i]->getIngredient()) {
                productList[j].count += offers[i]->getCount();
                sameIngredientName = true;
                break;
            }
        }
        if (!sameIngredientName) {
            Product newProduct;
            newProduct.ingredientName = offers[i]->getIngredient();
            newProduct.count = offers[i]->getCount();
            productList.push_back(newProduct);
        }
    }
    for (int i = 0; i < productList.size(); i++) {
        if (productList[i].count >= 20) {
            for (int j = 0; j < offers.size(); j++) {
                if (offers[j]->getIngredient() == productList[i].ingredientName) {
                    offers[j]->changeDiscount(10);
                }
            }
        }
        else {
            for (int j = 0; j < offers.size(); j++) {
                if (offers[j]->getIngredient() == productList[i].ingredientName) {
                    offers[j]->changeDiscount(0);
                }
            }
        }
    }
}

Goodeats::~Goodeats() {
    for (int i = 0; i < shelves.size(); i++) {
        delete shelves[i];
    }
    for (int i = 0; i < recipes.size(); i++) {
        delete recipes[i];
    }
    for (int i = 0; i < users.size(); i++) {
        delete users[i];
    }
    for (int i = 0; i < filters.size(); i++) {
        delete filters[i];
    }
    for (int i = 0; i < offers.size(); i++) {
        delete offers[i];
    }
    for (int i = 0; i < orders.size(); i++) {
        delete orders[i];
    }
    cout << "disractor called" << endl;
}