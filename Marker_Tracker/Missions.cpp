#include "Missions.h"

/*
* \brief returns the currently active missions of the game
* \return the array containing the missions
*/
std::array<Mission, 3> Missions::get_current_random_missions()
{
	return current_missions;
}

/**
	\brief initializes the missions object with 3 random questions of the form
	chooses 3 random questions for our missions and stores them in current_mission.
	The missions are stored in the format: <question text, function of question, color of that mission>
*/
Missions::Missions()
{
	std::array<Mission, 3> result;
	for (int i = 0; i < 3; i++)
	{
		/*int selected_question = std::rand() % possible_missions.size();
		int selected_color = i;
		*/
		int selected_question = i;
		int selected_color = i;
		auto chosen_question = possible_missions.at(selected_question);

		auto next_tuple = std::make_tuple(std::get<0>(chosen_question)+GameLogic_Utilities::get_name_of_color_by_index(selected_color),
			std::get<1>(chosen_question), selected_color);
		result[i] = next_tuple;

	}
	current_missions = result;
	output_missions();
}

/*
	\brief Definition of three tiles in a row: It means that you match a tile with another one in one color,
	and then match the marker opposite of that match again with the same color with a third tile. Requires 
	the tile in the middle to be full color, so that the two opposing markers are of the same color.
	\return if mission accomplished, the number of points you get for the mission, otherwise 0
*/
int Missions::three_tiles_in_a_row(int color, std::unordered_map<int, std::array<bool, 6>> matches_of_tiles)
{
	//check for each tile, if it is the middle of a row
	for (auto tiles : matches_of_tiles)
	{
		//only full color tiles can be the middle of a row
		int card_type = GameLogic_Utilities::determine_card_type(tiles.first);
		if (card_type != CARD_TYPE_ONE_COLOR)
			continue;
		int color_of_tile = GameLogic_Utilities::determine_marker_color(tiles.first * 6);
		if (color_of_tile == color)
		{
			//two opposing markers are matched?
			if (tiles.second[0] && tiles.second[3] ||
				tiles.second[1] && tiles.second[4] ||
				tiles.second[2] && tiles.second[5])
				return SCORE_PER_MISSION;
		}
	}
	return 0;
}

/*
	We want that in the game, there are three half-color tiles that are matched with all sides 
	All three of those tiles should have one side with the given color
	\return if mission accomplished, the number of points you get for the mission, otherwise 0
*/
int Missions::three_half_color(int color, std::unordered_map<int, std::array<bool, 6>> matches_of_tiles)
{
	int number_of_matched_tiles = 0;
	for (auto tiles : matches_of_tiles)
	{
		//only half-color tiles
		int card_type = GameLogic_Utilities::determine_card_type(tiles.first);
		if (card_type != CARD_TYPE_HALF_COLOR)
			continue;
		int color_of_tile1 = GameLogic_Utilities::determine_marker_color(tiles.first * 6);
		int color_of_tile2 = GameLogic_Utilities::determine_marker_color(tiles.first * 6+3);
		if (color_of_tile1 == color || color_of_tile2== color)
		{
			//all sides matched?
			if (tiles.second[0] && tiles.second[3] &&
				tiles.second[1] && tiles.second[4] &&
				tiles.second[2] && tiles.second[5])
				number_of_matched_tiles++;
			if (number_of_matched_tiles == 3)
				return SCORE_PER_MISSION;
		}
	}
	return 0;
}

/**
* \brief We want to check whether 7 independent tiles have all their sides of a certain color matched
* \return if mission accomplished, the number of points you get for the mission, otherwise 0
* 
*/
int Missions::seven_of_color(int color, std::unordered_map<int, std::array<bool, 6>> matches_of_tiles)
{
	int number_of_matched_tiles = 0;
	for (auto tiles : matches_of_tiles)
	{
		bool valid_tile = false;
		//check all sides (markers)
		for (int i = 0; i < 6; i++)
		{
			int color_of_marker = GameLogic_Utilities::determine_marker_color(tiles.first*6 + i);
			if (color_of_marker == color)
			{
				valid_tile = true;
				if (!tiles.second[i])
				{
					valid_tile = false;
					break;
				}
			}
		}
		if (valid_tile)
			number_of_matched_tiles++;
		if (number_of_matched_tiles >= 7)
			return SCORE_PER_MISSION;

	}
	return 0;
}

/**
* \brief computes how many points you get for achieved missions
* \return if mission accomplished, the number of points you get for the mission, otherwise 0
*/
int Missions::computeMissionScore()
{
	int score = 0;
	std::array<bool, 3> prevAchievedMissions = finished_missions;
	int i = 0;
	for (auto mission : current_missions)
	{
		int missionScore = std::get<1>(mission)(std::get<2>(mission), GameLogic_Utilities::get_current_matched_markers_per_tiles());
		score += missionScore;
		if (missionScore > 0)
			finished_missions[i] = true;
		else
			finished_missions[i] = false;
		i++;
	}
	for (i = 0; i < 3; i++) 
	{
		if (prevAchievedMissions[i] != finished_missions[i])
		{
			output_missions();
			break;
		}
	}
	return score;
}



/**
* \brief console output of the missions
*/
void Missions::output_missions()
{
	std::cout << "---------Update of current mission status----------" << std::endl;
	int i = 0;
	for (int i =0; i< current_missions.size();i++)
	{
		
		std::cout << get_mission_status_as_string(i) << std::endl;
	}
	std::cout << "---------------------------------------------------" << std::endl;
}
/**
* \brief creates a formatted summary of the mission's status
* \return formatted string
*/
std::string Missions::get_mission_status_as_string(int missionNr)
{
	if (missionNr < 0 || missionNr >= current_missions.size())
		return "";
	std::string status;
	Mission m = current_missions.at(missionNr);
	if (finished_missions[missionNr])
		status = "DONE";
	else
		status = "TODO";
	std::string result = "Mission " + to_string(missionNr + 1) + ": " + std::get<0>(m)
		+ " (" + status + "), Points for Mission: " + to_string(SCORE_PER_MISSION);
	return result;
}


/**
* \return whether the mission of index i is completed by the player
*/
bool Missions::status_of_mission(int index)
{
	if(current_missions.size()<= index || index <0)
		return false;
	return finished_missions[index];
}


