#ifndef NHL_PLAYER
#define NHL_PLAYER

#include <string>
#include <fstream>
using namespace std;

class nhl_player
{
	public:
		nhl_player();
		~nhl_player();

		void 	SetName(string strName);
		void	SetGoalie(bool bIsGoalie);
		void	SetNumber(int nNumber);
		void	SetRating(int nRating, char nValue);
		string	GetName();
		int		GetNumber();
		string	GetRating(int nRating);
		bool	IsGoalie();
		int		ParseChunk(ifstream& file, int nAdx);
		int		WriteChunk(ofstream& outfile);
		int 	GetNameLength();

	private:
		int		m_nCharLengths[7][2];
		char	m_chNumber;	// STORED AS HEX!
		string	m_strName;
		char	m_chRatings[7];
		bool	m_bIsGoalie;
		
};

#endif
