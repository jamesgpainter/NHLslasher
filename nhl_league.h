#ifndef NHL_LEAGUE
#define NHL_LEAGUE


#include <string>
#include "nhl_team.h"
using namespace std;

class nhl_league
{
	public:
		nhl_league();
		nhl_league(string strFilename);
		~nhl_league();
		void FindTeams();
		nhl_team* GetTeam(int nIndex);
		int GetNumTeams();
		void SetFilename(string strFilename);

	private:
		void     Initialize();
		string   m_strFilename;
		nhl_team m_teams[28];
		int      m_nStartAdx;
		int      m_nTeamGap;
		int      m_nTeamHeader;
		int      m_nNumTeams;
};

#endif
