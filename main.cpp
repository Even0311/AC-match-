#include <iostream>
#include <dirent.h>
#include<fstream>
#include<set>
#include<map>
#include <vector>
#include<algorithm>
#include<stdio.h>

using namespace std;
#define NUMBER_OF_PATTERN 300
#define CHARSIZE 128

int gototable[NUMBER_OF_PATTERN][CHARSIZE];
int failtable[NUMBER_OF_PATTERN];
map<string,double > file_frequency;
vector<string> pattern_to_match;
set<string> outputtable[NUMBER_OF_PATTERN];
int length;
string path;

void dopen(string path);
void buildgototable(vector<string> pattern);
void buildfailtable(vector<string> pattern);
bool sortfunction(pair<string,double>a,pair<string,double>b);
void acmatch(string filename,vector<string> pattern);
int main(int argc, char* argv[]) {
    path = argv[1];
    string indicater = argv[3];
    if(indicater == "-s"){
        for(int i=5;i<argc;i++){
            pattern_to_match.push_back(argv[i]);
        }
    }
    else {
        for(int i=3;i<argc;i++){
            pattern_to_match.push_back(argv[i]);
        }
    }
    length = pattern_to_match.size();
    dopen(path);
    // here is to change all upper in patt to low
    for(int i=0;i<length;i++){
        int k = pattern_to_match[i].length();
        for(int q = 0;q<k;q++){
            if(('A' <= pattern_to_match[i][q]) &&(pattern_to_match[i][q] <= 'Z')){
                pattern_to_match[i][q] += 32;
            }
        }
    }
    buildgototable(pattern_to_match);
    // This is to make gototable of upper the same with low
    for(int i=0;i<NUMBER_OF_PATTERN;i++){
        for(int k=65;k<91;k++){
            gototable[i][k] = gototable[i][k + 32];
        }
    }
    buildfailtable(pattern_to_match);
    map<string,double>::const_iterator mit;
    for(mit = file_frequency.begin();mit!=file_frequency.end();++mit){
        acmatch(mit->first,pattern_to_match);
    }
    // here is to sort the map
    vector<pair<string,double> > sorting_vector;
    map<string,double>::iterator it;
    for(it = file_frequency.begin();it !=file_frequency.end();++it)
    {
        sorting_vector.push_back(*it);
    }
    sort(sorting_vector.begin(),sorting_vector.end(),sortfunction);
    vector<pair<string,double> >::iterator myit;
    for(myit = sorting_vector.begin();myit != sorting_vector.end();++myit){
        //cout<<myit->first<<"here does noe work"<<endl;
        if(myit->second >0){
            cout<<myit->first<<endl;
        }
    }
    return 0;
}
void dopen(string path) {
    DIR*    dir;
    struct dirent* pdir;
    dir = opendir(path.c_str());
    while ((pdir = readdir(dir)) != NULL) {
        if(pdir->d_name[0] =='.'){
            continue;
        }
        else{
            file_frequency.insert(map<string,double>::value_type(pdir->d_name,0.0));

        }
    }
    closedir(dir);
}
void buildgototable(vector<string> pattern){
    int smax;
    int t;
    vector<string>::const_iterator vec_it;
    string::const_iterator str_it;
    for(vec_it = pattern.begin(), smax = 0; vec_it != pattern.end(); ++vec_it)
    {
        for(str_it = vec_it->begin(), t = 0; str_it != vec_it->end(); ++str_it)
        {
            if(gototable[t][(*str_it)] == 0)
            {
                gototable[t][(*str_it)] = ++smax;
                t = smax;
            }
            else
            {
                t = gototable[t][(*str_it)];
            }
        }
        outputtable[t].insert(*vec_it);
    }
}

void buildfailtable(vector<string> pattern)
{
    unsigned int t, m, last_state;
    unsigned int s[50];

    vector<string>::const_iterator vec_it;
    string::const_iterator str_it1, str_it2, str_it3;

    for(vec_it = pattern.begin(); vec_it != pattern.end(); ++vec_it)
    {
        t = 0;
        m = 0;
        str_it1 = vec_it->begin();

        while(str_it1 != vec_it->end() && gototable[t][*str_it1 ] != 0)
        {
            t = gototable[t][*str_it1];
            ++str_it1;
            s[m++] = t;
        }

        for(str_it1 = vec_it->begin() + 1; str_it1 != vec_it->end(); ++str_it1)
        {

            for(str_it2 = vec_it->begin() + 1; str_it2 != str_it1 + 1; ++str_it2)
            {
                t = 0;
                str_it3 = str_it2;


                while(str_it3 != str_it1 + 1  && gototable[t][*str_it3] != 0)
                {
                    t = gototable[t][*str_it3];
                    ++str_it3;
                }


                if(str_it3 == str_it1 + 1)
                {

                    last_state = s[str_it3-vec_it->begin() - 1];


                    if(failtable[last_state] == 0)
                    {
                        failtable[last_state] = t;
                    }


                    if(outputtable[last_state].size() > 0 && outputtable[t].size() > 0)
                    {

                        for(set<string>::const_iterator cit = outputtable[t].begin(); cit != outputtable[t].end(); ++cit)
                        {
                            outputtable[last_state].insert(*cit);
                        }
                    }
                }
            }
        }
    }
}

void acmatch(string filename,vector<string> pattern) {
    unsigned int t = 0;
    ifstream ifst1;
    string toopen = path + '/' + filename;
    ifst1.open(toopen,ios::in);
    if(!ifst1){
    }
    else {
        string mystring;
        map<string,int> mymap;
        for(int i=0;i<length;i++){
            mymap.insert(map<string,int>::value_type(pattern[i],0));
        }
        while (getline(ifst1, mystring)) {
            //t = 0;
            mystring = mystring + '\n';
            string::const_iterator str_it = mystring.begin();
            while (str_it != mystring.end()) {
                if(t > 300){
                    t = 0;
                    ++str_it;

                }
                if (outputtable[t].size() > 0) {
                    for (set<string>::const_iterator cit = outputtable[t].begin(); cit != outputtable[t].end(); ++cit) {
                        mymap[*cit] += 1;
                    }
                }

                if (gototable[t][*str_it] == 0) {
                    t = failtable[t];
                    while (t != 0 && gototable[t][*str_it] == 0) {
                        t = failtable[t];
                    }
                    if (t == 0) {
                        if (gototable[0][*str_it] == 0) {
                            ++str_it;
                        }
                        continue;
                    }
                }
                t = gototable[t][*str_it];
                ++str_it;
            }
        }
        map<string,int>::const_iterator mit;
        int flag = 0;
        double totalnumber = 0;
        for(mit = mymap.begin();mit != mymap.end();++mit){
            if(mit->second >0){
                totalnumber += mit->second;
                flag += 1;
            }
        }
        if(flag == length){
            file_frequency[filename] = (totalnumber/length);
        }
    }
}
bool sortfunction(pair<string,double>a,pair<string,double>b)
{
    if(a.second == b.second){
        return a.first<b.first;
    }
    else{
        return a.second > b.second;
    }
}

