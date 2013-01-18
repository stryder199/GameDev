/*
* UI.cpp
*
*  Created on: 2012-01-08
*      Author: stryder
*/
#include "UI.h"
#include "MiniMap.h"
#include "Tower.h"
#include "myButton.h"
#include "Enemy.h"
#include "GameData.h"
#include "Window.h"
#include "Resources.h"
#include "Camera.h"

UI::UI( GameData* game_data )
{
	minimap = new MiniMap( game_data );
	TowFocus = NULL;
	EnemyFocus = NULL;
	spawnedTower = NULL;
	cannonButton = new myButton( game_data->get_rect( "button1" ) );
	tankButton = new myButton( game_data->get_rect( "button2" ) );
	iceButton = new myButton( game_data->get_rect( "button3" ) );
	nukeButton = new myButton( game_data->get_rect( "button4" ) );
	laserButton = new myButton( game_data->get_rect( "button5" ) );
	sellButton = new myButton( game_data->get_rect( "costBox1" ) );
	upgradeButton1 = new myButton( game_data->get_rect( "upgradeBox1" ) );
	upgradeButton2 = new myButton( game_data->get_rect( "upgradeBox2" ) );
	upgradeButton3 = new myButton( game_data->get_rect( "upgradeBox3" ) );
	upgradeButton4 = new myButton( game_data->get_rect( "upgradeBox4" ) );
	upgradeButton5 = new myButton( game_data->get_rect( "upgradeBox5" ) );
	upgradePath1 = new myButton( game_data->get_rect( "evolveButton1" ) );
	upgradePath2 = new myButton( game_data->get_rect( "evolveButton2" ) );
}

void UI::showTopBar( Window* myWindow, GameData* game_data, Resources* myResources )
{
	//Show the top menu
	SDL_FillRect( myWindow->get_screen(), game_data->get_rect( "topMenu" ), SDL_MapRGB( myWindow->get_screen()->format, 0x77, 0x77, 0x77 ) );

	//The minerals/gas/supply as a string
	std::stringstream text;

	text << "Score:" << myResources->get_score();
	apply_text_surface( (myWindow->get_ScreenWidth()/5) - 50, 0, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid_robot_ttf" ), game_data->get_color( "textColor" ) );
	text.str("");

	text << "Lives:" << myResources->get_lives();
	apply_text_surface( (myWindow->get_ScreenWidth()/5)*2 - 50, 0, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid_robot_ttf" ), game_data->get_color( "textColor" ) );
	text.str("");

	text << "Current wave:" << myResources->get_wave_number();
	apply_text_surface( (myWindow->get_ScreenWidth()/5)*3 - 50, 0, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid_robot_ttf" ), game_data->get_color( "textColor" ) );
	text.str("");

	text << "Money:" << myResources->get_money() << "$";
	apply_text_surface( (myWindow->get_ScreenWidth()/5)*4 - 50, 0, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid_robot_ttf" ), game_data->get_color( "textColor" ) );
	text.str("");

}

void UI::showBotUI( Window* myWindow, GameData* game_data )
{
	std::stringstream text;

	SDL_Rect temp;

	temp.x =0;
	temp.y = game_data->get_rect( "botMenu" )->y;
	temp.w = myWindow->get_ScreenWidth();
	temp.h = game_data->get_rect( "botMenu" )->h;

	SDL_FillRect( myWindow->get_screen(), &temp, SDL_MapRGB( myWindow->get_screen()->format, 0x00, 0x00, 0x00 ) );

	minimap->show( game_data, myWindow );

	//Show the botmenu backgrond
	SDL_FillRect( myWindow->get_screen(), game_data->get_rect( "botMenu" ), SDL_MapRGB( myWindow->get_screen()->format, 0x00, 0x00, 0x00 ) );

	//Show the upgrade forground
	SDL_FillRect( myWindow->get_screen(), game_data->get_rect( "upgradeBox1Forground" ), SDL_MapRGB( myWindow->get_screen()->format, 0x77, 0x77, 0x77 ) );

	//Show the button forground
	SDL_FillRect( myWindow->get_screen(), game_data->get_rect( "infoForground" ), SDL_MapRGB( myWindow->get_screen()->format, 0x77, 0x77, 0x77 ) );

	//Show the info forground
	SDL_FillRect( myWindow->get_screen(), game_data->get_rect( "buttonForground" ), SDL_MapRGB( myWindow->get_screen()->format, 0x77, 0x77, 0x77 ) );


	if( hoverOver == true )
	{
		text << "Buy " << game_data->getSampleTower( hoverType )->getTitle();
		apply_text_surface( game_data->get_rect( "upgradeBox1Forground" )->x + game_data->get_rect( "upgradeBox1Forground" )->w/4, game_data->get_rect( "upgradeBox1Forground" )->y + game_data->get_rect( "upgradeBox1Forground" )->h/3, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid_conviction_ttf" ), game_data->get_color( "textColor" ) );
		text.str("");

		text << "Cost: " << game_data->getSampleTower( hoverType )->getBuyCost();
		apply_text_surface( game_data->get_rect( "upgradeBox1Forground" )->x + game_data->get_rect( "upgradeBox1Forground" )->w/3, game_data->get_rect( "upgradeBox1Forground" )->y + game_data->get_rect( "upgradeBox1Forground" )->h/3 + 50, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid_conviction_ttf" ), game_data->get_color( "textColor" ) );
		text.str("");
	}
	else if( TowFocus != NULL)
	{
		//Info side
		text << TowFocus->getTitle();
		apply_text_surface( game_data->get_rect( "infoForground" )->x + 3, game_data->get_rect( "infoForground" )->y, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid_conviction_ttf" ), game_data->get_color( "textColor" ) );
		text.str("");

		text << TowFocus->getDescription();
		apply_text_surface( game_data->get_rect( "infoForground" )->x + 3, game_data->get_rect( "infoForground" )->y + 40, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid/small_conviction_ttf" ), game_data->get_color( "textColor" ) );
		text.str("");

		text << "Damage-" << TowFocus->getDamage();
		apply_text_surface( game_data->get_rect( "infoForground" )->x + 3, game_data->get_rect( "infoForground" )->y + 60, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid/small_conviction_ttf" ), game_data->get_color( "textColor" ) );
		text.str("");

		text << "Attack Speed-" << TowFocus->getAttackSpeed();
		apply_text_surface( game_data->get_rect( "infoForground" )->x + 3, game_data->get_rect( "infoForground" )->y + 80, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid/small_conviction_ttf" ), game_data->get_color( "textColor" ) );
		text.str("");

		text << "Range-" << TowFocus->getRange();
		apply_text_surface( game_data->get_rect( "infoForground" )->x + 3, game_data->get_rect( "infoForground" )->y + 100, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid/small_conviction_ttf" ), game_data->get_color( "textColor" ) );
		text.str("");

		int towerUG = TowFocus->getUpgradeTeir();

		SDL_FillRect( myWindow->get_screen(), game_data->get_rect( "pictureBorder" ), SDL_MapRGB( myWindow->get_screen()->format, 0x00, 0x00, 0x00 ) );

		sellButton->show( myWindow );


		if( towerUG == 10 || towerUG == 20 || towerUG == 30 )
		{
			upgradePath1->show( myWindow );
			upgradePath2->show( myWindow );

			//End of upgrade borders
			if( towerUG == 10 )
				text << "Damage";

			if( towerUG == 20 )
				text << "Damage";

			if( towerUG == 30 )
				text << "Attack Speed";

			apply_text_surface( game_data->get_rect( "evolveButton1" )->x + 12, game_data->get_rect( "evolveButton1" )->y + 8, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "small_conviction_ttf" ), game_data->get_color( "textColor" ) );
			text.str("");

			//End of upgrade borders
			if( towerUG == 10 )
				text << "Attack Speed";

			if( towerUG == 20 )
				text << "Range";

			if( towerUG == 30 )
				text << "Range";
			apply_text_surface( game_data->get_rect( "evolveButton2" )->x + 12, game_data->get_rect( "evolveButton2" )->y + 8, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "small_conviction_ttf" ), game_data->get_color( "textColor" ) );
			text.str("");
		}

		else if( towerUG <= 70 )
		{
			if(towerUG%10 <= 1)
			{
				upgradeButton1->show( myWindow );
			}
			else{
				upgradeButton1->show( 0x94, 0x00, 0xd3, myWindow );
			}

			if(towerUG%10 <= 2)
			{
				upgradeButton2->show( myWindow );
			}
			else{
				upgradeButton2->show( 0x94, 0x00, 0xd3, myWindow );
			}
			if(towerUG%10 <= 3)
			{
				upgradeButton3->show( myWindow );
			}
			else{
				upgradeButton3->show( 0x94, 0x00, 0xd3, myWindow );
			}
			if(towerUG%10 <= 4)
			{
				upgradeButton4->show( myWindow );
			}
			else{
				upgradeButton4->show( 0x94, 0x00, 0xd3, myWindow );
			}
			if(towerUG%10 <= 5)
			{
				upgradeButton5->show( myWindow );
			}
			else{
				upgradeButton5->show( 0x94, 0x00, 0xd3, myWindow );
			}

			text << "->";
			apply_text_surface( game_data->get_rect( "upgradeBox1" )->x + game_data->get_rect( "upgradeBox1" )->w + 2, game_data->get_rect( "upgradeBox1" )->y + 4, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid/small_conviction_ttf" ), game_data->get_color( "textColor" ) );
			text.str("");
			text << "->";
			apply_text_surface( game_data->get_rect( "upgradeBox2" )->x + game_data->get_rect( "upgradeBox2" )->w + 2, game_data->get_rect( "upgradeBox2" )->y + 4, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid/small_conviction_ttf" ), game_data->get_color( "textColor" ) );
			text.str("");	
			text << "->";
			apply_text_surface( game_data->get_rect( "upgradeBox3" )->x + game_data->get_rect( "upgradeBox3" )->w + 2, game_data->get_rect( "upgradeBox3" )->y + 4, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid/small_conviction_ttf" ), game_data->get_color( "textColor" ) );
			text.str("");	
			text << "->";
			apply_text_surface( game_data->get_rect( "upgradeBox4" )->x + game_data->get_rect( "upgradeBox4" )->w + 2, game_data->get_rect( "upgradeBox4" )->y + 4, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid/small_conviction_ttf" ), game_data->get_color( "textColor" ) );
			text.str("");	

		}

		text << "Sell Cost : " << TowFocus->getSellCost();
		apply_text_surface( game_data->get_rect( "costBox1" )->x + game_data->get_rect( "costBox1" )->w/2 - 40, game_data->get_rect( "costBox1" )->y + 8, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "small_conviction_ttf" ), game_data->get_color( "textColor" ) );
		text.str("");

		text << "Upgrades";
		apply_text_surface( game_data->get_rect( "upgradeBox1Forground" )->x + game_data->get_rect( "upgradeBox1Forground" )->w/3, game_data->get_rect( "pictureBorder" )->y + game_data->get_rect( "pictureBorder" )->h - 5, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid_conviction_ttf" ), game_data->get_color( "textColor" ) );
		text.str("");

		apply_surface( game_data->get_rect( "pictureBorder" )->x + 10, game_data->get_rect( "pictureBorder" )->y + 10, TowFocus->getIcon(), myWindow->get_screen() );

	}
	else if( EnemyFocus != NULL )
	{
		//Enemy Status screen
		SDL_Rect HealthBarRedEnemy;
		SDL_Rect HealthBarGreenEnemy;

		HealthBarRedEnemy.x = game_data->get_rect( "upgradeBox1Forground" )->x + 20;
		HealthBarRedEnemy.y = game_data->get_rect( "upgradeBox1Forground" )->y + (game_data->get_rect( "upgradeBox1Forground" )->h*2)/3;
		HealthBarRedEnemy.w = game_data->get_rect( "upgradeBox1Forground" )->w - 40;
		HealthBarRedEnemy.h = 10;

		HealthBarGreenEnemy.x = HealthBarRedEnemy.x;
		HealthBarGreenEnemy.y = HealthBarRedEnemy.y;
		HealthBarGreenEnemy.w = (HealthBarRedEnemy.w*(EnemyFocus->getHealth()))/(EnemyFocus->getMaxHealth());
		HealthBarGreenEnemy.h = HealthBarRedEnemy.h;

		if( HealthBarGreenEnemy.w > HealthBarRedEnemy.w )
		{
			HealthBarGreenEnemy.w = 0;
		}

		SDL_FillRect( myWindow->get_screen(), &HealthBarRedEnemy, SDL_MapRGB( myWindow->get_screen()->format, 0xFF, 0x24, 0x00 ) );

		SDL_FillRect( myWindow->get_screen(), &HealthBarGreenEnemy, SDL_MapRGB( myWindow->get_screen()->format, 0x09, 0xF9, 0x11 ) );

		apply_surface( game_data->get_rect( "upgradeBox1Forground" )->x + game_data->get_rect( "upgradeBox1Forground" )->w/3 , game_data->get_rect( "upgradeBox1Forground" )->y + 10, game_data->get_surface( "enemyBig" ), myWindow->get_screen() );

		text << EnemyFocus->getHealth() << ":" << EnemyFocus->getMaxHealth();
		apply_text_surface( ( game_data->get_rect( "upgradeBox1Forground" )->x + (game_data->get_rect( "upgradeBox1Forground" )->w*2)/5 ), HealthBarRedEnemy.y + 10, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "small_conviction_ttf" ), game_data->get_color( "textColor" ) );
		text.str("");

		//Info side
		text << "Basic Enemy";
		apply_text_surface( game_data->get_rect( "infoForground" )->x, game_data->get_rect( "infoForground" )->y, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid_robot_ttf" ), game_data->get_color( "textColor" ) );
		text.str("");

		text << "Pure Evil";
		apply_text_surface( game_data->get_rect( "infoForground" )->x, game_data->get_rect( "infoForground" )->y + 30, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "small_conviction_ttf" ), game_data->get_color( "textColor" ) );
		text.str("");

		text << "Debug X-" << EnemyFocus->getX() << "   Y-" << EnemyFocus->getY();
		apply_text_surface( game_data->get_rect( "infoForground" )->x, game_data->get_rect( "infoForground" )->y + 60, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "small_conviction_ttf" ), game_data->get_color( "textColor" ) );
		text.str("");
	}

	text << "Cannon";
	apply_text_surface( game_data->get_rect( "button1" )->x, game_data->get_rect( "button1" )->y - 17, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "small_conviction_ttf" ), game_data->get_color( "textColor" ) );
	text.str("");
	cannonButton->show( game_data->get_surface( "cannonTowerSmall" ), myWindow );

	text << "Tank";
	apply_text_surface( game_data->get_rect( "button2" )->x, game_data->get_rect( "button2" )->y - 17, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "small_conviction_ttf" ), game_data->get_color( "textColor" ) );
	text.str("");
	tankButton->show( game_data->get_surface( "tankTowerSmall" ), myWindow );

	text << "Ice";
	apply_text_surface( game_data->get_rect( "button3" )->x, game_data->get_rect( "button3" )->y - 17, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "small_conviction_ttf" ), game_data->get_color( "textColor" ) );
	text.str("");
	iceButton->show( game_data->get_surface( "iceTowerSmall" ), myWindow );

	text << "Nuke";
	apply_text_surface( game_data->get_rect( "button4" )->x, game_data->get_rect( "button4" )->y - 17, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "small_conviction_ttf" ), game_data->get_color( "textColor" ) );
	text.str("");
	nukeButton->show( game_data->get_surface( "nukeTowerSmall" ), myWindow );

	text << "Laser";
	apply_text_surface( game_data->get_rect( "button5" )->x, game_data->get_rect( "button5" )->y - 17, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "small_conviction_ttf" ), game_data->get_color( "textColor" ) );
	text.str("");
	laserButton->show( game_data->get_surface( "laserTowerSmall" ), myWindow );

}

void UI::handle_events( GameData* game_data, Resources* myResources, Window* myWindow )
{
	//Handle moving the camera when clicking the minimap
	minimap->handle_events( game_data, myWindow );

	if( cannonButton->hover_over() == true )
	{
		hoverType = 1;
		hoverOver = true;
	}
	else if( tankButton->hover_over() == true )
	{
		hoverType = 2;
		hoverOver = true;
	}
	else if( iceButton->hover_over() == true )
	{
		hoverType = 3;
		hoverOver = true;
	}
	else if( nukeButton->hover_over() == true )
	{
		hoverType = 4;
		hoverOver = true;
	}
	else if( laserButton->hover_over() == true )
	{
		hoverType = 5;
		hoverOver = true;
	}
	else
	{
		hoverOver = false;
	}

	if(spawnedTower == NULL)
	{
		if( cannonButton->handle_events( game_data ))
		{
			//If you can afford the tower
			if(myResources->get_money() >= 100)
			{
				spawnedTower = spawnTower( game_data, 1, myWindow );
				myResources->add_money(-1*spawnedTower->getBuyCost());
				EnemyFocus = NULL;
				TowFocus = spawnedTower;
			}
			else
			{
				Mix_PlayChannel( -1, game_data->get_sound("strange"), 0 );
			}
		}
		if( tankButton->handle_events( game_data ))
		{
			if(myResources->get_money() >= 150)
			{
				spawnedTower = spawnTower( game_data, 2, myWindow );
				myResources->add_money(-1*spawnedTower->getBuyCost());
				EnemyFocus = NULL;
				TowFocus = spawnedTower;
			}
			else
			{
				Mix_PlayChannel( -1, game_data->get_sound("strange"), 0 );
			}
		}
		if( iceButton->handle_events( game_data ))
		{
			if(myResources->get_money() >= 200)
			{
				spawnedTower = spawnTower( game_data, 3, myWindow );
				myResources->add_money(-1*spawnedTower->getBuyCost());
				EnemyFocus = NULL;
				TowFocus = spawnedTower;
			}
			else
			{
				Mix_PlayChannel( -1, game_data->get_sound("strange"), 0 );
			}
		}
		if( nukeButton->handle_events( game_data ))
		{
			if(myResources->get_money() >= 300)
			{

				spawnedTower = spawnTower( game_data, 4 , myWindow );
				myResources->add_money(-1*spawnedTower->getBuyCost());
				EnemyFocus = NULL;
				TowFocus = spawnedTower;
			}
			else
			{
				Mix_PlayChannel( -1, game_data->get_sound("strange"), 0 );
			}
		}
		if( laserButton->handle_events( game_data ))
		{
			if(myResources->get_money() >= 300)
			{
				spawnedTower = spawnTower( game_data, 5 , myWindow  );
				myResources->add_money(-1*spawnedTower->getBuyCost());
				EnemyFocus = NULL;
				TowFocus = spawnedTower;
			}
			else
			{
				Mix_PlayChannel( -1, game_data->get_sound("strange"), 0 );
			}
		}

		if( sellButton->handle_events( game_data ) )
		{
			TowFocus->setDead();
			myResources->add_money( TowFocus->getSellCost() );
			TowFocus = NULL;
			EnemyFocus = NULL;
		}
	}

	if( (TowFocus != NULL) && (spawnedTower == NULL) )
	{
		int ug_teir = TowFocus->getUpgradeTeir();

		//If You Can Evolve your tower
		if( (ug_teir == 10) || (ug_teir == 20) || (ug_teir == 30) )
		{
			if(upgradePath1->handle_events( game_data ))
			{
				//Change your upgrade path appropriatly
				switch(ug_teir){
				case 10:
					//Check to see if you have enough money for the upgrade
					if(myResources->get_money() > 50)
					{
						TowFocus->setUpgradeTeir(11);
						myResources->add_money(-50);
						Mix_PlayChannel( -1, game_data->get_sound("powerup"), 0 );
						TowFocus->handleUpgrades();
					}
					else
					{
						Mix_PlayChannel( -1, game_data->get_sound("strange"), 0 );
					}
					break;
				case 20:
					//Check to see if you have enough money for the upgrade
					if(myResources->get_money() > 50)
					{
						TowFocus->setUpgradeTeir(31);
						myResources->add_money(-50);
						Mix_PlayChannel( -1, game_data->get_sound("powerup"), 0 );
						TowFocus->handleUpgrades();
					}
					else
					{
						Mix_PlayChannel( -1, game_data->get_sound("strange"), 0 );
					}
					break;
				case 30:
					//Check to see if you have enough money for the upgrade
					if(myResources->get_money() > 50)
					{
						TowFocus->setUpgradeTeir(51);
						myResources->add_money(-50);
						Mix_PlayChannel( -1, game_data->get_sound("powerup"), 0 );
						TowFocus->handleUpgrades();
					}
					else
					{
						Mix_PlayChannel( -1, game_data->get_sound("strange"), 0 );
					}
					break;
				}
			}
			if( upgradePath2->handle_events( game_data ))
			{
				//Change your upgrade path appropriatly
				switch(ug_teir){
				case 10:
					//Check to see if you have enough money for the upgrade
					if(myResources->get_money() > 50)
					{
						TowFocus->setUpgradeTeir(21);
						myResources->add_money(-50);
						Mix_PlayChannel( -1, game_data->get_sound("powerup"), 0 );
						TowFocus->handleUpgrades();
					}
					else
					{
						Mix_PlayChannel( -1, game_data->get_sound("strange"), 0 );
					}
					break;
				case 20:
					//Check to see if you have enough money for the upgrade
					if(myResources->get_money() > 50)
					{
						TowFocus->setUpgradeTeir(41);
						myResources->add_money(-50);
						Mix_PlayChannel( -1, game_data->get_sound("powerup"), 0 );
						TowFocus->handleUpgrades();
					}
					else
					{
						Mix_PlayChannel( -1, game_data->get_sound("strange"), 0 );
					}
					break;
				case 30:
					//Check to see if you have enough money for the upgrade
					if(myResources->get_money() > 50)
					{
						TowFocus->setUpgradeTeir(61);
						myResources->add_money(-50);
						Mix_PlayChannel( -1, game_data->get_sound("powerup"), 0 );
						TowFocus->handleUpgrades();
					}
					else
					{
						Mix_PlayChannel( -1, game_data->get_sound("strange"), 0 );
					}
					break;
				}
			}
		}
		//Non-Evolution upgrading
		else
		{
			if( upgradeButton1->handle_events( game_data ) && TowFocus->getUpgradeTeir()%10 == 1)
			{
				//Check to see if you have enough money for the upgrade
				if(myResources->get_money() > 50)
				{
					TowFocus->upgradeTeirPP();
					myResources->add_money(-50);
					Mix_PlayChannel( -1, game_data->get_sound("powerup"), 0 );
					TowFocus->handleUpgrades();
				}
				else
				{
					Mix_PlayChannel( -1, game_data->get_sound("strange"), 0 );
				}
			}
			if( upgradeButton2->handle_events( game_data ) && TowFocus->getUpgradeTeir()%10 == 2)
			{
				//Check to see if you have enough money for the upgrade
				if(myResources->get_money() > 50)
				{
					TowFocus->upgradeTeirPP();
					myResources->add_money(-50);
					Mix_PlayChannel( -1, game_data->get_sound("powerup"), 0 );
					TowFocus->handleUpgrades();
				}
				else
				{
					Mix_PlayChannel( -1, game_data->get_sound("strange"), 0 );
				}
			}
			if( upgradeButton3->handle_events( game_data )  && TowFocus->getUpgradeTeir()%10 == 3)
			{
				//Check to see if you have enough money for the upgrade
				if(myResources->get_money() > 50)
				{
					TowFocus->upgradeTeirPP();
					myResources->add_money(-50);
					Mix_PlayChannel( -1, game_data->get_sound("powerup"), 0 );
					TowFocus->handleUpgrades();
				}
				else
				{
					Mix_PlayChannel( -1, game_data->get_sound("strange"), 0 );
				}
			}
			if( upgradeButton4->handle_events( game_data ) && TowFocus->getUpgradeTeir()%10 == 4)
			{
				//Check to see if you have enough money for the upgrade
				if(myResources->get_money() > 50)
				{
					TowFocus->upgradeTeirPP();
					myResources->add_money(-50);
					Mix_PlayChannel( -1, game_data->get_sound("powerup"), 0 );
					TowFocus->handleUpgrades();
				}
				else
				{
					Mix_PlayChannel( -1, game_data->get_sound("strange"), 0 );
				}
			}
			if( upgradeButton5->handle_events( game_data ) && TowFocus->getUpgradeTeir()%10 == 5)
			{
				//Check to see if you have enough money for the upgrade
				if(myResources->get_money() > 50)
				{
					//Move the upgrade teir 5 points so it gets to the evolve section (evolve happens on multiples of 10)
					TowFocus->upgradeTeirPP();
					TowFocus->upgradeTeirPP();
					TowFocus->upgradeTeirPP();
					TowFocus->upgradeTeirPP();
					TowFocus->upgradeTeirPP();
					myResources->add_money(-50);
					Mix_PlayChannel( -1, game_data->get_sound("powerup"), 0 );
					TowFocus->handleUpgrades();
				}
				else
				{
					Mix_PlayChannel( -1, game_data->get_sound("strange"), 0 );
				}

			}
		}
	}

	if( ( game_data->get_EventStruc()->type == SDL_MOUSEBUTTONDOWN ) && ( game_data->get_EventStruc()->button.button == SDL_BUTTON_LEFT ) && (spawnedTower == NULL) )
	{
		std::list<Tower*>::iterator it;
		//Did you just click a tower?
		for( it=game_data->get_AllTowers()->begin(); it != game_data->get_AllTowers()->end(); it++)
		{
			if( game_data->get_EventStruc()->button.x > ( xRelCam((*it)->getX(), myWindow) - Tower::TOWER_WIDTH/2) && game_data->get_EventStruc()->button.x < ( xRelCam((*it)->getX(), myWindow) + Tower::TOWER_WIDTH/2))
			{
				if( game_data->get_EventStruc()->button.y > ( yRelCam((*it)->getY(), myWindow) - Tower::TOWER_HEIGHT/2) && game_data->get_EventStruc()->button.y < ( yRelCam((*it)->getY(), myWindow) + Tower::TOWER_HEIGHT/2))
				{
					TowFocus = *it;
					EnemyFocus = NULL;
				}
			}
		}

		std::list<Enemy*>::iterator it2;
		//Did you just click a Enemy?
		for( it2=game_data->get_AllEnemies()->begin(); it2 != game_data->get_AllEnemies()->end(); it2++)
		{
			if( game_data->get_EventStruc()->button.x > xRelCam((*it2)->getX(), myWindow) && game_data->get_EventStruc()->button.x < (xRelCam((*it2)->getX(), myWindow) + Enemy::ENEMY_WIDTH))
			{
				if( game_data->get_EventStruc()->button.y > yRelCam((*it2)->getY(), myWindow) && game_data->get_EventStruc()->button.y < (yRelCam((*it2)->getY(), myWindow) + Enemy::ENEMY_HEIGHT))
				{
					EnemyFocus = *it2;
					TowFocus = NULL;
				}
			}
		}
	}

	if(spawnedTower != NULL)
	{
		if( spawnedTower->getPlanted() == false )
		{
			//Check that you aren't trying to plant your tower ontop of another tower
			std::list<Tower*>::iterator it;

			spawnedTower->setTowOntop(false);

			for( it=game_data->get_AllTowers()->begin(); it != game_data->get_AllTowers()->end(); it++)
			{
				//Dont check yourself
				if( (*it) != spawnedTower )
				{
					if( spawnedTower->getX_cam( myWindow ) > ((*it)->getX_cam( myWindow ) - Tower::TOWER_WIDTH) && spawnedTower->getX_cam(myWindow) < ( (*it)->getX_cam(myWindow) + Tower::TOWER_WIDTH))
					{
						if( spawnedTower->getY_cam( myWindow ) > ( (*it)->getY_cam( myWindow ) - Tower::TOWER_HEIGHT) && spawnedTower->getY_cam( myWindow ) < ( (*it)->getY_cam( myWindow ) + Tower::TOWER_HEIGHT))
						{
							spawnedTower->setTowOntop(true);
							break;
						}
					}
				}
			}
		}
	}


	if( (game_data->get_EventStruc()->type == SDL_MOUSEBUTTONDOWN ) && ( game_data->get_EventStruc()->button.button == SDL_BUTTON_RIGHT) && (spawnedTower != NULL) )
	{
		if( spawnedTower->getTowOntop() == false )
		{
			spawnedTower->setPlanted( true );
			spawnedTower = NULL;
		}
	}
}

void UI::nullFocus()
{
	TowFocus = NULL;
	EnemyFocus = NULL;
}

Tower* UI::getTowFocus()
{
	return TowFocus;
}

Enemy* UI::getEnemyFocus()
{
	return EnemyFocus;
}

//Spawn a new tower
//Only Occurs during UI interaction so UI is a good place for it
Tower* spawnTower( GameData* game_data, int type, Window* myWindow )
{
	Tower* newTower = new Tower( type, myWindow, game_data );

	assert( newTower != NULL );

	game_data->get_AllTowers()->push_back(newTower);

	return newTower;
}

int xRelCam( int x, Window* myWindow )
{
	return(x - myWindow->get_camera()->getX());
}

int yRelCam( int y, Window* myWindow )
{
	return(y - myWindow->get_camera()->getY());
}
