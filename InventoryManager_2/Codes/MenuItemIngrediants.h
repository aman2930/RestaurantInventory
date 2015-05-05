#ifndef _MenuItemIngrediants
#define _MenuItemIngrediants

#include <vector>
#include <string>
#include <utility>

using namespace std;

// ExpiryDaysQuantity is used to store data for each ingredient.
struct ExpiryDaysQuantity
{
	int expiryDays;														// Number of days for a particular ingredient to expire.
	int quantity;														// Quantity in which that particular ingredient is present.
	ExpiryDaysQuantity(int expiryDays, int quantity):expiryDays(expiryDays), quantity(quantity)
	{}
};

/****************************************************************************************************************************************************************************************************/
// Class Name: MenuItem
// Purpose: To store the infomation related to each menu item from the menu, like ID, List of ingredients for its preperation, quantity of each ingredient.

class MenuItem
{
public:
	MenuItem(string name);												// Constructor (with Menu Item Name)
	~MenuItem();														// Destructor
	void addModifyIngredient(pair <string, int> ing);					// Add a new ingredient, quantity pair to the menu item
	string getMenuId();													// Getter for the menu ID.
	vector< pair <string, int> > getIngredientList();					// Get the ingredient list for the menu item

private:
	string nameID;
	vector< pair <string, int> > ingredientsList;
};

/****************************************************************************************************************************************************************************************************/
// Class Name: Ingredient
// Purpose: To store the infomation related to each Ingredient item.

class Ingredient
{
public:
	Ingredient(string name);
	~Ingredient();
	string GetIngredientID();

private:
	string nameID;
};

/****************************************************************************************************************************************************************************************************/
// Class Name: Order
// Purpose: To store the infomation related to each Ingredient item.

class Order
{
public:
	Order(int date, int time, string item, int quantity, string restaurantName):date(date), time(time), item(item), quantity(quantity), restaurantName(restaurantName)
	{}
	Order(){}
	~Order(){}
	
	// Getters and Setters of the Order.
	
	string GetMenuItem(){ return item; }
	int getQuantity(){ return quantity; }
	string GetRestaurantName() {return restaurantName;}
	int getDate() { return date;}
	int getTime(){ return time;}

	void setMenuItem(string item){ this->item = item; }
	void setQuantity(int quantity){ this->quantity = quantity; }
	void setRestaurantName(string restaurantName) {this->restaurantName = restaurantName;}
	void setDate(int date) { this->date = date;}
	void setTime(int time){ this->time = time;}

private:
	int date;						//YYYYMMDD format
	int time;						//HHMM with 24 hr format
	string item;
	int quantity;
	string restaurantName;
};

/****************************************************************************************************************************************************************************************************/
#endif