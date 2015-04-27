#include "MenuItemIngrediants.h"

MenuItem::MenuItem(string name)
{
	nameID = name;
}

MenuItem::~MenuItem()
{
	ingredientsList.clear();
	// Destructor
}

void MenuItem::addModifyIngredient(pair <string, int> ing)
{
	for(int i = 0; i < ingredientsList.size(); i++)
	{
		if(ing.first.compare(ingredientsList[i].first) == 0)
		{
			ingredientsList[i].second = ing.second;
			return;
		}
	}
	ingredientsList.push_back(ing);
}

vector< pair <string, int> > MenuItem::getIngredientList()
{
	return ingredientsList;
}

string MenuItem::getMenuId()
{
	return nameID;
}

/*********************************************************************************/

Ingredient::Ingredient(string name)
{
	nameID = name;
}

Ingredient::~Ingredient()
{
	// Destructor
}

string Ingredient::GetIngredientID()
{
	return nameID;
}

/*********************************************************************************/
