#include "nhl_team.h"

nhl_team::nhl_team()
{
	m_roster = 0;
	m_bInit = 0;
}

nhl_team::nhl_team(string strFilename, int nAdx)
{
	m_strFilename = strFilename;
	m_nAdx = nAdx;
	m_roster = 0;
	m_bInit = 0;
}

nhl_team::~nhl_team()
{
	if(m_roster) {
		delete[] m_roster;
	}
}

void nhl_team::GetRoster()
{
	int nCurPos;
	if(m_bInit == 1) {
		delete[] m_roster;
	}
	m_roster = new nhl_player[m_nNumPlayers];
	m_bInit = 1;
	
	ifstream file;
	
	file.open(m_strFilename.data(), ios::in | ios::out | ios::binary);
	file.seekg(m_nAdx);

	int nCurNameLength;
	m_nOrigRosterLength = 0;
	nCurPos = m_nAdx;
	for(int p=0; p<m_nNumPlayers; p++) {
		nCurPos = m_roster[p].ParseChunk(file,nCurPos);
		
		// first m_nNumGoalies players are goalies
		if(p < m_nNumGoalies) {
			m_roster[p].SetGoalie(true);
		}
		
		nCurNameLength = m_roster[p].GetName().length();
		if(nCurNameLength%2 == 1) {
			m_nOrigRosterLength += nCurNameLength + 1;
		}
		else {
			m_nOrigRosterLength += nCurNameLength;
		}
	}

	m_nSzRosterChunk = nCurPos - m_nAdx;
	file.close();
}

string nhl_team::GetName()
{
	return m_strName;
}

string nhl_team::GetCity()
{
	return m_strCity;
}

string nhl_team::GetArena()
{
	return m_strArena;
}

nhl_player* nhl_team::GetPlayer(int nIndex)
{
	return &m_roster[nIndex];
}

int nhl_team::GetNumPlayers()
{
	return m_nNumPlayers;
}

int nhl_team::GetLetterBudget()
{
	int nNewRosterLength = 0;
	int nCurNameLength;
	for(int p=0; p<m_nNumPlayers; p++) {
		nCurNameLength = m_roster[p].GetName().length();
		if(nCurNameLength%2 == 1) {
			nNewRosterLength += nCurNameLength + 1;
		}
		else {
			nNewRosterLength += nCurNameLength;
		}
	}
	return (m_nOrigRosterLength - nNewRosterLength);
}

void nhl_team::WriteFile(string strNewFilename)
{
	char *chMemblock1 = new char[m_nAdx];
	char *chMemblock2;
	int  nSzNewRosterChunk = 0;
	int  nAddedBytes;
	int  nSzInfile;
	int  nSzEndChunk;
	
	ifstream infile(m_strFilename.data(), ios::ate | ios::binary);
	ofstream outfile(strNewFilename.data(), ios::binary);
	if(outfile.is_open() && infile.is_open()) {
		// get infile size
		nSzInfile = infile.tellg();
		infile.seekg(0, ios::beg);

		// write all data up to this team
		infile.read(chMemblock1, m_nAdx);
		outfile.write(chMemblock1,m_nAdx);
		
		// write all players on this team
		for(int p=0; p<m_nNumPlayers; p++) {
			nSzNewRosterChunk += m_roster[p].WriteChunk(outfile);
		}
		
		// skip over player chunk in infile
		infile.seekg(m_nAdx + m_nSzRosterChunk);

		// write general team info
		// ...

		// write all data after this roster
		nSzEndChunk = nSzInfile - m_nAdx - m_nSzRosterChunk;
		chMemblock2 = new char[nSzEndChunk];
		infile.read(chMemblock2, nSzEndChunk);
		
		outfile.write(chMemblock2,nSzEndChunk);
	}
	else {
		printf("\nError opening file.");
	}

	// fix checksum (required if any changes are made to
	//  original bin file)
	// NOTE: online guide says address is 0x000FFACB
	nAddedBytes = nSzNewRosterChunk - m_nSzRosterChunk;
	int nOffset = 0x000FFACA + nAddedBytes;
	char chChecksum[2] = {78,117};
	outfile.seekp(nOffset, ios::beg);
	outfile.write(&chChecksum[0],2);
	

	infile.close();
	outfile.close();

	delete[] chMemblock1;
	delete[] chMemblock2;
}

void nhl_team::SetName(string strName)
{
	m_strName = strName;
}

void nhl_team::SetAbbr(string strAbbr)
{
	m_strAbbr = strAbbr;
}

void nhl_team::SetArena(string strArena)
{
	m_strArena = strArena;
}

void nhl_team::SetCity(string strCity)
{
	m_strCity = strCity;
}

void nhl_team::SetAdx(int nAdx)
{
	m_nAdx = nAdx;
}

void nhl_team::SetNumPlayers(int nNumPlayers)
{
	m_nNumPlayers = nNumPlayers;
}

void nhl_team::SetNumGoalies(int nNumGoalies)
{
	m_nNumGoalies = nNumGoalies;
}

void nhl_team::SetFilename(string strFilename)
{
	m_strFilename = strFilename;
}
