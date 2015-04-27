#include <iostream>
#include <string>
#include <fstream>
#include "RestaurantsManager.h"

using namespace std;

// Globals

int main (int argc, char **argv)
{
	string fileName = "./resourceFiles/RestaurantsList.txt";
	ifstream restaurantListFile(fileName.c_str());
	if(!restaurantListFile)
	{
		printf("Restaurant's Name list is not available. FileName: %s\n", fileName.c_str());
		return 0;
	}

	while(!restaurantListFile.eof())
	{
		string restaurantName;

		restaurantListFile >> restaurantName;
		RestaurantsManager::GetRestaurantsManagerInstance()->addRestaurant(restaurantName);
	}
	restaurantListFile.close();

	printf("GIVE THE NAME OF ORDER FILE(without Extension) FOR DAY %d: ", RestaurantsManager::GetRestaurantsManagerInstance()->getTotalNumberOfDays()+1);
	fileName;
	cin >> fileName;
	while(fileName.compare("NONE"))
	{
		RestaurantsManager::GetRestaurantsManagerInstance()->placeTodaysOrders(fileName);
		RestaurantsManager::GetRestaurantsManagerInstance()->accumulateLeftIngredients();
		RestaurantsManager::GetRestaurantsManagerInstance()->PrintLeftIngredientsOfAllRestaurantsToFile();
		RestaurantsManager::GetRestaurantsManagerInstance()->printOrderListForEachRestaurant();
		RestaurantsManager::GetRestaurantsManagerInstance()->ReStockIngredientForEachRestaurant(fileName + ".txt");
//		RestaurantsManager::GetRestaurantsManagerInstance()->accumulateLeftIngredients();
		cout << "Remaining ingredients of all Restaurants are saved to file: ./resourceFiles/Ingredients_left.txt" << endl;
		cout << "\n-----------------------------------------------------------------------\n";
		printf("GIVE THE NAME OF ORDER FILE(without Extension) FOR DAY %d: ", RestaurantsManager::GetRestaurantsManagerInstance()->getTotalNumberOfDays()+1);
		cin>>fileName;
	}

	printf("Thanks For using our system\n");
	system("pause");
}