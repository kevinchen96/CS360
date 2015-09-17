#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#include <set>
#include <map>
#include <vector>
#include <queue>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <windows.h>

using namespace std;

string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    string::size_type pos = string( buffer ).find_last_of( "\\/" );
    return string( buffer ).substr( 0, pos);
}

int main()
  {
  ifstream fin;
  string dir, filepath;
  int num;
  DIR *dp;
  struct dirent *dirp;
  struct stat filestat;
  string trainhamDir = ExePath() + "\\data\\train\\ham";
  string trainspamDir = ExePath() + "\\data\\train\\spam";
  string testhamDir = ExePath() + "\\data\\test\\ham";
  string testspamDir = ExePath() + "\\data\\test\\spam";
  string dictDir = ExePath() + "\\data";
//340 123 348 130


  set<string> dictionary;
  vector<map<string, int> > totalOccurences;
  map<pair<string, int>, double> SpamOccurences;
  map<pair<string, int>, double> HamOccurences;
  vector<bool> checker;
  vector<bool> actual;
  priority_queue<double> expectedH;
  map<double, string> common_wordsH;
  priority_queue<double> expectedS;
  map<double, string> common_wordsS;
  map<string, double> testS;
  map<string, double> testH;
  int spamAverage;
  //reading in the dictionary words into a set
  dp = opendir( dictDir.c_str() );
  if (dp == NULL)
    {
    cout << "Error(" << errno << ") opening " << dir << endl;
    return errno;
    }


  int count = 0;
  int countSpam = 0;
  int countHam = 0;
  int countTrain = 0;
  double SpamP, HamP;
  string temp;
  dirp = readdir( dp );
  filepath = dictDir + "\\dict.txt";
  fin.open( filepath.c_str() );
  fin >> temp;
  while(!fin.fail()) {
    dictionary.insert(temp);
    fin >> temp;
  }
  fin.close();
  closedir( dp );
  
  //reading in train ham email occurences
  dp = opendir( trainhamDir.c_str() );
  if (dp == NULL)
    {
    cout << "Error(" << errno << ") opening " << dir << endl;
    return errno;
    }

  //else cout << trainhamDir << endl;
    
  while ((dirp = readdir( dp ))){
    filepath = trainhamDir + "\\" + dirp->d_name;
    map <string, int> occurences;
    fin.open( filepath.c_str() );
    fin >> temp;
    if(!fin.fail()){
      while(!fin.fail()) {
        if(dictionary.count(temp) > 0){
          if(occurences.count(temp) == 0){
            occurences[temp] = 1;
          }
          else{
            occurences[temp]++;
          }
          //if(count == 0){
           // cout << temp << " " << occurences[temp] << endl;;
          //}
        }
        fin >> temp;
      }
      map<string, int> temporary;
      for(set<string>::iterator it=dictionary.begin(); it!=dictionary.end(); ++it){
        if(occurences.count(*it) == 0){
          temporary[*it] = 0;
        }
        else if(occurences[*it] > 3){
          temporary[*it] = 3;
        }
        else{
          temporary[*it] = occurences[*it];
        }
      }
      totalOccurences.push_back(temporary);
      checker.push_back(false);
      count++;
      countHam++;
      countTrain++;
      fin.close();
    }
  }
  //cout << count << endl;
  closedir( dp );

  //reading in train spam email occurences
  dp = opendir( trainspamDir.c_str() );
  if (dp == NULL)
    {
    cout << "Error(" << errno << ") opening " << dir << endl;
    return errno;
    }
    //count = 0;
  //else cout << trainspamDir << endl;

  while ((dirp = readdir( dp ))){
    filepath = trainspamDir + "\\" + dirp->d_name;
    map <string, int> occurences;
    fin.open( filepath.c_str() );
    fin >> temp;
    if(!fin.fail()){
      while(!fin.fail()) {
        if(dictionary.count(temp) > 0){
          if(occurences.count(temp) == 0){
            occurences[temp] = 1;
          }
          else{
            occurences[temp]++;
          }
        }
        fin >> temp;
      }
      map<string, int> temporary;
      for(set<string>::iterator it=dictionary.begin(); it!=dictionary.end(); ++it){
        if(occurences.count(*it) == 0){
          temporary[*it] = 0;
        }
        else if(occurences[*it] > 3){
          temporary[*it] = 3;
        }
        else{
          temporary[*it] = occurences[*it];
        }
      }
      totalOccurences.push_back(temporary);
      checker.push_back(true);
      count++;
      countSpam++;
      countTrain++;
      fin.close();
    }
  }

  closedir( dp );
  //cout << count << endl;
  //calculating total probabilities
  HamP = countHam*1.0/countTrain;
  SpamP = countSpam*1.0/countTrain;
  /*cout << HamP << endl;
  cout << SpamP << endl;
  cout << HamP + SpamP << endl;*/


  //calculating conditional probabilities of word occurences for ham
  for(set<string>::iterator it=dictionary.begin(); it!=dictionary.end(); ++it){
    for(int i = 0; i < 4; i++){
      for(int j = 0; j < countHam; j++){
        if(totalOccurences[j][*it] == i){
          if(HamOccurences.count(pair<string, int>(*it, i)) == 0){
            HamOccurences[pair<string, int>(*it, i)] = 1.0/countHam;
          }
          else{
            HamOccurences[pair<string, int>(*it, i)] += 1.0/countHam;
          }
        }
      }
      if(HamOccurences.count(pair<string,int>(*it, i)) == 0){
        HamOccurences[pair<string, int>(*it, i)] = 0.0;
      }
    }
  }
  
for(set<string>::iterator it=dictionary.begin(); it!=dictionary.end(); ++it){
    
    double tempo0 = HamOccurences[pair<string, int>(*it, 0)];
    double tempo1 = HamOccurences[pair<string, int>(*it, 1)];
    double tempo2 = HamOccurences[pair<string, int>(*it, 2)];
    double tempo3 = HamOccurences[pair<string, int>(*it, 3)];
    //double expected = tempo0*0 + tempo1*1 + tempo2*2 + tempo3*3;
    //cout << tempo0 << " " << tempo1 << " " << tempo2 << " " << tempo3 <<" " << endl;
   // cout << tempo0 + tempo1 + tempo2 + tempo3 << endl;
    double expected = tempo1 + tempo2 + tempo3;
    expectedH.push(expected);
    common_wordsH[expected] = *it;
    testH[*it] = expected;

}

/*double totalsumHamP = 0.0;

for(int j = 0; j < countHam; j++){
    
    for(set<string>::iterator it=dictionary.begin(); it!=dictionary.end(); ++it){
      int countOfWordInEmail = totalOccurences[j][*it];
      double change1, change2;
      double prob1 = HamOccurences[pair<string, int>(*it, countOfWordInEmail)];
      //double prob1 = testS[*it];
      if(prob1 == 0){
        change1 = -10000;
      }
      else{
        change1 = log(prob1);
      }      
      totalsumHamP += change1;
    }
  }
totalsumHamP = totalsumHamP/countHam;*/
 //calculating conditional probabilities of word occurences for spam
  for(set<string>::iterator it=dictionary.begin(); it!=dictionary.end(); ++it){
    for(int i = 0; i < 4; i++){
      for(int j = countHam; j < countTrain; j++){
        if(totalOccurences[j][*it] == i){
          if(SpamOccurences.count(pair<string, int>(*it, i)) == 0){
            SpamOccurences[pair<string, int>(*it, i)] = 1.0/countSpam;
          }
          else{
            SpamOccurences[pair<string, int>(*it, i)] += 1.0/countSpam;
          }
        }
      }
      if(SpamOccurences.count(pair<string,int>(*it, i)) == 0){
        SpamOccurences[pair<string, int>(*it, i)] = 0.0;
      }
    }
  }
for(set<string>::iterator it=dictionary.begin(); it!=dictionary.end(); ++it){
    double tempo0 = SpamOccurences[pair<string, int>(*it, 0)];
    double tempo1 = SpamOccurences[pair<string, int>(*it, 1)];
    double tempo2 = SpamOccurences[pair<string, int>(*it, 2)];
    double tempo3 = SpamOccurences[pair<string, int>(*it, 3)];
   //double expected = tempo0*0 + tempo1*1 + tempo2*2 + tempo3*3;

    //cout << tempo0 << " " << tempo1 << " " << tempo2 << " " << tempo3 <<" " << endl;
   double expected = tempo1 + tempo2 + tempo3;
    expectedS.push(expected);
    common_wordsS[expected] = *it;
    testS[*it] = expected;
}
/*double totalsumSpamP = 0.0;

for(int j = countHam; j < countTrain; j++){
    
    for(set<string>::iterator it=dictionary.begin(); it!=dictionary.end(); ++it){
      int countOfWordInEmail = totalOccurences[j][*it];
      double change1, change2;
      double prob1 = SpamOccurences[pair<string, int>(*it, countOfWordInEmail)];
      //double prob1 = testS[*it];
      if(prob1 == 0){
        change1 = -10000;
      }
      else{
        change1 = log(prob1);
      }      
      totalsumSpamP += change1;
    }
  }
totalsumSpamP = totalsumSpamP/countSpam;
//cout << totalsumSpamP << endl;*/

  //reading in test ham email occurences
 dp = opendir( testhamDir.c_str() );
  if (dp == NULL)
    {
    cout << "Error(" << errno << ") opening " << dir << endl;
    return errno;
    }

  //else cout << testhamDir << endl;
   
  while ((dirp = readdir( dp ))){
    filepath = testhamDir + "\\" + dirp->d_name;
    map <string, int> occurences;
    fin.open( filepath.c_str() );
    fin >> temp;
    if(!fin.fail()){
      while(!fin.fail()) {
        if(dictionary.count(temp) > 0){
          if(occurences.count(temp) == 0){
            occurences[temp] = 1;
          }
          else{
            occurences[temp]++;
          }
        }
        fin >> temp;
      }
      map<string, int> temporary;
      for(set<string>::iterator it=dictionary.begin(); it!=dictionary.end(); ++it){
        if(occurences.count(*it) == 0){
          temporary[*it] = 0;
        }
        else if(occurences[*it] > 3){
          temporary[*it] = 3;
        }
        else{
          temporary[*it] = occurences[*it];
        }
      }
      totalOccurences.push_back(temporary);
      checker.push_back(false);
      count++;
      fin.close();
    }
  }

  closedir( dp );

  //reading in test spam email occurences
  dp = opendir( testspamDir.c_str() );
  if (dp == NULL)
    {
    cout << "Error(" << errno << ") opening " << dir << endl;
    return errno;
    }
  //count = 0;
  //else cout << testspamDir << endl;

  while ((dirp = readdir( dp ))){
    filepath = testspamDir + "\\" + dirp->d_name;
    map <string, int> occurences;
    fin.open( filepath.c_str() );
    fin >> temp;
    if(!fin.fail()){
      while(!fin.fail()) {
        if(dictionary.count(temp) > 0){
          if(occurences.count(temp) == 0){
            occurences[temp] = 1;
          }
          else{
            occurences[temp]++;
          }
        }
        fin >> temp;
      }
      map<string, int> temporary;
      for(set<string>::iterator it=dictionary.begin(); it!=dictionary.end(); ++it){
        if(occurences.count(*it) == 0){
          temporary[*it] = 0;
        }
        else if(occurences[*it] > 3){
          temporary[*it] = 3;
        }
        else{
          temporary[*it] = occurences[*it];
        }
      }
      totalOccurences.push_back(temporary);
      checker.push_back(true);
      count++;
      fin.close();
    }
  }
 // cout << count << endl;
  closedir( dp );

  //checking for if spam or ham;
  for(int i = 0; i < totalOccurences.size(); i++){
    double sumSpamP = 0.0;
    double sumHamP = 0.0;

    for(set<string>::iterator it=dictionary.begin(); it!=dictionary.end(); ++it){
      int countOfWordInEmail = totalOccurences[i][*it];
      double change1, change2;
      double prob1 = SpamOccurences[pair<string, int>(*it, countOfWordInEmail)];
      //double prob1 = testS[*it];
      if(prob1 == 0){
        change1 = -10000;
      }
      else{
        change1 = log(prob1);
      }      
      sumSpamP += change1;

      double prob2 = HamOccurences[pair<string, int>(*it, countOfWordInEmail)];
      //double prob2 = testH[*it];
      if(prob2 == 0){
        change2 = -10000;
      }
      else{
        change2 = log(prob2);
      }      
      sumHamP += change2;
    }
    //cout << sumSpamP << endl;
    //cout << sumHamP << endl;
    sumSpamP += log(SpamP);
    sumHamP += log(HamP);
    if(sumSpamP >= sumHamP){
      actual.push_back(true);
      //cout <<"is spam" << endl;
    }
      /*if(abs(sumSpamP - totalsumSpamP)<20000 && abs(sumHamP - totalsumHamP) > 20000){
        actual.push_back(true);
      }*/
    else{
      actual.push_back(false);
      //cout <<"isn't spam" << endl;
    }
  }
  int predictCorrect = 0;
  int predicted = 0;
  int Correct = 0;
  for(int i = 0; i < countTrain; i++){
    if(checker[i] == true && actual[i] == checker[i]){
      predictCorrect++;
    }
    if(actual[i] == true){
      predicted++;
    }
    if(checker[i] == true){
      Correct++;
    }
  }
  //cout << predictCorrect << endl;
  double precisionTrain = predictCorrect * 1.0 / predicted;
  double recallTrain = predictCorrect * 1.0 / countSpam;
  //cout << Correct << endl;
  
  cout << "Train Precision: " << precisionTrain << endl;
  cout << "Train Recall: " << recallTrain << endl;

  int predictCorrectTest = 0;
  int predictedTest = 0;
  int CorrectTest = 0;
  for(int i = countTrain; i < actual.size(); i++){
    if(checker[i] && actual[i] == checker[i]){
      predictCorrectTest++;
    }
    if(actual[i]){
      predictedTest++;
    }
    if(checker[i]){
      CorrectTest++;
    }
  }
  double precisionTest = predictCorrectTest * 1.0 / predictedTest;
  double recallTest = predictCorrectTest * 1.0 / CorrectTest;
 
  cout << "Test Precision: " << precisionTest << endl;
  cout << "Test Recall: " << recallTest << endl;
  for(int i = 0; i < 3; i++){
    cout << "Ham Common word " << i + 1 << ": "<< common_wordsH[expectedH.top()] << endl;
    expectedH.pop();
  }
  for(int i = 0; i < 3; i++){
    cout << "Spam Common word " << i + 1<< ": "<<  common_wordsS[expectedS.top()] << endl;
    expectedS.pop();
  }
  return 0;
  }