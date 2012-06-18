#include "nhl_player.h"

nhl_player::nhl_player()
{
	//SetLookupTables();
	m_bIsGoalie = false;
}

nhl_player::~nhl_player()
{
}

int nhl_player::ParseChunk(ifstream& file, int nAdx)
{
	char chSpacer;

	// name length
	char chLengthCode;
	file.read(&chLengthCode,1);
	nAdx += 1;

	// add 1 since it doesn't count the space between
	//  first and last names
	int nNameLength = chLengthCode - 3 + 1;
	
	// actual name
	// (max "length" is 19, +1 for space, +1 for terminator)
	char chName[21];
	file.read(&chName[0],nNameLength);
	nAdx += nNameLength;

	// remove extra space if present
	if(nNameLength % 2 == 0) {
		chName[nNameLength] = '\0';
	}
	else {
		chName[nNameLength+1] = '\0';
	}
	m_strName = chName;

	// jersey number
	char chNumber;
	file.read(&chNumber,1);
	nAdx += 1;
	m_chNumber = chNumber;
	
	// ratings
	char chRatings[7];
	file.read(&chRatings[0], 7);
	nAdx += 7;
	memcpy(m_chRatings, chRatings, 7);

	// read 1 byte of empty space between players
	file.read(&chSpacer, 1);
	nAdx += 1;
	
	return nAdx;
}

string nhl_player::GetName()
{
	return m_strName;
}

int nhl_player::GetNumber()
{
	char chNumber[3];
	sprintf(chNumber, "%x", m_chNumber & 0xFF);
	return atoi(chNumber);
}

void nhl_player::SetName(string strName)
{
	m_strName = strName;
}

void nhl_player::SetNumber(int nNumber)
{
	m_chNumber = nNumber;
}

void nhl_player::SetRating(int nRating, char chValue)
{
	char chNibble1;
	char chNibble2;
	int nPos = (nRating-1) / 2;
	int nRem = (nRating-1) % 2;

	if(nRem) {
		// change low nibble
		m_chRatings[nPos] = (m_chRatings[nPos] & 0xF0) | (chValue & 0x0F);
	}
	else {
		// change high nibble
		m_chRatings[nPos] = (m_chRatings[nPos] & 0x0F) | ((chValue<<4) & 0xF0);
	}
}

string nhl_player::GetRating(int nRating)
{
	char chRating;
	char chTemp[50];
	string strRating;
	if(nRating == 1) {
		// weight
		chRating = (m_chRatings[0] & 0xF0) >> 4;
		int nLow = (int)floor(chRating * 7.5 + 140);
		int nHigh = (int)floor((chRating+1) * 7.5 + 140);
		sprintf(chTemp, "%d-%d lbs", nLow, nHigh);
		strRating = chTemp;
	}
	else if(nRating == 8) {
		// handedness
		chRating = m_chRatings[3] & 0x0F;
		if(chRating % 2) {
			strRating = "R";
		}
		else {
			strRating = "L";
		}
	}
	else {
		if((nRating-1) % 2) {
			chRating = m_chRatings[(nRating-1)/2] & 0x0F;
		}
		else {
			chRating = (m_chRatings[(nRating-1)/2] & 0xF0) >> 4;
		}
		sprintf(chTemp, "%d", chRating);
		strRating = chTemp;
	}

	return strRating;
}

void nhl_player::SetGoalie(bool bIsGoalie)
{
	m_bIsGoalie = bIsGoalie;
}

bool nhl_player::IsGoalie()
{
	return(m_bIsGoalie);
}

int nhl_player::GetNameLength()
{
	return m_strName.length();
}

int	nhl_player::WriteChunk(ofstream& outfile)
{
	int nTotalBytes = 0;
	char chSpacer = 0;

	// write name "length"
	// only allowed odd-numbered name lengths
	// +4 is the code conversion factor
	char chNameLength = (char) ceil((double)m_strName.length() / 2.0) * 2 + 2;
	outfile.write(&chNameLength, 1);
	nTotalBytes += 1;
	
	// write name
	outfile.write(m_strName.data(),m_strName.length());
	nTotalBytes += m_strName.length();
	if(m_strName.length() % 2) {
		outfile.write(&chSpacer,1);
		nTotalBytes += 1;
	}

	// write jersery number
	outfile.write(&m_chNumber,1);
	nTotalBytes += 1;

	// write player ratings
	outfile.write(&m_chRatings[0],7);
	nTotalBytes += 7;

	// write empty space
	outfile.write(&chSpacer,1);
	nTotalBytes += 1;

	return nTotalBytes;
}
