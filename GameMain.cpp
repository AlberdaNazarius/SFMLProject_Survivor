#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include "Location.h"
#include "Game.h"
#include "Button.h"
#include "Data.h"
#include "Character.h"
#include "UI.h"
#include "Container.h"
#include "Panel.h"
#include "Inventory.h"
#include "Menu.h"
#include "Craft.h"
#include "Label.h"
#include "Bar.h"

using namespace sf;
using namespace std;

enum Bars
{
	HungerBar,
	ThirstBar,
	EnergyBar,
	ConditionBar,
	WarmthBar,
};
Text eventsDescription;


#pragma region ChooseWhereToGo

string CaptionOfButton(int index)
{
	if (Data::GetEnvironment(index) == 1) return "Forest";
	if (Data::GetEnvironment(index) == 2) return "Lake";
	if (Data::GetEnvironment(index) == 3) return "River";
}

#pragma endregion

#pragma region Hunt

void HuntOveriwrite(int maxIndex, int spentEnergy, int indexOfEnvironment)
{
	map<int, string>::iterator it;
	srand(time(0));
	int randValue = rand() % 2;
	int minutes = 60 + rand() % 91;
	Character::ChangeEnergyLevel(spentEnergy);
	GeneralTime::AddTime(0, minutes);
	Character::ChangeHungerLevel(-10 * minutes / 60);
	Character::ChangeThirstLevel(-14 * minutes / 60);
	eventsDescription.setString("Energy" + to_string(spentEnergy) + "\nHunger" + to_string(-10 * minutes / 60) + "\nThirst" + to_string(-14 * minutes / 60));
	if (randValue)
	{
		Character::ChangeConditionLevel(-5);
		eventsDescription.setString("You were bitten by snake\nCondition-5\nEnergy" + to_string(spentEnergy) + "\nHunger" + to_string(-10 * minutes / 60) + "\nThirst" + to_string(-14* minutes / 60));
		cout << "Character::ChangeConditionLevel(-5);" << endl;
	}// because of beating by snake etc.
	randValue = rand() % 3;
	if (randValue)
	{
		if (indexOfEnvironment == 1)
		{
			Forest forest;
			it = forest.Hunt(maxIndex);
			Inventory::Change_Item("food", it->first);
			cout << it->second << "\nInventory::Change_Item('food', " << it->first << ");" << endl;
			eventsDescription.setString(it->second+"\nEnergy" + to_string(spentEnergy) + "\nHunger" + to_string(-10 * minutes / 60) + "\nThirst" + to_string(-14 * minutes / 60));
		}
		if (indexOfEnvironment == 2)
		{
			Lake lake;
			it = lake.Hunt(maxIndex);
			Inventory::Change_Item("food", it->first);
			cout << it->second << "\nInventory::Change_Item('food', " << it->first << ");" << endl;
			eventsDescription.setString(it->second + "\nEnergy" + to_string(spentEnergy) + "\nHunger" + to_string(-10 * minutes / 60) + "\nThirst" + to_string(-14 * minutes / 60));
		}
		if (indexOfEnvironment == 3)
		{
			River river;
			it = river.Hunt(maxIndex);
			Inventory::Change_Item("food",it->first);
			cout << it->second << "\nInventory::Change_Item('food', " << it->first << ");" << endl;
			eventsDescription.setString(it->second + "\nEnergy" + to_string(spentEnergy) + "\nHunger" + to_string(-10* minutes / 60) + "\nThirst" + to_string(-14 * minutes / 60));
		}
	}
	else
	{
		eventsDescription.setString("It's nothing hunted\nEnergy" + to_string(spentEnergy) + "\nHunger" + to_string(-10 * minutes / 60) + "\nThirst" + to_string(-14 * minutes / 60));
		cout << "It's nothing hunted" << endl;
	}
}

#pragma endregion

#pragma region ExploreArea

void UseAxe()
{
	srand(time(0));
	if (8 - Inventory::checkClickExploreArea > 0) Inventory::wood += 3 + rand() % 8 - Inventory::checkClickExploreArea;
		Character::ChangeEnergyLevel(-30);
		GeneralTime::AddTime(1, 0);
		Character::ChangeHungerLevel(-10);
		Character::ChangeThirstLevel(-14);
}
void DontUseAxe(Container* container)
{
	container->setVisible(false);
	Character::ChangeEnergyLevel(-15);
	GeneralTime::AddTime(0, 30);
	Character::ChangeHungerLevel(-5);
	Character::ChangeThirstLevel(-7);
}
#pragma endregion

#pragma region StartFire

void StayAtFireSetVisible(Container* cont, bool value, int minutes, int takenEnergy)
{
	cont->setVisible(value);
	GeneralTime::AddTime(0, minutes);
	Character::ChangeEnergyLevel(takenEnergy);
	Character::ChangeHungerLevel(-10 * minutes / 60);
	Character::ChangeThirstLevel(-14 * minutes / 60);
	Character::SetStayAtFire(true);
	if (-10 * minutes / 60) eventsDescription.setString("You have more than 2 hours until fire burns out\nEnergy" + to_string(takenEnergy) + "\nHunger" + to_string(-10*minutes/60) + "\nThirst" + to_string(-14*minutes/60));
	else eventsDescription.setString("You have more than 2 hours until fire burns out\nEnergy" + to_string(takenEnergy) + "\nThirst" + to_string(-14 * minutes / 60));
}
void ContainerSetVisible(Container* cont, bool value)
{
	cont->setVisible(value);
}

#pragma endregion

#pragma region OpenInventory

void InventorySetVisible(Container* cont, bool value)
{
	cont->setVisible(value);
}

#pragma endregion

#pragma region Menu

void CallMenu(RenderWindow& window, Menu& obj)
{
	window.create(sf::VideoMode(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height), "Menu"/*, sf::Style::Fullscreen*/);
	obj.Draw();
}

#pragma endregion

int main()
{
	srand(time(0));
	const int windowWidth = VideoMode::getDesktopMode().width;
	const int windowHeight = VideoMode::getDesktopMode().height;
	//const int windowWidth = 700;
	//const int windowHeight = 700;

	Menu menuObj;
	menuObj.Draw();

	if (Menu::OpenMainWindow())
	{
		//RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Game");
		RenderWindow window(sf::VideoMode(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height), "Game"/*, sf::Style::Fullscreen*/);
		Event event;
		Location location;

		Game::errorTexture.setRepeated(true);
		if (!Game::errorTexture.loadFromFile("Pictures/Button.png"))
			throw std::invalid_argument("Failed to load ErrorTexture.png");
		if (!Game::defFont.loadFromFile("Fonts/Roboto-Regular.ttf"))
			throw std::invalid_argument("Failed to load Roboto-Light.ttf");
		Game::defIdleBut = Game::errorTexture;
		Game::defClickedBut = Game::errorTexture;
		Game::defHoverBut = Game::errorTexture;
		if (!Game::defPanel.loadFromFile("Pictures/DefaultPanel.png"))
		{
			Game::defPanel = Game::errorTexture;
			Game::defPanel.setRepeated(true);
		}
		Container* MainContainer = new Container;
		Panel* MainPanel = new Panel(Vector2f((float)0, (float)windowHeight * 0.8), windowWidth, windowHeight * 0.2);
		MainContainer->addChild(MainPanel);

		// consts
		const Vector2f hungerIcoSize(40, 55);
		const Vector2f energyIcoSize(35, 55);
		const Vector2f thirstIcoSize(25, 60);


#pragma region Character

		Container* CharacterContainer = new Container;
		Panel* Character = new Panel("Pictures/Character.png", Vector2f((float)windowWidth / 4, (float)windowHeight / 2.85), windowWidth / 5.8, windowHeight / 2.4);

		CharacterContainer->addChild(Character);

#pragma endregion

#pragma region Events


		Panel* EventsPanel = new Panel("Pictures/defaultPanelForText.png", Vector2f(MainPanel->getWidth() / 32, MainPanel->getHeight() / 21.6), MainPanel->getWidth() / 2.29, MainPanel->getHeight() / 1.07);

		MainPanel->addChild(EventsPanel);

		Font font;
		font.loadFromFile("CALIBRI.TTF");

		eventsDescription.setFont(font);
		eventsDescription.setString("");
		eventsDescription.setCharacterSize(windowHeight / 50);
		eventsDescription.setPosition(Vector2f(windowWidth / 20, windowHeight / 1.2));
		eventsDescription.setFillColor(Color::Black);

		Text characteristics;
		characteristics.setFont(font);
		characteristics.setCharacterSize(windowHeight / 50);
		characteristics.setPosition(Vector2f(windowWidth / 8, windowHeight / 1.2));
		characteristics.setFillColor(Color::Black);

		Text warmthAndConditionDuringGame;
		warmthAndConditionDuringGame.setFont(font);
		warmthAndConditionDuringGame.setCharacterSize(windowHeight / 50);
		warmthAndConditionDuringGame.setPosition(Vector2f(windowWidth / 2.5, windowHeight / 1.2));
		warmthAndConditionDuringGame.setFillColor(Color::Black);

#pragma endregion

#pragma region StartFire

		SpriteWithText changeCharacteristicStayAtFire[] =
		{
			SpriteWithText("Pictures/Characteristics/Energy.png", "+10", Color::White, Vector2f(20, 40)),
			SpriteWithText("Pictures/Characteristics/Hunger.png", " -5", Color::White, Vector2f(25, 40)),
			SpriteWithText("Pictures/Characteristics/Thirst.png", " -7", Color::White, Vector2f(15, 43)),
			SpriteWithText("Pictures/Characteristics/Warmth.png", "+60", Color::White, Vector2f(30, 40)),
			SpriteWithText("Pictures/Characteristics/Condition.png", "???", Color::White, Vector2f(35, 35)),
		};

		SpriteWithText changeCharacteristicsStayAtFire[] =
		{
			SpriteWithText("Pictures/Characteristics/Energy.png", "???", Color::White, energyIcoSize),
			SpriteWithText("Pictures/Characteristics/Hunger.png", "???", Color::White, hungerIcoSize),
			SpriteWithText("Pictures/Characteristics/Thirst.png", "???", Color::White, thirstIcoSize),
		};

		for (int i = 0; i < 5; i++)
			changeCharacteristicStayAtFire[i].SetTextSize(22);

		Container* StartFireContainer = new Container;
		Panel* StartFirePanel = new Panel(Vector2f((float)windowWidth / 10, (float)windowHeight / 10), windowWidth * 0.8, windowHeight * 0.6);
		StartFireContainer->setVisible(false);

		Button<void(*)(Container*, bool)>* StartFire = new Button<void(*)(Container*, bool)>(Vector2f((float)MainPanel->getWidth() / 2, (float)MainPanel->getHeight() / 54), Vector2f((float)MainPanel->getWidth() / 6.4, (float)MainPanel->getHeight() / 2.16), "Start fire");
		StartFire->setDelegate(ContainerSetVisible);

		Container* StayAtFireContainer = new Container;
		Panel* StayAtFirePanel = new Panel("Pictures/transparent.png", Vector2f((float)windowWidth / 2.5, (float)windowHeight / 1.7), windowWidth / 10.5, windowHeight / 5.6);

		if (Character::IsStayAtFire())
		{
			StayAtFireContainer->setVisible(true);
			StayAtFireContainer->setActive(true);
		}
		else
			StayAtFireContainer->setVisible(false);

		Button<void(*)(int)>* StayAtFire = new Button<void(*)(int)>("Pictures/Fire.png", Vector2f((float)StayAtFirePanel->getWidth() * 0, (float)StayAtFirePanel->getHeight() * 0), Vector2f(StayAtFirePanel->getWidth(), StayAtFirePanel->getHeight()), "Stay at fire");
		StayAtFire->setDelegate(Character::ChangeWarmthLevel);

		Button<void(*)(Container*, bool, int, int)>* FireLighter = new Button<void(*)(Container*, bool, int, int)>(Vector2f((float)StartFirePanel->getWidth()/426.5, (float)StartFirePanel->getHeight() / 2.36), Vector2f(StartFirePanel->getWidth() / 5.12, StartFirePanel->getHeight() / 6.48), "Lighter");
		FireLighter->setDelegate(StayAtFireSetVisible);

		Button<void(*)(Container*, bool, int, int)>* FireStone = new Button<void(*)(Container*, bool, int, int)>(Vector2f((float)StartFirePanel->getWidth() / 4.94, (float)StartFirePanel->getHeight() / 2.36), Vector2f(StartFirePanel->getWidth() / 5.12, StartFirePanel->getHeight() / 6.48), "Flint and Stone");
		FireStone->setDelegate(StayAtFireSetVisible);

		Button<void(*)(Container*, bool, int, int)>* FireMatches = new Button<void(*)(Container*, bool, int, int)>(Vector2f((float)StartFirePanel->getWidth() / 2.49, (float)StartFirePanel->getHeight() / 2.36), Vector2f(StartFirePanel->getWidth() / 5.12, StartFirePanel->getHeight() / 6.48), "Matches");
		FireMatches->setDelegate(StayAtFireSetVisible);

		Button<void(*)(Container*, bool, int, int)>* FireBow = new Button<void(*)(Container*, bool, int, int)>(Vector2f((float)StartFirePanel->getWidth() / 1.66, (float)StartFirePanel->getHeight() / 2.36), Vector2f(StartFirePanel->getWidth() / 5.12, StartFirePanel->getHeight() / 6.48), "Bow method");
		FireBow->setDelegate(StayAtFireSetVisible);

		Button<void(*)(Container*, bool, int, int)>* FireLens = new Button<void(*)(Container*, bool, int, int)>(Vector2f((float)StartFirePanel->getWidth() / 1.25, (float)StartFirePanel->getHeight() / 2.36), Vector2f(StartFirePanel->getWidth() / 5.12, StartFirePanel->getHeight() / 6.48), "Lens");
		FireLens->setDelegate(StayAtFireSetVisible);

		StartFireContainer->addChild(StartFirePanel);

		StartFirePanel->addChild(FireStone);
		StartFirePanel->addChild(FireBow);
		StartFirePanel->addChild(FireLighter);
		StartFirePanel->addChild(FireLens);
		StartFirePanel->addChild(FireMatches);

		if (!Inventory::Check_Tool("lighter")) FireLighter->setVisible(false);
		if (!Inventory::Check_Tool("lens")) FireLens->setVisible(false);
		if (!Inventory::Check_Tool("matches")) FireMatches->setVisible(false);

		StayAtFireContainer->addChild(StayAtFirePanel);

		StayAtFirePanel->addChild(StayAtFire);

#pragma endregion 

#pragma region ExploreArea

		string value;

		SpriteWithText changeCharacteristicsUseAxe[] =
		{
			SpriteWithText("Pictures/Characteristics/Energy.png", "-30", Color::White, energyIcoSize),
			SpriteWithText("Pictures/Characteristics/Hunger.png", "-10", Color::White, hungerIcoSize),
			SpriteWithText("Pictures/Characteristics/Thirst.png", "-14", Color::White, thirstIcoSize),
		};

		SpriteWithText changeCharacteristicsDontUseAxe[] =
		{
			SpriteWithText("Pictures/Characteristics/Energy.png", "-15", Color::White, energyIcoSize),
			SpriteWithText("Pictures/Characteristics/Hunger.png", "-5", Color::White, hungerIcoSize),
			SpriteWithText("Pictures/Characteristics/Thirst.png", "-7", Color::White, thirstIcoSize),
		};

		Button<string(*)(int)>* ExploreArea = new Button<string(*)(int)>(Vector2f((float)MainPanel->getWidth() / 2, (float)MainPanel->getHeight() / 1.93), Vector2f((float)MainPanel->getWidth() / 6.4, (float)MainPanel->getHeight() / 2.16), "Explore Area");
		ExploreArea->setDelegate(Inventory::ExploreArea);

		Container* IsUsedAxeContainer = new Container;
		Panel* IsUsedAxePanel = new Panel(Vector2f((float)windowWidth / 10, (float)windowHeight / 10), windowWidth * 0.8, windowHeight * 0.6);
		IsUsedAxeContainer->setVisible(false);

		Button<void(*)()>* UseAxeButton = new Button<void(*)()>(Vector2f((float)IsUsedAxePanel->getWidth() / 4, (float)IsUsedAxePanel->getHeight() / 2.5), Vector2f(StartFirePanel->getWidth() / 5.12, StartFirePanel->getHeight() / 6.48), "Use Axe");
		UseAxeButton->setDelegate(UseAxe);

		Button<void(*)(Container*)>* DontUseAxeButton = new Button<void(*)(Container*)>(Vector2f((float)IsUsedAxePanel->getWidth() / 1.8, (float)IsUsedAxePanel->getHeight() / 2.5), Vector2f(StartFirePanel->getWidth() / 5.12, StartFirePanel->getHeight() / 6.48), "Don't Use Axe");
		DontUseAxeButton->setDelegate(DontUseAxe);	

		IsUsedAxeContainer->addChild(IsUsedAxePanel);

		IsUsedAxePanel->addChild(UseAxeButton);
		IsUsedAxePanel->addChild(DontUseAxeButton);

#pragma endregion

#pragma region ChooseWhereToGo
	
		SpriteWithText changeCharacteristicsChooseWhereToGo[] =
		{
			SpriteWithText("Pictures/Characteristics/Energy.png", "-15", Color::White, Vector2f(20, 35)),
			SpriteWithText("Pictures/Characteristics/Hunger.png", "-10", Color::White, Vector2f(20, 35)),
			SpriteWithText("Pictures/Characteristics/Thirst.png", "-15", Color::White, Vector2f(13, 40)),
		};

		Container* ChooseWhereToGoContainer = new Container;
		Panel* ChooseWhereToGoPanel = new Panel ("Pictures/transparent.png", Vector2f(0, (float)windowHeight / 7), windowWidth, windowHeight * 0.07);
		
		Button<void(*)(int)>* FirstVariantToTravel = new Button<void(*)(int)>("Pictures/Arrow.png", Vector2f((float)ChooseWhereToGoPanel->getWidth() / 100, (float)0), Vector2f(ChooseWhereToGoPanel->getWidth() * 0.1, ChooseWhereToGoPanel->getHeight()), "Forest");
		FirstVariantToTravel->setDelegate(Location::CheckWhatEnvironment);
		FirstVariantToTravel->setVisible(true);

		Button<void(*)(int)>* SecondVariantToTravel = new Button<void(*)(int)>("Pictures/Arrow.png", Vector2f((float)ChooseWhereToGoPanel->getWidth() / 2.3, (float)0), Vector2f(ChooseWhereToGoPanel->getWidth() * 0.1, ChooseWhereToGoPanel->getHeight()), "Lake");
		SecondVariantToTravel->setDelegate(Location::CheckWhatEnvironment);
		SecondVariantToTravel->setVisible(true);

		Button<void(*)(int)>* ThirdVariantToTravel = new Button<void(*)(int)>("Pictures/Arrow.png", Vector2f((float)ChooseWhereToGoPanel->getWidth() / 1.12, (float)0), Vector2f(ChooseWhereToGoPanel->getWidth() * 0.1, ChooseWhereToGoPanel->getHeight()), "River");
		ThirdVariantToTravel->setDelegate(Location::CheckWhatEnvironment);
		ThirdVariantToTravel->setVisible(true);		

		ChooseWhereToGoContainer->addChild(ChooseWhereToGoPanel);

		ChooseWhereToGoPanel->addChild(FirstVariantToTravel);
		ChooseWhereToGoPanel->addChild(SecondVariantToTravel);
		ChooseWhereToGoPanel->addChild(ThirdVariantToTravel);

#pragma endregion

#pragma region Hunt

		SpriteWithText changeCharacteristicsHunt[] =
		{
			SpriteWithText("Pictures/Characteristics/Energy.png", "  -5", Color::White, energyIcoSize),
			SpriteWithText("Pictures/Characteristics/Hunger.png", "???", Color::White, hungerIcoSize),
			SpriteWithText("Pictures/Characteristics/Thirst.png", "???", Color::White, thirstIcoSize),
		};

		Container* HuntContainer = new Container;
		Panel* HuntPanel = new Panel(Vector2f((float)windowWidth / 10, (float)windowHeight / 10), windowWidth * 0.8, windowHeight * 0.6);
		HuntContainer->setVisible(false);

		Button<void(*)(Container*, bool)>* HuntButton = new Button<void(*)(Container*, bool)>(Vector2f((float)MainPanel->getWidth() / 1.5, (float)MainPanel->getHeight() / 1.93), Vector2f((float)MainPanel->getWidth() / 6.4, (float)MainPanel->getHeight() / 2.16) , "Hunt");
		HuntButton->setDelegate(ContainerSetVisible);

		Button<void(*)(int, int, int)>* FallTrap = new Button<void(*)(int, int, int)>(Vector2f((float)HuntPanel->getWidth() / 14.5, (float)HuntPanel->getHeight() / 2.5), Vector2f(StartFirePanel->getWidth() / 5.12, StartFirePanel->getHeight() / 6.48), "Fall Trap");
		FallTrap->setDelegate(HuntOveriwrite);

		Button<void(*)(int, int, int)>* SpringTrap = new Button<void(*)(int, int, int)>(Vector2f((float)HuntPanel->getWidth() / 2.49, (float)HuntPanel->getHeight() / 2.5), Vector2f(StartFirePanel->getWidth() / 5.12, StartFirePanel->getHeight() / 6.48), "Spring Trap");
		SpringTrap->setDelegate(HuntOveriwrite);

		Button<void(*)(int, int, int)>* BirdTrap = new Button<void(*)(int, int, int)>(Vector2f((float)HuntPanel->getWidth() / 1.36, (float)HuntPanel->getHeight() / 2.5), Vector2f(StartFirePanel->getWidth() / 5.12, StartFirePanel->getHeight() / 6.48), "Birds Trap");
		BirdTrap->setDelegate(HuntOveriwrite);

		HuntContainer->addChild(HuntPanel);

		HuntPanel->addChild(FallTrap);
		HuntPanel->addChild(SpringTrap);
		HuntPanel->addChild(BirdTrap);

		SpriteWithText changeCharacteristicsFish[] =
		{
			SpriteWithText("Pictures/Characteristics/Energy.png", "???", Color::White, energyIcoSize),
			SpriteWithText("Pictures/Characteristics/Hunger.png", "???", Color::White, hungerIcoSize),
			SpriteWithText("Pictures/Characteristics/Thirst.png", "???", Color::White, thirstIcoSize),
		};

		Container* FishContainer = new Container;
		Panel* FishPanel = new Panel(Vector2f((float)windowWidth / 10, (float)windowHeight / 10), windowWidth * 0.8, windowHeight * 0.6);
		FishContainer->setVisible(false);

		Button<void(*)(Container*, bool)>* FishButton = new Button<void(*)(Container*, bool)>(Vector2f((float)MainPanel->getWidth() / 1.5, (float)MainPanel->getHeight() / 1.93), Vector2f((float)MainPanel->getWidth() /6.4, (float)MainPanel->getHeight() / 2.16), "Fish");
		FishButton->setDelegate(ContainerSetVisible);

		Button<void(*)(int, int, int)>* FishingRod = new Button<void(*)(int, int, int)>(Vector2f((float)FishPanel->getWidth() / 4, (float)FishPanel->getHeight() / 2.5), Vector2f(StartFirePanel->getWidth() / 5.12, StartFirePanel->getHeight() / 6.48), "Fishing Rod");
		FishingRod->setDelegate(HuntOveriwrite);

		Button<void(*)(int, int, int)>* SpearFishing = new Button<void(*)(int, int, int)>(Vector2f((float)FishPanel->getWidth() / 1.8, (float)FishPanel->getHeight() / 2.5), Vector2f(StartFirePanel->getWidth() / 5.12, StartFirePanel->getHeight() / 6.48), "Spear Fishing");
		SpearFishing->setDelegate(HuntOveriwrite);

		FishContainer->addChild(FishPanel);

		FishPanel->addChild(FishingRod);
		FishPanel->addChild(SpearFishing);

		if (!Inventory::Check_Tool("fishing rod")) FishingRod->setVisible(false);
		if (!Inventory::Check_Tool("spear")) SpearFishing->setVisible(false);

		if (Location::LocationCurrent != 1)
		{
			HuntButton->setVisible(false);
			FishButton->setVisible(true);
		}
		if (Location::LocationCurrent == 1)
		{
			FishButton->setVisible(false);
			HuntButton->setVisible(true);
		}

#pragma endregion

#pragma region Rest
		SpriteWithText changeCharacteristicsRest[] =
		{
			SpriteWithText("Pictures/Characteristics/Energy.png", "???", Color::White, Vector2f(20, 40)),
			SpriteWithText("Pictures/Characteristics/Hunger.png", "???", Color::White, Vector2f(25, 40)),
			SpriteWithText("Pictures/Characteristics/Thirst.png", "???", Color::White, Vector2f(15, 43)),
			SpriteWithText("Pictures/Characteristics/Warmth.png", "???", Color::White, Vector2f(30, 40)),
			SpriteWithText("Pictures/Characteristics/Condition.png", " ?", Color::White, Vector2f(35, 35)),
		};

		Button<void(*)(Container*, bool)>* RestButton = new Button<void(*)(Container*, bool)>(Vector2f((float)MainPanel->getWidth() / 1.5, (float)MainPanel->getHeight() / 54), Vector2f((float)MainPanel->getWidth() / 6.4, (float)MainPanel->getHeight() / 2.16), "Rest");
		RestButton->setDelegate(ContainerSetVisible);

		Container* RestContainer = new Container;
		Panel* RestPanel = new Panel(Vector2f((float)windowWidth / 10, (float)windowHeight / 10), windowWidth * 0.8, windowHeight * 0.6);
		RestContainer->setVisible(false);

		Button<void(*)(int)>* RestThreeHours = new Button<void(*)(int)>(Vector2f((float)HuntPanel->getWidth() / 14.5, (float)HuntPanel->getHeight() / 2.5), Vector2f(StartFirePanel->getWidth() / 5.12, StartFirePanel->getHeight() / 6.48), "3 hours");
		RestThreeHours->setDelegate(Character::Rest);

		Button<void(*)(int)>* RestSixHours = new Button<void(*)(int)>(Vector2f((float)HuntPanel->getWidth() / 2.49, (float)HuntPanel->getHeight() / 2.5), Vector2f(StartFirePanel->getWidth() / 5.12, StartFirePanel->getHeight() / 6.48), "6 hours");
		RestSixHours->setDelegate(Character::Rest);

		Button<void(*)(int) >* RestEightHours = new Button<void(*)(int)>(Vector2f((float)HuntPanel->getWidth() / 1.36, (float)HuntPanel->getHeight() / 2.5), Vector2f(StartFirePanel->getWidth() / 5.12, StartFirePanel->getHeight() / 6.48), "8 hours");
		RestEightHours->setDelegate(Character::Rest);

		RestContainer->addChild(RestPanel);
		RestPanel->addChild(RestThreeHours);
		RestPanel->addChild(RestSixHours);
		RestPanel->addChild(RestEightHours);

#pragma endregion

#pragma region EatAndDrink
		SpriteWithText changeCharacteristicHunger = SpriteWithText("Pictures/Characteristics/Hunger.png", "+5", Color::White, Vector2f(15, 22));
		SpriteWithText changeCharacteristicThirst = SpriteWithText("Pictures/Characteristics/Thirst.png", "+5", Color::White, Vector2f(8, 22));
		SpriteWithText changeCharacteristicCondition = SpriteWithText("Pictures/Characteristics/Condition.png", "+5", Color::White, Vector2f(19, 19));
		changeCharacteristicHunger.SetTextSize(15);
		changeCharacteristicThirst.SetTextSize(15);
		changeCharacteristicCondition.SetTextSize(15);

		Button<void(*)(int)>* EatButton = new Button<void(*)(int)>(Vector2f(windowWidth * 0.55, windowHeight / 6.9), Vector2f(windowWidth / 40, windowHeight / 40), "Use");
		EatButton->setDelegate(Character::ChangeHungerLevel);

		Button<void(*)(int)>* DrinkButton = new Button<void(*)(int)>(Vector2f(windowWidth * 0.55, windowHeight / 5.8), Vector2f(windowWidth / 40, windowHeight / 40), "Use");
		DrinkButton->setDelegate(Character::ChangeThirstLevel);

		Button<void(*)(int)>* CosumeMedicineButton = new Button<void(*)(int)>(Vector2f(windowWidth * 0.55, windowHeight / 4.25), Vector2f(windowWidth / 40, windowHeight / 40), "Use");
		CosumeMedicineButton->setDelegate(Character::ChangeConditionLevel);

#pragma endregion

#pragma region OpenInventory

		Container* OpenInventoryContainer = new Container;
		Panel* OpenInventoryPanel = new Panel(Vector2f((float)windowWidth / 10, (float)windowHeight / 10), windowWidth * 0.8, windowHeight * 0.6);
		OpenInventoryContainer->setVisible(false);

		Button<void(*)(Container*, bool)>* OpenInventory = new Button<void(*)(Container*, bool)>(Vector2f((float)MainPanel->getWidth() / 1.2, (float)MainPanel->getHeight() / 1.93), Vector2f((float)MainPanel->getWidth() / 6.4, (float)MainPanel->getHeight() / 2.16), "OpenInventory");
		OpenInventory->setDelegate(InventorySetVisible);

		OpenInventoryContainer->addChild(OpenInventoryPanel);
		OpenInventoryPanel->addChild(EatButton);
		OpenInventoryPanel->addChild(DrinkButton);
		OpenInventoryPanel->addChild(CosumeMedicineButton);

#pragma endregion

#pragma region Menu

		Container* OpenMenuContainer = new Container;
		Panel* OpenMenuPanel = new Panel("Pictutes/transparent.png", Vector2f((float)windowWidth / 1.07, 0), windowWidth * 0.07, windowHeight * 0.07);

		Button<void(*)(RenderWindow&, Menu&)>* OpenMenu = new Button<void(*)(RenderWindow&, Menu&)>(Vector2f(0, 0), Vector2f(OpenMenuPanel->getWidth(), OpenMenuPanel->getHeight()), "Menu");
		OpenMenu->setDelegate(CallMenu);

		OpenMenuContainer->addChild(OpenMenuPanel);
		OpenMenuPanel->addChild(OpenMenu);

#pragma endregion

#pragma region LostTheGame
		Container* FailMenu = new Container;
		Panel* FailPanel = new Panel(Vector2f((float)windowWidth / 10, (float)windowHeight / 10), windowWidth * 0.8, windowHeight * 0.6);

		Button<void(*)()>* Restart = new Button<void(*)()>(Vector2f((float)FailPanel->getWidth() / 4, (float)FailPanel->getHeight() / 2.5), Vector2f(FailPanel->getWidth() / 5.12, FailPanel->getHeight() / 6.48), "Restart");
		Restart->setDelegate(Data::SetDeffaultCharacteristics);

		Button<void(*)(RenderWindow&, Menu&)>* OpenMainMenu = new Button<void(*)(RenderWindow&, Menu&)>(Vector2f((float)FailPanel->getWidth() / 1.8, (float)FailPanel->getHeight() / 2.5), Vector2f(FailPanel->getWidth() / 5.12, FailPanel->getHeight() / 6.48), "Menu");
		OpenMainMenu->setDelegate(CallMenu);

		FailMenu->addChild(FailPanel);
		FailPanel->addChild(Restart);
		FailPanel->addChild(OpenMainMenu);
#pragma endregion


#pragma region Craft

		vector<Panel*> craftMenus;
		map<Button<bool(*)(string)>*, string> craftBut;
		map<Button<void(*)(vector<Panel*>*, Panel*)>*, Panel*> switchCraftBut;
		Button<void(*)(vector<Panel*>*, Panel*)>* openCraftBut = new Button<void(*)(vector<Panel*>*, Panel*)>(Vector2f((float)MainPanel->getWidth() / 1.2, (float)MainPanel->getHeight() / 54), Vector2f((float)MainPanel->getWidth() / 6.4, (float)MainPanel->getHeight() / 2.16), "Open craft menu");
		openCraftBut->setDelegate(Craft::changeCraftMenu);
		Craft::InitializeCraftMenus(&craftMenus, &craftBut, &switchCraftBut, MainContainer, Vector2f(windowWidth, windowHeight));
		MainPanel->addChild(openCraftBut);

#pragma endregion 

#pragma region Shelter

		Container* ShelterContainer = new Container;
		Panel* Shelter = new Panel("Pictures/Shelter.png", Vector2f((float)windowWidth / 2.23, (float)windowHeight / 2.45), windowWidth / 2.56, windowHeight / 2.4);

		ShelterContainer->addChild(Shelter);

#pragma endregion 

#pragma region StartFire

		MainPanel->addChild(StartFire);

#pragma endregion 

#pragma region ExploreArea

		MainPanel->addChild(ExploreArea);

#pragma endregion

#pragma region Hunt

		MainPanel->addChild(HuntButton);
		MainPanel->addChild(FishButton);

#pragma endregion

#pragma region Rest

		MainPanel->addChild(RestButton);

#pragma endregion

#pragma region DisplayStats
		Container* DisplayContainer = new Container;

		double timeCoefficient[] = {20, 20};
		double temperatureCoefficient[] = {6, 20};
		double characteristicsCoefficient[] = {50, 2.5};

		int barWidth = 150;
		int barHeitght = 50; 
		int distance = 60;

		Label* TimeLabel = new Label(Vector2f(windowWidth / timeCoefficient[0], windowHeight / timeCoefficient[1]), GeneralTime::GetTime(), 30, Color::White);

		TemperatureBar temperatureBar("Pictures/TemperatureScale/TemperatureHot.png", "Pictures/TemperatureScale/TemperatureCold.png", 25, 75, to_string(Location::GetTemperature()) + "�C", Color::White);
		temperatureBar.SetPosition(windowWidth / temperatureCoefficient[0], windowHeight / temperatureCoefficient[1]);

		Bar bars[] = 
		{
			Bar("Pictures/Bars/Hunger/BarOutlineHunger.png", "Pictures/Bars/Hunger/ScaleHunger.png", "Pictures/Bars/Hunger/BackScaleHunger.png", barWidth, barHeitght, Character::GetHungerLevel(), Color::Black),
			Bar("Pictures/Bars/Thirst/BarOutlineThirst.png", "Pictures/Bars/Thirst/ScaleThirst.png", "Pictures/Bars/Thirst/BackScaleThirst.png", barWidth, barHeitght, Character::GetThirstLevel(), Color::Black),
			Bar("Pictures/Bars/Energy/BarOutlineEnergy.png", "Pictures/Bars/Energy/ScaleEnergy.png", "Pictures/Bars/Energy/BackScaleEnergy.png", barWidth, barHeitght, Character::GetEnergyLevel(), Color::Black),
			Bar("Pictures/Bars/Condition/BarOutlineCondition.png", "Pictures/Bars/Condition/ScaleCondition.png", "Pictures/Bars/Condition/BackScaleCondition.png", barWidth, barHeitght, Character::GetConditionLevel(), Color::Black),
			Bar("Pictures/Bars/Warmth/BarOutlineWarmth.png", "Pictures/Bars/Warmth/ScaleWarmth.png", "Pictures/Bars/Warmth/BackScaleWarmth.png", barWidth, barHeitght, Character::GetWarmthLevel(), Color::Black),
		};

		for (int i = 0; i < 5; i++)
			bars[i].SetPosition(windowWidth / characteristicsCoefficient[0], windowHeight / characteristicsCoefficient[1] + i * distance);
		DisplayContainer->addChild(TimeLabel);
		MainContainer->addChild(DisplayContainer);
#pragma endregion

#pragma region OpenInventory

		MainPanel->addChild(OpenInventory);

#pragma endregion

		int hours = GeneralTime::GetHours();
		int days = GeneralTime::GetDay();

		// Attaching pictures to environments
		Forest::SetPicture("Pictures/Environments/Forest.png");
		Lake::SetPicture("Pictures/Environments/Lake.png");
		River::SetPicture("Pictures/Environments/River.png");

		//Make elements inactive
		StartFireContainer->setVisible(false);
		OpenInventoryContainer->setVisible(false);
		HuntContainer->setVisible(false);
		FishContainer->setActive(false);
		IsUsedAxeContainer->setVisible(false);

		//Main cycle
		while (window.isOpen())
		{
			// Event
			while (window.pollEvent(event))
			{
				switch (event.type)
				{
				case Event::Closed:
					Data::SaveAllStaticData();
					window.close();
					break;

				case Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Left)
					{

						characteristics.setString("");
						eventsDescription.setString("");
						warmthAndConditionDuringGame.setString("");

#pragma region StartFire

						if (StartFire->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "StartFire->checkClick() == true" << endl;
							if (Inventory::wood < 5) eventsDescription.setString("You need at  least 5 iteams of Wood to Start Fire");
							else
							{
								StartFire->Action(StartFireContainer, !StartFireContainer->getVisible());
								cout << "StartFireContainer->setVisible(true);" << endl;
								Character::startedHour = GeneralTime::GetHours();
								Character::startedDay = GeneralTime::GetDay();
							}
							HuntContainer->setVisible(false);
							FishContainer->setVisible(false);
							IsUsedAxeContainer->setVisible(false);
							RestContainer->setVisible(false);
							OpenInventoryContainer->setVisible(false);
							for (Panel* x : craftMenus)
								x->setVisible(false);
							cout << "________________________________________" << endl;
						}
						if (FireLighter->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "FireLighter->checkClick() == true" << endl;
							FireLighter->Action(StayAtFireContainer, true, 5, -3);
							Inventory::Change_Item("wood", -5);
							cout << "StayAtFireContainer->setVisible(true);" << endl;
							Character::DisplayCharacteristics();
							StartFireContainer->setVisible(false);
						}
						if (FireStone->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "FireStone->checkClick() == true" << endl;
							FireStone->Action(StayAtFireContainer, true, 40, -20);
							Inventory::Change_Item("wood", -5);
							cout << "StayAtFireContainer->setVisible(true);" << endl;
							Character::DisplayCharacteristics();
							StartFireContainer->setVisible(false);
						}
						if (FireMatches->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "FireMatches->checkClick() == true" << endl;
							FireMatches->Action(StayAtFireContainer, true, 5, -5);
							Inventory::Change_Item("wood", -5);
							cout << "StayAtFireContainer->setVisible(true);" << endl;
							Character::DisplayCharacteristics();
							StartFireContainer->setVisible(false);
						}
						if (FireBow->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "FireBow->checkClick() == true" << endl;
							FireBow->Action(StayAtFireContainer, true, 60, -25);
							Inventory::Change_Item("wood", -5);
							cout << "StayAtFireContainer->setVisible(true);" << endl;
							Character::DisplayCharacteristics();
							StartFireContainer->setVisible(false);
						}
						if (FireLens->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "FireLens->checkClick() == true" << endl;
							FireLens->Action(StayAtFireContainer, true, 30, -15);
							Inventory::Change_Item("wood", -5);
							cout << "StayAtFireContainer->setVisible(true);" << endl;
							Character::DisplayCharacteristics();
							StartFireContainer->setVisible(false);
						}
						if (StayAtFire->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "StayAtFire->checkClick() == true" << endl;
							eventsDescription.setString("Warmth+60\nEnergy+10\nHunger-5\nThirst-7");

							int minutes = 60;
							GeneralTime::AddTime(0, minutes);
							Character::ChangeHungerLevel(minutes / 60 * -10);
							Character::ChangeThirstLevel(minutes / 60 * -14);
							StayAtFire->Action(minutes);
							Character::ChangeEnergyLevel(minutes / 6);
							cout << "Character::ChangeWarmthLevel(60);" << endl;
							Character::DisplayCharacteristics();
						}
#pragma endregion 

#pragma region ExploreArea

						if (ExploreArea->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "ExploreArea->checkClick() == true" << endl;
							if (IsUsedAxeContainer->getVisible() || !Inventory::Check_Tool("axe")) IsUsedAxeContainer->setVisible(false);
							else
							{
								IsUsedAxeContainer->setVisible(true);
								cout << "IsUsedAxeContainer->setVisible(true);" << endl;
							}
							if (!Inventory::Check_Tool("axe"))
							{
								Inventory::checkClickExploreArea++;
								value = ExploreArea->Action(Inventory::checkClickExploreArea);
								Character::ChangeEnergyLevel(-15);
								GeneralTime::AddTime(0, 30);
								Character::ChangeHungerLevel(-5);
								Character::ChangeThirstLevel(-7);
								eventsDescription.setString(value);
								if (eventsDescription.getString() != "") characteristics.setString("Energy-15\nHunger-5\nThirst-7");
								else eventsDescription.setString("Energy-15\nHunger-5\nThirst-7");
								FirstVariantToTravel->setText(CaptionOfButton(0));
								SecondVariantToTravel->setText(CaptionOfButton(1));
								ThirdVariantToTravel->setText(CaptionOfButton(2));
								cout << "Inventory::checkClickExploreArea++;" << endl
									<< "Inventory::ExploreArea(Inventory::checkClickExploreArea);" << endl
									<< "FirstVariantToTravel->setText(CaptionOfButton(0));\nSecondVariantToTravel->setText(CaptionOfButton(1));\nThirdVariantToTravel->setText(CaptionOfButton(2)); " << endl;
								Character::DisplayCharacteristics();
							}
							StartFireContainer->setVisible(false);
							StayAtFireContainer->setVisible(!IsUsedAxeContainer->getVisible());
							HuntContainer->setVisible(false);
							FishContainer->setVisible(false);
							RestContainer->setVisible(false);
							OpenInventoryContainer->setVisible(false);
							for (Panel* x : craftMenus)
								x->setVisible(false);
							cout << "________________________________________" << endl;
						}
						if (UseAxeButton->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "UseAxeButton->checkClick() == true" << endl;
							Inventory::checkClickExploreArea++;
							int previousValue = Inventory::wood;
							value = ExploreArea->Action(Inventory::checkClickExploreArea);
							UseAxeButton->Action();
							cout << "Inventory::checkClickExploreArea++;" << endl
								<< "Inventory::ExploreArea(Inventory::checkClickExploreArea);" << endl
								<< "Inventory::wood += 3 + rand() % 8" << endl
								<< "FirstVariantToTravel->setText(CaptionOfButton(0));\nSecondVariantToTravel->setText(CaptionOfButton(1));\nThirdVariantToTravel->setText(CaptionOfButton(2)); " << endl;
							Character::DisplayCharacteristics();
							if (Inventory::wood - previousValue>0)
							{
								characteristics.setString("Energy-30\nHunger-10\nThirst-14");
								eventsDescription.setString("Wood+" + to_string(Inventory::wood - previousValue) + "\n" +value);
							}
							else
							{
								eventsDescription.setString(value);
								if (eventsDescription.getString() != "") characteristics.setString("Energy-30\nHunger-10\nThirst-14");
								else eventsDescription.setString("Energy-30\nHunger-10\nThirst-14");
							}
							FirstVariantToTravel->setText(CaptionOfButton(0));
							SecondVariantToTravel->setText(CaptionOfButton(1));
							ThirdVariantToTravel->setText(CaptionOfButton(2));
							IsUsedAxeContainer->setVisible(false);
						}
						if (DontUseAxeButton->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "DontUseAxeButton->checkClick() == true" << endl;
							Inventory::checkClickExploreArea++;
							value = ExploreArea->Action(Inventory::checkClickExploreArea);
							FirstVariantToTravel->setText(CaptionOfButton(0));
							SecondVariantToTravel->setText(CaptionOfButton(1));
							ThirdVariantToTravel->setText(CaptionOfButton(2));
							DontUseAxeButton->Action(IsUsedAxeContainer);
							eventsDescription.setString(value);
							cout << "Inventory::checkClickExploreArea++;" << endl
								<< "Inventory::ExploreArea(Inventory::checkClickExploreArea);" << endl
								<< "FirstVariantToTravel->setText(CaptionOfButton(0));\nSecondVariantToTravel->setText(CaptionOfButton(1));\nThirdVariantToTravel->setText(CaptionOfButton(2)); " << endl;
							Character::DisplayCharacteristics();
							if (eventsDescription.getString() != "") characteristics.setString("Energy-15\nHunger-5\nThirst-7\n");
							else eventsDescription.setString("Energy-15\nHunger-5\nThirst-7");

						}

#pragma endregion

#pragma region ChooseWhereToGo

						if (FirstVariantToTravel->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "FirstVariantToTravel->checkClick() == true" << endl;
							srand(time(0));
							Inventory::checkClickExploreArea = 0;
							int posibilityOfBeingHurted = rand() % 2;
							FirstVariantToTravel->Action(Data::GetEnvironment(0));
							Location::LocationCurrent = Data::GetEnvironment(0);
							Character::ChangeEnergyLevel(-15);
							GeneralTime::AddTime(1, 0);
							Character::ChangeHungerLevel(-10);
							Character::ChangeThirstLevel(-14);
							string value = Inventory::LoseInventory();
							eventsDescription.setString(value);
							cout << "Inventory::checkClickExploreArea = 0;\nLocation::CheckWhatEnvironment(Data::GetEnvironment(0));\nLocation::LocationCurrent = Data::GetEnvironment(0);\nInventory::LoseInventory();" << endl;
							if (eventsDescription.getString() != "") characteristics.setString("Energy-15\nHunger-10\nThirst-14");
							else eventsDescription.setString("Energy-15\nHunger-10\nThirst-14");
							if (posibilityOfBeingHurted)
							{
								Character::ChangeConditionLevel(-5);
								eventsDescription.setString(value);
								if (eventsDescription.getString() != "") characteristics.setString("Energy-15\nHunger-10\nThirst-14\nYou were hurted during travelling\nCondition-5");
								else eventsDescription.setString("Energy-15\nHunger-10\nThirst-14\nYou were hurted during travelling\nCondition-5");
								cout << "Character::ChangeConditionLevel(-5);" << endl;
							}
							Character::DisplayCharacteristics();
							StartFireContainer->setVisible(false);
							HuntContainer->setVisible(false);
							FishContainer->setVisible(false);
							IsUsedAxeContainer->setVisible(false);
							StayAtFireContainer->setVisible(false);
							Location::Shelter = false;
							RestContainer->setVisible(false);
							OpenInventoryContainer->setVisible(false);
							for (Panel* x : craftMenus)
								x->setVisible(false);
						}
						if (SecondVariantToTravel->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "SecondVariantToTravel->checkClick() == true" << endl;
							srand(time(0));
							Inventory::checkClickExploreArea = 0;
							int posibilityOfBeingHurted = rand() % 2;
							SecondVariantToTravel->Action(Data::GetEnvironment(1));
							Location::LocationCurrent = Data::GetEnvironment(1);
							Character::ChangeEnergyLevel(-15);
							GeneralTime::AddTime(1, 0);
							Character::ChangeHungerLevel(-10);
							Character::ChangeThirstLevel(-14);
							string value = Inventory::LoseInventory();
							eventsDescription.setString(value);
							cout << "Inventory::checkClickExploreArea = 0;\nLocation::CheckWhatEnvironment(Data::GetEnvironment(1));\nLocation::LocationCurrent = Data::GetEnvironment(1);\nInventory::LoseInventory();" << endl;
							if (eventsDescription.getString() != "") characteristics.setString("Energy-15\nHunger-10\nThirst-14");
							else eventsDescription.setString("Energy-15\nHunger-10\nThirst-14");
							if (posibilityOfBeingHurted)
							{
								Character::ChangeConditionLevel(-5);
								//cout << "You were hurted during travelling" << endl;
								eventsDescription.setString(value);
								if (eventsDescription.getString() != "") characteristics.setString("Energy-15\nHunger-10\nThirst-14\nYou were hurted during travelling\nCondition-5");
								else eventsDescription.setString("Energy-15\nHunger-10\nThirst-14\nYou were hurted during travelling\nCondition-5");
								cout << "Character::ChangeConditionLevel(-5);" << endl;
							}
							Character::DisplayCharacteristics();
							StartFireContainer->setVisible(false);
							HuntContainer->setVisible(false);
							FishContainer->setVisible(false);
							IsUsedAxeContainer->setVisible(false);
							StayAtFireContainer->setVisible(false);
							Location::Shelter = false;
							RestContainer->setVisible(false);
							OpenInventoryContainer->setVisible(false);
							for (Panel* x : craftMenus)
								x->setVisible(false);
						}
						if (ThirdVariantToTravel->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "ThirdVariantToTravel->checkClick() == true" << endl;
							srand(time(0));
							Inventory::checkClickExploreArea = 0;
							int posibilityOfBeingHurted = rand() % 2;
							ThirdVariantToTravel->Action(Data::GetEnvironment(2));
							Location::LocationCurrent = Data::GetEnvironment(2);
							Character::ChangeEnergyLevel(-15);
							GeneralTime::AddTime(1, 0);
							Character::ChangeHungerLevel(-10);
							Character::ChangeThirstLevel(-14);
							string value = Inventory::LoseInventory();
							eventsDescription.setString(value);
							cout << "Inventory::checkClickExploreArea = 0;\nLocation::CheckWhatEnvironment(Data::GetEnvironment(2));\nLocation::LocationCurrent = Data::GetEnvironment(2);\nInventory::LoseInventory();" << endl;
							if (eventsDescription.getString() != "") characteristics.setString("Energy-15\nHunger-10\nThirst-14");
							else eventsDescription.setString("Energy-15\nHunger-10\nThirst-14");
							if (posibilityOfBeingHurted)
							{
								Character::ChangeConditionLevel(-5);
								//cout << "You were hurted during travelling" << endl;
								eventsDescription.setString(value);
								if (eventsDescription.getString() != "") characteristics.setString("Energy-15\nHunger-10\nThirst-14\nYou were hurted during travelling\nCondition-5");
								else eventsDescription.setString("Energy-15\nHunger-10\nThirst-14\nYou were hurted during travelling\nCondition-5");
								cout << "Character::ChangeConditionLevel(-5);" << endl;
							}
							Character::DisplayCharacteristics();
							StartFireContainer->setVisible(false);
							HuntContainer->setVisible(false);
							FishContainer->setVisible(false);
							IsUsedAxeContainer->setVisible(false);
							StayAtFireContainer->setVisible(false);
							Location::Shelter = false;
							RestContainer->setVisible(false);
							OpenInventoryContainer->setVisible(false);
							for (Panel* x : craftMenus)
								x->setVisible(false);
						}

#pragma endregion

#pragma region Hunt

						if (HuntButton->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "HuntButton->checkClick() == true" << endl;
							eventsDescription.setString("Max costs:\nEnergy-5\nHunger-25\nThirst-35");
							HuntButton->Action(HuntContainer, !HuntContainer->getVisible());
							cout << "HuntContainer->setVisible(true);" << endl;
							StayAtFireContainer->setVisible(!HuntContainer->getVisible());
							IsUsedAxeContainer->setVisible(false);
							StartFireContainer->setVisible(false);
							RestContainer->setVisible(false);
							OpenInventoryContainer->setVisible(false);
							for (Panel* x : craftMenus)
								x->setVisible(false);
							cout << "________________________________________" << endl;
						}
						if (FallTrap->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "FallTrap->checkClick() == true" << endl;
							cout << "Inventory::Change_Item('fall trap', 0);\nHuntOverwrite(3, -5, Location::LocationCurrent); " << endl;
							Inventory::Change_Item("fall trap", 0);
							FallTrap->Action(3, -5, Location::LocationCurrent);
							Character::DisplayCharacteristics();
							HuntContainer->setVisible(false);
						}
						if (SpringTrap->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "SpringTrap->checkClick() == true" << endl;
							cout << "Inventory::Change_Item('spring trap', 0);\nHuntOverwrite(3, -5, Location::LocationCurrent); " << endl;
							Inventory::Change_Item("spring trap", 0);
							SpringTrap->Action(3, -5, Location::LocationCurrent);
							Character::DisplayCharacteristics();
							HuntContainer->setVisible(false);
						}
						if (BirdTrap->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "BirdTrap->checkClick() == true" << endl;
							cout << "Inventory::Change_Item('bird trap', 0);\nHuntOverwrite(15, -5, Location::LocationCurrent); " << endl;
							Inventory::Change_Item("bird trap", 0);
							BirdTrap->Action(15, -5, Location::LocationCurrent);
							Character::DisplayCharacteristics();
							HuntContainer->setVisible(false);
						}
						if (FishButton->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "FishButton->checkClick() == true" << endl;
							eventsDescription.setString("Max costs:\nEnergy-10\nHunger-25\nThirst-35");
							FishButton->Action(FishContainer, !FishContainer->getVisible());
							cout << "FishContainer->setVisible(true);" << endl;
							StayAtFireContainer->setVisible(!FishContainer->getVisible());
							IsUsedAxeContainer->setVisible(false);
							StartFireContainer->setVisible(false);
							RestContainer->setVisible(false);
							OpenInventoryContainer->setVisible(false);
							for (Panel* x : craftMenus)
								x->setVisible(false);
							cout << "________________________________________" << endl;
						}
						if (FishingRod->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "FishingRod->checkClick() == true" << endl;
							cout << "HuntOverwrite(5, -7, Location::LocationCurrent); " << endl;
							FishingRod->Action(5, -7, Location::LocationCurrent);
							Character::DisplayCharacteristics();
							FishContainer->setVisible(false);
						}
						if (SpearFishing->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "SpearFishing->checkClick() == true" << endl;
							cout << "HuntOverwrite(2, -10, Location::LocationCurrent); " << endl;
							SpearFishing->Action(2, -10, Location::LocationCurrent);
							Character::DisplayCharacteristics();
							FishContainer->setVisible(false);
						}

#pragma endregion

#pragma region Rest

						if (RestButton->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "RestButton->checkClick() == true" << endl;
							RestButton->Action(RestContainer, !RestContainer->getVisible());
							cout << "RestContainer->setVisible(true);" << endl;
							StayAtFireContainer->setVisible(!RestContainer->getVisible());
							StartFireContainer->setVisible(false);     
							HuntContainer->setVisible(false);
							FishContainer->setVisible(false);
							IsUsedAxeContainer->setVisible(false);
							OpenInventoryContainer->setVisible(false);
							for (Panel* x : craftMenus)
								x->setVisible(false);
							cout << "________________________________________" << endl;
						}
						if (RestThreeHours->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "RestThreeHours->checkClick() == true" << endl;
							RestThreeHours->Action(3);
							cout << "Character::Rest(3);" <<  endl;
							if (!Location::Shelter)
							{
								int condition = Character::GetConditionLevel();
								int warmth = Character::GetWarmthLevel();
								Character::ChangeWarmthLevel(-10);
								warmth = Character::GetWarmthLevel() - warmth;
								condition = Character::GetConditionLevel() - condition;
								eventsDescription.setString("Energy+39\nHunger-21\nThirst-15\nWarmth" + to_string(warmth) + "\nCondition" + to_string(condition));
								cout << "Location::Shelter == false;\nCharacter::ChangeWarmthLevel(-10);" << endl;
							}
							else eventsDescription.setString("Energy+39\nHunger-21\nThirst-15");
							Character::DisplayCharacteristics();
							RestContainer->setVisible(false);
						}
						if (RestSixHours->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "RestSixHours->checkClick() == true" << endl;
							cout << "Character::Rest(6);" << endl;
							RestSixHours->Action(6);
							if (!Location::Shelter)
							{
								int condition = Character::GetConditionLevel();
								int warmth = Character::GetWarmthLevel();
								Character::ChangeWarmthLevel(-20);
								warmth = Character::GetWarmthLevel() - warmth;
								condition = Character::GetConditionLevel() - condition;
								eventsDescription.setString("Energy+78\nHunger-42\nThirst-30\nWarmth" + to_string( warmth) + "\nCondition" + to_string(condition));
								cout << "Location::Shelter == false;\nCharacter::ChangeWarmthLevel(-20);" << endl;
							}
							else eventsDescription.setString("Energy+78\nHunger-42\nThirst-30");
							Character::DisplayCharacteristics();
							RestContainer->setVisible(false);
						}
						if (RestEightHours->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "RestEightHours->checkClick() == true" << endl;
							cout << "Character::Rest(8);" << endl;
							RestEightHours->Action(8);
							if (!Location::Shelter)
							{
								int condition = Character::GetConditionLevel();
								int warmth = Character::GetWarmthLevel();
								Character::ChangeWarmthLevel(-30);
								warmth = Character::GetWarmthLevel() - warmth;
								condition = Character::GetConditionLevel() - condition;
								eventsDescription.setString("Energy+100\nHunger-56\nThirst-40\nWarmth" + to_string( warmth) + "\nCondition" + to_string(condition));
								cout << "Location::Shelter == false;\nCharacter::ChangeWarmthLevel(-30);" << endl;
							}
							else eventsDescription.setString("Energy+100\nHunger-56\nThirst-40");
							Character::DisplayCharacteristics();
							RestContainer->setVisible(false);
						}

#pragma endregion 

#pragma region OpenInventory

						if (OpenInventory->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "OpenInventory->checkClick() == true" << endl;
							OpenInventory->Action(OpenInventoryContainer, !OpenInventoryContainer->getVisible());
							cout << "OpenInventoryContainer->setVisible(true);" << endl;
							StayAtFireContainer->setVisible(!OpenInventoryContainer->getVisible());
							StartFireContainer->setVisible(false);
							HuntContainer->setVisible(false);
							FishContainer->setVisible(false);
							IsUsedAxeContainer->setVisible(false);
							RestContainer->setVisible(false);
							for (Panel* x : craftMenus)
								x->setVisible(false);
							cout << "________________________________________" << endl;
						}

#pragma endregion

#pragma region EatAndDrink

						if (EatButton->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "EatButton->checkClick() == true" << endl;
							EatButton->Action(5); 
							eventsDescription.setString("Hunger+5");
							Inventory::food -= 1;
							cout << "Character::ChangeHungerLevel(5);\nInventory::food -= 1;" << endl;
							Character::DisplayCharacteristics();
						}
						if (DrinkButton->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "DrinkButton->checkClick() == true" << endl;
							DrinkButton->Action(5);
							eventsDescription.setString("Thirst+5");
							Inventory::water -= 1;
							cout << "Character::ChangeThirstLevel(5);\nInventory::water -= 1;" << endl;
							Character::DisplayCharacteristics();
						}
						if (CosumeMedicineButton->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							cout << "CosumeMedicineButton->checkClick() == true" << endl;
							CosumeMedicineButton->Action(5);
							eventsDescription.setString("Condition+5");
							Inventory::medicine -= 1;
							cout << "Character::ChangeConditionLevel(5);\nInventory::medicine -= 1;" << endl;
							Character::DisplayCharacteristics();
						}

#pragma endregion

#pragma region LostTheGame

						if (Restart->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							eventsDescription.setString("");
							MainContainer->setActive(true);
							OpenInventoryContainer->setActive(true);
							StartFireContainer->setActive(true);
							StayAtFireContainer->setActive(true);
							IsUsedAxeContainer->setActive(true);
							FailMenu->setActive(false);
							Restart->Action();
							StartFireContainer->setActive(false);
							IsUsedAxeContainer->setActive(false);
							OpenInventoryContainer->setVisible(false);

							Generator::GenerateEnvironments(3, 3);
							Location::LocationCurrent = Data::GetEnvironment(0);
							Location::CheckWhatEnvironment(Location::LocationCurrent);

							hours = GeneralTime::GetHours();
							days = GeneralTime::GetDay();
						}

#pragma endregion

#pragma region Menu

						if (OpenMenu->checkClick((Vector2f)Mouse::getPosition(window)) || OpenMainMenu->checkClick((Vector2f)Mouse::getPosition(window)))
						{
							FailMenu->setActive(false);
							MainContainer->setActive(true);
							HuntContainer->setActive(true);
							StayAtFireContainer->setVisible(true);
							HuntContainer->setVisible(false);
							FishContainer->setActive(true);
							FishContainer->setVisible(false);
							OpenInventoryContainer->setActive(true);
							OpenInventoryContainer->setVisible(false);
							StartFireContainer->setActive(true);
							StartFireContainer->setVisible(false);
							StayAtFireContainer->setActive(true);
							IsUsedAxeContainer->setVisible(false);
							RestContainer->setVisible(false);
							for (Panel* x : craftMenus)
								x->setVisible(false);

							Data::SaveAllStaticData();
							window.close();
							OpenMenu->Action(Menu::MenuWindow, menuObj);
							if (Menu::OpenMainWindow()) window.create(sf::VideoMode(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height), "Game"/*, sf::Style::Fullscreen*/);
						}

#pragma endregion

#pragma region Craft
						if (openCraftBut->checkClick((Vector2f)Mouse::getPosition(window)))
						{	
							cout << "openCraftBut->checkClick() == true" << endl;
							if (craftMenus[0]->getVisible())
							{
								openCraftBut->Action(&craftMenus, NULL);
								cout << "openCraftBut->Action(&craftMenus, NULL);" << endl;
							}
							else
							{
								openCraftBut->Action(&craftMenus, craftMenus[0]);
								cout << "openCraftBut->Action(&craftMenus, craftMenus[0]);" << endl;
							}
								
							StayAtFireContainer->setVisible(!craftMenus[0]->getVisible());
							StartFireContainer->setVisible(false);
							HuntContainer->setVisible(false);
							FishContainer->setVisible(false);
							IsUsedAxeContainer->setVisible(false);
							OpenInventoryContainer->setVisible(false);
							cout << "________________________________________" << endl;
						}
							
						for (map<Button<bool(*)(string)>*, string>::iterator it = craftBut.begin(); it != craftBut.end(); ++it)
						{
							if (it->first->checkClick((Vector2f)Mouse::getPosition(window)))
							{
								cout << it->first << "->checkClick() == true" << endl;
								if (it->first->Action(it->second))
								{
									eventsDescription.setString("Successfully crafted");
									cout << "Craft::canCraft(" << it->second << ") == true;" << endl;
								}
								else
								{
									eventsDescription.setString("Not enough resources or this tool already exist");
									cout << "Craft::canCraft(" << it->second << ") == false;" << endl;
								}
							}
						}
						for (map<Button<void(*)(vector<Panel*>*, Panel*)>*, Panel*>::iterator it = switchCraftBut.begin(); it != switchCraftBut.end(); ++it)
						{
							if (it->first->checkClick((Vector2f)Mouse::getPosition(window)))
							{
								it->first->Action(&craftMenus, it->second);
								break;
							}
						}
#pragma endregion

					}
					break;
				}
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel);
				else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel);
			}
		
			// Update 
#pragma region StartFire

			FireLighter->update((Vector2f)Mouse::getPosition(window));
			FireBow->update((Vector2f)Mouse::getPosition(window));
			FireMatches->update((Vector2f)Mouse::getPosition(window));
			FireLens->update((Vector2f)Mouse::getPosition(window));
			FireStone->update((Vector2f)Mouse::getPosition(window));
			StayAtFire->update((Vector2f)Mouse::getPosition(window));
			StartFire->update((Vector2f)Mouse::getPosition(window));

#pragma endregion 

#pragma region ExploreArea

			ExploreArea->update((Vector2f)Mouse::getPosition(window));
			UseAxeButton->update((Vector2f)Mouse::getPosition(window));
			DontUseAxeButton->update((Vector2f)Mouse::getPosition(window));

#pragma endregion

#pragma region ChooseWhereToGo
			FirstVariantToTravel->update((Vector2f)Mouse::getPosition(window));
			SecondVariantToTravel->update((Vector2f)Mouse::getPosition(window));
			ThirdVariantToTravel->update((Vector2f)Mouse::getPosition(window));

#pragma endregion

#pragma region Hunt

			HuntButton->update((Vector2f)Mouse::getPosition(window));
			FallTrap->update((Vector2f)Mouse::getPosition(window));
			SpringTrap->update((Vector2f)Mouse::getPosition(window));
			BirdTrap->update((Vector2f)Mouse::getPosition(window));
			FishButton->update((Vector2f)Mouse::getPosition(window));
			FishingRod->update((Vector2f)Mouse::getPosition(window));
			SpearFishing->update((Vector2f)Mouse::getPosition(window));

#pragma endregion

#pragma region EatAndDrink

			EatButton->update((Vector2f)Mouse::getPosition(window));
			DrinkButton->update((Vector2f)Mouse::getPosition(window));
			CosumeMedicineButton->update((Vector2f)Mouse::getPosition(window));

#pragma endregion

#pragma region Rest

			RestButton->update((Vector2f)Mouse::getPosition(window));
			RestThreeHours->update((Vector2f)Mouse::getPosition(window));
			RestSixHours->update((Vector2f)Mouse::getPosition(window));
			RestEightHours->update((Vector2f)Mouse::getPosition(window));

#pragma endregion

#pragma region OpenInventory

			OpenInventory->update((Vector2f)Mouse::getPosition(window));

#pragma endregion

#pragma region LostTheGame

			Restart->update((Vector2f)Mouse::getPosition(window));
			OpenMainMenu->update((Vector2f)Mouse::getPosition(window));

#pragma endregion		

#pragma region Menu

			OpenMenu->update((Vector2f)Mouse::getPosition(window));

#pragma endregion

#pragma region Craft

			openCraftBut->update((Vector2f)Mouse::getPosition(window));
			for (map<Button<bool(*)(string)>*, string>::iterator it = craftBut.begin(); it != craftBut.end(); ++it)
			{
				it->first->update((Vector2f)Mouse::getPosition(window));
			}
			for (map<Button<void(*)(vector<Panel*>*, Panel*)>*, Panel*>::iterator it = switchCraftBut.begin(); it != switchCraftBut.end(); ++it)
			{
				it->first->update((Vector2f)Mouse::getPosition(window));
			}
#pragma endregion

			// Draw
			window.clear();
			window.draw(location.Sprite);

#pragma region Character

			CharacterContainer->render(window, Vector2f(0, 0));

#pragma endregion

#pragma region ChooseWhereToGo

			ChooseWhereToGoContainer->render(window, Vector2f(0, 0));

#pragma endregion

#pragma region Shelter

			if (Location::Shelter) ShelterContainer->setVisible(true);
			else ShelterContainer->setVisible(false);

			ShelterContainer->render(window, Vector2f(0, 0));
#pragma endregion

#pragma region Rest

			RestContainer->render(window, Vector2f(0, 0));

#pragma endregion

#pragma region StartFire

			if (!Character::IsStayAtFire())
			{
				StayAtFireContainer->setVisible(false);
				StayAtFireContainer->setActive(false);
			}
			if (GeneralTime::DeltaTime(Character::startedDay, Character::startedHour) > 2)
			{
				StayAtFireContainer->setVisible(false); // an example; it's not necessary must be 2
				Character::SetStayAtFire(false);
			}
			if (StayAtFireContainer->getVisible()) StartFire->setActive(false);
			else StartFire->setActive(true);

			StartFireContainer->render(window, Vector2f(0, 0));
			StayAtFireContainer->render(window, Vector2f(0, 0));

#pragma endregion 

#pragma region ExploreArea

			if (!Inventory::checkClickExploreArea || Character::CheckIFCharacteristicsBelowZero()) ChooseWhereToGoContainer->setVisible(false);
			else ChooseWhereToGoContainer->setVisible(true);

#pragma endregion

#pragma region EatAndDrink

			if (!Inventory::food) EatButton->setVisible(false);
			else EatButton->setVisible(true);
			if (!Inventory::water) DrinkButton->setVisible(false);
			else DrinkButton->setVisible(true);
			if (!Inventory::medicine) CosumeMedicineButton->setVisible(false);
			else CosumeMedicineButton->setVisible(true);

#pragma endregion

#pragma region Hunt

			if (Location::LocationCurrent != 1)
			{
				HuntButton->setVisible(false);
				FishButton->setVisible(true);
			}
			if (Location::LocationCurrent == 1)
			{
				FishButton->setVisible(false);
				HuntButton->setVisible(true);
			}

			if (!Inventory::Check_Tool("fall trap")) FallTrap->setVisible(false);
			if (!Inventory::Check_Tool("spring trap")) SpringTrap->setVisible(false);
			if (!Inventory::Check_Tool("bird trap")) BirdTrap->setVisible(false);

#pragma endregion

#pragma region TemperatureAndWarmth

			if (GeneralTime::GetHours() >= 7 && GeneralTime::GetHours() <= 17)
			{
				if (GeneralTime::DeltaTime(days, hours) >= 2)
				{
					Location::ChangeTemperature(1 + rand() % 3);
					hours = GeneralTime::GetHours();
					days = GeneralTime::GetDay();
					int condition = Character::GetConditionLevel();
					int warmth = Character::GetWarmthLevel();
					Character::ChangeWarmthLevel(5);
					if (Character::GetWarmthLevel() - warmth) warmthAndConditionDuringGame.setString("Condition+" + to_string(Character::GetConditionLevel() - condition) + "\nWarmth+" + to_string(Character::GetWarmthLevel() - warmth));
				}
			}
			if (GeneralTime::GetHours() > 17 || GeneralTime::GetHours() < 7)
			{
				if (GeneralTime::DeltaTime(days, hours) >= 2)
				{
					Location::ChangeTemperature(-3 + rand() % 3);
					hours = GeneralTime::GetHours();
					days = GeneralTime::GetDay();
					int condition = Character::GetConditionLevel();
					int warmth = Character::GetWarmthLevel();
					Character::ChangeWarmthLevel(-5);
					if (Character::GetWarmthLevel() - warmth) warmthAndConditionDuringGame.setString("Condition" + to_string(Character::GetConditionLevel() - condition) + "\nWarmth" + to_string(Character::GetWarmthLevel() - warmth));
				}
			}

#pragma endregion

#pragma region OpenInventory

			OpenInventoryContainer->render(window, Vector2f(0, 0));

#pragma endregion
			
			//Update text
			TimeLabel->setText(GeneralTime::GetTime());
			temperatureBar.SetText(to_string(Location::GetTemperature()) + "�C");

			//Update Bars
			bars[Bars::HungerBar].SetTextAndScale(Character::GetHungerLevel());
			bars[Bars::HungerBar].Render(window);

			bars[Bars::ThirstBar].SetTextAndScale(Character::GetThirstLevel());
			bars[Bars::ThirstBar].Render(window);
			
			bars[Bars::WarmthBar].SetTextAndScale(Character::GetWarmthLevel());
			bars[Bars::WarmthBar].Render(window);

			bars[Bars::EnergyBar].SetTextAndScale(Character::GetEnergyLevel());
			bars[Bars::EnergyBar].Render(window);

			bars[Bars::ConditionBar].SetTextAndScale(Character::GetConditionLevel());
			bars[Bars::ConditionBar].Render(window);

			temperatureBar.ChangeTemperaturePicture(Location::GetTemperature());
			temperatureBar.Render(window);


			Data::SaveGamePerSomeTime(23); // Save data per some time (in hours)	
			MainContainer->render(window, Vector2f(0, 0)); // render themself and all ui that contain

#pragma region Hunt	

			if (Inventory::Check_Tool("fall trap")) FallTrap->setVisible(true);
			if (Inventory::Check_Tool("spring trap")) SpringTrap->setVisible(true);
			if (Inventory::Check_Tool("bird trap")) BirdTrap->setVisible(true);

			HuntContainer->render(window, Vector2f(0, 0));
			FishContainer->render(window, Vector2f(0, 0));

#pragma endregion

			OpenInventoryContainer->render(window, Vector2f(0, 0));

			IsUsedAxeContainer->render(window, Vector2f(0, 0));

#pragma region Menu

			OpenMenuContainer->render(window, Vector2f(0, 0));

#pragma endregion

#pragma region OpenInventory

			if (OpenInventoryContainer->getVisible())
				Inventory::DisplayInventory(window, Vector2f(windowWidth, windowHeight));

#pragma endregion

#pragma region DisplayCharacteristicsIcos
			// Explore area
				// Use axe
			if (UseAxeButton->isContains())
			{
				changeCharacteristicsUseAxe->SetPosition(Vector2f(UseAxeButton->getRealPosition().x * 1.038, UseAxeButton->getRealPosition().y + UseAxeButton->getHeight() / 6));
				changeCharacteristicsUseAxe->SetDistanceBetweenObjects(Vector2f(100, 0), changeCharacteristicsUseAxe, 3);

				UseAxeButton->setTextRender(false);
				changeCharacteristicsUseAxe[0].Render(window);
				changeCharacteristicsUseAxe[1].Render(window);
				changeCharacteristicsUseAxe[2].Render(window);
			}
			else
				UseAxeButton->setTextRender(true);

				// Don't use axe
			if (DontUseAxeButton->isContains())
			{
				changeCharacteristicsDontUseAxe->SetPosition(Vector2f(DontUseAxeButton->getRealPosition().x * 1.021, DontUseAxeButton->getRealPosition().y + DontUseAxeButton->getHeight() / 6));
				changeCharacteristicsDontUseAxe->SetDistanceBetweenObjects(Vector2f(100, 0), changeCharacteristicsDontUseAxe, 3);	

				DontUseAxeButton->setTextRender(false);
				changeCharacteristicsDontUseAxe[0].Render(window);
				changeCharacteristicsDontUseAxe[1].Render(window);
				changeCharacteristicsDontUseAxe[2].Render(window);
			}
			else
				DontUseAxeButton->setTextRender(true);
			
			// Choose where to go 	
			if (FirstVariantToTravel->isContains() || SecondVariantToTravel->isContains() || ThirdVariantToTravel->isContains())
			{
				changeCharacteristicsChooseWhereToGo->SetDistanceBetweenObjects(Vector2f(60, 0), changeCharacteristicsChooseWhereToGo, 3);
				if (FirstVariantToTravel->isContains())
				{
					changeCharacteristicsChooseWhereToGo->SetPosition(Vector2f(FirstVariantToTravel->getRealPosition().x * 1.5, FirstVariantToTravel->getRealPosition().y + FirstVariantToTravel->getHeight() / 5));
					FirstVariantToTravel->setTextRender(false);
				}
				else if (SecondVariantToTravel->isContains())
				{
					changeCharacteristicsChooseWhereToGo->SetPosition(Vector2f(SecondVariantToTravel->getRealPosition().x * 1.015, SecondVariantToTravel->getRealPosition().y + SecondVariantToTravel->getHeight() / 5));
					SecondVariantToTravel->setTextRender(false);
				}
				else
				{
					changeCharacteristicsChooseWhereToGo->SetPosition(Vector2f(ThirdVariantToTravel->getRealPosition().x * 1.008, ThirdVariantToTravel->getRealPosition().y + ThirdVariantToTravel->getHeight() / 5));
					ThirdVariantToTravel->setTextRender(false);
				}

				changeCharacteristicsChooseWhereToGo[0].Render(window);
				changeCharacteristicsChooseWhereToGo[1].Render(window);
				changeCharacteristicsChooseWhereToGo[2].Render(window);
			}
			else
			{
				FirstVariantToTravel->setTextRender(true);
				SecondVariantToTravel->setTextRender(true);
				ThirdVariantToTravel->setTextRender(true);
			}

			// Eat and drink
				// Eat
			if (EatButton->isContains())
			{
				changeCharacteristicHunger.SetPosition(Vector2f(EatButton->getRealPosition().x * 1.004, EatButton->getRealPosition().y + EatButton->getHeight() / 8));
				changeCharacteristicHunger.SetTexPosition(Vector2f(changeCharacteristicHunger.GetTextPosition().x, EatButton->getRealPosition().y + EatButton->getHeight() / 6));

				EatButton->setTextRender(false);
				changeCharacteristicHunger.Render(window);
			}
			else
				EatButton->setTextRender(true);

				// Drink
			if (DrinkButton->isContains())
			{
				changeCharacteristicThirst.SetPosition(Vector2f(DrinkButton->getRealPosition().x * 1.004, DrinkButton->getRealPosition().y + DrinkButton->getHeight() / 8));
				changeCharacteristicThirst.SetTexPosition(Vector2f(changeCharacteristicThirst.GetTextPosition().x, DrinkButton->getRealPosition().y + DrinkButton->getHeight() / 6));

				DrinkButton->setTextRender(false);
				changeCharacteristicThirst.Render(window);
			}
			else
				DrinkButton->setTextRender(true);

				// Medicine
			if (CosumeMedicineButton->isContains())
			{
				changeCharacteristicCondition.SetPosition(Vector2f(CosumeMedicineButton->getRealPosition().x * 1.004, CosumeMedicineButton->getRealPosition().y + CosumeMedicineButton->getHeight() / 8));
				changeCharacteristicCondition.SetTexPosition(Vector2f(changeCharacteristicCondition.GetTextPosition().x, CosumeMedicineButton->getRealPosition().y + CosumeMedicineButton->getHeight() / 6));

				CosumeMedicineButton->setTextRender(false);
				changeCharacteristicCondition.Render(window);
			}
			else
				CosumeMedicineButton->setTextRender(true);

			// Hunt
			if (FallTrap->isContains() || SpringTrap->isContains() || BirdTrap->isContains())
			{
				changeCharacteristicsHunt->SetDistanceBetweenObjects(Vector2f(100, 0), changeCharacteristicsHunt, 3);
				if (FallTrap->isContains())
				{
					changeCharacteristicsHunt->SetPosition(Vector2f(FallTrap->getRealPosition().x * 1.038, FallTrap->getRealPosition().y + FallTrap->getHeight() / 6));
					FallTrap->setTextRender(false);
				}
				else if (SpringTrap->isContains())
				{
					changeCharacteristicsHunt->SetPosition(Vector2f(SpringTrap->getRealPosition().x * 1.021, SpringTrap->getRealPosition().y + SpringTrap->getHeight() / 6));
					SpringTrap->setTextRender(false);
				}
				else
				{
					changeCharacteristicsHunt->SetPosition(Vector2f(BirdTrap->getRealPosition().x * 1.008, BirdTrap->getRealPosition().y + BirdTrap->getHeight() / 6));
					BirdTrap->setTextRender(false);
				}

				changeCharacteristicsHunt[0].Render(window);
				changeCharacteristicsHunt[1].Render(window);
				changeCharacteristicsHunt[2].Render(window);
			}
			else
			{
				FallTrap->setTextRender(true);
				SpringTrap->setTextRender(true);
				BirdTrap->setTextRender(true);
			}

			// Fish 
			if (FishingRod->isContains() || SpearFishing->isContains())
			{
				changeCharacteristicsFish->SetDistanceBetweenObjects(Vector2f(100, 0), changeCharacteristicsFish, 3);
				if (FishingRod->isContains())
				{
					changeCharacteristicsFish[0].SetText("  -7");
					changeCharacteristicsFish->SetPosition(Vector2f(FishingRod->getRealPosition().x * 1.038, FishingRod->getRealPosition().y + FishingRod->getHeight() / 6));
					FishingRod->setTextRender(false);
				}
				else
				{
					changeCharacteristicsFish[0].SetText(" -10");
					changeCharacteristicsFish->SetPosition(Vector2f(SpearFishing->getRealPosition().x * 1.021, SpearFishing->getRealPosition().y + SpearFishing->getHeight() / 6));
					SpearFishing->setTextRender(false);
				}

				changeCharacteristicsFish[0].Render(window);
				changeCharacteristicsFish[1].Render(window);
				changeCharacteristicsFish[2].Render(window);
			}
			else
			{
				FishingRod->setTextRender(true);
				SpearFishing->setTextRender(true);
			}

			// Stay at fire
				//  Methods
			if (FireLighter->isContains() || FireStone->isContains() || FireMatches->isContains() || FireBow->isContains() || FireLens->isContains() )
			{
				changeCharacteristicsStayAtFire->SetDistanceBetweenObjects(Vector2f(100, 0), changeCharacteristicsStayAtFire, 3);
				if (FireLighter->isContains())
				{
					changeCharacteristicsStayAtFire[0].SetText(" -3");
					changeCharacteristicsStayAtFire[1].SetText("  0");
					changeCharacteristicsStayAtFire[2].SetText(" -1");
					changeCharacteristicsStayAtFire->SetPosition(Vector2f(FireLighter->getRealPosition().x * 1.038, FireLighter->getRealPosition().y + FireLighter->getHeight() / 6));
					FireLighter->setTextRender(false);
				}
				else if (FireStone->isContains())
				{
					changeCharacteristicsStayAtFire[0].SetText("-20");
					changeCharacteristicsStayAtFire[1].SetText(" -6");
					changeCharacteristicsStayAtFire[2].SetText(" -9");
					changeCharacteristicsStayAtFire->SetPosition(Vector2f(FireStone->getRealPosition().x * 1.021, FireStone->getRealPosition().y + FireStone->getHeight() / 6));
					FireStone->setTextRender(false);
				}
				else if (FireMatches->isContains())
				{
					changeCharacteristicsStayAtFire[0].SetText(" -5");
					changeCharacteristicsStayAtFire[1].SetText("  0");
					changeCharacteristicsStayAtFire[2].SetText(" -1");
					changeCharacteristicsStayAtFire->SetPosition(Vector2f(FireMatches->getRealPosition().x * 1.021, FireMatches->getRealPosition().y + FireMatches->getHeight() / 6));
					FireMatches->setTextRender(false);
				}
				else if (FireBow->isContains())
				{
					changeCharacteristicsStayAtFire[0].SetText("-25");
					changeCharacteristicsStayAtFire[1].SetText("-10");
					changeCharacteristicsStayAtFire[2].SetText("-14");
					changeCharacteristicsStayAtFire->SetPosition(Vector2f(FireBow->getRealPosition().x * 1.021, FireBow->getRealPosition().y + FireBow->getHeight() / 6));
					FireBow->setTextRender(false);
				}
				else
				{
					changeCharacteristicsStayAtFire[0].SetText("-15");
					changeCharacteristicsStayAtFire[1].SetText(" -5");
					changeCharacteristicsStayAtFire[2].SetText(" -7");
					changeCharacteristicsStayAtFire->SetPosition(Vector2f(FireLens->getRealPosition().x * 1.008, FireLens->getRealPosition().y + FireLens->getHeight() / 6));
					FireLens->setTextRender(false);
				}

				changeCharacteristicsStayAtFire[0].Render(window);				
				changeCharacteristicsStayAtFire[1].Render(window);				
				changeCharacteristicsStayAtFire[2].Render(window);
			}
			else
			{
				FireLighter->setTextRender(true);
				FireStone->setTextRender(true);
				FireMatches->setTextRender(true);
				FireBow->setTextRender(true);
				FireLens->setTextRender(true);
			}

				// Stay at fire action 
			if (StayAtFire->isContains())
			{
				changeCharacteristicStayAtFire->SetPosition(Vector2f(StayAtFire->getRealPosition().x * 0.9, StayAtFire->getRealPosition().y + StayAtFire->getHeight() / 6));
				changeCharacteristicStayAtFire->SetDistanceBetweenObjects(Vector2f(65, 0), changeCharacteristicStayAtFire, 5);
				changeCharacteristicStayAtFire[4].SetPosition(changeCharacteristicStayAtFire[4].GetPosition().x + 15, StayAtFire->getRealPosition().y + StayAtFire->getHeight() / 4.5);
				changeCharacteristicStayAtFire[4].SetTexPosition(Vector2f(changeCharacteristicStayAtFire[4].GetTextPosition().x, StayAtFire->getRealPosition().y + StayAtFire->getHeight() / 4.5));

				StayAtFire->setTextRender(false);
				changeCharacteristicStayAtFire[0].Render(window);
				changeCharacteristicStayAtFire[1].Render(window);
				changeCharacteristicStayAtFire[2].Render(window);
				changeCharacteristicStayAtFire[3].Render(window);
				changeCharacteristicStayAtFire[4].Render(window);
			}
			else
				StayAtFire->setTextRender(true);

			// Rest
			if (RestThreeHours->isContains() || RestSixHours->isContains() || RestEightHours->isContains())
			{
				changeCharacteristicsRest[0].SetTextSize(16);
				changeCharacteristicsRest[3].SetTextSize(16);

				changeCharacteristicsRest->SetDistanceBetweenObjects(Vector2f(60, 0), changeCharacteristicsRest, 5);
				changeCharacteristicsRest[4].SetPosition(Vector2f(changeCharacteristicsRest[4].GetPosition().x, changeCharacteristicsRest[4].GetPosition().y + 6));
				changeCharacteristicsRest[4].SetTexPosition(Vector2f(changeCharacteristicsRest[4].GetPosition().x, changeCharacteristicsRest[4].GetPosition().y + 3));

				if (RestThreeHours->isContains())
				{
					changeCharacteristicsRest[0].SetText("+39");
					changeCharacteristicsRest[1].SetText("-21");
					changeCharacteristicsRest[2].SetText("-15");
					changeCharacteristicsRest[3].SetText("-10");
					changeCharacteristicsRest->SetPosition(Vector2f(RestThreeHours->getRealPosition().x * 1.018, RestThreeHours->getRealPosition().y + RestThreeHours->getHeight() / 6));
					RestThreeHours->setTextRender(false);
				}
				else if (RestSixHours->isContains())
				{
					changeCharacteristicsRest[0].SetText("+78");
					changeCharacteristicsRest[1].SetText("-42");
					changeCharacteristicsRest[2].SetText("-30");
					changeCharacteristicsRest[3].SetText("-20");
					changeCharacteristicsRest->SetPosition(Vector2f(RestSixHours->getRealPosition().x * 1.009, RestSixHours->getRealPosition().y + RestSixHours->getHeight() / 6));
					RestSixHours->setTextRender(false);
				}
				else
				{
					changeCharacteristicsRest[0].SetTextSize(13);
					changeCharacteristicsRest[0].SetText("+100");
					changeCharacteristicsRest[1].SetText("-56");
					changeCharacteristicsRest[2].SetText("-40");
					changeCharacteristicsRest[3].SetText("-30");
					changeCharacteristicsRest->SetPosition(Vector2f(RestEightHours->getRealPosition().x * 1.005, RestEightHours->getRealPosition().y + RestEightHours->getHeight() / 6));
					RestEightHours->setTextRender(false);
				}

				changeCharacteristicsRest[0].Render(window);
				changeCharacteristicsRest[1].Render(window);
				changeCharacteristicsRest[2].Render(window);
				changeCharacteristicsRest[3].Render(window);
				changeCharacteristicsRest[4].Render(window);
			}
			else
			{
				RestThreeHours->setTextRender(true);
				RestSixHours->setTextRender(true);
				RestEightHours->setTextRender(true);
			}

#pragma endregion


#pragma region LostMenu
			if (Character::CheckIFCharacteristicsBelowZero())
			{
				MainContainer->setActive(false);
				HuntContainer->setActive(false);
				FishContainer->setActive(false);
				OpenInventoryContainer->setActive(false);
				StartFireContainer->setActive(false);
				StayAtFireContainer->setActive(false);
				ChooseWhereToGoContainer->setActive(false);
				IsUsedAxeContainer->setActive(false);
				FailMenu->setActive(true);

				StayAtFireContainer->setVisible(false);
				ChooseWhereToGoContainer->setVisible(false);
				HuntContainer->setVisible(false);
				OpenInventoryContainer->setVisible(false);
				StartFireContainer->setVisible(false);
				IsUsedAxeContainer->setVisible(false);
				Craft::changeCraftMenu(&craftMenus, NULL);

				FailMenu->render(window, Vector2f(0, 0));
			}
#pragma endregion

			window.draw(eventsDescription);
			window.draw(characteristics);
			window.draw(warmthAndConditionDuringGame);

			window.display();
		}
	}

	// Deleteing dynamic objects

	return 0;
}