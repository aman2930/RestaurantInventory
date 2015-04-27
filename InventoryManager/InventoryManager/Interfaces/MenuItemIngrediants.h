#ifndef _MenuItemIngrediants
#define _MenuItemIngrediants

#include <vector>
#include <string>
#include <utility>

#include "CommonDefines.h"

using namespace std;

struct ExpiryDaysQuantity
{
	int expiryDays;
	int quantity;
	ExpiryDaysQuantity(int expiryDays, int quantity):expiryDays(expiryDays), quantity(quantity)
	{}
};

class MenuItem
{
public:
	MenuItem(string name);
	~MenuItem();
	void addModifyIngredient(pair <string, int> ing);
	string getMenuId();
	vector< pair <string, int> > getIngredientList();

private:
	string nameID;
	vector< pair <string, int> > ingredientsList;
};

class Ingredient
{
public:
	Ingredient(string name);
	~Ingredient();
	string GetIngredientID();

private:
	string nameID;
};

class Order
{
public:
	Order(int date, int time, string item, int quantity, string restaurantName):date(date), time(time), item(item), quantity(quantity), restaurantName(restaurantName)
	{}
	Order(){}
	~Order(){}
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

#endif