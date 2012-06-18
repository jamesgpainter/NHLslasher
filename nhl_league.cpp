#include "nhl_league.h"

nhl_league::nhl_league(string strFilename)
{
	m_strFilename = strFilename;
	Initialize();
}

nhl_league::nhl_league()
{
	Initialize();
}

nhl_league::~nhl_league()
{
}

void nhl_league::Initialize()
{
	//m_nStartAdx   = 0x00009F9;
	m_nStartAdx   = 0x00009B6;
	m_nTeamHeader = 65;
	m_nTeamGap    = 148 - m_nTeamHeader - 2;
	m_nNumTeams   = 28-2; // ignore all star teams for now
}

// just get general team data (not players)
void nhl_league::FindTeams()
{
	ifstream infile(m_strFilename.data());
	infile.seekg(m_nStartAdx);
	
	char chLengthCode;
	char chTeamFlag;
	char chSpacer;
	char chTeamCityLength;
	char chTeamAbbrLength;
	char chTeamNameLength;
	char chArenaNameLength;
	int  nNumPlayers;
	int  nNumGoalies;
	char chGoalies;
	char chTemp;
	char chPlayerName[21];
	char chPlayerData[9];
	char chTeamCity[25];
	char chTeamName[25];
	char chTeamAbbr[6];
	char chArenaName[35];
	
	// team loop:
	for(int t=0; t<m_nNumTeams; t++) {
		nNumPlayers = 0;

		// number of goalies
		nNumGoalies = 0;
		for(int b=0; b<2; b++) {
			infile.read(&chGoalies, 1);

			// read high nibble
			chTemp = chGoalies & 0xF0;
			if(chTemp != 0) {
				nNumGoalies++;
			}

			// read low nibble
			chTemp = chGoalies & 0x0F;
			if(chTemp != 0) {
				nNumGoalies++;
			}
		}
	
		// skip over garbage to team roster
		infile.seekg((int)(infile.tellg()) + m_nTeamHeader);
		m_teams[t].SetAdx((int)(infile.tellg()));
		
		// player loop:
		// (use infinite loop because each team has different number of players)
		while(1) {
			infile.read(&chLengthCode,1);
			if(chLengthCode == 0x02) {
				// no more players
				break;
			}
		
			// probably better to use seekg to move to next player
			// since we're not really using the name or data
			nNumPlayers++;
			infile.read(&chPlayerName[0], chLengthCode-3+1);
			infile.read(&chPlayerData[0], 9);
		}

		// one more byte before team name data
		infile.read(&chSpacer,1);

		infile.read(&chTeamCityLength,1);
		infile.read(&chTeamCity[0], chTeamCityLength-2);
		infile.read(&chSpacer,1);
		infile.read(&chTeamAbbrLength,1);
		infile.read(&chTeamAbbr[0], chTeamAbbrLength-1);

		// don't know why 2-letter abbrevs. have less space
		//  before arena name
		if(chTeamAbbrLength == 4) {
			chTeamAbbr[2] = '\0';
		}
		else {
			chTeamAbbr[3] = '\0';
		}
	
		infile.read(&chTeamNameLength,1);
		infile.read(&chTeamName[0], chTeamNameLength-1); 
		infile.read(&chArenaNameLength,1);
		infile.read(&chArenaName[0], chArenaNameLength-3);

		// put retrieved team data into team structure
		m_teams[t].SetName(chTeamName);
		m_teams[t].SetCity(chTeamCity);
		m_teams[t].SetAbbr(chTeamAbbr);
		m_teams[t].SetArena(chArenaName);
		m_teams[t].SetNumPlayers(nNumPlayers);
		m_teams[t].SetNumGoalies(nNumGoalies);
		m_teams[t].SetFilename(m_strFilename);
	
		// skip over garbage to next team
		infile.seekg((int)(infile.tellg()) + m_nTeamGap);
	}

	infile.close();
}

nhl_team* nhl_league::GetTeam(int nIndex)
{
	return &(m_teams[nIndex]);
}

int nhl_league::GetNumTeams()
{
	return m_nNumTeams;
}

void nhl_league::SetFilename(string strFilename)
{
	m_strFilename = strFilename;
}
