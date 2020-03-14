//
// Facebook Programming Puzzle #1: Evil Gambling Monster
//                                 http://www.facebook.com/jobs_puzzles/?puzzle_id=1
//                         Author: David H. Flint
//                                 davidflint@ameritech.net
//                                 Thursday, February 1, 2007
//

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

// constants (easy to change for problem variations)
#define DAYS 30
#define CASINOS 9

const short STARTING_CASINO = 0;
const char CASINO[CASINOS][2] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

const short wins1[DAYS] = {	 53, -84,  50, -73,  54,  60,  74,  22, -63, -78,
							 75,  72, -46,  99, -33,  24,   6, -66,  77, -61,
							-60, -46, -52,  84,  91, -21, -52, -72, -39, -41};
const short wins2[DAYS] = {	 77, -86, -25,  27, -59, -71, -13, -85,  50,  24,
							-63,  26,  -4, -10,  25,  62, -85, -68,  96,  92,
							-29, -64, -54,  18, -79, -62,  97, -32, -35, -42};
const short wins3[DAYS] = {	 27, -57, -28, -98,  69,  12, -70, -43,  27,  80,
							 80,  64,   6, -23, -45, -68, -60, -31, -36, -63,
							-39,  34, -27,   7, -47,  -7,  44, -50,  60, -90};
const short wins4[DAYS] = {	  7, -12, -48,  79, -11, -78,  -8,  19, -21, -81,
							 -1, -40,  83, -95,  36, -62, -63,  76,   6,   0,
							-87,  67, -66, -15, -26, -14,  78, -81,  36,  38};
const short wins5[DAYS] = {	-71, -56, -73, -20, -77,  15,   2,  14, -66,  81,
							 33,  33, -59,  16,  37,  77,  53,  73,  53, -40,
							-26,  66, -73,   7, -48,   1,  93, -70,  19,  30};
const short wins6[DAYS] = {	 68,  47,  73,  94, -72,  96,  10,  30,  11,  44,
							 11, -56, -23,  51,  60, -86,  29,  13,  87, -17,
							 73, -39, -51, -99,  68,   1,   1,  62,  30, -79};
const short wins7[DAYS] = {	 -8,  -1,  68, -34,  -7,  96, -37, -96,  26,  73,
							 47, -62, -83, -76,  89,  77, -62,  18,  -9, -75,
							-99, -36, -14, -50, -36, -45,  50,  64, -83, -19};
const short wins8[DAYS] = {	 85,   9,  79,  53,  75, -28,  49, -62, -25, -24,
							-89, -77,  13, -72, -54,   2, -95, -17, -80,  -5,
							  8, -79,  59,  93, -30, -77, -51, -79,  87, -35};
const short wins9[DAYS] = {	  1,  72,  74, -20,  26,  49,  52, -25,  86, -72,
							 50,  97, -50, -36, -74,  -4,  65, -70,  78,  85,
							 25, -14, -93, -16, -20, -24,   7,  28,  -3,  -5};

const bool adj[CASINOS][CASINOS] =	{ {1, 1, 0, 1, 0, 0, 0, 0, 0},
									  {1, 1, 1, 0, 1, 0, 0, 0, 0},
									  {0, 1, 1, 0, 0, 1, 0, 0, 0},
									  {1, 0, 0, 1, 1, 0, 1, 0, 0},
									  {0, 1, 0, 1, 1, 1, 0, 1, 0},
									  {0, 0, 1, 0, 1, 1, 0, 0, 1},
									  {0, 0, 0, 1, 0, 0, 1, 1, 0},
									  {0, 0, 0, 0, 1, 0, 1, 1, 1},
									  {0, 0, 0, 0, 0, 1, 0, 1, 1} };

const short* wins[CASINOS] = { wins1, wins2, wins3, wins4, wins5, wins6, wins7, wins8, wins9 };

struct node
{
	short day;
	short casino;
	short winnings;
	vector<string> path;
	bool visited;
};

bool compare(const struct node* first, const struct node* second);

int main()
{
	vector<vector<struct node> > winnings_nodes(DAYS);
	vector<struct node*> sorted_winnings_nodes;

	short current_day;
	unsigned short current_casino;
	unsigned short possible_casino;
	unsigned short best_casino;
	vector<string> current_path;
	stack<short> path;

	vector<string>::reverse_iterator vector_reverse_iterator;

	unsigned short N;
	unsigned short M;
	unsigned short K;
	short WINNINGS;

	// initialize each winnings node
	for (current_day = 0; current_day < DAYS; current_day++)
	{
		winnings_nodes[current_day].resize(CASINOS);

		for (current_casino = 0; current_casino < CASINOS; current_casino++)
		{
			winnings_nodes[current_day][current_casino].day = current_day;
			winnings_nodes[current_day][current_casino].casino = current_casino;
			winnings_nodes[current_day][current_casino].visited = false;
		}
	}

	// initialize last day
	for (current_casino = 0; current_casino < CASINOS; current_casino++)
	{
		winnings_nodes[DAYS - 1][current_casino].winnings = wins[current_casino][DAYS - 1];
		winnings_nodes[DAYS - 1][current_casino].path.push_back(CASINO[current_casino]);
	}

	// calculate total winnings and path if we start playing at casino, current_casino, on day, current_day
	for (current_day = DAYS - 2; current_day >= 0; current_day--)
	{
		for (current_casino = 0; current_casino < CASINOS; current_casino++)
		{
			// we assume that a casino is always adjacent to itself, i.e. you can play at the same casino two days in a row
			best_casino = current_casino;

			// find the best casino to travel to next
			for (possible_casino = 0; possible_casino < CASINOS; possible_casino++)
			{
				if (adj[current_casino][possible_casino] && current_casino != possible_casino)
				{
					if (winnings_nodes[current_day + 1][possible_casino].winnings > winnings_nodes[current_day + 1][best_casino].winnings)
					{
						best_casino = possible_casino;
					}
				}
			}

			// mark this node with our findings
			winnings_nodes[current_day][current_casino].winnings = wins[current_casino][current_day];
			if (winnings_nodes[current_day + 1][best_casino].winnings < 0)
			{
			}
			else
			{
				winnings_nodes[current_day][current_casino].winnings += winnings_nodes[current_day + 1][best_casino].winnings;
				winnings_nodes[current_day][current_casino].path = winnings_nodes[current_day + 1][best_casino].path;
			}
			winnings_nodes[current_day][current_casino].path.push_back(CASINO[current_casino]);
		}
	}

	// determine where and when to start playing
	current_day = 0;
	current_casino = STARTING_CASINO;

	// add the starting node
	sorted_winnings_nodes.push_back(&winnings_nodes[current_day][current_casino]);
	winnings_nodes[current_day][current_casino].visited = true;

	current_path.push_back(CASINO[current_casino]);
	path.push(current_casino);

	// initialize for loop
	current_day++;
	current_casino = 0;

	// important variables: current_day, current_casino, current_path, path
	do
	{
		// find an unvisited node from here
		for (; current_casino < CASINOS; current_casino++)
		{
			if (adj[path.top()][current_casino])
			{
				if (!winnings_nodes[current_day][current_casino].visited)
				{
					break;
				}
			}
		}

		// check if we found a node
		if (CASINOS != current_casino)
		{
			// add node to our list
			winnings_nodes[current_day][current_casino].path.insert(winnings_nodes[current_day][current_casino].path.end(), current_path.rbegin(), current_path.rend());
			sorted_winnings_nodes.push_back(&winnings_nodes[current_day][current_casino]);
			winnings_nodes[current_day][current_casino].visited = true;

			// check if we are already on the last day
			if ((current_day + 1) >= DAYS)
				continue;

			// follow up on this node
			current_path.push_back(CASINO[current_casino]);
			path.push(current_casino);

			current_day++;
			current_casino = 0;
			continue;
		}

		// did not find a node, backtrack
		current_day--;
		current_casino = path.top();
		path.pop();
		current_path.pop_back();
		current_casino++;
	} while (!path.empty());

	// sort possibilities so that the best one is at the top
	sort(sorted_winnings_nodes.begin(), sorted_winnings_nodes.end(), compare);

	// You wish to determine how many N >= 0 days to not play,
	// how many subsequent M >= 0 consecutive days to play
	// (and how many remaining K >= 0 days you also don't play),
	// and the path to travel between casinos during this 30-day time period
	// such that you maximize your total winnings.

	N = sorted_winnings_nodes[0]->day;
	M = (short)sorted_winnings_nodes[0]->path.size() - N;
	K = DAYS - (M + N);
	WINNINGS = sorted_winnings_nodes[0]->winnings;

	// do we do better not playing at all
	if (WINNINGS < 0)
	{
		N = 0;
		M = 0;
		K = DAYS;
		WINNINGS = 0;
	}

	cout << "N: " << N << endl;
	cout << "M: " << M << endl;
	cout << "K: " << K << endl;
	cout << "PATH:" << endl;
	cout << "Day  Casino" << endl;

	if (M > 0)
	{
		for (current_day = 1, vector_reverse_iterator = sorted_winnings_nodes[0]->path.rbegin(); vector_reverse_iterator != sorted_winnings_nodes[0]->path.rend(); current_day++, vector_reverse_iterator++)
		{
			cout.width(4);
			cout << current_day << "  ";
			cout.width(4);
			cout << *vector_reverse_iterator << endl;
		}
		for (; current_day <= DAYS; current_day++)
		{
			cout.width(4);
			cout << current_day << "  ";
			cout.width(4);
			cout << sorted_winnings_nodes[0]->path.front() << endl;
		}
	}
	else
	{
		for (current_day = 1; current_day <= DAYS; current_day++)
		{
			cout.width(4);
			cout << current_day << "  ";
			cout.width(4);
			cout << CASINO[STARTING_CASINO] << endl;
		}
	}

	cout << "WINNINGS: $" << WINNINGS << endl;

	return 0;
}

bool compare(const struct node* first, const struct node* second)
{
	return (first->winnings > second->winnings);
}
