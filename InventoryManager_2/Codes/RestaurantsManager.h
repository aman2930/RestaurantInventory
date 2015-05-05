#ifndef _RestaurantsManager
#define _RestaurantsManager

#include "MenuItemIngrediants.h"
#include "Restaurants.h"
#include <fstream>
#include <iostream>
#include <math.h>

// Class Name: 	RestaurantsManager
// Purpose: 	To add and manage a new restaurants, placing orders, restocking of ingredients.
// Property: 	This RestaurantManager is a singleton class having a list of restaurants. It is managing adding of new restaurants, placing of orders, restocking of ingredients.
class RestaurantsManager
{
public:
	static RestaurantsManager * GetRestaurantsManagerInstance();						// static Function to create singleton instance
	bool addRestaurant(string restaurantName);											// Adding a new restaurant with given name to the list. If restaurant name already exist, it will not be added
	void addNewMenuItem(MenuItem * m);													// Adding a new menu item to the global list of menu Items. (gMenuItemList)
	void placeTodaysOrders(string orderFile);											// Place orders from the input file. It has orders for all restaurants.
	void accumulateLeftIngredients();													// Accumulate and print left ingredients from all the restaurants at the end of the day
	void PrintLeftIngredientsOfAllRestaurantsToFile();									// Print remaining ingredients from each restaurant to their corresponding files. This is called at the end of the day
	void ReStockIngredientForEachRestaurant(string &allOrderFile);						// Calculate ingredients to be restocked at each restaurant before opening it next day. Input parameter is the file where all the orders are kept for calculation purposes
	void stockedFromIngredientFile(Restaurant * r);										// Stock the ingredients which were calculated at each restaurant.
	void printOrderListForEachRestaurant();												// Segregate the order of each restaurant from the common order file. These segregated orders are placed in seperate files.
	int getTotalNumberOfDays();															// Number of days passed since restaurant chain has started.

private:
	RestaurantsManager(): totalDays(0){};												// constructor

	static RestaurantsManager* rManager;
	map <string, Restaurant *> restaurantLists;
	int totalDays;
};

/****************************************************************************************************************************************************************************************************/
// Global function to convert menuID to menuItem instance.
// Input Parameters: 
//				id: Name of menu item
MenuItem * GetMenuItemFromMenuItemID(string id);										

/****************************************************************************************************************************************************************************************************/

// Add remaining ingredients to the list at the end of the day from all the restaurant.
// Input Parameters: 
//				ingredientId: 	Name of ingredient item
//				quantity:		Quantity left at the restaurant X.
void AddRemainingIngredientsToList(string ingredientId, int quantity);					

/****************************************************************************************************************************************************************************************************/

// Global function used to calculate the Restock of Ingredients. This function is passed as function pointer in my case. This can be replaced by another function or module.
// Input Parameters: 
//				allOrderFile: 									File Name having all the orders till now
//				prevDayOrderFile:								File Name having order of previous day only
//				averageIngredientRequirementTillPrevDay:		File Name having the average of each ingredients required excluding prev day. (Total quantity of each ingredient used till day before yesterday)/Number of day	
//				ingredientsLeft:								File Name telling how much quantity of each ingredient are left at the given restaurant
//				stockInventoryFile:								File Name storing how much more quantity of each ingredient has to be ordered
//				totalDays:										Number of days passed, restaurant chain has started and restaurant manager has started its work.
// LOGIC:		For each restaurant calculate how many orders are given till date and take an average of ingredients required based on number of days and total ingredients required.
//				To avoid recalculations, I am storing this average into a file and next day just using the order of that day, and this average we can calculate the new average.
void RestockIngredients(string &allOrderFile, string &prevDayOrderFile, string &averageIngredientRequirementTillPrevDay, string &ingredientsLeft, string &stockInventoryFile, int totalDays);

/****************************************************************************************************************************************************************************************************/


#endif
