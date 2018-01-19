#include <iostream>

using namespace std;

/*typedef Catalogue*/

int main(int argc, char const *argv[])
{
	/*gérer les erreurs propres au main*/

	/*générer catalogue*/

	/*en fonction des arguments générerGraphe ou pas
	puis affichage de la réponse*/
	
	return 0;
}

void genererGraph(String dotFileName){

	ofstream dotStream;
	dotStream.open(dotFileName, ios::out);
	if(dotStream.is_open()){
		cout<<dotFileName<<" existe déjà, voulez vous le remplaçer ? (o/n)"<<endl;
		String answer;
		cin>>answer;

		}
	
}	