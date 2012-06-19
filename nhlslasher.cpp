#include <string>
#include <iostream>
#include <fstream>
#include "nhl_player.h"
#include "nhl_team.h"
#include "nhl_league.h"
using namespace std;

int TeamPicker(nhl_league* league)
{
	// assume league->FindTeams() has already been called
	//  (teams already populated in object)
	int nNumTeams = league->GetNumTeams();
	printf("\n\nAvailable teams:");
	for(int t=0; t<nNumTeams; t++) {
		printf("\n%d. %s", t+1, league->GetTeam(t)->GetName().data());
	}
	printf("\n0. Go back");
	printf("\n\nEnter number of team to edit: ");
	int nTeam;
	do {
		cin >> nTeam;
	} while(nTeam<0 || nTeam>nNumTeams);

	return nTeam-1;
}

void LetterChecker(nhl_team* team)
{
	int nBudget = team->GetLetterBudget();
	if(nBudget != 0) {
		printf("\n\nWARNING: Player roster has %d letter(s)", (int)abs((double)nBudget));

		if(nBudget<0) {
			printf(" too many.");
		}
		else {
			printf(" too few.");
		}

		printf(" This will likely make the game unstable. Adjust player names accordingly.");
	}
}

int PlayerPicker(nhl_team* team)
{
	// assume team->GetRoster() has already been called
	//  (player roster already populated in object)
	int nNumPlayers = team->GetNumPlayers();
	printf("\n\nAvailable players:");
	for(int p=0; p<nNumPlayers; p++) {
		printf("\n%d. %s", p+1, team->GetPlayer(p)->GetName().data());
		if(team->GetPlayer(p)->IsGoalie()) {
			printf(" (goalie)");
		}
	}
	printf("\n0. Go back");

	LetterChecker(team);
	
	printf("\n\nEnter number of player to edit: ");
	int nPlayer;
	do {
		cin >> nPlayer;
	} while(nPlayer<0 || nPlayer>nNumPlayers);

	return nPlayer-1;
}

int ChangePicker(nhl_player* curplayer)
{
	printf("\n\nAvailable changes for %s:", curplayer->GetName().data());
	printf("\n1. Name");
	printf("\n2. Jersey number (%d)", curplayer->GetNumber());
	printf("\n3. Attributes");
	printf("\n0. Go back");
	printf("\nEnter number of item to change: ");
	int nChange;
	do {
		//cin.get(); // flush input buffer (should only contain '\n')
		cin.clear();
		//cin.ignore(INT_MAX, '\n');
		cin >> nChange;
	} while(nChange<0 || nChange>3);

	return nChange;
}

int RatingPicker(nhl_player* curplayer)
{
	printf("\n\n%-26sCurrent Values", curplayer->IsGoalie() ? "Goalie attributes" : "Player attributes");
	printf("\n----------------------------------------");
	//if(curplayer->IsGoalie()) {
	//	printf(" (goalie)");
	//}
	//printf(":");
	printf("\n1. %-23s%s", "Weight",  curplayer->GetRating(1).data());
	printf("\n2. %-23s%s", "Agility", curplayer->GetRating(2).data());
	printf("\n3. %-23s%s", "Speed",   curplayer->GetRating(3).data());
	if(!curplayer->IsGoalie()) {
		printf("\n4. %-23s%s", "Offensive awareness", curplayer->GetRating(4).data());
	}
	printf("\n5. %-23s%s", "Defensive awareness", curplayer->GetRating(5).data());
	if(curplayer->IsGoalie()) {
		printf("\n6. %-23s%s", "Puck control", curplayer->GetRating(6).data());
	}
	else {
		printf("\n6. %-23s%s", "Shot power", curplayer->GetRating(6).data());
	}
	if(!curplayer->IsGoalie()) {
		printf("\n7. %-23s%s", "Checking",   curplayer->GetRating(7).data());
	}
	printf("\n8. %-23s%s", "Handedness", curplayer->GetRating(8).data());
	if(!curplayer->IsGoalie()) {
		printf("\n9. %-23s%s",  "Stick handling", curplayer->GetRating(9).data());
		printf("\n10. %-22s%s", "Shot accuracy",  curplayer->GetRating(10).data());
		printf("\n11. %-22s%s", "Endurance",      curplayer->GetRating(11).data());
		printf("\n13. %-22s%s", "Pass accuracy",  curplayer->GetRating(13).data());
		printf("\n14. %-22s%s", "Aggressiveness", curplayer->GetRating(14).data());
	}
	else {
		printf("\n11. %-22s%s", "Stick handling right",  curplayer->GetRating(11).data());
		printf("\n12. %-22s%s", "Stick handling, left",  curplayer->GetRating(12).data());
		printf("\n13. %-22s%s", "Glove handling, right", curplayer->GetRating(13).data());
		printf("\n14. %-22s%s", "Glove handling, left",  curplayer->GetRating(14).data());
	}
	printf("\n0. Go back");
	printf("\nEnter number of attribute to change for %s: ", curplayer->GetName().data());

	int nRating;
	do {
		cin >> nRating;
	} while(nRating<0 || nRating>14);

	return nRating;
}

char RatingValuePicker(int nRating, nhl_player* curplayer)
{
	int nRatingValue;
	if(nRating == 1) {
		printf("\nEnter %s's new weight, 140-259 lbs: ", curplayer->GetName().data());
		do {
			cin >> nRatingValue;
		} while(nRatingValue<140 || nRatingValue>259);

		nRatingValue = (int)floor(((double)nRatingValue - 140.0) / 7.5);
		if(nRatingValue<0 || nRatingValue>15) {
			printf("\n\nError: player weight value out of range");
		}
	}
	else if(nRating==8){
		printf("\nEnter %s's new handedness (0=left, 1=right): ", curplayer->GetName().data());
		do {
			cin >> nRatingValue;
		} while(nRatingValue != 0 && nRatingValue != 1);
		//} while(nRatingValue!='L' && nRatingValue!='l' && nRatingValue!='R' && nRatingValue!='r');

		/*
		if(nRatingValue=='L' || nRatingValue=='l') {
			nRatingValue = 0;
		}
		else {
			nRatingValue = 1;
		}
		*/
	}
	else {
		printf("\nEnter %s's new attribute value, 0-6 (0 is worst): ", curplayer->GetName().data());
		do {
			cin >> hex >> nRatingValue;
			cin >> dec;
		} while(nRatingValue<0 || nRatingValue>6);
	}
	
	return (char)nRatingValue;
}

int MainMenu()
{
	printf("\n\nActions:");
	printf("\n1. Edit team");
	printf("\n2. Write file");
	printf("\n0. Quit");
	printf("\nEnter action number: ");
	int nAction;
	do {
		cin >> nAction;
	} while(nAction<0 || nAction>2);

	return nAction;
}

int main()
{
	int nMain;
	int nTeam;
	int nPlayer;
	int nChange;
	int nRating;
	char chValue;
	string strFilename;
	nhl_league league;
	nhl_team *curteam;
	nhl_player *curplayer;

	printf("\n\n===================================");
	printf(  "\nNHL Slasher");
	printf(  "\nHacking Utility for EA Sports NHL94");
	printf(  "\nWritten by James Painter");
	printf(  "\n===================================");

	printf("\n\nEnter ROM file name: ");
	cin >> strFilename;

	do {
		// MAIN LOOP ----------------------
		nMain = MainMenu();

		if(nMain == 1) {
			// EDIT TEAM ------------------
			league.SetFilename(strFilename);
			league.FindTeams();
			nTeam = TeamPicker(&league);
			curteam = league.GetTeam(nTeam);
			curteam->GetRoster();

			do {
				// EDIT PLAYER ------------
				nPlayer = PlayerPicker(curteam);
				// need to put check here for player==0
				if(nPlayer == -1) {
					break;
				}
				curplayer = curteam->GetPlayer(nPlayer);
				
				do {
					nChange = ChangePicker(curplayer);
					if(nChange == 1) {
						// EDIT NAME ---------
						string strNewName;
						printf("\nEnter new name for %s: ", curplayer->GetName().data());
						cin.get(); // flush input buffer (should only contain '\n')
						getline(cin,strNewName);
						curplayer->SetName(strNewName);
						LetterChecker(curteam);
					}
					else if(nChange == 2) {
						// EDIT NUMBER -------
						int nNewNumber;
						printf("\nEnter new number for %s: ", curplayer->GetName().data());
						do {
							cin >> hex >> nNewNumber;
						} while((nNewNumber > 0x99) || (nNewNumber < 0x00));
						cin >> dec;
							
						curplayer->SetNumber(nNewNumber);
					}
					else if(nChange == 3) {
						// EDIT RATINGS ------
						do {
							nRating = RatingPicker(curplayer);
							if(nRating == 0) {
								break;
							}
							chValue = RatingValuePicker(nRating,curplayer);
							curplayer->SetRating(nRating, chValue);
						} while(nRating != 0);
					}
				} while(nChange != 0);
			} while(nPlayer != -1);

		}

		if(nMain == 2) {
			// WRITE FILE ---------------
			string strResponse;
			string strNewFilename;
			printf("\n\nEnter new ROM filename: ");
			cin >> strNewFilename;
			printf("\nWrite new ROM now? (y or n) ");
			cin >> strResponse;
			if(strResponse == "y") {
				curteam->WriteFile(strNewFilename);
			}
		}

	} while(nMain != 0);
		

	return 0;
}
