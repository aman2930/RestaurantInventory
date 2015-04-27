#ifndef _RestaurantsManager
#define _RestaurantsManager

#include "MenuItemIngrediants.h"
#include "Restaurants.h"
#include <fstream>
#include <iostream>
#include <math.h>

// This RestaurantManager is a singleton class having a list of restaurants. It is managing adding of new restaurants, placing of orders, restocking of ingredients.
class RestaurantsManager
{
public:
	static RestaurantsManager * GetRestaurantsManagerInstance();
	bool addRestaurant(string restaurantName);
	void addNewMenuItem(MenuItem * m);
	void placeTodaysOrders(string orderFile);
	void accumulateLeftIngredients();
	void PrintLeftIngredientsOfAllRestaurantsToFile();
	void ReStockIngredientForEachRestaurant(string &allOrderFile);
	void stockedFromIngredientFile(Restaurant * r);
	void printOrderListForEachRestaurant();
	int getTotalNumberOfDays();

private:
	RestaurantsManager(): totalDays(0){};

	static RestaurantsManager* rManager;
	map <string, Restaurant *> restaurantLists;
	int totalDays;
};

MenuItem * GetMenuItemFromMenuItemID(string id);
void AddRemainingIngredientsToList(string ingredientId, int quantity);


// Function used to calculate the Restock of Ingredients. 
void RestockIngredients(string &allOrderFile, string &prevDayOrderFile, string &averageIngredientRequirementTillPrevDay, string &ingredientsLeft, string &stockInventoryFile, int totalDays);



#endif
