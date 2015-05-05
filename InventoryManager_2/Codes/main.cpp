#include <iostream>
#include <string>
#include <fstream>
#include "RestaurantsManager.h"

using namespace std;

// Globals

int main (int argc, char **argv)
{
	// Reading the list of restaurants from file "./resourceFiles/RestaurantsList.txt". 
	// If you want to add any new restaurant. Add a unique name of Restaurant to this file.
	string fileName = "./resourceFiles/RestaurantsList.txt";
	ifstream restaurantListFile(fileName.c_str());
	if(!restaurantListFile)
	{
		printf("Restaurant's Name list is not available. FileName: %s\n", fileName.c_str());			// If the file is not present then we should show the error and return from there only.
		return 0;
	}

	// Loop through each restaurant name from the file "./resourceFiles/RestaurantsList.txt"
	while(!restaurantListFile.eof())
	{
		string restaurantName;

		restaurantListFile >> restaurantName;
		RestaurantsManager::GetRestaurantsManagerInstance()->addRestaurant(restaurantName);				// Initialize the restaurant manager and add the restaurant to it.
	}
	
	// Close the file "./resourceFiles/RestaurantsList.txt".
	restaurantListFile.close();																			

	// Ask for file name which has the orders for a given day. This file should be present in folder "./resourceFiles/...". 
	// Here I keep on looping this the name of the file is not NONE. Give the file name without any extension.
	printf("GIVE THE NAME OF ORDER FILE(without Extension) FOR DAY %d: ", RestaurantsManager::GetRestaurantsManagerInstance()->getTotalNumberOfDays()+1);
	cin >> fileName;
	while(fileName.compare("NONE"))
	{
		// Using the order filename place the order one by one.
		RestaurantsManager::GetRestaurantsManagerInstance()->placeTodaysOrders(fileName);
		
		// After the orders of current day, it is time to accumulate left ingredients from each restaurant.
		RestaurantsManager::GetRestaurantsManagerInstance()->accumulateLeftIngredients();
		
		// Print the Ingredients left at each restaurant to their corresponding files.
		RestaurantsManager::GetRestaurantsManagerInstance()->PrintLeftIngredientsOfAllRestaurantsToFile();
		
		// The order file given as input is having orders for all the restaurants. The call below will seggregate the orders to seperate files so that it can be used to calculate 
		// further stocking of resources.  
		RestaurantsManager::GetRestaurantsManagerInstance()->printOrderListForEachRestaurant();
		
		// Finally restock the ingredients based on the orders of previous days and what was left. Input file is the file where all orders of previous day are mentioned.
		string tempFileName = fileName + ".txt";
		RestaurantsManager::GetRestaurantsManagerInstance()->ReStockIngredientForEachRestaurant(tempFileName);
		cout << "Remaining ingredients of all Restaurants are saved to file: ./resourceFiles/Ingredients_left.txt" << endl;
		cout << "\n-----------------------------------------------------------------------\n";
		printf("GIVE THE NAME OF ORDER FILE(without Extension) FOR DAY %d: ", RestaurantsManager::GetRestaurantsManagerInstance()->getTotalNumberOfDays()+1);
		cin>>fileName;
	}

	printf("Thanks For using our system\n");
	system("pause");
}
