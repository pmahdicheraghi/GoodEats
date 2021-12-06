#include "handlers.hpp"
#include <sstream>

using namespace std;

string to_string_with_precision(const float number, const int precision = 2) {
  ostringstream out;
  out.precision(precision);
  out << fixed << number;
  return out.str();
}

int string_to_int(string str) {
  try {
    return stoi(str);
  }
  catch(exception& e) {
    return 0;
  }
}

Response* ErrorHandler::callback(Request* req) {
  Response *res = new Response;
  res->setHeader("Content-Type", "text/html");
  string body;
  body += "<!DOCTYPE html>";
  body += "<html>";
  body += "<body style=\"text-align: center;\">";
  string error = req->getQueryParam("err");
  string errorCode = req->getQueryParam("code");
  body += "<h2>Error: " + errorCode + "</h2>";
  body += "<h1>" + error + "</h1>";
  body += "<a href=\"/home\">Home</a>";
  body += "</body>";
  body += "</html>";
  res->setBody(body);
  return res;
}

Response* LoginHandler::callback(Request* req) {
  Response *res = Response::redirect("/home");
  string type = req->getBodyParam("type");

  try {
    if (type == "supplier") {
      string email = req->getBodyParam("email");
      string password = req->getBodyParam("password");
      res->setSessionId(to_string(goodeats->loginSupplier(email, password)));
    }
    else {
      string username = req->getBodyParam("username");
      string password = req->getBodyParam("password");
      res->setSessionId(to_string(goodeats->login(username, password)));
    }
  }
  catch(Error &error) {
    res = Response::redirect("/error?err=" + error.whatType() + "&code=" + error.whatCode());
  }
  return res;
}

Response* SignupHandler::callback(Request* req) {
  Response *res = Response::redirect("/home");
  string type = req->getBodyParam("type");

  try {
    if (type == "supplier") {
      string name = req->getBodyParam("name");
      string email = req->getBodyParam("email");
      string password = req->getBodyParam("password");
      res->setSessionId(to_string(goodeats->signupSupplier(name, email, password)));
    }
    else if (type == "user" || type == "chef"){
      string username = req->getBodyParam("username");
      string password = req->getBodyParam("password");
      res->setSessionId(to_string(goodeats->signup(username, password, type)));
    }
  }
  catch(Error &error) {
    res = Response::redirect("/error?err=" + error.whatType() + "&code=" + error.whatCode());
  }
  return res;
}

Response* logoutHandler::callback(Request* req) {
  Response* res;

  if (req->getSessionId() == "0" || req->getSessionId() == "") {
    res = Response::redirect("/error?err=Bad Request&code=400");
  }
  else {
    res = Response::redirect("/");
    res->setSessionId("0");
  }
  return res;
}


Response* homeHandler::createUserPage(int userId) {
  Response* res = new Response();
  res->setHeader("Content-Type", "text/html");
  string body = "<!DOCTYPE html>  <html lang='en'> <head> <title>Goodeats</title> <meta charset='utf-8'>";
  body += "<meta name='viewport' content='width=device-width, initial-scale=1'> ";
  body += readFile("static/style.html");
  body += "<style> td:nth-child(5){background-color: transparent; margin: 0px}</style> </head>";
  body += "<body> <div class='top'> <a href='/home'>Home</a> <a href='/shelves'>Shelves</a> <a href='/logout'>Logout</a>";
  body += "<b style='float:right; text-align: center; padding: 16px 16px; font-size: 100%; color: khaki;'>Home</b> </div> ";
  body += "<div class='content'> <h2>Recipes</h2> <div class='filter'>";
  body += "<h2>Filters</h2>";
  body += "<form action='/add_filter' method='post'>";
  body += "<p style='margin-bottom: 1px;'>Type: </p> <input name='tag' type='text' placeholder='Tag'/>";
  body += "<p style='margin-bottom: 1px;'>Time: </p> <input name='min_time' type='number' placeholder='Min'/> <input name='max_time' type='number' placeholder='Max'/>";
  body += "<p style='margin-bottom: 1px;'>Rate: </p> <input name='min_rate' type='number' placeholder='Min'/> <input name='max_rate' type='number' placeholder='Max'/>";
  body += "<br> <button type='submit' name='type' value='apply'>Apply</button>";
  body += "<button type='submit' name='type' value='delete'>Delete All Filters</button>";
  body += "</form> </div> <table id='recipes'>";
  vector<RecipeData> data = goodeats->getAllRecipes(userId);
  body += "<tr> <th>ID</th> <th>Title</th> <th>Vegetarian</th> <th>Min To Ready</th> </tr>";
  for (int i = 0; i < data.size(); i++) {
    body += "<tr> <td>";
    body += "<a href='/recipe?id=" + to_string(data[i].id) + "'>" + to_string(data[i].id) + "</a>";
    body += "</td> <td>";
    body += data[i].title;
    body += "</td> <td>";
    body += data[i].vegetarian;
    body += "</td> <td>";
    body += to_string(data[i].minutesToReady);
    body += "</td> <td>";
    body += " <div class='dropdown'> <a class='dropbtn'>⭐</a> <div class='dropdown-content'>";
    body += "<a href='/rate?id=" + to_string(data[i].id) + "&score=1'>1️⃣: Bad</a>";
    body += "<a href='/rate?id=" + to_string(data[i].id) + "&score=2'>2️⃣: Normal</a>";
    body += "<a href='/rate?id=" + to_string(data[i].id) + "&score=3'>3️⃣: Good</a>";
    body += "<a href='/rate?id=" + to_string(data[i].id) + "&score=4'>4️⃣: Excellent</a>";
    body += "<a href='/rate?id=" + to_string(data[i].id) + "&score=5'>5️⃣: Perfect</a>";
    body += "</div> </div>";
    body += "</td> </tr>";
  }
  body += "</table> </div>";
  body += "<div class='footer'> <p>👨🏻‍💻: Mahdi Cheraghi</p> <p>💬: @pmahdicheraghi@gmail.com</p> <p>📞: 09192936453</p> </div> </body> </html>";
  res->setBody(body);
  return res;
}

Response* homeHandler::createChefPage(int userId) {
  Response* res = new Response();
  res->setHeader("Content-Type", "text/html");
  string body = "<!DOCTYPE html>  <html lang='en'> <head> <title>Goodeats</title> <meta charset='utf-8'>";
  body += "<meta name='viewport' content='width=device-width, initial-scale=1'> ";
  body += readFile("static/style.html");
  body += "<style> td:nth-child(8){background-color: transparent;}</style> </head> <body> <div class='top'> <a href='/home'>Home</a> <a href='/logout'>Logout</a>";
  body += "<b style='float:right; text-align: center; padding: 16px 16px; font-size: 100%; color: khaki;'>Home</b> </div> ";
  vector<RecipeData> data = goodeats->getMyRecipes(userId);
  body += "<div class='content'> <h2>Your Recipes</h2> <table id='recipes'>";
  body += "<tr> <th>ID</th> <th>Title</th> <th>Vegetarian</th> <th>Min To Ready</th> <th>Ingredients</th> <th>Tags</th> <th>Rating</th> </tr>";
  for (int i = 0; i < data.size(); i++) {
    body += "<tr> <td>";
    body += to_string(data[i].id);
    body += "</td> <td>";
    body += data[i].title;
    body += "</td> <td>";
    body += data[i].vegetarian;
    body += "</td> <td>";
    body += to_string(data[i].minutesToReady);
    body += "</td> <td>";
    for(int j = 0; j < data[i].ingredients.size(); j++) {
      body += data[i].ingredients[j];
      (j + 1 < data[i].ingredients.size()) ? body += "," : body;
    }
    body += "</td> <td>";
    for(int j = 0; j < data[i].tags.size(); j++) {
      body += data[i].tags[j];
      (j + 1 < data[i].tags.size()) ? body += "," : body;
    }
    body += "</td> <td>";
    body += to_string_with_precision(data[i].avarageRate);
    body += "</td> <td>";
    body += "<a href='/remove_recipe?id=" + to_string(data[i].id) + "'>🚮</a>";
    body += "</td> </tr>";
  }
  body += "</table>";
  body += "<a href='/add_recipe' class='addbtn'>Add New Recipe</a> </div>";
  body += "<div class='footer'> <p>👨🏻‍💻: Mahdi Cheraghi</p> <p>💬: @pmahdicheraghi@gmail.com</p> <p>📞: 09192936453</p> </div> </body> </html>";
  res->setBody(body);
  return res;
}

Response* homeHandler::callback(Request* req) {
  Response* res;
  UserType type;

  try {
    int userId = string_to_int(req->getSessionId());
    type = goodeats->userIdToType(userId);
    switch (type) {
    case UserType::user:
      res = createUserPage(userId);
      break;
    case UserType::chef:
      res = createChefPage(userId);
      break;
    default:
      res = Response::redirect("/");
      break;
    }
  }
  catch(Error &error) {
    res = Response::redirect("/error?err=" + error.whatType() + "&code=" + error.whatCode());
  }
  return res;
}


Response* filterHandler::callback(Request* req) {
  Response* res = Response::redirect("/home");
  try {
    int userId = string_to_int(req->getSessionId());
    if (req->getBodyParam("type") == "apply") {
      string filteredTag = req->getBodyParam("tag");
      string minTime = req->getBodyParam("min_time");
      string maxTime = req->getBodyParam("max_time");
      string minRate = req->getBodyParam("min_rate");
      string maxRate = req->getBodyParam("max_rate");
      if (filteredTag != "") {
        goodeats->addTagFilter(filteredTag, userId);
      }
      if (minTime != "" && maxTime != "") {
        goodeats->addTimeFilter(string_to_int(minTime), string_to_int(maxTime), userId);
      }
      if (minRate != "" && maxRate != "") {
        goodeats->addTimeFilter(string_to_int(minRate), string_to_int(maxRate), userId);
      }
    }
    else {
      goodeats->removeFilters(userId);
    }
  }
  catch(Error &error) {
    res = Response::redirect("/error?err=" + error.whatType() + "&code=" + error.whatCode());
  }
  return res;
}


Response* showRecipeHandler::createRecipePage(int userId, int recipeId) {
  Response* res = new Response();
  res->setHeader("Content-Type", "text/html");
  string body = "<!DOCTYPE html>  <html lang='en'> <head> <title>Goodeats</title> <meta charset='utf-8'>";
  body += "<meta name='viewport' content='width=device-width, initial-scale=1'> ";
  body += readFile("static/style.html");
  body += "<style> td:nth-child(5){background-color: transparent; margin: 0px}</style> </head> <body>";
  body += "<body> <div class='top'> <a href='/home'>Home</a> <a href='/shelves'>Shelves</a> <a href='/logout'>Logout</a>";
  body += "<b style='float:right; text-align: center; padding: 16px 16px; font-size: 100%; color: khaki;'>Home</b> </div> ";
  RecipeData data = goodeats->getRecipeByRecipeId(recipeId, userId);
  body += "<div class='content'> <h2>Recipe " + to_string(recipeId) + "</h2> <table id='recipes'>";
  body += "<tr> <th>ID</th> <td>";
  body += to_string(data.id);
  body += "</td> </tr> <tr> <th>Title</th> <td>";
  body += data.title;
  body += "</td> </tr> <tr> <th>Vegetarian</th> <td>";
  body += data.vegetarian;
  body += "</td> </tr> <tr> <th>Min To Ready</th> <td>";
  body += to_string(data.minutesToReady);
  body += "</td> </tr> <tr> <th>Ingredients</th> <td>";
  for(int j = 0; j < data.ingredients.size(); j++) {
      body += data.ingredients[j];
      (j + 1 < data.ingredients.size()) ? body += "," : body;
    }
  body += "</td> </tr> <tr> <th>Tags</th> <td>";
  for(int j = 0; j < data.tags.size(); j++) {
      body += data.tags[j];
      (j + 1 < data.tags.size()) ? body += "," : body;
    }
  body += "</td> </tr> <tr> <th>Rating</th> <td>";
  body += to_string_with_precision(data.avarageRate);
  body += "<div class='dropdown'> <a class='dropbtn'>⭐</a> <div class='dropdown-content'>";
  body += "<a href='/rate?id=" + to_string(data.id) + "&score=1'>1️⃣: Bad</a>";
  body += "<a href='/rate?id=" + to_string(data.id) + "&score=2'>2️⃣: Normal</a>";
  body += "<a href='/rate?id=" + to_string(data.id) + "&score=3'>3️⃣: Good</a>";
  body += "<a href='/rate?id=" + to_string(data.id) + "&score=4'>4️⃣: Excellent</a>";
  body += "<a href='/rate?id=" + to_string(data.id) + "&score=5'>5️⃣: Perfect</a>";
  body += "</div> </div>";
  body += "</td> </tr>";
  body += "</table> <div class='image'> <img src='";
  body += data.imageAddress;
  body += "' alt='Image Not Found'> </div> </div>";
  body += "<div class='footer'> <p>👨🏻‍💻: Mahdi Cheraghi</p> <p>💬: @pmahdicheraghi@gmail.com</p> <p>📞: 09192936453</p> </div> </body> </html>";
  res->setBody(body);
  return res;
}

Response* showRecipeHandler::callback(Request* req) {
  Response* res;
  
  try {
    int userId = string_to_int(req->getSessionId());
    int recipeId = string_to_int(req->getQueryParam("id"));
    res = createRecipePage(userId, recipeId);
  }
  catch(Error &error) {
    res = Response::redirect("/error?err=" + error.whatType() + "&code=" + error.whatCode());
  }
  return res;
}


Response* showShelfHandler::createShelfPage(int userId, int shelfId) {
  Response* res = new Response();
  res->setHeader("Content-Type", "text/html");
  string body = "<!DOCTYPE html>  <html lang='en'> <head> <title>Goodeats</title> <meta charset='utf-8'>";
  body += "<meta name='viewport' content='width=device-width, initial-scale=1'> ";
  body += readFile("static/style.html");
  body += "<style> td:nth-child(4){background-color: transparent; margin: 0px}</style> </head> <body>";
  body += "<div class='top'> <a href='/home'>Home</a> <a href='/shelves'>Shelves</a> <a href='/logout'>Logout</a>";
  body += "<b style='float:right; text-align: center; padding: 16px 16px; font-size: 100%; color: khaki;'>Shelves</b> </div> ";
  ShelfData data = goodeats->getShelf(shelfId, userId);
  body += "<div class='content'> <h2>Shelf " + to_string(shelfId) + "</h2> <table id='shelf'>";
  body += "<tr> <th>ID</th> <td>";
  body += to_string(data.id);
  body += "</td> </tr> <tr> <th>Name</th> <td>";
  body += data.name;
  body += "</td> </tr> <tr> <th>Recipes</th> <td>";
  for (int i = 0; i < data.recipes.size(); i++) {
    RecipeData recipeData = data.recipes[i]->getRecipeData();
    body += "<table id='recipe'>";
    body += "<tr> <th>ID</th> <td>";
    body += "<a href='/recipe?id=" + to_string(recipeData.id) + "'>" + to_string(recipeData.id) + "</a>";
    body += "</td> </tr> <tr> <th>Title</th> <td>";
    body += recipeData.title;
    body += "</td> </tr> <tr> <th>Vegetarian</th> <td>";
    body += recipeData.vegetarian;
    body += "</td> </tr> <tr> <th>Min To Ready</th> <td>";
    body += to_string(recipeData.minutesToReady);
    body += "</td> </tr> <tr> <td>";
    body += "<a href='/remove_recipe_from_shelf?shelf_id=" + to_string(data.id) + "&recipe_id=" + to_string(recipeData.id) + "'>🚮</a>";
    body += "</td> </tr> </table>";
    if (i + 1 < data.recipes.size()) body += "<hr>";
  }
  body += "</td> </tr> </table> <div style='white-space: nowrap;'>";
  body += "<form action='/add_recipe_to_shelf?shelf_id=" + to_string(data.id) + "' method='post'>";
  body += "<input required name='recipe_id' type='number' placeholder='Recipe ID'";
  body += "style='margin-top: 10px; padding: 8px; width: max-content;' />";
  body += "<button type='submit'";
  body += "style='color: white; background-color: #333; width: max-content; border-radius: 5px; text-decoration: none; margin: 2px; padding: 8px;'>";
  body += "Add</button> </form> </div> </div>";
  body += "<div class='footer'> <p>👨🏻‍💻: Mahdi Cheraghi</p> <p>💬: @pmahdicheraghi@gmail.com</p> <p>📞: 09192936453</p> </div> </body> </html>";
  res->setBody(body);
  return res;
}

Response* showShelfHandler::callback(Request* req) {
  Response* res;
  
  try {
    int userId = string_to_int(req->getSessionId());
    int recipeId = string_to_int(req->getQueryParam("id"));
    res = createShelfPage(userId, recipeId);
  }
  catch(Error &error) {
    res = Response::redirect("/error?err=" + error.whatType() + "&code=" + error.whatCode());
  }
  return res;
}


Response* shelvesHandler::createShelvesPage(int userId) {
  Response* res = new Response();
  res->setHeader("Content-Type", "text/html");
  string body = "<!DOCTYPE html>  <html lang='en'> <head> <title>Goodeats</title> <meta charset='utf-8'>";
  body += "<meta name='viewport' content='width=device-width, initial-scale=1'> ";
  body += readFile("static/style.html");
  body += "<style> td:nth-child(3){background-color: transparent; margin: 0px}</style> </head>";
  body += "<body> <div class='top'> <a href='/home'>Home</a> <a href='/shelves'>Shelves</a> <a href='/logout'>Logout</a>";
  body += "<b style='float:right; text-align: center; padding: 16px 16px; font-size: 100%; color: khaki;'>Shelves</b> </div> ";
  vector<ShelfData> data = goodeats->getMyShelves(userId);
  body += "<div class='content'> <h2>Your shelves</h2> <table id='shelves'>";
  body += "<tr> <th>ID</th> <th>Name</th> </tr>";
  for (int i = 0; i < data.size(); i++) {
    body += "<tr> <td>";
    body += "<a href='/shelf?id=" + to_string(data[i].id) + "'>" + to_string(data[i].id) + "</a>";
    body += "</td> <td>";
    body += data[i].name;
    body += "</td> <td>";
    body += "<a>🚮</a>";
    body += "</td> </tr>";
  }
  body += "</table> <div style='white-space: nowrap;'>";
  body += "<form action='/add_shelf' method='post'>";
  body += "<input required name='name' type='txt' placeholder='New Shelf Name'";
  body += "style='margin-top: 10px; padding: 8px; width: max-content;' />";
  body += "<button type='submit'";
  body += "style='color: white; background-color: #333; width: max-content; border-radius: 5px; text-decoration: none; margin: 2px; padding: 8px;'>";
  body += "Add</button> </form> </div> </div>";
  body += "<div class='footer'> <p>👨🏻‍💻: Mahdi Cheraghi</p> <p>💬: @pmahdicheraghi@gmail.com</p> <p>📞: 09192936453</p> </div> </body> </html>";
  res->setBody(body);
  return res;
}

Response* shelvesHandler::callback(Request* req) {
  Response* res;
  
  try {
    int userId = string_to_int(req->getSessionId());
    res = createShelvesPage(userId);
  }
  catch(Error &error) {
    res = Response::redirect("/error?err=" + error.whatType() + "&code=" + error.whatCode());
  }
  return res;
}

Response* addShelfHandler::callback(Request* req) {
  Response *res = Response::redirect("/shelves");

  try {
    string name = req->getBodyParam("name");
    goodeats->addShelf(name, string_to_int(req->getSessionId()));
  }
  catch(Error &error) {
    res = Response::redirect("/error?err=" + error.whatType() + "&code=" + error.whatCode());
  }
  return res;
}


Response* addRecipeToShelfHandler::callback(Request* req) {
  string shelfId = req->getQueryParam("shelf_id");
  string recipeId = req->getBodyParam("recipe_id");
  Response *res = Response::redirect("/shelf?id=" + shelfId);

  try {
    goodeats->putRecipeInShelf(string_to_int(shelfId), string_to_int(recipeId), string_to_int(req->getSessionId()));
  }
  catch(Error &error) {
    res = Response::redirect("/error?err=" + error.whatType() + "&code=" + error.whatCode());
  }
  return res;
}


Response* removeRecipeFromShelfHandler::callback(Request* req) {
  string shelfId = req->getQueryParam("shelf_id");
  string recipeId = req->getQueryParam("recipe_id");
  Response *res = Response::redirect("/shelf?id=" + shelfId);

  try {
    goodeats->deleteRecipeFromShelf(string_to_int(shelfId), string_to_int(recipeId), string_to_int(req->getSessionId()));
  }
  catch(Error &error) {
    res = Response::redirect("/error?err=" + error.whatType() + "&code=" + error.whatCode());
  }
  return res;
}


Response* addRecipeHandler::callback(Request* req) {
  Response *res = Response::redirect("/home");

  try {
    string title = req->getBodyParam("title");
    string ingredients = req->getBodyParam("ingredients");
    string vegetarian = req->getBodyParam("Vegetarian");
    string minutsToReady = req->getBodyParam("minutes_to_ready");
    string tags = req->getBodyParam("tags");
    string imageAddress = req->getBodyParam("image_address");
    goodeats->addRecipe(title, split(ingredients, ","), vegetarian, string_to_int(minutsToReady), split(tags, ","), imageAddress, string_to_int(req->getSessionId()));
  }
  catch(Error &error) {
    res = Response::redirect("/error?err=" + error.whatType() + "&code=" + error.whatCode());
  }
  return res;
}

Response* removeRecipeHandler::callback(Request* req) {
  Response *res = Response::redirect("/home");

  try {
    string recipeId = req->getQueryParam("id");
    goodeats->deleteRecipe(string_to_int(recipeId), string_to_int(req->getSessionId()));
  }
  catch(Error &error) {
    res = Response::redirect("/error?err=" + error.whatType() + "&code=" + error.whatCode());
  }
  return res;
}

Response* rateHandler::callback(Request* req) {
  Response *res = Response::redirect("/home");

  try {
    string recipeId = req->getQueryParam("id");
    string score = req->getQueryParam("score");
    goodeats->addRate(string_to_int(recipeId), string_to_int(score), string_to_int(req->getSessionId()));
  }
  catch(Error &error) {
    try {
      string recipeId = req->getQueryParam("id");
      string score = req->getQueryParam("score");
      goodeats->editRate(string_to_int(recipeId), string_to_int(score), string_to_int(req->getSessionId()));
    }
    catch(Error& error2) {
      res = Response::redirect("/error?err=" + error2.whatType() + "&code=" + error2.whatCode());
    }
  }
  return res;
}