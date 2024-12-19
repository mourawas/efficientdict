#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

const string NOT_IN_CAPITAL_LETTERS("The word is not purely in capital letters");
const string DOUBLE_WORD("Each word can be present only once");
const string EMPTY_DICO("The dictionnary cannot be empty");
const string NO_ANAGRAM("There is no anagram for this message and this dictionary");

struct Word {
	string mot;
	unsigned int nbT;
	unsigned int nbD;
	string alpha;
};

void entree(vector<Word>& Dico, vector<string>& anagrammes);

void attribution_nbT(vector<Word>& Dico);

void attribution_nbD(vector<Word>& Dico);

void attribution_alpha(vector<Word>& Dico, int& min, int& max, int& i);

unsigned int recherche_dichotomique(vector<Word>& Dico, unsigned int N, unsigned int& pivot);

void tri_insertion_dichotomique(vector<Word>& Dico);

bool verif_anagrammes(string& anagramme, size_t& longueur_anagramme);

void enlever_blanks(string& anagramme, size_t& longueur_anagramme);

bool message_contient_mot(string alpha, string alpha_m);

bool rech_anagramme(vector<Word> Dico, string alpha_m, string anagramme);

string message_soustraire_mot(string alpha_m, string alpha);

vector<Word> enlever_mot(vector<Word>& Dico, size_t i);

void verif_dico(string& dico, size_t longueur_dico);

void remplissage_dico(vector<Word>& Dico, string& d, size_t longueur_dico);

void lance_anagrammes(vector<string>& anagrammes, vector<Word>& Dico);

int main()
{
	vector<Word> Dico;
	vector<string> anagrammes;

	entree(Dico, anagrammes);

	const int taille_dico = Dico.size();

	for (int i = 0; i < taille_dico; ++i) {
		Dico[i].alpha = Dico[i].mot;
	}

	attribution_nbT(Dico);

	attribution_nbD(Dico);

	for (int i = 0; i < taille_dico; ++i) {
		int zero = 0;
		int max = Dico[i].nbT - 1;
		attribution_alpha(Dico, zero, max, i);
	}

	tri_insertion_dichotomique(Dico);

	for (int i = 0; i < taille_dico; ++i)
	{
		cout << Dico[i].mot << "\n";
	}

	// ANAGRAMMES

	lance_anagrammes(anagrammes, Dico);
}

void entree(vector<Word>& Dico, vector<string>& anagrammes)
{
	string tout;
	getline(cin, tout, '*');
	const size_t longueur = tout.length();

	stringstream X(tout);

	string d;
	getline(X, d, '.');
	const size_t longueur_dico = d.length();

	verif_dico(d, longueur_dico);
	Dico.push_back(Word());
	remplissage_dico(Dico, d, longueur_dico);

	//ANAGRAMMES

	if (tout[longueur - 1] != ' ') {
		if (tout[longueur - 1] != '\n') {
			if (tout[longueur - 1] != '\t') {
				if (tout[longueur - 1] != '\r') {
					cout << NOT_IN_CAPITAL_LETTERS << "\n";
					exit(0);
				}
			}
		}
	}
	//Anagrammes commencent à longueur_dico +1

	string anag_full = tout.substr(longueur_dico + 1, longueur - longueur_dico - 1);
	string temp;
	stringstream ss(anag_full);

	while (getline(ss, temp, '.')) {
		anagrammes.push_back(temp);
	}

	anagrammes.pop_back();
}

void verif_dico(string& d, size_t longueur_dico)
{
	if (d.find_first_not_of(" \t\n\r") == SIZE_MAX) {
		cout << EMPTY_DICO << "\n";
		exit(0);
	}
	if (d[longueur_dico - 1] != ' ') {
		if (d[longueur_dico - 1] != '\n') {
			if (d[longueur_dico - 1] != '\t') {
				if (d[longueur_dico - 1] != '\r') {
					cout << NOT_IN_CAPITAL_LETTERS << "\n";
					exit(0);
				}
			}
		}
	}
	for (size_t i = 0; i < longueur_dico - 1; ++i) {
		if ((d[i] < 65) or (d[i] > 90)) {
			if (d[i] != ' ') {
				if (d[i] != '\n') {
					if (d[i] != '\t') {
						if (d[i] != '\r') {
							cout << NOT_IN_CAPITAL_LETTERS << "\n";
							exit(0);
						}
					}
				}
			}
		}
	}
}

void remplissage_dico(vector<Word>& Dico, string& d, size_t longueur_dico)
{
	size_t j = 0;
	size_t i = d.find_first_not_of(" \t\n\r");

	while(i < longueur_dico - 1) {

		if ((d[i] != ' ') and (d[i] != '\n') and (d[i] != '\t') and (d[i] != '\r')) {
			Dico[j].mot = Dico[j].mot + d[i];
			++i;
		}
		else if (d.find_first_not_of(" \t\n\r", i) != SIZE_MAX) {
			++j;
			i = d.find_first_not_of(" \t\n\r", i);
			Dico.push_back(Word());
			continue;
		}
		else {
			break;
		}
	}

	for (size_t i = 0; i < Dico.size() - 1; ++i) {
		for (size_t j = Dico.size() - 1; j > i; --j) {
			if (Dico[i].mot == Dico[j].mot) {
				cout << DOUBLE_WORD << "\n";
				exit(0);
			}
		}
	}
}

void lance_anagrammes(vector<string>& anagrammes, vector<Word>& Dico)
{
	vector<string> anagrammes_rech;
	anagrammes_rech.push_back("");

	for (int i = 0; i < static_cast<int>(anagrammes.size()); ++i) {
		cout << "\n";
		size_t longueur_anagramme = anagrammes[i].length();
		bool a, trouve;
		vector<Word> pour_alpha(anagrammes.size());

		a = verif_anagrammes(anagrammes[i], longueur_anagramme);
		if (a == false) {
			continue;
		}

		enlever_blanks(anagrammes[i], longueur_anagramme);

		pour_alpha[i].alpha = anagrammes[i];

		int zero = 0;
		int max = pour_alpha[i].alpha.length() - 1;
		attribution_alpha(pour_alpha, zero, max, i);
		string alpha_m = pour_alpha[i].alpha;

		anagrammes_rech.push_back("");
		trouve = rech_anagramme(Dico, alpha_m, anagrammes_rech[i]);

		if (!trouve) {
			cout << NO_ANAGRAM << "\n";
		}
	}
}

void attribution_nbT(vector<Word>& Dico)
{
	for (unsigned int i = 0; i < Dico.size(); ++i) {
		Dico[i].nbT = Dico[i].mot.length();
	}
}

void attribution_nbD(vector<Word>& Dico)
{
	for (unsigned int i = 0; i < Dico.size(); ++i) {
		unsigned int compte = 0;

		for (unsigned int j = 0; j < Dico[i].mot.length(); ++j) {
			bool deja = false;
			for (unsigned int k = 0; k < j; ++k) {
				if (Dico[i].mot[k] == Dico[i].mot[j]) {
					deja = true;
					break;
				}
			}
			if (not deja) {
				++compte;
			}
		}
		Dico[i].nbD = compte;
	}
}

void attribution_alpha(vector<Word>& Dico, int& min, int& max, int& i)
{
	int inf = min, sup = max;
	int pivot = Dico[i].alpha[(min + max) / 2];
	do {
		while ((Dico[i].alpha[inf] < pivot) and (inf < max)) {
			inf++;
		}
		while ((pivot < Dico[i].alpha[sup]) and (sup > min)) {
			sup--;
		}
		if (inf <= sup) {
			swap(Dico[i].alpha[inf], Dico[i].alpha[sup]);
			inf++;
			sup--;
		}
	} while (inf <= sup);

	if (min < sup) {
		attribution_alpha(Dico, min, sup, i);
	}
	if (inf < max) {
		attribution_alpha(Dico, inf, max, i);
	}
}

unsigned int recherche_dichotomique(vector<Word>& Dico, unsigned int N, unsigned int& key)
{
	unsigned int inf = 0;
	unsigned int pos = N;
	while (inf < N) {
		int pivot = (inf + N) / 2;

		if (Dico[pivot].nbT < Dico[pos].nbT) {
			inf = pivot + 1;
		}
		else if (Dico[pivot].nbT > Dico[pos].nbT) {
			N = pivot;
		}
		else if (Dico[pivot].nbD < Dico[pos].nbD) {
			inf = pivot + 1;
		}
		else if (Dico[pivot].nbD > Dico[pos].nbD) {
			N = pivot;
		}
		else if (Dico[pivot].alpha < Dico[pos].alpha) {
			inf = pivot + 1;
		}
		else if (Dico[pivot].alpha > Dico[pos].alpha) {
			N = pivot;
		}
		else if (Dico[pivot].mot < Dico[pos].mot) {
			inf = pivot + 1;
		}
		else if (Dico[pivot].mot > Dico[pos].mot) {
			N = pivot;
		}
	}
	return inf;
}

void tri_insertion_dichotomique(vector<Word>& Dico)
{
	for (unsigned int i = 1; i < Dico.size(); ++i) {
		unsigned int pptSup = recherche_dichotomique(Dico, i, Dico[i].nbT);
		unsigned int j = i;

		while (j > pptSup) {
			swap(Dico[j], Dico[j - 1]);
			j = j - 1;
		}
	}
}

//DICO TERMINÉ

bool verif_anagrammes(string& anagramme, size_t& longueur_anagramme)
{
	size_t k = anagramme.find_first_not_of(" \n\t\r");
	bool invalide = false;
	while (k < longueur_anagramme) {
		size_t j = anagramme.find_first_of(" \n\t\r", k);
		if (j == SIZE_MAX) {
			cout << NOT_IN_CAPITAL_LETTERS << "\n";
			invalide = true;
			break;
		}

		while(k < j) {
			if ((anagramme[k] < 65) or (anagramme[k] > 90)) {
				if (anagramme[k] != ' ') {
					if (anagramme[k] != '\n') {
						if (anagramme[k] != '\t') {
							if (anagramme[k] != '\r') {
								cout << NOT_IN_CAPITAL_LETTERS << "\n";
								invalide = true;
								k = anagramme.find_first_of(" \n\t\r", k);
							}
						}
					}
				}
			}
			++k;
		}
		k = anagramme.find_first_not_of(" \n\t\r", j + 1);
	}
	if (invalide == true) {
		return false;
	}
	return true;
}

void enlever_blanks(string& anagramme, size_t& longueur_anagramme)
{
	for (int j = longueur_anagramme - 1; j >= 0; --j) {
		if ((anagramme[j] == ' ') or (anagramme[j] == '\n') or (anagramme[j] == '\t') or (anagramme[j] == '\r')) {
			anagramme.erase(j, 1);
		}
	}
}

//Si alpha inclut dans alpha_m
bool message_contient_mot(string alpha, string alpha_m)
{
	unsigned int compte = 0;
	size_t longueur_alpha = alpha.length();
	size_t i = 0, j = 0;
	while (j < alpha_m.length() && i < longueur_alpha) {
		if (alpha[i] == alpha_m[j]) {
			++compte;
			++j;
			++i;
		}
		else {
			++j;
		}
		if (compte == longueur_alpha) {
			return true;
		}
	}
	return false;
}

//Enlever alpha de alpha_m
string message_soustraire_mot(string alpha_m, string alpha)
{
	for (size_t i = 0; i < alpha.length(); ++i) {
		for (size_t j = 0; j < alpha_m.length(); ++j) {
			if (alpha[i] == alpha_m[j]) {
				alpha_m.erase(j, 1);
				break;
			}
		}
	}
	return alpha_m;
}

vector<Word> enlever_mot(vector<Word>& Dico, size_t i)
{
	vector<Word> dico_next;
	for (size_t j = 0; j < Dico.size(); ++j) {
		if (j == i) {
			continue;
		}
		dico_next.push_back(Dico[j]);
	}
	return dico_next;
}

bool rech_anagramme(vector<Word> Dico, string alpha_m, string anagramme)
{
	size_t taille_dico = Dico.size();
	if (taille_dico == 0) {
		return false;
	}
	bool succes = false;
	for (size_t u = 0; u < taille_dico; ++u)
	{
		if (message_contient_mot(Dico[u].alpha, alpha_m)) {
			string anag_next;
			anag_next = anagramme + " " + Dico[u].mot;
			string alpha_m_next = message_soustraire_mot(alpha_m, Dico[u].alpha);
			if (alpha_m_next.length() == 0) {
				cout << anag_next.erase(0, 1) << "\n";
				succes = true;
			}
			else {
				vector<Word> dico_next = enlever_mot(Dico, u);
				if (rech_anagramme(dico_next, alpha_m_next, anag_next)) {
					succes = true;
				}
			}
		}
	}
	return succes;
}
