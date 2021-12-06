#ifndef _MY_HANDLERS_
#define _MY_HANDLERS_

#include <iostream>
#include "classes/goodeats.hpp"
#include "classes/error.hpp"
#include "server/server.hpp"

class ErrorHandler : public RequestHandler {
public:
    Response* callback(Request *);
};

class LoginHandler : public RequestHandler {
public:
    Response* callback(Request *);
    LoginHandler(Goodeats* goodeats) : goodeats(goodeats) {}
private:
    Goodeats* goodeats;
};

class SignupHandler : public RequestHandler {
public:
    Response* callback(Request *);
    SignupHandler(Goodeats* goodeats) : goodeats(goodeats) {}
private:
    Goodeats* goodeats;
};

class logoutHandler : public RequestHandler {
public:
    Response* callback(Request *);
};

class homeHandler : public RequestHandler {
public:
    Response* callback(Request *);
    homeHandler(Goodeats* goodeats) : goodeats(goodeats) {}
private:
    Goodeats* goodeats;
    Response* createUserPage(int userId);
    Response* createChefPage(int userId);
};

class filterHandler : public RequestHandler {
public:
    Response* callback(Request *);
    filterHandler(Goodeats* goodeats) : goodeats(goodeats) {}
private:
    Goodeats* goodeats;
};

class showRecipeHandler : public RequestHandler {
public:
    Response* callback(Request *);
    showRecipeHandler(Goodeats* goodeats) : goodeats(goodeats) {}
private:
    Goodeats* goodeats;
    Response* createRecipePage(int userId, int recipeId);
};

class shelvesHandler : public RequestHandler {
public:
    Response* callback(Request *);
    shelvesHandler(Goodeats* goodeats) : goodeats(goodeats) {}
private:
    Goodeats* goodeats;
    Response* createShelvesPage(int userId);
};

class showShelfHandler : public RequestHandler {
public:
    Response* callback(Request *);
    showShelfHandler(Goodeats* goodeats) : goodeats(goodeats) {}
private:
    Goodeats* goodeats;
    Response* createShelfPage(int userId, int recipeId);
};

class addShelfHandler : public RequestHandler {
public:
    Response* callback(Request *);
    addShelfHandler(Goodeats* goodeats) : goodeats(goodeats) {}
private:
    Goodeats* goodeats;
};

class addRecipeToShelfHandler : public RequestHandler {
public:
    Response* callback(Request *);
    addRecipeToShelfHandler(Goodeats* goodeats) : goodeats(goodeats) {}
private:
    Goodeats* goodeats;
};

class removeRecipeFromShelfHandler : public RequestHandler {
public:
    Response* callback(Request *);
    removeRecipeFromShelfHandler(Goodeats* goodeats) : goodeats(goodeats) {}
private:
    Goodeats* goodeats;
};

class addRecipeHandler : public RequestHandler {
public:
    Response* callback(Request *);
    addRecipeHandler(Goodeats* goodeats) : goodeats(goodeats) {}
private:
    Goodeats* goodeats;
};

class removeRecipeHandler : public RequestHandler {
public:
    Response* callback(Request *);
    removeRecipeHandler(Goodeats* goodeats) : goodeats(goodeats) {}
private:
    Goodeats* goodeats;
};

class rateHandler : public RequestHandler {
public:
    Response* callback(Request *);
    rateHandler(Goodeats* goodeats) : goodeats(goodeats) {}
private:
    Goodeats* goodeats;
};

#endif
