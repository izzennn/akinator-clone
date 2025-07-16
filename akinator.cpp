#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <climits>
using namespace std;


string trim(string s) {
    while (!s.empty() && (s.back() == '\r' || s.back() == '\n')) s.pop_back();
    return s;
}

void SetUp(unordered_map<string, unordered_map<string, double>>& character_traits, unordered_map<string, double>& character_probs, string filename) {
    ifstream file(filename);
    string line1, line2, line3;
    while (getline(file, line1)) {

        character_probs[trim(line1)];
        getline(file, line2); // traits
        getline(file, line3); // values

        stringstream traitStream(line2);
        stringstream valueStream(line3);
        string trait, value;

        while (getline(traitStream, trait, ',') && getline(valueStream, value, ',')) {
            trait = trim(trait);
            value = trim(value);
            double val = stod(value); // safe conversion
            character_traits[trim(line1)][trait] = val;
        }
    }
}

void InitliazeTraits(unordered_map<string, unordered_map<string, double>>& character_traits, vector<string>& traits){
    unordered_map<string, int> map;
    for (auto p = character_traits.begin(); p != character_traits.end(); p++){
        unordered_map<string, double> Traits = p->second;
        for (auto q = Traits.begin(); q != Traits.end(); q++){
            if (map.find(q->first) == map.end()){
                traits.push_back(q->first);
                map[q->first]++;
            }

        }
    }
}

void InitialProbabilities(unordered_map<string, unordered_map<string, double>>& character_traits, unordered_map<string, double>& character_probs){
    unordered_map<string, double>:: iterator p;
    double initialProb = 1.0/character_traits.size();
    for (p = character_probs.begin(); p != character_probs.end(); p++){
        p->second = initialProb;
    }
}

string nextQuestion(unordered_map<string, unordered_map<string, double>>& character_traits, unordered_map<string, double>& character_probs, vector<string>& traits){
    double total_characters = character_traits.size();
    double maxVar = 0.0;
    string question = "";
    for (int i = 0; i < traits.size(); i++){
        double p = 0.0;
        unordered_map<string, unordered_map<string, double>>::iterator q;
        for (q = character_traits.begin(); q != character_traits.end(); q++){
            if (q->second.count(traits[i]) && character_probs[q->first] > 0.01){
                p++;
            }
        }
        double p_frac = p/total_characters;
        double var = p_frac * (1 - p_frac);
        if (var > maxVar){
            maxVar = var;
            question = traits[i];
        }
    }
    return question;
}

void updateProbabilities(unordered_map<string, unordered_map<string, double>>& character_traits, unordered_map<string, double>& character_probs, string UserAnswer, string question) {
    double AnsVal = 0.0;
    if (UserAnswer == "yes" || UserAnswer == "Yes") {
        AnsVal = 1.0;
    }
    else if (UserAnswer == "Prob Yes" || UserAnswer  == "Prob No"){
        AnsVal = 0.3;
    }
    else if (UserAnswer == "No"){
        AnsVal = 0.0;
    }

    unordered_map<string, double> unnormalized_probs;
    double total = 0.0;

    unordered_map<string, unordered_map<string, double>>:: iterator q;
    for (q = character_traits.begin(); q != character_traits.end(); q++) {
        // Default to 0 if trait not found
        double traitValue = 0.0;
        if (q->second.find(trim(question)) != q->second.end()) {
            traitValue = q->second[trim(question)];
        }

        double likelihood = 1.0 - abs(AnsVal - traitValue);
        double unnormalized = likelihood * character_probs[q->first];

        unnormalized_probs[q->first] = unnormalized;
        total += unnormalized;
    }
    unordered_map<string, double>:: iterator p;
    for (p = character_probs.begin(); p != character_probs.end(); p++) {
        if (total > 0.0) {
            p->second = unnormalized_probs[p->first] / total;
        } else {
            p->second = 0.0;
        }
    }
}




int main() {
    unordered_map<string, unordered_map<string, double>> character_traits;
    unordered_map<string, int> prob_counts;
    unordered_map<string, double> character_probs;
    vector<string> traits;

    SetUp(character_traits, character_probs, "characters.txt");
    InitliazeTraits(character_traits, traits);
    InitialProbabilities(character_traits, character_probs);


    bool run = true;
    while (run){
        for (auto qt = character_probs.begin(); qt != character_probs.end(); ++qt) {
            cout << "Character: " << qt->first << ": " << qt->second;
            cout << endl;
        }

        for (auto p = character_probs.begin(); p != character_probs.end(); p++){
            if (p->second == 1.0){
                prob_counts[p->first] = p->second;
            }
        }

        for (auto q = character_probs.begin(); q != character_probs.end(); q++){
            if (q->second == 1.0 && prob_counts.size() == 1){
                cout << q->first << " is your character" << endl;
                run = false;
                break;
            }
        }


        if (!run || traits.empty()) {
            break;
        }

        string question = nextQuestion(character_traits, character_probs, traits);
        cout << question << " [Yes, No, Prob Yes, Prob No] " << endl;

        string UserAnswer;
        cin >> UserAnswer;

        while (UserAnswer != "Yes" && UserAnswer != "No" && UserAnswer != "Prob Yes" && UserAnswer != "Prob No"){
            cout << "invalid answer, enter again" << endl;
            cin >> UserAnswer;
        }
        
        traits.erase(remove(traits.begin(), traits.end(), question), traits.end());
        string winner;

        updateProbabilities(character_traits, character_probs, UserAnswer, question);


    }

    return 0;
}

