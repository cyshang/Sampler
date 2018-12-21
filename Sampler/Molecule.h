#ifndef MOLECULE_H_
#define MOLECULE_H_

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <Eigen/Core>

#ifdef DEBUG
#define DEBUG_MOLECULE
#endif

enum class moleculeError
{
	Success,
	InputEOF,
	VarInitFailed,
	VarUninit
};

typedef enum moleculeError mError_t;

class Molecule
{
public:
	static mError_t InputInfo(std::istream &);
	inline mError_t InputEnergy(std::istream & fin) {
		return (fin >> energy) ? mError_t::Success : mError_t::VarInitFailed;
	}

	// input X
	mError_t InputX(std::istream &);
	// input energy_string and atom_string, then calculate X
	friend std::istream & operator >> (std::istream &, Molecule &);

	mError_t CalcVectorR();

	Molecule();
	inline double operator - (const Molecule & m) const { return (vectorR - m.vectorR).norm(); }	
	friend std::ostream & operator << (std::ostream &, const Molecule &);

private:
	// ---------- typedef ----------
	typedef std::map<std::string, int> str2int;
	typedef std::map<int, std::string> int2str;
	
	// ---------- molecule description ----------
	static std::string name;
	
	static int elem_num;
	static std::vector<std::string> elem_list;
	static str2int elem2num;
	static int2str num2elem;

	static int tot_atom_num;
	static std::vector<int> elem_atom_num;
	static std::vector<int> atom_list;

	// ---------- molecule string data ----------
	std::string energy_str;
	std::vector<std::string> atom_str;

	// ---------- molcule numerical data ----------
	double energy;
	Eigen::MatrixXd X;
	Eigen::VectorXd vectorR;

	Eigen::MatrixXd matrixR;
	Eigen::MatrixXd matrixR2;

};

#endif // !MOLECULE_H_