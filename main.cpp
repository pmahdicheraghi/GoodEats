#include <cstdlib>
#include <ctime>
#include <iostream>

#include "classes/goodeats.hpp"
#include "classes/error.hpp"
#include "server/server.hpp"
#include "handlers.hpp"

using namespace std;

int main(int argc, char** argv) {
    Goodeats* goodeats = Goodeats::getInstance();
    try {
        Server* server = new Server(argc > 1 ? atoi(argv[1]) : 5000);
        server->get("/", new ShowPage("static/main.html"));
        server->get("/login", new ShowPage("static/login.html"));
        server->get("/signup", new ShowPage("static/signup.html"));
        server->get("/logout", new logoutHandler());
        server->post("/login", new LoginHandler(goodeats));
        server->post("/signup", new SignupHandler(goodeats));
        server->get("/home", new homeHandler(goodeats));
        server->post("/add_filter", new filterHandler(goodeats));
        server->get("/add_recipe", new ShowPage("static/add_recipe.html"));
        server->post("/add_recipe", new addRecipeHandler(goodeats));
        server->get("/remove_recipe", new removeRecipeHandler(goodeats));
        server->get("/shelves", new shelvesHandler(goodeats));
        server->get("/shelf", new showShelfHandler(goodeats));
        server->post("/add_shelf", new addShelfHandler(goodeats));
        server->post("/add_recipe_to_shelf", new addRecipeToShelfHandler(goodeats));
        server->get("/remove_recipe_from_shelf", new removeRecipeFromShelfHandler(goodeats));
        server->get("/rate", new rateHandler(goodeats));
        server->get("/recipe", new showRecipeHandler(goodeats));
        server->get("/goodeats.png", new ShowImage("static/goodeats.png"));
        server->get("/food.jpg", new ShowImage("static/food.jpg"));
        server->get("/favicon.ico", new ShowImage("static/favicon.ico"));
        server->get("/error", new ErrorHandler());
        server->run();
    }
    catch (Server::Exception &e) {
        cerr << e.getMessage() << endl;
    }
}