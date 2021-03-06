using namespace std;

#include <Parc.h>

Parc::Parc (fstream &inFile) {
	/*! Déclaration des variables temporaires */
	string t_type, t_immat, t_marque, t_modele, donnee;
	int t_kilom, t_nbPlaces;
	float t_volumeUtile, t_poidsUtile;
	
	/*! Lecture du fichier et création des  véhicules*/
	inFile >> t_type;
	while (!inFile.eof()) {	
		inFile >> t_immat;
		inFile >> t_marque;
		inFile >> t_modele;
		inFile >> t_kilom;
		if (t_type == "c") { // Données spécifiques aux camions
			inFile >> t_poidsUtile;
			inFile >> t_volumeUtile;
			m_parcAuto.push_back(Location(new Camion(t_poidsUtile,t_volumeUtile, t_immat, t_marque, t_modele), t_kilom));
		} else if (t_type == "u") { // Données spécifiques aux utilitaires
			inFile >> t_volumeUtile;
			m_parcAuto.push_back(Location(new Utilitaire(t_volumeUtile, t_immat, t_marque, t_modele), t_kilom));
		} else if (t_type == "v") { // Données spécifiques aux VP
			inFile >> t_nbPlaces;
			m_parcAuto.push_back(Location(new VP(t_immat, t_marque, t_modele, t_nbPlaces), t_kilom));
		}
		inFile >> t_type;
	}
}

Parc::Parc() {
}

Parc::~Parc () {
}

void Parc::ajouterLocation () {
	// Déclaration des variables temporaires
	string t_type;
	string t_marque, t_modele, t_immat;
	int t_kilom, t_nbPlaces = 0;
	float t_poidsUtile, t_volumeUtile;
	string t_input = "";
//----------------------------------------------------------------------
	entrerTypeVeh(t_type);
	entrerImmat(t_immat);
	entrerMarqueModele(t_marque, t_modele);
	entrerKilometrage(t_kilom);
		if (t_type == "v") {
			entrerNbPlaces(t_nbPlaces);
			m_parcAuto.push_back(Location(new VP(t_immat, t_marque, t_modele, t_nbPlaces), t_kilom));
		}
		if (t_type == "c" || t_type == "u") {
			entrerVolumeUtile(t_volumeUtile);
			if (t_type == "u") {
				m_parcAuto.push_back(Location(new Utilitaire(t_volumeUtile, t_immat, t_marque, t_modele), t_kilom));
			}
			else {
				entrerPoidsUtile(t_poidsUtile);
				m_parcAuto.push_back(Location(new Camion(t_poidsUtile,t_volumeUtile, t_immat, t_marque, t_modele), t_kilom));
			}
		}
	cout << "Véhicule ajouté !" << endl;
}

void Parc::modifierLocation (Location loc) {
}

bool Parc::estDansLeParc(string immat, Location &loc) {
	bool trouve = false;
	m_parcAutoI=m_parcAuto.begin();
	while(!trouve && m_parcAutoI !=m_parcAuto.end()) {
		if (m_parcAutoI->getVehicule()->getImmatriculation() == immat) {
			trouve = true;
			loc = *m_parcAutoI;
		}
		else m_parcAutoI++;
	}
	return trouve;
}

list<Location>::iterator Parc::rechercherLocation(string immat) {
	bool trouve = false;
	m_parcAutoI=m_parcAuto.begin();
	while(!trouve && m_parcAutoI !=m_parcAuto.end()) {
		if (m_parcAutoI->getVehicule()->getImmatriculation() == immat) {
			trouve = true;
		}
		else m_parcAutoI++;
	}
	return m_parcAutoI;
}

void Parc::enregistrerRetour(int kilom, string immat) {
	Location oldLoc;
	if (estDansLeParc(immat,oldLoc)) {
		rechercherLocation(immat)->setKilom(kilom);
		cout << "Retour enregistré" << endl;
	} else {
		cout << "Véhicule non trouvé" << endl;
	}
}

void Parc::afficher() {
	cout << left
		 << setw(10) << "Marque"
		 << setw(10) << "Modèle"
		 << setw(10) << "Immat"
		 << setw(10) << "Places"
		 << setw(10) << "P.U"
		 << setw(10) << "M3"
		 << setw(6) << "Kms"
		 <<endl;
	for (m_parcAutoI=m_parcAuto.begin();m_parcAutoI !=m_parcAuto.end(); m_parcAutoI++) {
		m_parcAutoI->afficher();
	}
}

void Parc::sauvegarder(string nomFichier) {
	fstream vehFile;
	vehFile.open(nomFichier.c_str(),ios::out);
	if (vehFile.fail()) {
		cerr << "Ouverture du fichier " << nomFichier << " impossible : fichier introuvable !" << endl;
		exit (-2);
	}
	for (m_parcAutoI=m_parcAuto.begin();m_parcAutoI !=m_parcAuto.end(); m_parcAutoI++) {
		m_parcAutoI->save(vehFile);
	}
	vehFile.close();
}

void Parc::entrerTypeVeh(string &t_type) {
	while (t_type != "c" && t_type != "v" && t_type != "u") {
		cout << "Type de véhicule (v/c/u) : ";
		cin >> t_type;
		Tools::stringToLower(t_type);
		if (t_type != "c" && t_type != "v" && t_type != "u")
			cout << "Type incorrect ! " << endl;
	}
}

void Parc::entrerImmat(string &t_immat) {
	bool existe = true;
	while (existe) {
		cout << "Immatriculation  : ";
		cin >> t_immat;
		Tools::stringToUpper(t_immat);
		Location tmp;
		existe = estDansLeParc(t_immat,tmp);
		if (existe)
			cout << "Véhicule existant !" << endl;
	}
}

void Parc::entrerMarqueModele(string &t_marque, string &t_modele) {
	cout << "Marque  : ";
	cin >> t_marque;
	cout << "Modele  : ";
	cin >> t_modele;
	Tools::stringToUpper(t_marque);
	Tools::stringToUpper(t_modele);	
}

void Parc::entrerKilometrage(int &t_kilom) {
	string t_input = "";
	while (!Tools::estEntier(t_input)){
		cout << "Kilométrage  : ";
		cin >> t_input;
		if (Tools::estEntier(t_input)){
			t_kilom = Tools::stringToInt(t_input);
		} else 
			cout << "Veuillez entrer un nombre !"<< endl;
	}	
}

void Parc::entrerVolumeUtile(float &t_volumeUtile){
	string t_input = "";
	while(!Tools::estReel(t_input)){
		cout << "Volume : ";
		cin >> t_input;
		if (Tools::estReel(t_input)){
			t_volumeUtile = Tools::stringToFloat(t_input);
		} else 
			cout << "Veuillez entrer un nombre !"<< endl;
		}
}

void Parc::entrerPoidsUtile(float &t_poidsUtile){
	string t_input = "";
	while(!Tools::estReel(t_input)){
		cout << "Poids  : ";
		cin >> t_input;
		if (Tools::estReel(t_input)){
			t_poidsUtile = Tools::stringToFloat(t_input);
		} else
			cout << "Veuillez entrer un nombre !"<< endl;
	}
}

void Parc::entrerNbPlaces(int &t_nbPlaces){
	string t_input = "";
	while (t_nbPlaces < 1) {
		t_input = "";
		while(!Tools::estEntier(t_input)){
			cout << "Nombre de places  : ";
			cin >> t_input;
				if (Tools::estEntier(t_input)){
					t_nbPlaces = Tools::stringToInt(t_input);
				} else
					cout << "Veuillez entrer un nombre !"<< endl;
			}			
		if (t_nbPlaces < 1 ) {
			cout << "Nombre de places insuffisant" << endl;
			t_nbPlaces = 0;
		}
	}
} 
