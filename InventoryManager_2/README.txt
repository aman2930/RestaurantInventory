LIST OF COMMAND TO EXECUTE:
===========================
-	Install g++ compiler
-	Navigate to folder "Codes/"
-	Execute following commands:
	g++ -c main.cpp 
	g++ -c RestaurantsManager.cpp 
	g++ -c Restaurants.cpp 
	g++ -c MenuItemIngrediants.cpp 
	g++ -o finalExe main.o MenuItemIngrediants.o Restaurants.o RestaurantsManager.o 
	./finalExe 
	
-	Give the name of files of orders(without extension), like Orders, Orders_1 etc. These test files are already present there. To stop type "NONE" in the name of the file.	


INITIAL DESCRIPTION
===================
- 	For flexibility I have implemented the whole system using files. Here is the description of files used.
	RestaurantsList.txt  --> Format[RestaurantName(string)]. It will give the name of the restaurants.
	For each restaurant we should have files:
		*	<RestaurantName>_Menu.txt --> Format[MenuItemName(string)]
			For each menuItem name we should have a file:
				./MenuItems/<MenuItemName>.txt  --> Format [Ingredient Name, Ingredient quantity]
		*	<RestaurantName>_Ingredients_stocked.txt --> Format[ingredient name, expiry date, quantity]
		*	<RestaurantName>_Ingredients_Required.txt --> Format[ingredient name, quantity]. This file stores the average of ingredients required per day for a give restaurant.
		
	This will create the structure with restaurants, their menuItems with proper ingredients required and initial stock of ingredients.
	
-	Orders will be read from the file given by user, but it should have format[date(yyyymmdd), time(hhmm), Menu Item Name, Item quantity to order, restaurant Name]


DESCRIPTION OF CODE AND CONCEPTS USED
=====================================
-	RestaurantsManager is a singleton class which is taking care of adding the restaurants and its menu items. It is also used to place orders and restocking of ingredients.
-	I have declared a function pointer in each restaurant which take the function used for restocking the ingredients. 
-	Different files are used to provide run time flexibility and scalability. User can add any number of restaurants, menu items and ingredients using these files.
-	In my restocking algorithm function, I took the average of ingredients required till previous day and based on that I calculated how much ingredients I have to stock more. And these calculations are stored in the file <RestaurantName>_Ingredients_Required.txt.
-	Once the ingredient expired it is not further being used.
-	Ingredient items are sorted daily based on expirty date. This is done so that the items which are going to expire soon will be used first.