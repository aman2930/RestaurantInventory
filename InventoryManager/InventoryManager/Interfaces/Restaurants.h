#ifndef _Restaurants
#define _Restaurants

#include "MenuItemIngrediants.h"
#include "CommonDefines.h"
#include <vector>
#include <map>

using namespace std;

class Restaurant
{
public:
	Restaurant(string name, void (*func)(string &allOrderFile, string &prevDayOrderFile, string &averageIngredientRequirementTillPrevDay, string &ingredientsLeft, string &stockInventoryFile, int totalDays));
	~Restaurant();
	void addMenuItem(MenuItem *m);
	bool isMenuItemPresent(string id);
	void addIngredient(string ingredientID, ExpiryDaysQuantity ingredientStock);
	bool canPlaceOrder(Order *o);
	bool placeOrder(Order *o);
	void useIngredientsForOrder(Order *o);
	void collectLeftIngredients();
	string GetNameID();
	void clearOrderList();
	void printOrderListToFile();

	void (*RestockIngredients_funct)(string &allOrderFile, string &prevDayOrderFile, string &averageIngredientRequirementTillPrevDay, string &ingredientsLeft, string &stockInventoryFile, int totalDays);

private:
	string nameID;
	vector<string> menu;
	map< string, vector<ExpiryDaysQuantity> > ingredientPresent;
	vector<Order> orderList;
};

#endif
