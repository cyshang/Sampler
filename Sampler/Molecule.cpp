#include "Molecule.h"
#include <sstream>

using namespace Eigen;
using std::vector;
using std::string;
using std::istream;
using std::ostream;
using std::istringstream;
using std::ostringstream;
using std::endl;

// ---------- define static member variable ----------
string Molecule::name;

int Molecule::elem_num;
vector<string> Molecule::elem_list;
Molecule::str2int Molecule::elem2num;
Molecule::int2str Molecule::num2elem;

int Molecule::tot_atom_num;
vector<int> Molecule::elem_atom_num;
vector<int> Molecule::atom_list;

Molecule::Molecule():
	energy(0.0)
{

}

mError_t Molecule::InputInfo(istream & fin)
{
	string line;
	while (std::getline(fin, line)) {
		if (line[0] == '#' || line[0] == '\n' || line[0] == ' ')
			continue;
		
		for (size_t i = 0; i < line.size(); ++i) {
			if (line[i] == '=' || line[i] == '(' || line[i] == ')' || line[i]=='\'' || line[i]=='"')
				line[i] = ' ';
		}
		
		istringstream sin(line);
		string key_word;

		sin >> key_word;
		if (key_word == "molecule") {
			sin >> name;
		}
		else if (key_word == "elem_num") {
			sin >> elem_num;
		}
		else if (key_word == "elem_list") {
			elem_list.resize(elem_num);

			for (size_t i = 0; i < elem_list.size(); ++i) {
				sin >> elem_list[i];
				elem2num[elem_list[i]] = i;
				num2elem[i] = elem_list[i];
			}
		}
		else if (key_word == "atom_num") {
			sin >> tot_atom_num;
		}
		else if (key_word == "atom_list") {
			atom_list.resize(tot_atom_num);

			elem_atom_num.resize(elem_num);
			for (size_t i = 0; i < elem_atom_num.size(); ++i) {
				elem_atom_num[i] = 0;
			}

			string elem;
			for (size_t i = 0; i < atom_list.size(); ++i) {
				sin >> elem;
				atom_list[i] = elem2num[elem];
				elem_atom_num[atom_list[i]]++;
			}
		}
	}

	return mError_t::Success;
}

mError_t Molecule::InputX(std::istream & fin)
{
	int tmp;
	if (!(fin >> tmp)) return mError_t::InputEOF;

	X.resize(3, tot_atom_num);
	for (size_t i = 0; i < tot_atom_num; ++i) {
		string elem;
		fin >> elem;

		for (int j = 0; j < 3; ++j) {
			fin >> X(j, i);
		}
	}

	return mError_t::Success;
}

std::istream & operator >> (std::istream & fin, Molecule & m)
{
	using std::getline;
	
	string line;
	if (getline(fin, line)) {
		m.atom_str.resize(m.tot_atom_num);
		m.X.resize(3, m.tot_atom_num);

		getline(fin, m.energy_str);
		for (int i = 0; i < m.tot_atom_num; ++i) {
			getline(fin, m.atom_str[i]);

			istringstream sin(m.atom_str[i]);
			string elem;
			sin >> elem;
			for (int j = 0; j < 3; ++j) {
				sin >> m.X(j, i);
			}
		}
	}

	return fin;
}

mError_t Molecule::CalcVectorR()
{
	vectorR.resize(tot_atom_num * (tot_atom_num - 1) / 2);

	int pos = 0;
	for (int i = 0; i < tot_atom_num - 1; ++i) {
		for (int j = i + 1; j < tot_atom_num; ++j) {
			vectorR(pos++) = (X.col(i) - X.col(j)).norm();
		}
	}

	return mError_t::Success;
}

ostream & operator << (ostream & fout, const Molecule & m)
{
	fout << m.tot_atom_num << endl;
	fout << m.energy_str << endl;
	for (size_t i = 0; i < m.atom_str.size(); ++i) {
		fout << m.atom_str[i] << endl;
	}

	return fout;
}