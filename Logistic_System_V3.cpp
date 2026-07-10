#include<bits/stdc++.h>
using namespace std;
#define rep(i,n) for(int i=0;i<n;i++)
const int mod=1e9+7;
typedef unordered_map<string,int> umsi;
typedef unordered_map<string,umsi> ums;
typedef long long int lli;

ums lab;
umsi supply; 
umsi unclassified_data;

//Command hashing function
lli command(string c){
    lli ans=0;
    rep(i,c.size()) ans=(ans*42+((int)toupper(c[i])-48))%mod;
    return ans;
}
//Command instruction output
void instruction(){
    cout<<"Please input the command in this type of format:"<<endl;
    cout<<"function name(without the parenthsis) or the numeric tag"<<endl;
    cout<<"Commands that are available to adjust the database:"<<endl;
    cout<<"1. addLab(string lab_name)"<<endl;
    cout<<"The purpose of the command: Adding new lab category in the database."<<endl;
    cout<<"How to use: User inputs the lab name. The system will create the lab category in the database."<<endl;
    cout<<"2. removeLab(string lab_name)"<<endl;
    cout<<"The purpose of the command: Removing existed lab category in the database."<<endl;
    cout<<"How to use: User inputs the lab name. The system will remove the lab category from the system if it has existed."<<endl;
    cout<<"3. searchLab()"<<endl;
    cout<<"The purpose of the command: Showing all the lab name in the database."<<endl;
    cout<<"How to use: User inputs the command. The system will output all the information of the lab category in the database."<<endl;
    cout<<"4. searchLabClassification(string lab_name)"<<endl;
    cout<<"The purpose of the command: Showing all the supply classification in the lab category."<<endl;
    cout<<"How to use: User inputs the lab name. The system will output all the supply classifications in the lab category."<<endl;
    cout<<"5. searchAllLabClassification()"<<endl;
    cout<<"The purpose of the command: Showing all the supply classification in every lab category."<<endl;
    cout<<"How to use: User inputs the command. The system will output all the supply classifications in every lab category."<<endl;
    cout<<"6. searchSupplyClassification()"<<endl;
    cout<<"The purpose of the command: Showing all the supply classifications in the database."<<endl;
    cout<<"How to use: User inputs the command. The system will output all the supply classifications with their total amounts."<<endl;
    cout<<"7. addSupply(string supply_name, string lab_name, int amount)"<<endl;
    cout<<"The purpose of the command: Adding supply amount to the existed supply classification in the lab category."<<endl;
    cout<<"How to use: User inputs the supply name, amount and the lab name. The system will add the amount to the supply classification in the lab category."<<endl;
    cout<<"8. removeSupply(string supply_name, string lab_name, int amount)"<<endl;
    cout<<"The purpose of the command: Removing supply amount from the existed supply classification in the lab category."<<endl;
    cout<<"How to use: User inputs the supply name, amount and the lab name. The system will remove the amount from the supply classification in the lab category."<<endl;
    cout<<"9. searchSupply(string supply_name)"<<endl;
    cout<<"The purpose of the command: Showing the information of a supply classification in the database."<<endl;
    cout<<"How to use: User inputs the supply name. The system will output all the labs that have the supply classification inside."<<endl;
    cout<<"10. searchUnclassifiedData()"<<endl;
    cout<<"The purpose of the command: Showing all the unclassified data in the database."<<endl;
    cout<<"How to use: User inputs the command. The system will output all the unclassified data in the database."<<endl;
    cout<<"11. deleteUnclassifiedData()"<<endl;
    cout<<"The purpose of the command: Deleting all the unclassified data in the database."<<endl;
    cout<<"How to use: User inputs the command. The system will delete all the unclassified data in the database."<<endl;
    cout<<"12. classifyData(string supply_name, string lab_name)"<<endl;
    cout<<"The purpose of the command: Classifying the unclassified data into the supply classification in the lab category."<<endl;
    cout<<"How to use: User inputs the supply name and the lab name. The system will classify all unclassified data of the supply into the supply classification in the lab category."<<endl;
    cout<<"13. Help()"<<endl;
    cout<<"The purpose of the command: Providing the command to modify the system and the database"<<endl;
    cout<<"How to use: User inputs the command. The system will output the commands that are available in the system."<<endl;
    cout<<"14. End the system()"<<endl;
    cout<<"The purpose of the command: Closing the system and the database."<<endl;
    cout<<"How to use: User inputs the command. The system and the database will shut down."<<endl;
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0);
    instruction();
    while(true){
        string c;
        getline(cin,c);
        lli key=command(c);
        //addLab()
        if(key==285516734||key==1){
            string lab_name;
            getline(cin,lab_name);
            if(lab.insert({lab_name, umsi{}}).second) cout<<lab_name<<" has successfully added into the system database."<<endl;
            else cout<<lab_name<<" has already existed in the system database."<<endl;
        }
        //removeLab()
        else if(key==844736042||key==2){
            string lab_name;
            getline(cin,lab_name);
            if(!lab.count(lab_name)) cout<<lab_name<<" doesn't exist in the system database."<<endl;
            else{
                auto temp=lab.find(lab_name);
                for(const auto& i:temp->second){
                    supply[i.first]-=i.second;
                    unclassified_data[i.first]+=i.second;
                }
                lab.erase(temp);
                cout<<lab_name<<" has successfully removed from the system database."<<endl;
            }
        }
        //searchLab()
        else if(key==961457947||key==3){
            if(lab.empty()) cout<<"There isn't any lab in the system"<<endl;
            else{
                int index=1;
                for(const auto& i:lab) cout<<index++<<". "<<i.first<<endl;
            }
        }
        //searchLabClassification()
        else if(key==403052850||key==4){
            string lab_name;
            getline(cin,lab_name);
            if(!lab.count(lab_name)) cout<<lab_name<<" doesn't exist in the system database."<<endl;
            else{
                auto temp=lab.find(lab_name);
                if(temp!=lab.end()){
                    if(temp->second.empty()) cout<<"No supplies recorded for this lab yet."<<endl;
                    else{
                        int index=1;
                        for(const auto& i:temp->second) cout<<index++<<". "<<i.first<<" "<<i.second<<endl;
                    }
                }
            }
        }
        //searchAllLabClassification()
        else if(key==851355098||key==5){
            if(lab.empty()) cout<<"There isn't any lab in the system database."<<endl;
            else{
                for(const auto& i:lab){
                    cout<<i.first<<endl;
                    if(i.second.empty()) cout<<"No supplies recorded for this lab yet."<<endl;
                    else{
                        int index=1;
                        for(const auto& j:i.second) cout<<index++<<". "<<j.first<<" "<<j.second<<endl;
                    }
                }
            }
        }
        //searchSupplyClassification()
        else if(key==276693854||key==6){
            if(supply.empty()) cout<<"There isn't any supply classification in the system"<<endl;
            else{
                int index=1;
                for(const auto& i:supply) cout<<index++<<". "<<i.first<<endl;
            }
        }
        //addSupply()
        else if(key==341807955||key==7){
            string supply_name, lab_name;
            int amount;
            getline(cin,supply_name);
            getline(cin,lab_name);
            cin>>amount;
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cout<<"Invalid amount"<<endl;
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            amount=abs(amount);
            auto temp = lab.find(lab_name);
            if(temp!=lab.end()){
                supply[supply_name]+=amount;
                lab[lab_name][supply_name]+=amount; 
                cout<<"Supplies have successfully be added to the system database."<<endl;
            }
            else{
                unclassified_data[supply_name]+=amount;
                cout<<"Lab not found. Supplies have been stored in the unclassified database."<<endl;
            }
        }
        //removeSupply()
        else if(key==781609042||key==8){
            string supply_name, lab_name;
            int amount;
            getline(cin,supply_name);
            getline(cin,lab_name);
            cin>>amount;
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cout<<"Invalid amount"<<endl;
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            amount=abs(amount);
            auto temp=lab.find(lab_name);
            if(temp!=lab.end()){
                auto temp1=supply.find(supply_name);
                auto& temp2=temp->second;
                auto temp3=temp2.find(supply_name);
                if(temp1!=supply.end()&&temp3!=temp2.end()){
                    if(temp3->second<=amount) temp2.erase(temp3);
                    else temp3->second-=amount;
                    if(temp1->second<=amount) supply.erase(temp1);
                    else temp1->second-=amount;
                    cout<<"Supplies have been removed successfully."<<endl;
                }
                else cout<<supply_name<<" doesn't exist in the system database."<<endl;
            }
            else cout<<lab_name<<" doesn't exist in the system database."<<endl;
        }
        //searchSupply()
        else if(key==474046146||key==9){
            string supply_name;
            getline(cin,supply_name);
            if(lab.empty()) cout<<"There isn't any lab in the system database."<<endl;
            else{
                bool avail=0;
                for(const auto& i:lab){
                    if(i.second.count(supply_name)){ 
                        cout<<i.first<<" "<<i.second.at(supply_name)<<endl; 
                        avail=1;
                    }
                }
                if(!avail) cout<<supply_name<<" doesn't exist in the system database."<<endl;
            }
        }
        //searchUnclassifiedData()
        else if(key==238344928||key==42){ 
            if(unclassified_data.empty()) cout<<"There isn't any supplies in the unclassified database."<<endl;
            else for(const auto& i:unclassified_data) cout<<i.first<<" "<<i.second<<endl;
        }
        //deleteUnclassifiedData()
        else if(key==794087791||key==43){ 
            unclassified_data.clear(); 
        }
        //classifyData()
        else if(key==149426950||key==44){
            string supply_name,lab_name;
            getline(cin,supply_name);
            getline(cin,lab_name);
            auto temp=unclassified_data.find(supply_name);
            if(temp!=unclassified_data.end()){
                if(lab.count(lab_name)){
                    supply[supply_name]+=temp->second;
                    lab[lab_name][supply_name]+=temp->second;
                    unclassified_data.erase(temp);
                    cout<<supply_name<<" has successfully moved to "<<lab_name<<endl;
                }
                else cout<<lab_name<<" doesn't exist in the system"<<endl;
            }
            else cout<<supply_name<<" doesn't exist in unclassified database."<<endl;
        }
        //Help()
        else if(key==1816364||key==45){
            instruction();
        }
        //End the system()
        else if(key==169923701||key==46){
            return 0;
        }
        //Error command avoiding mechanism
        else{
            cout<<"Error! command doesn't exist"<<endl;
            cout<<"Type 'Help' or '13' to see all commands."<<endl;
        }
    }
}