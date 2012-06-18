#ifndef NHL_TEAM
#define NHL_TEAM

#include <string>
#include <fstream>
#include <iostream>
#include "nhl_player.h"
using namespace std;

class nhl_team
{
	public:
		nhl_team();
		nhl_team(string strFilename, int nAdx);
		~nhl_team();
		
		void GetRoster();
		nhl_player* GetPlayer(int nIndex);
		string      GetArena();
		string      GetCity();
		string      GetName();
		int         GetNumPlayers();
		int			GetLetterBudget();
		void WriteFile(string strNewFilename);
		void SetAdx(int nAdx);
		void SetName(string strName);
		void SetAbbr(string strAbbr);
		void SetCity(string strCity);
		void SetArena(string strArena);
		void SetNumPlayers(int nNumPlayers);
		void SetNumGoalies(int nNumGoalies);
		void SetFilename(string strFilename);

	private:
		int         m_nAdx;
		int			m_nSzRosterChunk;
		int         m_nNumPlayers;
		int			m_nNumGoalies;
		int			m_nOrigRosterLength;
		string      m_strFilename;
		string      m_strName;
		string      m_strCity;
		string      m_strAbbr;
		string      m_strArena;
		nhl_player* m_roster;
		bool		m_bInit;	// used to track whether dynamic memory was allocated
								// (couldn't seem to track it accurately w/ constructor)
	
};

#endif
