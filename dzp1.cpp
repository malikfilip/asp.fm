#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

struct Node
{
	string question;
	vector<Node*> answerVector;
	vector<int> ratingVector;
	int rating = 0,height=0;
	Node* father = nullptr;
	Node(string s) : question(s){}
	
};
string tab(int x) {
	string s = "";
	for (int i = 0; i < x; i++) { s += "  "; }
	return s;
}

void addQuestion(vector<Node*>& v,string s) {
	Node* newQuest = new Node(s);
	if (v.size() >= 10) { cout << "Nema mesta."; }
	else { v.push_back(newQuest); }
}

void addAnswer(Node* n,string s) {
	Node* newAnsw = new Node(s);
	newAnsw->father = n;
	newAnsw->height = n->height + 1;
	if (n->answerVector.size() >= 10) { cout << "Nema mesta."; }
	else { n->answerVector.push_back(newAnsw); }
}

Node* searchQuestion(vector<Node*>& v,string s) {
	for(Node* n:v){
		if (n->question == s) { return n; }
	}
	cout << "\033[1;31mPitanje nije pronadjeno.\n\033[0m\n" << endl;
	return nullptr;
}

Node* searchAnswer(Node* root,string answ) {
	queue <Node*> que;
	Node* next = root;

	que.push(next);

	while (!que.empty()) {
		next = que.front();
		que.pop();
		//cout << next->question << endl;
		if (next->question == answ) { 
			return next;
		}
		for (Node* n : next->answerVector) { que.push(n);}
	}
	cout << "\033[1;31mOdgovor nije pronadjen.\n\033[0m\n" << endl;
	return nullptr;
}

void rateAnswer(Node* ans) {
	int newRating;
	cout << "Unesite ocenu za odgovor["<<ans->question<<"]->" ;
	cin >> newRating;
	if (newRating < 0 || newRating>10) { cout << "Problem prilikom unosa ocene."; return; }
	ans->rating += newRating;
	cout << "\nOdgovor je ocenjen!";
}

void sortAnswers(Node* root){
	queue <Node*> que;//dodavanje u red.
	Node* next = root;
	que.push(next);
	while (!que.empty()) {
		next = que.front();
		que.pop();
		int size = next->answerVector.size();
		for (int i = 0; i < size - 1; i++) {//sortiranje
			for (int j = i + 1; j < size; j++) {
				if (next->answerVector[i]->rating < next->answerVector[j]->rating) {
					Node* tmp = next->answerVector[i];
					next->answerVector[i] = next->answerVector[j];
					next->answerVector[j] = tmp;
				}
			}
		}
		for (Node* n : next->answerVector) { que.push(n); }
	}
}

void printQuestions(vector<Node*>& v) {
	for (Node* n : v) { cout << n->question << '[' << n->rating << ']' << endl; }
}

void printAll(Node* root) {
	Node* next;
	stack<Node*> stc;
	stc.push(root);
	while (!stc.empty()) {
		next = stc.top();
		stc.pop();
		while(next !=nullptr){
			if (next == root) { cout << "\nPitanje: " << next->question << endl; }
			else if (next->rating == -1) { cout <<tab(next->height)<< "[OBRISANO]:"<<next->question<<endl; }
			else {
				cout <<tab(next->height)<< "Odgovor: " << next->question <<"["<<next->rating<<"*]"<< endl;
			}
			int size = next->answerVector.size();
			for (int i = size; i > 1; i--) { stc.push(next->answerVector[i-1]); }
			next = (size == 0 ? nullptr : next->answerVector[0]);
		}
	}
}

void deleteAnswer(Node* ans) {
	ans->question = "Odgovor vise ne postoji.";
	queue<Node*> q;
	Node* next = ans;
	for (Node* n : ans->answerVector) { q.push(n); }
	while (!q.empty()) {
		next = q.front();
		q.pop();
		for (Node* n : next->answerVector) { q.push(n); }
		delete next;

	}
	ans->answerVector.clear(); ans->ratingVector.clear(); ans->rating = -1;
}

void deleteQuestion(vector<Node*>& v,Node* qst) {
	deleteAnswer(qst);
	int i = 0;
	for (Node* n : v) {
		
		if (n == qst) {
			v.erase(v.begin() + i);
			return;
		}
		i++;
	}
}

void findBestAnswer(Node* root) {
	queue<Node*> q;
	Node* next = root,*curentMax = nullptr;
	int maxRating = 0;
	for (Node* n : next->answerVector) { q.push(n); }
	while (!q.empty()) {
		next = q.front();
		q.pop();
		if (next->rating > maxRating) { maxRating = next->rating; }
		for (Node* n : next->answerVector) { q.push(n); }
	}
	for (Node* n : root->answerVector) { q.push(n); }
	while (!q.empty()) {
		next = q.front();
		q.pop();
		if (next->rating == maxRating) { cout << (next->height == 1 ? "Odgovor: " : "Komentar: ") << next->question << '[' << next->rating << "*]" << endl; }
		for (Node* n : next->answerVector) { q.push(n); }
	}
}

int main(){
	cout << "\033[1;36mDobro dosli na platformu za pitanja i odgovore -> \033[0m" << " \033[4;31mASP.FM\033[0m\n";
	vector <Node*> questionVector;
	bool login = true;
	int optionID;
	
	while (login) {
		Node* current = nullptr, * question = nullptr;
		string s;
		char flag;
		cout << "\nMolim vas izaberite opciju.\n";
		cout << "[1] Dodajte pitanje na platformu.\n[2] Dodajte odgovor na pitanje ili komentar.\n[3] Proverite da li zadato pitanje postoji na nasoj platformi\n[4] Proverite da li se zadati odgovor/komentar postoji na platformi\n";
		cout << "[5] Zelim da ocenim neki odgovor na platformi.\n[6] Zelim da platforma sortira odgovore po relevantnosti.\n[7] Zelim da vidim koja se sve pitanja nalaze na platformi.\n[8] Zelim da vidim sve odgovore na zadato pitanje\n";
		cout << "[9] Zeleo bih da obrisem neko pitanje.\n[10] Zelim da obrisem neki komentar ili odgovor.\n[11] Zelim da vidim najbolje ocenjene odgovore na dato pitanje.\n[0] Zelim da se odjavim sa platforme.\n\nOPCIJA->";
		cin >> optionID;
		if (optionID < 0 || optionID>11) { cout << "\033[1;31mTakva opcija ne postoji,pokusajte ponovo.\n\033[0m\n"; continue; }
		
		switch (optionID) {	
		case 0:
			cout << "Zao nam je sto odlazite,vidimo se sledeci put :)";
			login = false;
			break;
		case 1:
			cout << "Dodajte pitanje:"<<endl;
			getline(cin>>ws,s);
			addQuestion(questionVector,s);
			break;
		case 2:
			cout << "Ukoliko zelite da dodate odgovor napisite 'O',ukoliko zelite komentar napisite 'K'-> ";
			cin >> flag;
			if (flag != 'O' && flag != 'K') { cout << "\033[1;31mGreska.\n\033[0m\n"; break; }
			switch (flag) {
			case 'O':
				cout << "\nNa koje pitanje zelite da odgovorite: "<<endl;
				getline(cin>>ws, s);
				question = searchQuestion(questionVector, s);
				if (question == nullptr) { break; }
				cout << "Dodajte odgovor: "<<endl;
				getline(cin >> ws, s);
				addAnswer(question,s);
				cout << "\nOdgovor dodat!";
				break;
			case 'K':
				cout << "\nPitanje na ciji odgovor/komentar zelite da odgovorite: " << endl;;
				getline(cin>>ws, s);
				question = searchQuestion(questionVector, s);
				if (question == nullptr) { break; }
				cout << "Na koji odgovor/komentar zelite da odgovorite:" << endl;;
				getline(cin>>ws, s);
				current = searchAnswer(question, s);
				if (current == nullptr) { break; }
				cout << "Dodajte komentar :"<<endl;
				getline(cin >> ws, s);
				addAnswer(current,s);
				cout << "\nKomentar dodat!";
				break;
			}
			break;
		case 3:
			cout << "Unesite pitanje koje trazite : "<<endl;
			getline(cin >> ws, s);
			question = searchQuestion(questionVector,s);
			if (question != nullptr) { cout << "\033[1;32mPitanje je pronadjeno.\n\033[0m\n"; }
			break;
		case 4:
			cout << "Unesite pitanje na koje trazite odgovor : " << endl;
			getline(cin >> ws, s);
			question = searchQuestion(questionVector, s);
			if (question == nullptr) { break; }
			cout << "Unesite odgovor za trazeno pitanje : " << endl;
			getline(cin >> ws, s);
			current = searchAnswer(question, s);
			if(current !=nullptr){ cout<<"\033[1;32mOdgovor je pronadjen.\n\033[0m\n"; }
			break;
		case 5:
			cout << "Unesite pitanje sa koji je vezan odgovor : " << endl;
			getline(cin >> ws, s);
			question = searchQuestion(questionVector, s);
			if (question == nullptr) { break; }
			cout << "Unesite odgovor koji zelite da ocenite : " << endl;
			getline(cin >> ws, s);
			current = searchAnswer(question, s);
			if (current != nullptr) { rateAnswer(current); }
			break;
		case 6:
			for (Node* q : questionVector) { sortAnswers(q); }
			cout << "Odgovori i komentari su soritrani!"<<endl;
			break;
		case 7:
			for (Node* q : questionVector) { printAll(q); }
			break;
		case 8:
			cout << "Unesite pitanje koje zelite da procitate : " << endl;
			getline(cin >> ws, s);
			question = searchQuestion(questionVector, s);
			if (question == nullptr) { break; }
			printAll(question);
			break;
		case 9:
			cout << "Unesite pitanje koje zelite da obrisete : " << endl;
			getline(cin >> ws, s);
			question = searchQuestion(questionVector, s);
			if (question == nullptr) { break; }
			deleteQuestion(questionVector,question);
			break;
		case 10:
			cout << "Unesite pitanje sa koji je vezan odgovor : " << endl;
			getline(cin >> ws, s);
			question = searchQuestion(questionVector, s);
			if (question == nullptr) { break; }
			cout << "Unesite odgovor koji zelite da obrisete: " << endl;
			getline(cin >> ws, s);
			current = searchAnswer(question, s);
			if (current != nullptr) { deleteAnswer(current); cout << "Odgovor je uspesno uklonjen"; }
			break;
		case 11:
			cout << "Unesite pitanje cije najbolje odgovore zelite da vidite : " << endl;
			getline(cin >> ws, s);
			question = searchQuestion(questionVector, s);
			if (question == nullptr) { break; }
			findBestAnswer(question);
			break;
		}
	}
	return 0;	
}