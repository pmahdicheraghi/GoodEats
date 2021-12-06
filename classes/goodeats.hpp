#include "user.hpp"
#include "shelf.hpp"
#include "filter.hpp"
#include "offer.hpp"
#include "order.hpp"

using namespace std;

#ifndef __GOODEATS__
#define __GOODEATS__

struct ChefData {
    string username;
    float rating;
    vector<RecipeData> recipe;
};

class Goodeats {
    public:
    static Goodeats* getInstance();
    ~Goodeats();
    UserType userIdToType(int id);
    int signup(string username, string password, string type);////
    int login(string username, string password);////
    vector<string> getUsers(int userId);
    vector<ChefData> getChefs(int userId);
    ChefData getChefs(string username, int userId);
    int addRecipe(string title, vector<string> ingredients, string vegetarian, int minutsToReady, vector<string> tags, string imageAddress, int userId);
    vector<RecipeData> getAllRecipes(int userId);
    vector<RecipeData> getMyRecipes(int userId);
    RecipeData getRecipeByRecipeId(int recipeId, int userId);
    void addRate(int recipeId, int score, int userId);
    void editRate(int recipeId, int score, int userId);
    void deleteRecipe(int recipeid, int userId);
    int addShelf(string name, int userId);
    void putRecipeInShelf(int shelfId, int recipeId, int userId);
    void deleteRecipeFromShelf(int shelfId, int recipeId, int userId);
    ShelfData getShelf(int shelfId, int userId);
    vector<ShelfData> getShelves(string username, int limit, int userId);//
    vector<ShelfData> getMyShelves(int userId);//
    void addTagFilter(string tag, int userId);
    void addVegetarianFilter(int userId);
    void addTimeFilter(int minTime, int maxTime, int userId);
    void addRateFilter(int minRate, int maxRate, int userId);
    void removeFilters(int userId);
    int signupSupplier(string name, string email, string password);////
    int loginSupplier(string email, string password);////
    void addOffer(string ingredient, float price, int count, float packaging, int userId);
    void editOffer(string ingredient, float price, int count, int userId);
    void deleteOffer(string ingredient, int userId);
    void addCredit(float money, int userId);
    float getCredit(int userId);
    OrderData addOrder(int recipeId, int userId);
    vector<OrderData> getMyOrders(int userId);
    private:
    Goodeats();
    static Goodeats* instance;
    void calculateDiscount();
    float calculateChefRate(const vector<RecipeData> &recipesData);
    User* findUserByUsername(string username);
    Recipe* findRecipeById (int recipeId);
    User* findUserById(int userId);
    Shelf* findShelfById(int shelfId);
    void addFilter(Filter* filter, int userId);
    vector<Recipe*> applyFilter(int userId);
    vector<RecipeData> getRecipe(User* user);
    void checkUserPermission(vector<UserType> neededTypes, int userId);
    void sortRecipes();
    void sortUsers();
    vector<Shelf*> shelves;
    vector<Recipe*> recipes;
    vector<User*> users;
    vector<Filter*> filters;
    vector<Offer*> offers;
    vector<Order*> orders;
    int lastRecipeId;
    int lastUserId;
    int lastShelfId;
    User* enteredUser;
};


#endif