#include "Restaurants.h"
#include "RestaurantsManager.h"
#include <algorithm>

bool compareExpiryDaysQuantity (ExpiryDaysQuantity i,ExpiryDaysQuantity j) 
{
	return (i.expiryDays < j.expiryDays); 
}

Restaurant::Restaurant(string name, void (*func)(string &allOrderFile, string &prevDayOrderFile, string &averageIngredientRequirementTillPrevDay, string &ingredientsLeft, string &stockInventoryFile, int totalDays))
{
	nameID = name;
	RestockIngredients_funct = func;
}

Restaurant::~Restaurant()
{
	// Destructor
	menu.clear();
	ingredientPresent.clear();
}

void Restaurant::addMenuItem(MenuItem *m)
{
	if(!isMenuItemPresent(m->getMenuId()))
		menu.push_back(m->getMenuId());
	else
	{
		// Menu Item Already Present
	}
}

bool Restaurant::isMenuItemPresent(string id)
{
	for(int i = 0; i < menu.size(); i++)
	{
		if(menu[i].compare(id) == 0)
			return true;
	}
	return false;
}

void Restaurant::addIngredient(string ingredientID, ExpiryDaysQuantity ingredientStock)
{
	map< string, vector<ExpiryDaysQuantity> >::iterator it;
	it = ingredientPresent.find(ingredientID);
	if (it != ingredientPresent.end())
	{
		(it->second).push_back(ingredientStock);
	}
	else
	{
		vector<ExpiryDaysQuantity> v;
		v.push_back(ingredientStock);
		ingredientPresent[ingredientID] = v;
	}

	it = ingredientPresent.find(ingredientID);
	sort((it->second).begin(), (it->second).end(),compareExpiryDaysQuantity);
}

bool Restaurant::canPlaceOrder(Order *o)
{
	bool canPlaceOrder = true;
	MenuItem * menuItem = GetMenuItemFromMenuItemID(o->GetMenuItem());
	if(menuItem != NULL)
	{
		vector< pair <string, int> > ingredientList = menuItem->getIngredientList();
		for(int i=0;i<ingredientList.size();i++)
		{
			string id = ingredientList[i].first;
			int quantityRequired = ingredientList[i].second*o->getQuantity();

			map< string, vector<ExpiryDaysQuantity> >::iterator it;
			it = ingredientPresent.find(id);
			if(it != ingredientPresent.end())
			{
				vector<ExpiryDaysQuantity> v = it->second;
				while(quantityRequired > 0 && v.size() > 0)
				{
					int quantity = v[0].quantity;
					if(quantity > quantityRequired)
					{
						v[0].quantity = quantity - quantityRequired;
						quantityRequired = 0;
					}
					else if(quantity == quantityRequired)
					{
						v.erase(v.begin());
						quantityRequired = 0;
					}
					else
					{
						quantityRequired -= quantity;
						v.erase(v.begin());
					}
				}
			}
			if(quantityRequired > 0)
			{
				canPlaceOrder = false;
				break;
			}
		}
	}
	else
	{
		canPlaceOrder = false;			// Menu Item does not exist
	}
	return canPlaceOrder;
}

void Restaurant::useIngredientsForOrder(Order *o)
{
	MenuItem * menuItem = GetMenuItemFromMenuItemID(o->GetMenuItem());
	if(menuItem != NULL)
	{
		vector< pair <string, int> > ingredientList = menuItem->getIngredientList();
		for(int i=0;i<ingredientList.size();i++)
		{
			string id = ingredientList[i].first;
			int quantityRequired = ingredientList[i].second*o->getQuantity();

			map< string, vector<ExpiryDaysQuantity> >::iterator it;
			it = ingredientPresent.find(id);
			if(it != ingredientPresent.end())
			{
				vector<ExpiryDaysQuantity> v = it->second;
				while(quantityRequired > 0 && v.size() > 0)
				{
					int quantity = v[0].quantity;
					if(quantity > quantityRequired)
					{
						v[0].quantity = quantity - quantityRequired;
						quantityRequired = 0;
					}
					else if(quantity == quantityRequired)
					{
						v.erase(v.begin());
						quantityRequired = 0;
					}
					else
					{
						quantityRequired -= quantity;
						v.erase(v.begin());
					}
				}
				it->second = v;
			}
		}
	}
}

bool Restaurant::placeOrder(Order *o)
{
	Order order = *o;
	orderList.push_back(order);
	if(canPlaceOrder(o))
	{
		useIngredientsForOrder(o);
		return true;
	}
	else
	{
		return false;
	}
// 	Order order;
// 	order.setDate(o->getDate());
// 	order.setTime(o->getTime());
// 	order.setMenuItem(o->GetMenuItem());
// 	order.setQuantity(o->getQuantity());
// 	order.setRestaurantName(o->GetRestaurantName());
// 	orderList.push_back(order);
}

void Restaurant::collectLeftIngredients()
{
	string fileBase = "./resourceFiles/";
	string fileName = fileBase + nameID + "_Ingredients_left.txt";
	std::ofstream ofs (fileName.c_str(), std::ofstream::out);

	map < string, vector<ExpiryDaysQuantity> >::iterator it, it1;
	for(it = ingredientPresent.begin(); it != ingredientPresent.end(); it++)
	{
		vector<ExpiryDaysQuantity> v = it->second;
		int totalQuantityLeft = 0;
		vector<ExpiryDaysQuantity>::iterator vecIt;
		for(vecIt = v.begin(); vecIt != v.end();)
		{
			vecIt->expiryDays--;
			if(vecIt->expiryDays <= 0)
			{
				v.erase(vecIt);
				continue;
			}
			AddRemainingIngredientsToList(it->first, vecIt->quantity);
			totalQuantityLeft += vecIt->quantity;
			vecIt++;
		}

		ofs << it->first << " ";
		ofs << totalQuantityLeft << " ";
//		it1 = it + 1;
//		if(it1 != ingredientPresent.end())
//			ofs << endl;
		it->second = v;
	}
	ofs.close();
}


string Restaurant::GetNameID()
{
	return nameID;
}

void Restaurant::clearOrderList()
{
	orderList.clear();
}

void Restaurant::printOrderListToFile()
{
	string fileBase = "./resourceFiles/";
	string fileName = fileBase + nameID + "_Orders.txt";
	std::ofstream ofs (fileName.c_str(), std::ofstream::out);
	for(int i=0; i<orderList.size(); i++)
	{
		ofs << orderList[i].getDate() << " ";
		ofs << orderList[i].getTime() << " ";
		ofs << orderList[i].GetMenuItem() << " ";
		ofs << orderList[i].getQuantity() << " ";
		ofs << orderList[i].GetRestaurantName();
		if(i != orderList.size()-1)
			ofs << "\n";
	}

	ofs.close();
	clearOrderList();
}
