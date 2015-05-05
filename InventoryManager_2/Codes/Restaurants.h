#ifndef _Restaurants
#define _Restaurants

#include "MenuItemIngrediants.h"
#include <vector>
#include <map>

using namespace std;

class Restaurant
{
public:
	// Constructor with input parameters of name of restaurant and function pointer of method which is used to calculate the ingredients to be restocked.
	Restaurant(string name, void (*func)(string &allOrderFile, string &prevDayOrderFile, string &averageIngredientRequirementTillPrevDay, string &ingredientsLeft, string &stockInventoryFile, int totalDays));
	
	// Destructor
	~Restaurant();

	// Add a new menuItem to the restaurant
	void addMenuItem(MenuItem *m);
	
	// Check if the given menuItem is already present in the restaurant's menu Card
	bool isMenuItemPresent(string id);
	
	// Add ingredients with quantity and given expiry date to the restaurant
	void addIngredient(string ingredientID, ExpiryDaysQuantity ingredientStock);
	
	// Check if with the current quantity of ingredients left we can place the given order or not.
	bool canPlaceOrder(Order *o);
	
	// Place the given order at the restaurant and return true if order is placed successfully else return false.
	bool placeOrder(Order *o);
	
	// Once the order is placed successfully, reduce the ingredients used for this order.
	void useIngredientsForOrder(Order *o);
	
	// Collect the quantity of non expired ingredients at the end of each day which can be used tomorrow.
	void collectLeftIngredients();
	
	// return the ID of the restaurant.
	string GetNameID();
	
	// Clear the orderlist at the end of the day to store the new order for tomorrow.
	void clearOrderList();
	
	// Print the order list to the file before clearing it. It is done so that it can be used in calculation for restocking of ingredients.
	void printOrderListToFile();

	// Function pointer used to store the actual method reference. This method will be used to calculate the restocking of ingredients.
	void (*RestockIngredients_funct)(string &allOrderFile, string &prevDayOrderFile, string &averageIngredientRequirementTillPrevDay, string &ingredientsLeft, string &stockInventoryFile, int totalDays);

private:
	string nameID;														// Name of the restaurant
	vector<string> menu;												// List of menu items
	map< string, vector<ExpiryDaysQuantity> > ingredientPresent;		// list of ingredients present along with the quantity in which it is present and the expiry date.
	vector<Order> orderList;											// Order list for a particular day
};

/****************************************************************************************************************************************************************************************************/

#endif
