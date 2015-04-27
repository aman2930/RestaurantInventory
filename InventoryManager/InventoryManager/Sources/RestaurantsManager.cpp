#include "RestaurantsManager.h"

map <string, MenuItem *> gMenuItemList;
map <string, int> gRemainingIngredients;

MenuItem * GetMenuItemFromMenuItemID(string id)
{
	map< string, MenuItem* >::iterator it;
	it = gMenuItemList.find(id);
	if(it != gMenuItemList.end())
		return it->second;
	
	return NULL;
}

void AddRemainingIngredientsToList(string ingredientId, int quantity)
{
	map< string, int >::iterator it;
	it = gRemainingIngredients.find(ingredientId);
	if(it != gRemainingIngredients.end())
		it->second = it->second + quantity;
	else
		gRemainingIngredients[ingredientId] = quantity;
}

void RestockIngredients(string &allOrderFile, string &prevDayOrderFile, string &averageIngredientRequirementTillPrevDay, string &ingredientsLeft, string &stockInventoryFile, int totalDays)
{
	/// Reading the average Ingredient required per day from the file and put it into the map for further calculation
	string fileName = averageIngredientRequirementTillPrevDay;
	ifstream averageIngredientPerDayFile(fileName.c_str());
	if(!averageIngredientPerDayFile)
	{
		printf("File does not exist. FileName: %s\n", fileName.c_str());
		return;
	}

	map <string , int> ingredientsRequirementCalc;
	map <string , int>::iterator it;

	while(!averageIngredientPerDayFile.eof())
	{
		string ingredientStr;
		int quantity;

		averageIngredientPerDayFile >> ingredientStr;
		averageIngredientPerDayFile >> quantity; 

		it = ingredientsRequirementCalc.find(ingredientStr);
		if(it != ingredientsRequirementCalc.end())
		{
			int temp = it->second + quantity*(totalDays-1);
			it->second = temp;
		}
		else
		{
			ingredientsRequirementCalc[ingredientStr] = quantity*(totalDays-1);
		}
	}
	averageIngredientPerDayFile.close();

	/// Reading the Previous day orders from the file and add it into the map for further calculation
	fileName = prevDayOrderFile;
	ifstream prevDayOrderFilePtr(fileName.c_str());
	if(!prevDayOrderFilePtr)
	{
		printf("File does not exist. FileName: %s\n", fileName.c_str());
		return;
	}
	
	while(!prevDayOrderFilePtr.eof())
	{
		int date;
		int time;
		string menuID;
		int itemQuantity;
		string restaurantName;

		prevDayOrderFilePtr >> date;
		prevDayOrderFilePtr >> time;
		prevDayOrderFilePtr >> menuID;
		prevDayOrderFilePtr >> itemQuantity;
		prevDayOrderFilePtr >> restaurantName;

		MenuItem* menuItem = GetMenuItemFromMenuItemID(menuID);
		vector< pair<string, int> > ingredientList = menuItem->getIngredientList();
		for(int i=0;i<ingredientList.size();i++)
		{
			string ingredientStr = ingredientList[i].first;
			int q1 = ingredientList[i].second * itemQuantity;

			it = ingredientsRequirementCalc.find(ingredientStr);
			if(it != ingredientsRequirementCalc.end())
			{
				int temp = it->second + q1;
				it->second = temp;
			}
			else
			{
				ingredientsRequirementCalc[ingredientStr] = q1;
			}
		}
	}
	prevDayOrderFilePtr.close();

	for(it=ingredientsRequirementCalc.begin();it!=ingredientsRequirementCalc.end();it++)
	{
		it->second = ceil((double)it->second/(double)totalDays);
	}

	// Save the average calculated to the file

	fileName = averageIngredientRequirementTillPrevDay;
	std::ofstream ofs1 (fileName.c_str(), std::ofstream::out);

	bool once = false;
	for(it=ingredientsRequirementCalc.begin();it!=ingredientsRequirementCalc.end();it++)
	{
		string ingredientStr = it->first;
		int quantityToBuy = it->second;
		if(quantityToBuy == 0)
			continue;

		if(once)
			ofs1 << endl;
		ofs1 << ingredientStr << " ";
		ofs1 << quantityToBuy;
		once = true;
	}
	ofs1.close();

	/// Subtracting the quntity of ingredients left from previous day and finally calculating ingredients to stock
	fileName = ingredientsLeft;
	ifstream ingredientsLeftFile(fileName.c_str());
	if(!ingredientsLeftFile)
	{
		printf("File does not exist. FileName: %s\n", fileName.c_str());
		return;
	}

	while(!ingredientsLeftFile.eof())
	{
		string ingredientStr;
		int quantity;

		ingredientsLeftFile >> ingredientStr;
		ingredientsLeftFile >> quantity; 

		it = ingredientsRequirementCalc.find(ingredientStr);
		if(it != ingredientsRequirementCalc.end())
		{
			int temp = it->second - quantity;
			if(temp < 0)
				temp = 0;
			it->second = temp;
		}
	}
	ingredientsLeftFile.close();

	fileName = stockInventoryFile;
	std::ofstream ofs (fileName.c_str(), std::ofstream::out);

	once = false;
	for(it=ingredientsRequirementCalc.begin();it!=ingredientsRequirementCalc.end();it++)
	{
		string ingredientStr = it->first;
		int quantityToBuy = it->second;
		if(quantityToBuy == 0)
			continue;

		if(once)
			ofs << endl;
		int expiryDays = 5;
		ofs << ingredientStr << " ";
		ofs << expiryDays << " ";
		ofs << quantityToBuy;
		once = true;
	}
	ofs.close();
}

RestaurantsManager* RestaurantsManager::rManager = NULL;

RestaurantsManager* RestaurantsManager::GetRestaurantsManagerInstance()
{
	if(rManager == NULL)
	{
		rManager = new RestaurantsManager();
	}
	return rManager;
}

bool RestaurantsManager::addRestaurant(string restaurantName)
{
	map< string, Restaurant* >::iterator it;
	it = restaurantLists.find(restaurantName);
	if(it != restaurantLists.end())
	{
		printf("Restaurant already exist.\n");
		return false;
	}
	else
	{
		Restaurant * r = new Restaurant(restaurantName, RestockIngredients);
		string fileBase = "./resourceFiles/";
		string fileName = fileBase + restaurantName + "_Menu.txt";

		ifstream file(fileName.c_str());
		if(!file)
		{
			printf("Restaurant Menu specific file does not exist. FileName: %s\n", fileName.c_str());
			return false;
		}
		
		string str;
		while(!file.eof())
		{
			file >> str;

			if(!GetMenuItemFromMenuItemID(str))
			{
				fileName = fileBase + "MenuItems/" + str + ".txt";
				ifstream menuItemFile(fileName.c_str());
				if(!menuItemFile)
				{
					printf("Menu Item specific file does not exist. FileName: %s\n", fileName.c_str());
					return false;
				}

				MenuItem *mItem = new MenuItem(str);
				while(!menuItemFile.eof())
				{
					int quantity;
					menuItemFile >> str;
					menuItemFile >> quantity;
					
					Ingredient *ing = new Ingredient(str);
					pair <string, int> ing_quantity(str, quantity);
					mItem->addModifyIngredient(ing_quantity);
					addNewMenuItem(mItem);
				}
				menuItemFile.close();
				r->addMenuItem(mItem);
			}
		}
		file.close();

		stockedFromIngredientFile(r);

		restaurantLists[restaurantName] = r;
	}
	return true;
}

void RestaurantsManager::stockedFromIngredientFile(Restaurant * r)
{
	string fileBase = "./resourceFiles/";
	string restaurantName = r->GetNameID();
	string fileName = fileBase + restaurantName + "_Ingredients_stoked.txt";
	ifstream ingredientFile(fileName.c_str());
	if(!ingredientFile)
	{
		printf("Restaurant Initial Ingredient specific file does not exist. FileName: %s\n", fileName.c_str());
		return;
	}

	while(!ingredientFile.eof())
	{
		string ingredientStr;
		int quantity;
		int expiryDays;

		ingredientFile >> ingredientStr;
		ingredientFile >> expiryDays;
		ingredientFile >> quantity; 

		r->addIngredient(ingredientStr, ExpiryDaysQuantity(expiryDays, quantity));
	}
	ingredientFile.close();
}

void RestaurantsManager::addNewMenuItem(MenuItem *m)
{
	gMenuItemList[m->getMenuId()] = m;
}

void RestaurantsManager::placeTodaysOrders(string orderFile)
{
	totalDays++;						// Increment the number of days restaurant started

	string fileBase = "./resourceFiles/";
	string fileName = fileBase + orderFile + ".txt";

	ifstream file(fileName.c_str());
	if(!file)
	{
		printf("Todays Order file does not exist. FileName: %s\n", fileName.c_str());
		return;
	}

	while(!file.eof())
	{
		int date;
		int time;
		string menuItem;
		int quantity;
		string restaurantName;

		file >> date;
		file >> time;
		file >> menuItem;
		file >> quantity;
		file >> restaurantName;

		Order *o = new Order(date, time, menuItem, quantity, restaurantName);

		map< string, Restaurant* >::iterator it;
		it = restaurantLists.find(restaurantName);
		if(it != restaurantLists.end())
		{
			bool orderPlaced = (it->second)->placeOrder(o);
			if(orderPlaced)
			{
				string fileName = fileBase + "FinalPlacedOrders.txt";
				std::ofstream ofs (fileName.c_str(), std::ofstream::out | std::ofstream::app);
				ofs << date << " ";
				ofs << time << " ";
				ofs << menuItem << " ";
				ofs << quantity << " ";
				ofs << restaurantName << "\n";
				ofs.close();
				cout << "ORDER PLACED: " << menuItem << " at " << restaurantName << endl;
			}
			else
				cout << "ORDER NOT PLACED: " << menuItem << " at " << restaurantName << endl;
		}
		else
			cout << restaurantName << " Not exist." << endl;
		delete o;
	}
	file.close();
}

void RestaurantsManager::accumulateLeftIngredients()
{
	gRemainingIngredients.clear();
	map <string, Restaurant *>::iterator it;
	for(it = restaurantLists.begin(); it != restaurantLists.end(); it++)
	{
		(it->second)->collectLeftIngredients();
	}
}

void RestaurantsManager::PrintLeftIngredientsOfAllRestaurantsToFile()
{
	string fileBase = "./resourceFiles/";
	string fileName = fileBase + "Ingredients_left.txt";
	std::ofstream ofs (fileName.c_str(), std::ofstream::out);

	map< string, int >::iterator it;
	for(it = gRemainingIngredients.begin(); it != gRemainingIngredients.end(); it++)
	{
		ofs << it->first << " ";
		ofs << it->second << endl;
	}
	ofs.close();
}

void RestaurantsManager::ReStockIngredientForEachRestaurant(string &allOrderFile)
{
	string fileBase = "./resourceFiles/";
	map <string, Restaurant *>::iterator it;
	for(it = restaurantLists.begin(); it != restaurantLists.end(); it++)
	{
		string restaurantName = (it->second)->GetNameID();
		string averageIngredientsRequirementTillPreviousDay = fileBase + restaurantName + "_Ingredients_Required.txt";
		string stockInventoryFile = fileBase + restaurantName + "_Ingredients_stoked.txt";
		string previousDayOrderFile = fileBase + restaurantName + "_Orders.txt";
		string ingredientsLeft = fileBase + restaurantName + "_Ingredients_left.txt";
		(it->second)->RestockIngredients_funct(allOrderFile, previousDayOrderFile, averageIngredientsRequirementTillPreviousDay, ingredientsLeft, stockInventoryFile, totalDays);

		cout << "Inventory of ingredients to be stocked for Restaurant(" << restaurantName << ") is present in file: " << stockInventoryFile << endl;

		stockedFromIngredientFile(it->second);
	}
}

void RestaurantsManager::printOrderListForEachRestaurant()
{
	map <string, Restaurant *>::iterator it;
	for(it = restaurantLists.begin(); it != restaurantLists.end(); it++)
	{
		(it->second)->printOrderListToFile();
	}
}

int RestaurantsManager::getTotalNumberOfDays()
{
	return totalDays;
}