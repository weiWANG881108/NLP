#include <iostream>
#include <vector>
#include <string>
#include <clocale>
#include <cassert>
#include <fstream>
#include <map>

using namespace std;

wstring s2ws(const string & str){
	if(str.empty()){
		return L"";
	}
	unsigned len = str.size() + 1;
	setlocale(LC_ALL, "");
	wchar_t *p = new wchar_t[len];
	mbstowcs(p, str.c_str(), len);
	wstring w_str(p);
	delete[] p;
	return w_str;
}

string ws2s(const std::wstring& w_str) {
    if (w_str.empty()) {
      return "";
    }

    unsigned len = w_str.size() * 4 + 1;
    setlocale(LC_CTYPE, "");
    char *p = new char[len];
    wcstombs(p, w_str.c_str(), len);
    std::string str(p);
    delete[] p;
    return str;
}


class DICT{
public:
	int maxWordLen;
	DICT(const string & filePath){
		maxWordLen = 0;
		const char * chr = filePath.c_str();
		ifstream inFile(chr, ios::in);
		string buffer;

		while(getline(inFile, buffer)){
			if(buffer.size() == 0){
				continue;
			}
			wstring wbuffer = s2ws(buffer);
			if(wbuffer.size() > maxWordLen) maxWordLen = wbuffer.size();
			dictionary[wbuffer] ++;
		}
	}
	bool checkExist(wstring& w_str){
		return dictionary.count(w_str);
	}
	void print2000(){
		map<wstring, int>::iterator iter = dictionary.begin();
		int cnt=0;
		for(; iter!=dictionary.end(); iter++){
			wstring wbuffer = iter->first;
			string result = ws2s(wbuffer);
			cout << result << endl;
			cnt ++;
			if(cnt==2000){
				return;
			}
		}
	}
private:
	
	map<wstring, int> dictionary;
};


vector<wstring> forward_MM(const wstring & str, DICT * dict){
	int p1 = 0;
	vector<wstring> result;
	int n = str.size();
	while(p1<n){
		int nu = n - p1;
		int m = dict->maxWordLen;
		if(m>nu) m=nu;
		while(m>=1){
			wstring subWstr = str.substr(p1,m);
			if(m==1) {
				result.push_back(subWstr);
				p1 = p1+m;
				break;
			}
			if( dict->checkExist(subWstr) ){
				result.push_back(subWstr);
				p1 = p1+m;
				break;
			} else {
				m = m -1;
			}
		}
	}
	return result;
}

int main(int argc, char * argv[]){

	// step1 dictionary
	string filePath = "pku_training_words.utf8";
	DICT * dict = new DICT(filePath);
	//dict->print2000();

	// step2 
	string testFile = "pku_test.utf8";
	const char * chr = testFile.c_str();
	ifstream inFile(chr, ios::in);

/*
	vector<wstring> lines;
	string buffer;
	while(getline(inFile, buffer) ){
		if(buffer.size() == 0){
			continue;
		}
		wstring wbuffer = s2ws(buffer);		
		lines.push_back(wbuffer);
	}

	inFile.close();

	string fileOutput = "segmented.txt";
	const char * chr1 = fileOutput.c_str();
	ofstream outFule(chr1, ios::out);
	for(int i=0; i<lines.size(); i++){

	}
*/
	string test = "共同创造美好的新世纪——二○○一年新年贺词";
	wstring ss = s2ws(test);
	vector<wstring> result = forward_MM(ss, dict);
	for(int i=0; i<result.size(); i++){
		ss = result[i];
		string presult = ws2s(ss);
		cout << presult << endl;
	}


	return 0;
}
