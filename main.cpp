// Project UID db1f506d06d84ab787baf250c265e24e
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include "csvstream.h"
#include <set>
#include <map>
#include <cstdlib>
#include <bits/stdc++.h>

using namespace std;

//REQUIRES: The filenames passed in are strings.
//EFFECTS: Takes in two files (training and testing) that are of CSV
//         format and trains itself using the training file to detect 
//         labels for the posts provided in the testing file.
//MODIFIES: None.
class Classifier {
private:
    //training and testing files
    string training_file;
    string test_file;
    
    //variables for the training file
    int num_Posts = 0;
    int num_Words = 0;
    map<string, double> numPosts_containW;
    map<string, double> numPosts_labelC;
    map<pair<string, string>, double> labelC_containW;
    set<string> unique_Words;
    //vector of posts with just the unique words
    vector<set<string>> unique_postWords;
    set<string> unique_Labels;
    vector<pair<string, string>> trainerPosts;

    //variables for the testing file
    vector<pair<string, string>> testingPosts;
    vector<set<string>> unique_postWords_test;
    vector<map<string, double>> pVec_AllPost;
    vector<double> log_prior;
    map<pair<string, string>, double> log_like;
    vector<string> win_Labels;
    vector<double> win_Scores; 

    //REQUIRES: The CSV files don't encounter any error during opening.
    //EFFECTS: opens the training file using a csvstream, and stores the
    //         posts, number of posts, its unique words,
    //         and unique labels in global variable
    //MODIFIES: The global variables trainerPosts, unique_postWords,
    //          unique_Labels, and num_Posts.
    void training_starter() {
        csvstream training_in(training_file);
        map<string, string> input;

        while (training_in >> input) {
            pair<string, string> pair;
            ++num_Posts;
            pair.first = input["tag"];
            pair.second = input["content"];
            trainerPosts.push_back(pair);
            istringstream source(input["content"]);
            string word;
            set<string> post;
            while (source >> word) {
                unique_Words.insert(word);
                post.insert(word);
            }
            unique_postWords.push_back(post);

            unique_Labels.insert(input["tag"]);
        }
    }

    //REQUIRES: The iterator is a size_t type    
    //EFFECTS: Takes in an iterator and uses that to calculate
    //         the log-prior and stores them in a vector. Uses
    //         another helper function named log_calculator
    //         to calculate the log-likelihood and stores them
    //         in a map.
    //         
    //MODIFIES: Modifies the global vectors log_prior, pVec_AllPost
    void prediction_helper(size_t i) {
        map<string, double> pMap_AllLabel;
        set<string>::iterator L_iter = unique_Labels.begin();
        while (L_iter != unique_Labels.end()) {
            double prob_curLabel = log(numPosts_labelC[*L_iter] / num_Posts);
            log_prior.push_back(prob_curLabel);
            set<string>::iterator W_iter = unique_postWords_test[i].begin();
            while (W_iter != unique_postWords_test[i].end()) {
                if (unique_Words.find(*W_iter) == unique_Words.end()) {
                    prob_curLabel += log(1.0 / num_Posts);
                    //how to store this probability.. look tab for idea
                }
                else if (labelC_containW[make_pair(*L_iter, *W_iter)] == 0) {
                    prob_curLabel += log(numPosts_containW[*W_iter] 
                                    / num_Posts);
                }
                else {
                    prob_curLabel += log_like[{*L_iter, *W_iter}];
                }
                ++W_iter;
            }
            pMap_AllLabel[*L_iter] = prob_curLabel;
            ++L_iter;
        }
        pVec_AllPost.push_back(pMap_AllLabel);
    }

    //REQUIRES: The iterator is a size_t type
    //EFFECTS: helps to calculate the log-likelihood in a certain
    //         condition when the word is not unique AND the word
    //         appears for the specific label that's being looked
    //         at.
    //MODIFIES: global vector log_like
    void log_calculator(size_t i) {
        set<string>::iterator L_iter = unique_Labels.begin();
        while (L_iter != unique_Labels.end()) {
            log_prior.push_back(log(numPosts_labelC[*L_iter] / num_Posts));
            set<string>::iterator W_iter = unique_postWords[i].begin();
            while (W_iter != unique_postWords[i].end()) {
                if (!(unique_Words.find(*W_iter) == unique_Words.end()
                    && (labelC_containW[{*L_iter, *W_iter}] == 0)))
                {
                    log_like[{*L_iter, *W_iter}]
                            = log(labelC_containW[{*L_iter, *W_iter}]
                            / numPosts_labelC[*L_iter]);
                }
                ++W_iter;
            }
            ++L_iter;
        }
    }

    //REQUIRES: the Label and Word iterators are of type 
    //          set<string>::Iterator.
    //EFFECTS: Takes in Label and Word iterator and uses 
    //         them to print the count of words and the log-
    //         likelihood of a label appearing given a word,
    //MODIFIES: None.
    void print_helper(set<string>::iterator L_iter, 
                        set<string>::iterator W_iter) {
        if (labelC_containW[{*L_iter, *W_iter}] != 0) {
            cout << "  " << *L_iter << ":"  << *W_iter
                << ", count = " << labelC_containW[{*L_iter, *W_iter}]
                << ", log-likelihood = "
                << log_like[{*L_iter, *W_iter}] << endl;
        }
    }

public:
    Classifier(string training_file_in, string test_file_in)
                : training_file(training_file_in), 
                  test_file(test_file_in) {
    }
    

    //REQUIRES: None.
    //EFFECTS: Uses the training_starter helper function to 
    //         set up the relevant variables, then collects the
    //         training data as required by the project 
    //         specification.
    //MODIFIES: numPosts_containW, numPosts_labelC, labelC_containW
    void training() {

        training_starter();

        num_Words = unique_Words.size();

        //need number of posts containing each word W
        set<string>::iterator iter = unique_Words.begin(); 
        for (size_t i = 0; i < unique_Words.size(); i++) {
            numPosts_containW.insert({*iter, 0.0});
            ++iter;
        }


        for (size_t i = 0; i < unique_postWords.size(); i++) {
            iter = unique_postWords[i].begin();
            while (iter != unique_postWords[i].end()) {
                numPosts_containW[*iter] += 1;
                ++iter;
            }
        }

        set<string>::iterator iter2 = unique_Labels.begin(); 
        for (size_t i = 0; i < unique_Labels.size(); i++) {
            numPosts_labelC.insert({*iter2, 0.0});
            ++iter2;
        }

        for (size_t i = 0; i < trainerPosts.size(); i++) {
            numPosts_labelC[trainerPosts[i].first] += 1;
        }

        iter = unique_Labels.begin();
        iter2 = unique_Words.begin();
        //vector<pair<string,string>> LWVec;

        while(iter != unique_Labels.end()) {
            //pair<label, word>
            pair<string, string> LW_combination;
            while (iter2 != unique_Words.end()) {
                LW_combination.first = *iter;
                LW_combination.second = *iter2;
                ++iter2;
                //LWVec.push_back(LW_combination);
                labelC_containW.insert({LW_combination, 0.0});
            }
        ++iter;
        }

        

        for (size_t i = 0; i < trainerPosts.size(); i++) {
            iter = unique_postWords[i].begin();
            pair<string, string> key;
            while (iter != unique_postWords[i].end()) {
                key = make_pair(trainerPosts[i].first, *iter);
                labelC_containW[key] += 1;
                ++iter;
            }
        }
    }

    //REQUIRES: The CSV files don't encounter any error during opening.
    //EFFECTS: Opens the test file and stores its posts and unique words
    //         in separate vectors, then uses the log_calculator and
    //         prediction_helper helper functions to calculate the prediction.
    //         Determines the winning label and scores.
    //MODIFIES: unique_postWords_test, win_Labels, win_Scores

    void prediction() {
        csvstream testing_in(test_file);
        map<string, string> input;
        while (testing_in >> input) {
            pair<string, string> pair;
            pair.first = input["tag"];
            pair.second = input["content"];
            testingPosts.push_back(pair);
            istringstream source(input["content"]);
            string word;
            set<string> post;
            while (source >> word) {
                post.insert(word);
            }
            unique_postWords_test.push_back(post);
        }

        for (size_t i = 0; i < unique_postWords.size(); i++) {
            log_calculator(i);
        }

        for (size_t i = 0; i < unique_postWords_test.size(); i++) {
            prediction_helper(i);
        }

        for (size_t i = 0; i < pVec_AllPost.size(); i++) {
            double curMax = -INFINITY;
            string winLabel;
            map<string, double>::iterator iter = pVec_AllPost[i].begin();
            while (iter != pVec_AllPost[i].end()) {
                if (iter->second > curMax) {
                    curMax = iter->second;
                    winLabel = iter->first;
                }
                ++iter;
            }
            win_Labels.push_back(winLabel);
            win_Scores.push_back(curMax);
        }

        
    }

    //REQUIRES: argc is either 3 or 4
    //EFFECTS:  if argc is 4, then prints debug data as directed in
    //          specification. If argc is 3, prints other information as 
    //          spcification directs. Calculates the performance of the
    //          classifier by calculating correct number of posts (when 
    //          argc is 4).
    //MODIFIES: None.cd
    void print(int argc) {
        if (argc == 4) {
            cout << "training data:" << endl;
            for (size_t i = 0; i < trainerPosts.size(); i++) {
                cout << "  label = " << trainerPosts[i].first << ","
                     << " content = " << trainerPosts[i].second << endl;
            }
        }

        cout << "trained on " << num_Posts << " examples" << endl;

        if (argc == 4) {
            cout << "vocabulary size = " << num_Words << endl;
        }

        cout << endl;

        if (argc == 4) {
            cout << "classes:" << endl;
            set<string>::iterator iter = unique_Labels.begin();
            size_t i = 0;
            while (iter != unique_Labels.end()) {
                cout << "  " << *iter << ", " << numPosts_labelC[*iter]
                     << " examples, log-prior = "
                     << log_prior[i]  << endl;
                ++iter;
                ++i;
            }

            cout << "classifier parameters:" << endl;
            set<string>::iterator L_iter = unique_Labels.begin();
            while (L_iter != unique_Labels.end()) {
                set<string>::iterator W_iter = unique_Words.begin();
                while (W_iter != unique_Words.end()) {
                    print_helper(L_iter, W_iter);
                    ++W_iter;
                }
                ++L_iter;
            }
            cout << endl;
        }
    
        int correct = 0;
        

        cout << "test data:" << endl;
        for (size_t j = 0; j < pVec_AllPost.size(); j++) {
            cout << "  correct = " << testingPosts[j].first
                 << ", predicted = " << win_Labels[j]
                 << ", log-probability score = "
                 << win_Scores[j] << endl;
            cout << "  content = " << testingPosts[j].second << endl <<endl;

            if (win_Labels[j] == testingPosts[j].first) {
                ++correct;
            }
            
        }

        cout << "performance: " << correct << " / " << pVec_AllPost.size()
             << " posts predicted correctly" << endl;
    
    }

};

int main(int argc, char* argv[]) {

    cout.precision(3);

    if ((argc != 3 && argc != 4)) {
        cout <<"Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" 
             << endl;
        return 1;
    }
    if (argc == 4) {
        string arg3 = argv[3];
        if (arg3 != "--debug") {
            cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" 
                << endl;
            return 1;
        }
    }

    string training_file = argv[1];
    string test_file = argv[2];

    ifstream training_file_check(training_file);
    ifstream test_file_check(test_file);
    if (!training_file_check.is_open() || !test_file_check.is_open()) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" 
             << endl;
        return 1;
    }

    training_file_check.close();
    test_file_check.close();

    Classifier classifier(training_file, test_file);

    classifier.training();

    classifier.prediction();

    classifier.print(argc);
    
}

