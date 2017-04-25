#include "save.h"


int save(std::string &filename, Game &g){
	std::cout << "save" << std::endl;
	json j;
	// datum?
	j["m"] = g.m;
	j["decks"] = json::array();

	for(int i = 0; i < 13; ++i){
		Deck &deck = *(g.decks[i]);
		json tmp;
		tmp["permissions"] = deck.permissions;
		tmp["position"] = deck.position;
		tmp["cards"] = json::array();
	
		for(auto card = deck.cards.begin(); card != deck.cards.end(); card++){
			json c = {
				json::array({"color",card->color}),
				json::array({"number", card->number}),
				json::array({"visible", card->visible})
			};
			tmp["cards"] += c;
		}
		j["decks"] += tmp;

	}
	std::cout << ">" << filename << "<" << std::endl;
	std::ofstream save_file(filename);
	save_file << std::setw(2) << j << std::endl;

	save_file.close();

	return 0;
}

int load_game(std::string &filename, session_t *session){
	(void)filename;
	(void)session;

	int pos;
	if ((pos = session->isSpace()) == -1){
		err_msg("There is no open slot!");
		return 1;
	}
	Game *g = new Game();
	session->slot[pos] = g;
	session->currentGame = pos;
	session->openSlot[pos] = true;
	std::cout << "Game added to a slot No." << pos << std::endl;

	std::ifstream load_file(filename);
	json j_data;
	load_file >> j_data;
	load_file.close();

	g->m = j_data["m"];

	int deck_index = 0;

	for (json::iterator it = j_data["decks"].begin(); it != j_data["decks"].end(); ++it) {
		Deck *deck = new Deck();
		deck->deck = deck_index;
		load_deck(it, deck);
		g->decks[deck_index++] = deck;
	}


	return 0;
}




int load_deck(json::iterator element, Deck *deck){
	// std::cout << deck->deck << std::endl;
	deck->permissions = (*element)["permissions"];
	deck->position = (*element)["position"];

	for(auto& card: (*element)["cards"]){
		Card *c = new Card(static_cast<int>(card["number"]), static_cast<Color>(card["color"]), true);
		deck->cards.push_back(*c);
	}

	return 0;
}