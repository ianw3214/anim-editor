#include "serializer.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

#include "controller.hpp"

using json = nlohmann::json;

Serializer::Serializer()
{

}

bool Serializer::WriteToFile(const std::string& file)
{
	std::ofstream outfile(file);
	if (outfile.is_open())
	{
		json output;

		output["path"] = Controller::GetCurrentSpritesheetPath();
		output["frame_width"] = Controller::GetFrameWidth();
		output["frame_height"] = Controller::GetFrameHeight();
		output["states"] = json::array();
		for (const AnimData::State& state : Controller::GetStates())
		{
			json out_state;
			out_state["name"] = state.m_name;
			out_state["start"] = state.m_start;
			out_state["end"] = state.m_end;
			json transitions;
			for (const auto& pair : state.m_transitions)
			{
				transitions[pair.first] = pair.second->m_name;
			}
			out_state["transitions"] = transitions;
			output["states"].push_back(out_state);
		}

		outfile << output;
		return true;
	}

	return false;

}