PROGNAME=nhlslasher
CC=g++

OPTS=-g -o $(PROGNAME)

SRCS=\
nhlslasher.cpp\
nhl_league.cpp\
nhl_player.cpp\
nhl_team.cpp

main:
	$(CC) $(OPTS) $(SRCS)
