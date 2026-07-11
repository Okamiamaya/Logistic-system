#include<bits/stdc++.h>
using namespace std;
#define rep(i,n) for(int i=0;i<n;i++)
const int mod=1e9+7;

// SupplyMap:      supply name  -> amount in stock
// LabMap:         lab name     -> SupplyMap of everything stocked in that lab
// HashType:       the numeric hash produced by hashCommand() for a typed command
typedef unordered_map<string,int> SupplyMap;
typedef unordered_map<string,SupplyMap> LabMap;
typedef long long int HashType;

LabMap labDatabase;            // every lab, and what each lab currently has in stock
SupplyMap supplyTotals;        // supply name -> total amount across every lab combined
SupplyMap unclassifiedSupplies;// supplies that were added before their lab existed

// CommandKey mirrors the commands recognised by hashCommand()/main(). It is not
// consulted anywhere in the dispatch logic below (main() compares raw hash
// values directly), but it documents which hash belongs to which command.
enum class CommandKey{
    ADDLAB=285516734,
    REMOVELAB=844736042,
    SEARCHLAB=961457947,
    SEARCHLABCLASSIFICATION=403052850,
    SEARCHALLLABCLASSIFICATION=851355098,
    SEARCHSUPPLYCLASSIFICATION=276693854,
    ADDSUPPLY=341807955,
    REMOVESUPPLY=781609042,
    SEARCHSUPPLY=474046146,
    SEARCHUNCLASSIFIEDDATA=238344928,
    DELETEUNCLASSIFIEDDATA=794087791,
    CLASSIFYDATA=149426950,
    HELP=1816364,
    ENDTHESYSTEM=937733000
};

// hashCommand()
// Purpose: Turns whatever the user typed into a single numeric fingerprint
// (a rolling/polynomial hash) so that main() can identify the intended
// command with a handful of integer comparisons instead of string
// comparisons. Case-insensitive because every character is upper-cased
// before being folded into the hash.
HashType hashCommand(const string& rawInput){
    HashType hashValue=0;
    rep(charIndex,rawInput.size())
        hashValue=(hashValue*42+((int)toupper(rawInput[charIndex])-48))%mod;
    return hashValue;
}

// printInstructions()
// Purpose: Prints the full command reference (name, purpose, and usage) for
// every command the system understands. Called once at startup and again
// whenever the user asks for Help.
void printInstructions(){
    cout<<"Please input the command in this type of format:"<<endl;
    cout<<"function name(without the parenthsis) or the numeric tag"<<endl;
    cout<<"Commands that are available to adjust the database:"<<endl<<endl;
    cout<<"1. addLab(string lab_name)"<<endl;
    cout<<"The purpose of the command: Adding new lab category in the database."<<endl;
    cout<<"How to use: User inputs the lab name. The system will create the lab category in the database."<<endl<<endl;
    cout<<"2. removeLab(string lab_name)"<<endl;
    cout<<"The purpose of the command: Removing existed lab category in the database."<<endl;
    cout<<"How to use: User inputs the lab name. The system will remove the lab category from the system if it has existed."<<endl<<endl;
    cout<<"3. searchLab()"<<endl;
    cout<<"The purpose of the command: Showing all the lab name in the database."<<endl;
    cout<<"How to use: User inputs the command. The system will output all the information of the lab category in the database."<<endl<<endl;
    cout<<"4. searchLabClassification(string lab_name)"<<endl;
    cout<<"The purpose of the command: Showing all the supply classification in the lab category."<<endl;
    cout<<"How to use: User inputs the lab name. The system will output all the supply classifications in the lab category."<<endl<<endl;
    cout<<"5. searchAllLabClassification()"<<endl;
    cout<<"The purpose of the command: Showing all the supply classification in every lab category."<<endl;
    cout<<"How to use: User inputs the command. The system will output all the supply classifications in every lab category."<<endl<<endl;
    cout<<"6. searchSupplyClassification()"<<endl;
    cout<<"The purpose of the command: Showing all the supply classifications in the database."<<endl;
    cout<<"How to use: User inputs the command. The system will output all the supply classifications with their total amounts."<<endl<<endl;
    cout<<"7. addSupply(string supply_name, string lab_name, int amount)"<<endl;
    cout<<"The purpose of the command: Adding supply amount to the existed supply classification in the lab category."<<endl;
    cout<<"How to use: User inputs the supply name, amount and the lab name. The system will add the amount to the supply classification in the lab category."<<endl<<endl;
    cout<<"8. removeSupply(string supply_name, string lab_name, int amount)"<<endl;
    cout<<"The purpose of the command: Removing supply amount from the existed supply classification in the lab category."<<endl;
    cout<<"How to use: User inputs the supply name, amount and the lab name. The system will remove the amount from the supply classification in the lab category."<<endl<<endl;
    cout<<"9. searchSupply(string supply_name)"<<endl;
    cout<<"The purpose of the command: Showing the information of a supply classification in the database."<<endl;
    cout<<"How to use: User inputs the supply name. The system will output all the labs that have the supply classification inside."<<endl<<endl;
    cout<<"10. searchUnclassifiedData()"<<endl;
    cout<<"The purpose of the command: Showing all the unclassified data in the database."<<endl;
    cout<<"How to use: User inputs the command. The system will output all the unclassified data in the database."<<endl<<endl;
    cout<<"11. deleteUnclassifiedData()"<<endl;
    cout<<"The purpose of the command: Deleting all the unclassified data in the database."<<endl;
    cout<<"How to use: User inputs the command. The system will delete all the unclassified data in the database."<<endl<<endl;
    cout<<"12. classifyData(string supply_name, string lab_name)"<<endl;
    cout<<"The purpose of the command: Classifying the unclassified data into the supply classification in the lab category."<<endl;
    cout<<"How to use: User inputs the supply name and the lab name. The system will classify all unclassified data of the supply into the supply classification in the lab category."<<endl<<endl;
    cout<<"13. Help()"<<endl;
    cout<<"The purpose of the command: Providing the command to modify the system and the database"<<endl;
    cout<<"How to use: User inputs the command. The system will output the commands that are available in the system."<<endl<<endl;
    cout<<"14. EndTheSystem()"<<endl;
    cout<<"The purpose of the command: Closing the system and the database."<<endl;
    cout<<"How to use: User inputs the command. The system and the database will shut down."<<endl<<endl;
}

// saveData()
// Purpose: Serializes labDatabase, supplyTotals, and unclassifiedSupplies to
// a plain-text file so the database survives between runs of the program.
void saveData(const string& filename){
    ofstream out(filename, ios::out|ios::trunc);
    if(!out.is_open()){
        cerr<<"Error: could not open "<<filename<<" for saving."<< endl;
        return;
    }
    out<<labDatabase.size()<<"\n";
    for(const auto& labEntry:labDatabase){
        out<<labEntry.first<<"\n";
        out<<labEntry.second.size()<<"\n";
        for(const auto& supplyEntry:labEntry.second) out<<supplyEntry.first<<"\n"<<supplyEntry.second<<"\n";
    }
    out<<supplyTotals.size()<<"\n";
    for(const auto& supplyEntry:supplyTotals) out<<supplyEntry.first<<"\n"<<supplyEntry.second<<"\n";
    out<<unclassifiedSupplies.size()<<"\n";
    for(const auto& unclassifiedEntry:unclassifiedSupplies) out<<unclassifiedEntry.first<<"\n"<<unclassifiedEntry.second<<"\n";
    out.close();
    cout<<"Data saved to "<<filename<<endl;
}

// loadData()
// Purpose: Restores labDatabase, supplyTotals, and unclassifiedSupplies from
// a save file previously written by saveData(). If the file is missing or
// corrupted, the system simply starts with empty data instead of crashing.
void loadData(const string& filename){
    ifstream in(filename);
    if(!in.is_open()) return;

    labDatabase.clear();
    supplyTotals.clear();
    unclassifiedSupplies.clear();

    int labCount;
    if(!(in>>labCount)||labCount<0){
        cerr<<"Warning: save file missing or corrupted. Starting with empty data."<<endl;
        return;
    }
    in.ignore();
    rep(labIndex, labCount){
        string labName;
        if(!getline(in, labName)) break;
        int supplyCount;
        if(!(in>>supplyCount)||supplyCount<0) break;
        in.ignore();
        SupplyMap labSupplies;
        rep(supplyIndex, supplyCount){
            string supplyName;
            int amount;
            if(!getline(in, supplyName)||!(in>>amount)) break;
            in.ignore();
            labSupplies[supplyName]=amount;
        }
        labDatabase[labName]=labSupplies;
    }

    int supplyTotalCount;
    if(in>>supplyTotalCount&&supplyTotalCount>=0){
        in.ignore();
        rep(supplyIndex, supplyTotalCount){
            string supplyName;
            int amount;
            if(!getline(in, supplyName)||!(in>>amount)) break;
            in.ignore();
            supplyTotals[supplyName]=amount;
        }
    }

    int unclassifiedCount;
    if(in>>unclassifiedCount&&unclassifiedCount>=0){
        in.ignore();
        rep(unclassifiedIndex, unclassifiedCount){
            string supplyName;
            int amount;
            if(!getline(in, supplyName)||!(in>>amount)) break;
            in.ignore();
            unclassifiedSupplies[supplyName]=amount;
        }
    }

    in.close();
    cout<<"Data loaded from "<<filename<<endl;
}


// main()
// Purpose: Runs the read-command / hash-it / act-on-it loop for the whole
// program. Every typed command is hashed by hashCommand(), and the resulting
// number is compared against the fixed hash constants below (each block is
// annotated with which command it implements) to decide what to do. This
// if/else-by-hash-value chain is the command-determination mechanism for
// this version of the program and is left structurally unchanged.
int main(){
    ios_base::sync_with_stdio(0); cin.tie(0);
    loadData("logistic_data.txt");
    printInstructions();
    while(true){
        string rawInput;
        getline(cin,rawInput);
        HashType hashValue=hashCommand(rawInput);

        // addLab(string lab_name)
        // Purpose: Adds a new lab category to the database if it doesn't already exist.
        if(hashValue==285516734||hashValue==1){
            string labName;
            cout<<"Please input your lab name: "<<flush;
            getline(cin,labName);
            if(labDatabase.insert({labName, SupplyMap{}}).second) cout<<labName<<" has successfully added into the system database."<<endl;
            else cout<<labName<<" has already existed in the system database."<<endl;
        }
        // removeLab(string lab_name)
        // Purpose: Removes an existing lab category. Everything that lab was
        // stocking is moved back into unclassifiedSupplies rather than being
        // deleted outright, so no supply data is lost.
        else if(hashValue==844736042||hashValue==2){
            string labName;
            cout<<"Please input your lab name: "<<flush;
            getline(cin,labName);
            if(!labDatabase.count(labName)) cout<<labName<<" doesn't exist in the system database."<<endl;
            else{
                auto labIterator=labDatabase.find(labName);
                for(const auto& supplyEntry:labIterator->second){
                    supplyTotals[supplyEntry.first]-=supplyEntry.second;
                    unclassifiedSupplies[supplyEntry.first]+=supplyEntry.second;
                }
                labDatabase.erase(labIterator);
                cout<<labName<<" has successfully removed from the system database."<<endl;
            }
        }
        // searchLab()
        // Purpose: Lists the name of every lab currently in the database.
        else if(hashValue==961457947||hashValue==3){
            if(labDatabase.empty()) cout<<"There isn't any lab in the system"<<endl;
            else{
                int index=1;
                for(const auto& labEntry:labDatabase) cout<<index++<<". "<<labEntry.first<<endl;
            }
        }
        // searchLabClassification(string lab_name)
        // Purpose: Lists every supply (and its amount) stocked by one specific lab.
        else if(hashValue==403052850||hashValue==4){
            string labName;
            cout<<"Please input your lab name: "<<flush;
            getline(cin,labName);
            if(!labDatabase.count(labName)) cout<<labName<<" doesn't exist in the system database."<<endl;
            else{
                auto labIterator=labDatabase.find(labName);
                if(labIterator!=labDatabase.end()){
                    if(labIterator->second.empty()) cout<<"No supplies recorded for this lab yet."<<endl;
                    else{
                        int index=1;
                        for(const auto& supplyEntry:labIterator->second) cout<<index++<<". "<<supplyEntry.first<<" "<<supplyEntry.second<<endl;
                    }
                }
            }
        }
        // searchAllLabClassification()
        // Purpose: Lists every supply (and its amount) stocked by every lab.
        else if(hashValue==851355098||hashValue==5){
            if(labDatabase.empty()) cout<<"There isn't any lab in the system database."<<endl;
            else{
                for(const auto& labEntry:labDatabase){
                    cout<<labEntry.first<<endl;
                    if(labEntry.second.empty()) cout<<"No supplies recorded for this lab yet."<<endl;
                    else{
                        int index=1;
                        for(const auto& supplyEntry:labEntry.second) cout<<index++<<". "<<supplyEntry.first<<" "<<supplyEntry.second<<endl;
                    }
                }
            }
        }
        // searchSupplyClassification()
        // Purpose: Lists the name of every supply classification tracked in the database.
        else if(hashValue==276693854||hashValue==6){
            if(supplyTotals.empty()) cout<<"There isn't any supply classification in the system"<<endl;
            else{
                int index=1;
                for(const auto& supplyEntry:supplyTotals) cout<<index++<<". "<<supplyEntry.first<<endl;
            }
        }
        // addSupply(string supply_name, string lab_name, int amount)
        // Purpose: Adds stock to a supply classification inside a given lab.
        // If the lab doesn't exist yet, the stock is parked in
        // unclassifiedSupplies instead of being rejected.
        else if(hashValue==341807955||hashValue==7){
            string supplyName, labName;
            int amount;
            cout<<"Please input your supply name: "<<flush;
            getline(cin,supplyName);
            cout<<"Please input your lab name: "<<flush;
            getline(cin,labName);
            cout<<"Please input your supply amount: "<<flush;
            cin>>amount;
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cout<<"Invalid amount"<<endl;
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            amount=abs(amount);
            auto labIterator = labDatabase.find(labName);
            if(labIterator!=labDatabase.end()){
                supplyTotals[supplyName]+=amount;
                labDatabase[labName][supplyName]+=amount;
                cout<<"Supplies have successfully be added to the system database."<<endl;
            }
            else{
                unclassifiedSupplies[supplyName]+=amount;
                cout<<"Lab not found. Supplies have been stored in the unclassified database."<<endl;
            }
        }
        // removeSupply(string supply_name, string lab_name, int amount)
        // Purpose: Removes stock from a supply classification inside a given
        // lab, deleting that entry entirely if the removal empties it out.
        else if(hashValue==781609042||hashValue==8){
            string supplyName, labName;
            int amount;
            cout<<"Please input your supply name: "<<flush;
            getline(cin,supplyName);
            cout<<"Please input your lab name: "<<flush;
            getline(cin,labName);
            cout<<"Please input your supply amount: "<<flush;
            cin>>amount;
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cout<<"Invalid amount"<<endl;
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            amount=abs(amount);
            auto labIterator=labDatabase.find(labName);
            if(labIterator!=labDatabase.end()){
                auto supplyTotalIterator=supplyTotals.find(supplyName);
                auto& labSupplies=labIterator->second;
                auto supplyIterator=labSupplies.find(supplyName);
                if(supplyTotalIterator!=supplyTotals.end()&&supplyIterator!=labSupplies.end()){
                    if(supplyIterator->second<=amount) labSupplies.erase(supplyIterator);
                    else supplyIterator->second-=amount;
                    if(supplyTotalIterator->second<=amount) supplyTotals.erase(supplyTotalIterator);
                    else supplyTotalIterator->second-=amount;
                    cout<<"Supplies have been removed successfully."<<endl;
                }
                else cout<<supplyName<<" doesn't exist in the system database."<<endl;
            }
            else cout<<labName<<" doesn't exist in the system database."<<endl;
        }
        // searchSupply(string supply_name)
        // Purpose: Shows every lab that stocks a given supply, along with the amount each holds.
        else if(hashValue==474046146||hashValue==9){
            string supplyName;
            cout<<"Please input your supply name: "<<flush;
            getline(cin,supplyName);
            if(labDatabase.empty()) cout<<"There isn't any lab in the system database."<<endl;
            else{
                bool found=false;
                for(const auto& labEntry:labDatabase){
                    if(labEntry.second.count(supplyName)){
                        cout<<labEntry.first<<" "<<labEntry.second.at(supplyName)<<endl;
                        found=true;
                    }
                }
                if(!found) cout<<supplyName<<" doesn't exist in the system database."<<endl;
            }
        }
        // searchUnclassifiedData()
        // Purpose: Lists every supply currently sitting in unclassifiedSupplies.
        else if(hashValue==238344928||hashValue==42){
            if(unclassifiedSupplies.empty()) cout<<"There isn't any supplies in the unclassified database."<<endl;
            else for(const auto& unclassifiedEntry:unclassifiedSupplies) cout<<unclassifiedEntry.first<<" "<<unclassifiedEntry.second<<endl;
        }
        // deleteUnclassifiedData()
        // Purpose: Permanently wipes out every entry in unclassifiedSupplies.
        else if(hashValue==794087791||hashValue==43){
            unclassifiedSupplies.clear();
        }
        // classifyData(string supply_name, string lab_name)
        // Purpose: Moves a supply out of unclassifiedSupplies and files it under a named lab.
        else if(hashValue==149426950||hashValue==44){
            string supplyName,labName;
            cout<<"Please input your supply name: "<<flush;
            getline(cin,supplyName);
            cout<<"Please input your lab name: "<<flush;
            getline(cin,labName);
            auto unclassifiedIterator=unclassifiedSupplies.find(supplyName);
            if(unclassifiedIterator!=unclassifiedSupplies.end()){
                if(labDatabase.count(labName)){
                    supplyTotals[supplyName]+=unclassifiedIterator->second;
                    labDatabase[labName][supplyName]+=unclassifiedIterator->second;
                    unclassifiedSupplies.erase(unclassifiedIterator);
                    cout<<supplyName<<" has successfully moved to "<<labName<<endl;
                }
                else cout<<labName<<" doesn't exist in the system"<<endl;
            }
            else cout<<supplyName<<" doesn't exist in unclassified database."<<endl;
        }
        // Help()
        // Purpose: Re-prints the full command reference.
        else if(hashValue==1816364||hashValue==45){
            printInstructions();
        }
        // EndTheSystem()
        // Purpose: Persists the database to disk and stops the program.
        else if(hashValue==937733000||hashValue==46){
            saveData("logistic_data.txt");
            return 0;
        }
        // Error command avoiding mechanism
        // Purpose: Catches any hash that didn't match a known command and
        // points the user toward Help instead of silently doing nothing.
        else{
            cout<<"Error! command doesn't exist"<<endl;
            cout<<"Type 'Help' or '13' to see all commands."<<endl;
        }
    }
}
